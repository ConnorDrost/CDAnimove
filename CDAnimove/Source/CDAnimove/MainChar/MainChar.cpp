// Fill out your copyright notice in the Description page of Project Settings.


#include "MainChar.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "../HealthComponent/HealthComponent.h"
#include "../AI/AICharacterGuard.h"
#include "../Checkpoint/Checkpoint.h"
#include "Components/CapsuleComponent.h"

AMainChar::AMainChar()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;


	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	SpringArmComponent->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	SpringArmComponent->SetUsingAbsoluteRotation(true);
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->bAutoActivate = true;

	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.f, -1.f, 0.f));

	AttackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LowAttackBox"));
	AttackBox->SetupAttachment(GetSprite());
	AttackBox->SetCollisionProfileName("OverlapAll");
	AttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HealthComp = CreateDefaultSubobject<UHealthComponent>("HealthComp");

	NoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("Noise Emitter"));

	bIsAttacking = false;
	bCanBeDamaged = true;
	HealthComp->MaxHealth = 10.f;
	bIsAlive = true;
	bCanAttack = true;
	WeaponDelay = 0.5f;
	Speed = 4.f;

	Tags.Add("Player");
}

void AMainChar::BeginPlay()
{
	Super::BeginPlay();

	RespawnLocation = GetActorLocation();

	AttackBox->OnComponentBeginOverlap.AddDynamic(this, &AMainChar::CombatBeginOverlap);
	AttackBox->OnComponentEndOverlap.AddDynamic(this, &AMainChar::CombatEndOverlap);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMainChar::CPBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMainChar::CPEndOverlap);
}

void AMainChar::Tick(float DeltaSeconds)
{

	Super::Tick(DeltaSeconds);

	if (bIsAlive)
	{
		UpdateCharacter();

		if (!bIsAlive)
		{
			GetWorldTimerManager().SetTimer(deathTimerHandle, this, &AMainChar::IfDead, 0.75f);
		}
	}

}

void AMainChar::Jump()
{
	if (bIsAlive)
	{
		Super::Jump();
	}
}

void AMainChar::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainChar::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainChar::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMainChar::StopJumping);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMainChar::Attack);
}

void AMainChar::MoveRight(float value)
{
	if (!bIsAttacking && bIsAlive && bCanBeDamaged)
	{
		AddMovementInput(FVector(1.f, 0.f, 0.f), value * Speed);
	}
}

void AMainChar::UpdateCharacter()
{
	bIsAlive = HealthComp->isAlive();

	UpdateAnimation();

	const FVector PlayerVelocity = GetVelocity();
	float TravelDirection = PlayerVelocity.X;

	if (Controller != nullptr)
	{
		if (TravelDirection < 0.f)
		{
			Controller->SetControlRotation(FRotator(0.f, 180.f, 0.f));
		}
		else if (TravelDirection > 0.f)
		{
			Controller->SetControlRotation(FRotator(0.f, 0.f, 0.f));
		}
	}

}

void AMainChar::UpdateAnimation()
{
	bool bIsFalling = GetMovementComponent()->IsFalling();
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	if (bIsAlive)
	{
		if (!bIsFalling)
		{
			if (!bIsAttacking)
			{
				if (bCanBeDamaged)
				{
					UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.f) ? RunAnimation : IdleAnimation;

					if (GetSprite()->GetFlipbook() != DesiredAnimation)
					{
						GetSprite()->SetFlipbook(DesiredAnimation);
					}
				}
			}
			else
			{
				GetSprite()->SetFlipbook(AttackAnimation);
			}
		}
		else
		{
			if (GetSprite()->GetFlipbook() != JumpAnimation)
			{
				GetSprite()->SetFlipbook(JumpAnimation);
			}
		}
	}
	else
	{
		GetSprite()->SetFlipbook(DeathAnimation);
	}
}

void AMainChar::Attack()
{

	bool bIsFalling = GetMovementComponent()->IsFalling();

	if (!bIsFalling && bIsAlive)
	{
		if (!bIsAttacking && bCanAttack)
		{

			bIsAttacking = true;

			ActivateCollision(AttackBox);

			bCanAttack = false;

			GetWorldTimerManager().SetTimer(timerHandle, this, &AMainChar::EndAttack, 0.35f);
		}
	}
}

void AMainChar::EndAttack()
{
	bIsAttacking = false;

	DeactivateCollision(AttackBox);

	GetWorldTimerManager().SetTimer(attackDelayHandle, this, &AMainChar::ResetWeaponDelay, WeaponDelay);

}

void AMainChar::IfDead()
{
	HealthComp->ResetHealth();
	bIsAlive = HealthComp->isAlive();
	SetActorLocation(FVector(0, 100, 240));
}


void AMainChar::ResetWeaponDelay()
{
	bCanAttack = true;
}

void AMainChar::ActivateCollision(UBoxComponent* Comp)
{
	Comp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMainChar::DeactivateCollision(UBoxComponent* Comp)
{
	Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMainChar::CombatBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->IsA<AAICharacterGuard>())
		{
			if (OverlappedComponent == AttackBox)
			{
				AAICharacterGuard* HitEnemy = static_cast<AAICharacterGuard*>(OtherActor);

				HitEnemy->HealthComp->DecreaseHealth(2.f);
				HitEnemy->SetGuardState(EAIState::Alerted);
				HitEnemy->SetTargetActor(this);
				DeactivateCollision(AttackBox);
			}
		}
		else if (OtherActor->IsA<ACheckpoint>())
		{
			ACheckpoint* NewCP = static_cast<ACheckpoint*>(OtherActor);

			if (!NewCP->IsChecked)
			{
				RespawnLocation = NewCP->GetActorLocation();

				NewCP->IsChecked = true;
			}
		}
	}
}

void AMainChar::CombatEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AMainChar::MakeSomeNoise()
{
	MakeNoise(1.0f, this, GetActorLocation());
}

void AMainChar::CPBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->IsA<ACheckpoint>())
		{
			ACheckpoint* NewCP = static_cast<ACheckpoint*>(OtherActor);

			if (!NewCP->IsChecked)
			{
				RespawnLocation = NewCP->GetActorLocation();

				NewCP->IsChecked = true;
			}
		}
	}
}

void AMainChar::CPEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}
