// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boids.h"
#include "BoidsSpawner.generated.h"

UCLASS()
class BOIDSPROJECT_API ABoidsSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoidsSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    
public:
    UPROPERTY(EditAnywhere)
    int SpawnCount = 30;
    
    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> ActorToSpawn;
    
    UPROPERTY(EditAnywhere)
    int SpawnArea = 2000;
    
    UPROPERTY(EditAnywhere)
    bool EnableCentreSpawn = true;
    
protected:
    FVector GetRandomVector(double Min, double Max);
    
};
