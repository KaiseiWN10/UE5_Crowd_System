// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HSFM_Manager.generated.h"

UCLASS()
class BOIDSPROJECT_API AHSFM_Manager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHSFM_Manager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    
public:
    UPROPERTY(EditAnywhere)
    float RepulsionStrength;
    
    UPROPERTY(EditAnywhere)
    float DistanceFalloff;
    
    UPROPERTY(EditAnywhere)
    float ObstacleRepulsionStrength;
    
    UPROPERTY(EditAnywhere)
    float ObstacleDistanceFalloff;
    
    UPROPERTY(EditAnywhere)
    float ObstacleRadius;
    
    UPROPERTY(EditAnywhere)
    float RelaxationTime;
    
public:
    float GetRepulsionStrength();
    float GetDistanceFalloff();
    float GetObstacleRepulsionStrength();
    float GetObstacleDistanceFalloff();
    float GetObstacleRadius();
    float GetRelaxationTime();
};
