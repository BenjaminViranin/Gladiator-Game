// Fill out your copyright notice in the Description page of Project Settings.

#include "GCharacter.h"
#include "GSword.h"
#include "GShield.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GEnemyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

// Sets default values
AGCharacter::AGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	HealthComponent = CreateDefaultSubobject<UGHealthComponent>(TEXT("HealthComp"));
	Inventory = CreateDefaultSubobject<UGInventory>(TEXT("Inventory"));
	LockSystem = CreateDefaultSubobject<UGEnemyLockSystem>(TEXT("Target System"));
	IsDead = false;
}

USkeletalMeshComponent* AGCharacter::GetPawnMesh() const
{
	return GetMesh();
}

FName AGCharacter::GetWeaponSocketName() const
{
	return WeaponAttachPoint;
}

FName AGCharacter::GetArmorSocketName() const
{
	return ArmorAttachPoint;
}

UGHealthComponent* AGCharacter::GetHealthComponent() const
{
	return HealthComponent;
}

void AGCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SpawnDefaultInventory();
	SpawnDefaultShieldInventory();
}

// Called when the game starts or when spawned
void AGCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (HealthComponent)
		HealthComponent->OnHealthChanged.AddDynamic(this, &AGCharacter::OnHealthChanged);
}

void AGCharacter::MoveForward(float p_value)
{
	if ((Controller != nullptr) && (p_value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, p_value);
	}
}

void AGCharacter::StrafeLeft(float p_value)
{
	if (LockSystem->GetNearestTarget() && Controller != nullptr && p_value != 0.0f)
	{
		FVector OldLocation = this->GetActorLocation();
		FVector RelativeLocation = LockSystem->GetNearestTarget()->GetActorLocation();
		
		float Xnew = cosf(p_value* 0.01f) *(OldLocation.X - RelativeLocation.X) - sinf(p_value* 0.01f) * (OldLocation.Y - RelativeLocation.Y) + RelativeLocation.X;
		float Ynew = sinf(p_value* 0.01f) *(OldLocation.X - RelativeLocation.X) + cosf(p_value* 0.01f) * (OldLocation.Y - RelativeLocation.Y) + RelativeLocation.Y;

		FVector NewLocation(Xnew, Ynew, OldLocation.Z);

		NewLocation -= OldLocation;
		NewLocation.Normalize();
		AddMovementInput(NewLocation, -NewLocation.Size());
	}
}

void AGCharacter::StrafeRight(float p_value)
{
	if (LockSystem->GetNearestTarget() && Controller != nullptr && p_value != 0.0f)
	{
		FVector OldLocation = this->GetActorLocation();
		FVector RelativeLocation = LockSystem->GetNearestTarget()->GetActorLocation();
		float Xnew = cosf(p_value * 0.01f) *(OldLocation.X - RelativeLocation.X) - sinf(p_value* 0.01f) * (OldLocation.Y - RelativeLocation.Y) + RelativeLocation.X;
		float Ynew = sinf(p_value* 0.01f) *(OldLocation.X - RelativeLocation.X) + cosf(p_value* 0.01f) * (OldLocation.Y - RelativeLocation.Y) + RelativeLocation.Y;

		FVector NewLocation(Xnew, Ynew, OldLocation.Z);

		NewLocation -= OldLocation;
		NewLocation.Normalize();
		AddMovementInput(NewLocation, -NewLocation.Size());
	}
}

void AGCharacter::MoveBackward(float p_value)
{
	if ((Controller != nullptr) && (p_value != 0.0f))
	{
		if (LockSystem->GetIsLocked())
			p_value = 0;

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, p_value);
	}
}

void AGCharacter::MoveLeft(float p_value)
{
	if (LockSystem->GetIsLocked())
	{ 
		StrafeLeft(p_value);
	}
	else
	{
		if ((Controller != nullptr) && (p_value != 0.0f))
		{
			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction, p_value);
		}
	}
}

void AGCharacter::TargetNext()
{
	LockSystem->TargetNextEnemy();
}

void AGCharacter::TargetPrev()
{
	LockSystem->TargetPreviousEnemy();
}

void AGCharacter::TargetLock()
{
	if (LockSystem)
	{
		if (LockSystem->GetIsLocked())
		{
			UnlockEnemy();
		}
		else if(!LockSystem->GetIsLocked())
		{
			LockEnemy();
		}
	}
}

void AGCharacter::LockEnemy()
{
	if (LockSystem)
	{
		LockSystem->CalculateClosestTargetDistance();
		if (LockSystem->GetNearestTarget())
		{
			OnTarget();
		}
	}
}

void AGCharacter::UnlockEnemy()
{
	LockSystem->UnLock();
	OffTarget();
}

void AGCharacter::PitchInput(float p_val)
{
	if (p_val != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		if (LockSystem->GetIsLocked())
			p_val = 0;
		APlayerController* const PC = CastChecked<APlayerController>(Controller);
		PC->AddPitchInput(p_val);
	}
}

void AGCharacter::YawInput(float p_val)
{
	if (p_val != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		if (LockSystem->GetIsLocked())
			p_val = 0;
		APlayerController* const PC = CastChecked<APlayerController>(Controller);
		PC->AddYawInput(p_val);
	}
}

