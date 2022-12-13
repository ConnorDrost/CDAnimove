// Fill out your copyright notice in the Description page of Project Settings.


#include "MainChar.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "../ZLockedSpringArmComponent/ZLockedSpringArmComponent.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "../HealthComponent/HealthComponent.h"
#include "../AI/AICharacterGuard.h"
#include "../Checkpoint/Checkpoint.h"
#include "Components/CapsuleComponent.h"
#include "../InventoryComponent/InventoryComponent.h"
#include "../Pickup/Pickup.h"
#include "Components/SphereComponent.h"

AMainChar::AMainChar()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

	SpringArmComponent = CreateDefaultSubobject<UZLockedSpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	SpringArmComponent->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, UZLockedSpringArmComponent::SocketName);
	CameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);
	CameraComponent->SetOrthoWidth(1850.f);

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

	PickupSphere = CreateDefaultSubobject<USphereComponent>("Pickup Sphere");
	PickupSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickupSphere->SetSimulatePhysics(false);
	PickupSphere->SetCollisionProfileName("OverlapAll");
	PickupSphere->SetupAttachment(RootComponent);

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

	SpringArmComponent->SetOriginalZ(GetActorLocation().Z);

	AllFlipbooks.Add("Ninja", NinjaFlipbooks);
	AllFlipbooks.Add("Monkey", MonkeyFlipbooks);

	RespawnLocation = GetActorLocation();

	CurrentMainCharState = EMainCharState::Ninja;
	CurrentFlipbooks = AllFlipbooks["Ninja"];

	AttackBox->OnComponentBeginOverlap.AddDynamic(this, &AMainChar::CombatBeginOverlap);
	AttackBox->OnComponentEndOverlap.AddDynamic(this, &AMainChar::CombatEndOverlap);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMainChar::CPBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMainChar::CPEndOverlap);
}

void AMainChar::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const FVector PlayerVelocity = GetVelocity();
	SpringArmComponent->LockZAxis(GetActorLocation().Z, GetVelocity().Z, DeltaSeconds);

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
	PlayerInputComponent->BindAction("Transform", IE_Pressed, this, &AMainChar::ChangeMainCharState);
	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &AMainChar::PickupObject);

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

	SetCorrectFlipbooks();

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

void AMainChar::SetCorrectFlipbooks()
{
	if (CurrentMainCharState == EMainCharState::Ninja)
	{
		CurrentFlipbooks = AllFlipbooks["Ninja"];
	}
	if (CurrentMainCharState == EMainCharState::Monkey)
	{
		CurrentFlipbooks = AllFlipbooks["Monkey"];
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
					UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.f) ? CurrentFlipbooks.RunAnimation : CurrentFlipbooks.IdleAnimation;

					if (GetSprite()->GetFlipbook() != DesiredAnimation)
					{
						GetSprite()->SetFlipbook(DesiredAnimation);
					}
				}
			}
			else
			{
				GetSprite()->SetFlipbook(CurrentFlipbooks.AttackAnimation);
			}
		}
		else
		{
			if (GetSprite()->GetFlipbook() != CurrentFlipbooks.JumpAnimation)
			{
				GetSprite()->SetFlipbook(CurrentFlipbooks.JumpAnimation);
			}
		}
	}
	else
	{
		GetSprite()->SetFlipbook(CurrentFlipbooks.DeathAnimation);
	}
}

void AMainChar::ChangeMainCharState()
{
	if (CurrentMainCharState == EMainCharState::Ninja)
	{
		if (InventoryComponent->CheckInventoryForMonkey())
		{
			CurrentMainCharState = EMainCharState::Monkey;
		}
	}
	else if (CurrentMainCharState == EMainCharState::Monkey)
	{
		CurrentMainCharState = EMainCharState::Ninja;
	}
	else
	{

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
	SetActorLocation(RespawnLocation);
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
				RespawnLocation = FVector(NewCP->GetActorLocation().X, NewCP->GetActorLocation().Y + 10, NewCP->GetActorLocation().Z);

				NewCP->IsChecked = true;
			}
		}
	}
}

void AMainChar::CPEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AMainChar::PickupObject()
{
	TArray<AActor*> pickups;

	PickupSphere->GetOverlappingActors(pickups, APickup::StaticClass());

	if (pickups.Num() > 0)
	{
		for (int i = 0; i < pickups.Num(); i++)
		{
			InventoryComponent->AddToInventory(Cast<APickup>(pickups[i]));
		}
	}
}

