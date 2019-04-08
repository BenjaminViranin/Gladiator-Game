// Fill out your copyright notice in the Description page of Project Settings.

#include "GShield.h"
#include "GCharacter.h"
#include "GEnemyCharacter.h"

// Sets default values
AGShield::AGShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldBoxComponent"));
	CollisionBox->BodyInstance.SetCollisionProfileName("OverlapAll");
	CollisionBox->SetBoxExtent(FVector(5, 25, 70));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShieldMesh"));
	Mesh->CastShadow = true;
	RootComponent = Mesh;
	CollisionBox->SetupAttachment(Mesh);
}

void AGShield::SetOwner(ACharacter * p_owner, bool p_isEnemy)
{
	if (Owner != p_owner)
	{
		Owner = p_owner;
		isEnemy = p_isEnemy;
	}
}

void AGShield::AttachMeshToCharacter()
{
	if (Owner && Mesh)
	{
		USkeletalMeshComponent* CharacterMesh = Owner->GetMesh();
		FName AttachPoint = isEnemy ? Cast<AGEnemyCharacter>(Owner)->GetArmorSocketName() : Cast<AGCharacter>(Owner)->GetArmorSocketName();
		Mesh->AttachTo(CharacterMesh, AttachPoint);
	}
}

void AGShield::OnEquip(const AGShield* p_lastShield)
{
	AttachMeshToCharacter();
}

void AGShield::OnUnEquip()
{
	if (Mesh)
		Mesh->DetachFromParent();
}

void AGShield::OnShieldBoxBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
}