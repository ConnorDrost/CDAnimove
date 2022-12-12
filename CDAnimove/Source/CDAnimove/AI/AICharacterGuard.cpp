// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI/AICharacterGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "../HealthComponent/HealthComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"

#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Perception/AISense_Sight.h"

AAICharacterGuard::AAICharacterGuard()

{
    PrimaryActorTick.bCanEverTick = true;

    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

    PawnSensingComp->OnSeePawn.AddDynamic(this, &AAICharacterGuard::OnPawnSeen);

    GuardState = EAIState::Idle;

    HealthComp->MaxHealth = 10.f;

    Tags.Empty();
    Tags.Add("Enemy");
}

void AAICharacterGuard::BeginPlay()
{
    Super::BeginPlay();

    pController = GetController();

    bIsAlive = HealthComp->isAlive();

    if (bPatrol)
    {
        MoveToNextPatrolPoint();
    }
}

void AAICharacterGuard::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsAlive)
    {
        UpdateCharacter();

        CheckAggression();

        if (!bIsAlive)
        {
            GetWorldTimerManager().SetTimer(deathTimerHandle, this, &AAICharacterGuard::IfDead, 0.5f);
        }
    }

    if (CurrentPatrolPoint && TargetActor == nullptr)
    {
        FVector Delta = GetActorLocation() - CurrentPatrolPoint->GetActorLocation();
        float DistanceToGoal = Delta.Size();

        if (DistanceToGoal < 100)
        {
            MoveToNextPatrolPoint();
        }
    }

    if (TargetActor)
    {
        if (pController)
        {
            UAIBlueprintHelperLibrary::SimpleMoveToActor(pController, TargetActor);
        }
    }
}

void AAICharacterGuard::MoveToNextPatrolPoint()
{
    if (CurrentPatrolPoint == nullptr || CurrentPatrolPoint == SecondPatrolPoint)
    {
        CurrentPatrolPoint = FirstPatrolPoint;
    }
    else
    {
        CurrentPatrolPoint = SecondPatrolPoint;
    }

    if (pController)
    {
        UAIBlueprintHelperLibrary::SimpleMoveToActor(pController, CurrentPatrolPoint);
    }
}

void AAICharacterGuard::OnPawnSeen(APawn* SeenPawn)
{
    if (SeenPawn == nullptr)
    {
        return;
    }

    TargetActor = SeenPawn;

    SetGuardState(EAIState::Alerted);

}

void AAICharacterGuard::CheckAggression()
{
    if (bIsAlive)
    {
        for (TActorIterator<AMainChar> ActorItr(GetWorld()); ActorItr; ++ActorItr)
        {
            if (ActorItr->ActorHasTag("Player"))
            {
                if (GuardState == EAIState::Alerted)
                {
                    float DistanceToMainChar = abs(GetActorLocation().X - ActorItr->GetActorLocation().X);

                    if (DistanceToMainChar < 100)
                    {

                        if (!bIsAttacking && bCanAttack)
                        {
                            bIsAttacking = true;

                            ActivateCollision(AttackBox);

                            bCanAttack = false;

                            GetWorldTimerManager().SetTimer(timerHandle, this, &AAICharacterGuard::EndAttack, 2.f);
                        }


                    }
                    else if (DistanceToMainChar > 500)
                    {
                        SetGuardState(EAIState::Idle);
                        TargetActor = nullptr;

                        if (bPatrol)
                        {
                            MoveToNextPatrolPoint();
                        }
                    }

                }
            }
        }
    }
}

void AAICharacterGuard::IfDead()
{
    Destroy();
}

void AAICharacterGuard::CombatBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        if (OtherActor->IsA<AMainChar>())
        {
            if (OverlappedComponent == AttackBox)
            {
                AMainChar* HitPlayer = static_cast<AMainChar*>(OtherActor);

                HitPlayer->HealthComp->DecreaseHealth(2.f);
                DeactivateCollision(AttackBox);

            }
        }
    }

}

void AAICharacterGuard::CombatEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AAICharacterGuard::UpdateAnimation()
{

    const FVector Velocity = GetVelocity();
    const float SpeedSqr = Velocity.SizeSquared();

    if (bIsAlive)
    {
        if (!bIsAttacking)
        {
            UPaperFlipbook* DesiredAnimation = (SpeedSqr > 0.f) ? CurrentFlipbooks.RunAnimation : CurrentFlipbooks.IdleAnimation;

            if (GetSprite()->GetFlipbook() != DesiredAnimation)
            {
                GetSprite()->SetFlipbook(DesiredAnimation);
            }
        }
        else
        {
            GetSprite()->SetFlipbook(CurrentFlipbooks.AttackAnimation);
        }
    }
    else
    {
        GetSprite()->SetFlipbook(CurrentFlipbooks.DeathAnimation);
    }
}

void AAICharacterGuard::SetGuardState(EAIState NewState)
{
    if (GuardState == NewState)
    {
        return;
    }

    GuardState = NewState;
}