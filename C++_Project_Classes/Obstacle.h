// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UCLASS()
class BOIDSPROJECT_API AObstacle : public AActor
{
	GENERATED_BODY()
    
public:
    AObstacle();
	
protected:
    virtual void BeginPlay() override;

    // Function to handle overlap events
    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

private:
    int32 HitCount; // Counter for hits

    // Store unique actors that have overlapped
    UPROPERTY()
    TArray<AActor*> OverlappingActors;
};
