// Fill out your copyright notice in the Description page of Project Settings.


#include "HSFM_Manager.h"

// Sets default values
AHSFM_Manager::AHSFM_Manager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHSFM_Manager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHSFM_Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AHSFM_Manager::GetRepulsionStrength()
{
    return RepulsionStrength;
}

float AHSFM_Manager::GetDistanceFalloff()
{
    return DistanceFalloff;
}

float AHSFM_Manager::GetObstacleRepulsionStrength()
{
    return ObstacleRepulsionStrength;
}

float AHSFM_Manager::GetObstacleDistanceFalloff()
{
    return ObstacleDistanceFalloff;
}

float AHSFM_Manager::GetObstacleRadius()
{
    return ObstacleRadius;
}

float AHSFM_Manager::GetRelaxationTime()
{
    return RelaxationTime;
}
