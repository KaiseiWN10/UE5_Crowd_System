// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidsManager.h"

// Sets default values
ABoidsManager::ABoidsManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoidsManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoidsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ABoidsManager::GetMinSpeed()
{
    return MinSpeed;
}

float ABoidsManager::GetMaxSpeed()
{
    return MaxSpeed;
}

float ABoidsManager::GetLocalFlockRadius()
{
    return LocalFlockRadius;
}

float ABoidsManager::GetSeparationRadius()
{
    return SeparationRadius;
}

float ABoidsManager::GetSeparationStrength()
{
    return SeparationStrength;
}

float ABoidsManager::GetAlignmentRadius()
{
    return AlignmentRadius;
}

float ABoidsManager::GetAlignmentStrength()
{
    return AlignmentStrength;
}

float ABoidsManager::GetCohesionRadius()
{
    return CohesionRadius;
}

float ABoidsManager::GetCohesionStrength()
{
    return CohesionStrength;
}
