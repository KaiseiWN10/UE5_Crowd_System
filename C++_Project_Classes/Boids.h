// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BoidsManager.h"
#include "HSFM_Manager.h"
#include "Boids.generated.h"

UCLASS()
class BOIDSPROJECT_API ABoids : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoids();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
    FVector Velocity;
    
protected:
    void Steer(float DeltaTime);
    void StayInBoundary();
    
    ABoidsManager* BoidsManager;
    AHSFM_Manager* HSFM_Manager;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USphereComponent* LocalFlockArea;
    
    FVector Separate(TArray<AActor*> LocalFlock);//boids-SEPARATION
    FVector Alignment(TArray<AActor*> LocalFlock);//boids-ALIGNMENT
    FVector Cohesion(TArray<AActor*> LocalFlock);//boids-COHESION
    
    FVector SocialForces(TArray<AActor*> LocalFlock);//Helbing's SFM
    FVector ObstacleForces();//Avoid environment
    
    UPROPERTY(BlueprintReadOnly)
    float BoidSpeed;
    
private:
    FVector PreviousLocation;
    
    UFUNCTION(BlueprintPure)
    float GetBoidSpeed() const { return BoidSpeed; }

};