void AGCharacter::MoveRight(float p_value)
{
	if (LockSystem->GetIsLocked())
	{
		StrafeRight(p_value);
	}
	else
	{
		if ((Controller != nullptr) && (p_value != 0.0f))
		{
			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction, p_value);
		}
	}
}

void AGCharacter::SetCurrentWeapon(AGSword * p_newWeapon, AGSword * p_lastWeapon)
{
	AGSword* tempWeapon = nullptr;
	if (p_lastWeapon)
	{
		tempWeapon = p_lastWeapon;
	}
	else if (p_newWeapon != CurrentWeapon)
	{
		tempWeapon = CurrentWeapon;
	}
	if (tempWeapon)
	{
		tempWeapon->OnUnEquip();
	}
	CurrentWeapon = p_newWeapon;
	if (p_newWeapon)
	{
		p_newWeapon->SetOwner(this);
		p_newWeapon->OnEquip(p_lastWeapon);
	}
}

void AGCharacter::SetCurrentArmor(AGShield * p_newShield, AGShield * p_lastShield)
{
	AGShield* tempShield = nullptr;
	if (p_lastShield)
	{
		tempShield = p_lastShield;
	}
	else if (p_newShield != CurrentArmor)
	{
		tempShield = CurrentArmor;
	}
	if (tempShield)
	{
		tempShield->OnUnEquip();
	}
	CurrentArmor = p_newShield;
	if (p_newShield)
	{
		p_newShield->SetOwner(this);
		p_newShield->OnEquip(p_lastShield);
	}
}

void AGCharacter::StayDead()
{
	GetMesh()->GlobalAnimRateScale = 0.0f;
}

// Called every frame
void AGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MoveLeft", this, &AGCharacter::MoveLeft);
	PlayerInputComponent->BindAxis("MoveBackward", this, &AGCharacter::MoveBackward);
	PlayerInputComponent->BindAxis("LookUp", this, &AGCharacter::PitchInput);
	PlayerInputComponent->BindAxis("TurnAround", this, &AGCharacter::YawInput);

	InputComponent->BindAction("TargetNextEnemy", IE_Pressed, this, &AGCharacter::TargetNext);
	InputComponent->BindAction("TragetPeviousEnemy", IE_Pressed, this, &AGCharacter::TargetPrev);
	InputComponent->BindAction("StartAttack", IE_Pressed, this, &AGCharacter::WeaponAttack);
	InputComponent->BindAction("LockTarget", IE_Pressed, this, &AGCharacter::TargetLock);
}

void AGCharacter::EquipWeapon(AGSword * p_weapon)
{
	if (p_weapon)
	{
		SetCurrentWeapon(p_weapon, CurrentWeapon);
	}
}

void AGCharacter::EquipShield(AGShield * p_shield)
{
	if (p_shield)
	{
		SetCurrentArmor(p_shield, CurrentArmor);
	}
}

void AGCharacter::WeaponAttack()
{
	if (CurrentWeapon && !CurrentWeapon->GetAttacking())
	{
		CurrentWeapon->StartAttack();
		CurrentWeapon->GetAttackDuration() = PlayAnimMontage(AttackAnim);

		FTimerHandle TimerHandle_StopAttack;
		GetWorldTimerManager().SetTimer(TimerHandle_StopAttack, this, &AGCharacter::EndWeaponAttack, CurrentWeapon->GetAttackDuration() * 0.5f, false);
	}
}

void AGCharacter::Bleed()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, GetActorLocation(), FRotator::ZeroRotator, true);
}

void AGCharacter::OnHealthChanged(UGHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if(OwningHealthComp->GetHealth() <= 0.0f && !IsDead)
	{
		IsDead = true;
		Die();
	}
}

void AGCharacter::EndWeaponAttack()
{
	if (CurrentWeapon && CurrentWeapon->GetAttacking())
	{
		CurrentWeapon->StopAttack();
	}
}

void AGCharacter::SpawnDefaultInventory()
{
	Inventory->SpawnDefaultInventory();
	if (Inventory->GetWeaponInventory().Num() > 0)
	{
		EquipWeapon(Inventory->GetWeaponInventory()[0]);
	}
}

UCameraComponent* AGCharacter::GetCameraComponent()
{
	return CameraComp;
}

USpringArmComponent* AGCharacter::GetCameraBoom()
{
	return SpringArmComp;
}


void AGCharacter::SpawnDefaultShieldInventory()
{
	Inventory->SpawnDefaultShieldInventory();
	if (Inventory->GetArmorInventory().Num() > 0)
	{
		EquipShield(Inventory->GetArmorInventory()[0]);
	}
}

void AGCharacter::Die()
{
	GetWorldTimerManager().ClearAllTimersForObject(this);
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_Pawn, ECR_Ignore);
		GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
	}

	if (Controller != nullptr)
	{
		Controller->UnPossess();
	}
	float DeathAnimDuration = PlayAnimMontage(DeathAnim);
	FTimerHandle TimerHandle_Dead;
	GetWorldTimerManager().SetTimer(TimerHandle_Dead, this, &AGCharacter::StayDead, DeathAnimDuration * 0.5f, false);
}