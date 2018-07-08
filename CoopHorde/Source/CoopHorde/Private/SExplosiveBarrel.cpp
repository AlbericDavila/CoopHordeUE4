// Fill out your copyright notice in the Description page of Project Settings.

#include "SExplosiveBarrel.h"
#include "SHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealtComp"));
	HealthComp->OnHealthChanged.AddDynamic(this, &ASExplosiveBarrel::OnHealthChanged);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionObjectType(ECC_PhysicsBody);	// Allow for radial component to affect this object
	RootComponent = MeshComp;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialFroceComp"));
	RadialForceComp->SetupAttachment(MeshComp);
	RadialForceComp->Radius = 275;
	RadialForceComp->bImpulseVelChange = true;	// Get consistent application of velocity
	RadialForceComp->bAutoActivate = false;		// Disable ticking in compnent and allow it to rely on FireImpulse()
	RadialForceComp->bIgnoreOwningActor = true;

	ExplosionImpulse = 475;

	SetReplicates(true);
	SetReplicateMovement(true);
}


void ASExplosiveBarrel::OnHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (bExploded)
	{
		return;
	}

	if (Role == ROLE_Authority)
	{
		if (Health <= 0)
		{
			bExploded = true;
			OnRep_Exploded();

			// Boost barrel up into the air
			FVector BoostIntensity = FVector::UpVector * ExplosionImpulse;
			MeshComp->AddImpulse(BoostIntensity, NAME_None, true);

			// Activate radial impulse
			RadialForceComp->FireImpulse();
		}
	}
}


void ASExplosiveBarrel::OnRep_Exploded()
{
	// Play particles and change material
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	MeshComp->SetMaterial(0, ExplodedMaterial);
}


void ASExplosiveBarrel::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to any relevant client connected to us
	DOREPLIFETIME(ASExplosiveBarrel, bExploded);
}