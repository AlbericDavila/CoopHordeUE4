// Fill out your copyright notice in the Description page of Project Settings.

#include "SGranadeLauncher.h"




void ASGranadeLauncher::Fire()
{
	// Trace world from pawn eyes to crosshair location

	AActor* MyOwner = GetOwner();
	if (MyOwner && ProjectileObject)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<AActor>(ProjectileObject, MuzzleLocation, EyeRotation, SpawnParams);
	}

	// spawn projectile at muzzle

	// give it a speed and angle
}
