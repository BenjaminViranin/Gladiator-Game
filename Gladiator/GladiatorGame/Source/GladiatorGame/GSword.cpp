// Fill out your copyright notice in the Description page of Project Settings.

#include "GSword.h"
#include "GCharacter.h"
#include "Engine.h"
#include "GEnemyCharacter.h"

// Sets default values
AGSword::AGSword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBoxComponent"));
	CollisionBox->BodyInstance.SetCollisionProfileName("OverlapAll");
	CollisionBox->SetBoxExtent(FVector(5, 25, 70));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	Mesh->CastShadow = true;
	RootComponent = Mesh;
	CollisionBox->SetupAttachment(Mesh);
	Attacking = false;
	SoundPlayer = CreateDefaultSubobject<UGSoundPlayer>(TEXT("AUDIO"));
}

void AGSword::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AGSword::OnSwordWeaponBoxBeginOverlap);
	CollisionBox->OnComponentHit.AddDynamic(this, &AGSword::OnHit);
	SoundPlayer->PrePareSoundArray();
}

void AGSword::SetOwner(ACharacter * p_owner, bool p_isEnemy)
{
	if (Owner != p_owner)
	{
		Owner = p_owner;
		isEnemy = p_isEnemy;
	}
}

void AGSword::AttachMeshToCharacter()
{
	if (Owner && Mesh)
	{
		USkeletalMeshComponent* CharacterMesh = Owner->GetMesh();
		FName AttachPoint = isEnemy ? Cast<AGEnemyCharacter>(Owner)->GetWeaponSocketName() : Cast<AGCharacter>(Owner)->GetWeaponSocketName();
		Mesh->AttachTo(CharacterMesh, AttachPoint);
	}
}

void AGSword::OnEquip(const AGSword * p_lastWeapon)
{
	AttachMeshToCharacter();
}

void AGSword::OnUnEquip()
{
	if (Mesh)
		Mesh->DetachFromParent();
}

void AGSword::StartAttack()
{
	if (Owner->ActorHasTag("Player"))
	{
		CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECollisionResponse::ECR_Overlap);
	}
	else if (Owner->ActorHasTag("Enemy"))
	{
		CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Overlap);
	}
	SetAttacking();
}

void AGSword::StopAttack()
{
	if (Owner->ActorHasTag("Player"))
	{
		CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECollisionResponse::ECR_Block);
	}
	else if (Owner->ActorHasTag("Enemy"))
	{
		CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Block);
	}
	if (Attacking)
	{
		Attacking = false;
	}
	//FTimerHandle TimerHandle_StopAttack;
	//GetWorldTimerManager().SetTimer(TimerHandle_StopAttack, this, &AGSword::SetAttacking, AttackDuration * 0.5f, false);
}

void AGSword::SetAttacking()
{
	Attacking = !Attacking;
}

bool& AGSword::GetAttacking()
{
	return Attacking;
}

float & AGSword::GetAttackDuration()
{
	return AttackDuration;
}

void AGSword::OnSwordWeaponBoxBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp && Attacking)
	{
		if (OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_WorldStatic)
		{
			SoundPlayer->GetSounds()[1]->Play();
			return;
		}
	}
	if (OtherActor && Attacking)
	{
		if (Owner->ActorHasTag("Enemy") && OtherActor->ActorHasTag("Player"))
		{
			FVector HitLocation = SweepResult.ImpactPoint;

			const FVector ForwardDir = Owner->GetActorForwardVector();
			const FVector AddForce = ForwardDir * 200 + FVector(0, 0, 1) * 200;
			AGCharacter* Player = Cast<AGCharacter>(OtherActor);
			Player->Bleed();
			Player->LaunchCharacter(AddForce, false, true);
			Player->GetHealthComponent()->HandleTakeAnyDamage(Player, 1, nullptr, Owner->GetController(), Owner);
			SetAttacking();
		}
		else if (Owner->ActorHasTag("Player") && OtherActor->ActorHasTag("Enemy"))
		{
			FVector HitLocation = SweepResult.ImpactPoint;
			SoundPlayer->GetSounds()[0]->Play();
			const FVector ForwardDir = Owner->GetActorForwardVector();
			const FVector AddForce = ForwardDir * 300 + FVector(0, 0, 1) * 200;
			AGEnemyCharacter* Enemy = Cast<AGEnemyCharacter>(OtherActor);
			Enemy->Bleed();
			Enemy->LaunchCharacter(AddForce, false, true);
			Enemy->GetHealthComponent()->HandleTakeAnyDamage(Enemy, 1, nullptr, Owner->GetController(), Owner);
			SetAttacking();
		}
	}
}

void AGSword::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
}
