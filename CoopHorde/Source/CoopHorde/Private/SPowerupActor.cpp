// Fill out your copyright notice in the Description page of Project Settings.

#include "SPowerupActor.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ASPowerupActor::ASPowerupActor()
{
	PowerupInterval = 0;
	TotalNumberOfTicks = 0;

	bIsPowerupActive = false;

	SetReplicates(true);
}


// Called when the game starts or when spawned
void ASPowerupActor::BeginPlay()
{
	Super::BeginPlay();
	
}


void ASPowerupActor::OnTickPowerup()
{
	TicksProcessed++;

	OnPowerupTicked();

	if (TicksProcessed >= TotalNumberOfTicks)
	{
		OnExpired();

		// Replicate to all clients
		bIsPowerupActive = false;
		OnRep_PowerupActive();

		// Reset timer
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
	}
}


void ASPowerupActor::OnRep_PowerupActive()
{
	OnPowerupStateChanged(bIsPowerupActive);
}


void ASPowerupActor::ActivatePowerup()
{
	OnActivated();

	// Replicate to all clients
	bIsPowerupActive = true;
	OnRep_PowerupActive();

	if (PowerupInterval > 0)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &ASPowerupActor::OnTickPowerup, PowerupInterval, true);
	}
	else
	{
		OnTickPowerup();
	}
}


void ASPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to any relevant client connected to us
	DOREPLIFETIME(ASPowerupActor, bIsPowerupActive);
}
