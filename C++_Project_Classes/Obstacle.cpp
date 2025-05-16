// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AObstacle::AObstacle()
{
    PrimaryActorTick.bCanEverTick = false; // No need to tick every frame

        // Root component
        RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

        // Create a collision sphere
        USphereComponent* CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
        CollisionSphere->SetupAttachment(RootComponent);
        CollisionSphere->SetSphereRadius(100.0f); // Adjust if needed
        CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        CollisionSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
        CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

        // Bind overlap event
        CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AObstacle::OnOverlapBegin);

        // Initialize variables
        HitCount = 0;
}

// Called when the game starts
void AObstacle::BeginPlay()
{
    Super::BeginPlay();
}

void AObstacle::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    UE_LOG(LogTemp, Warning, TEXT("Overlap detected!"));  // Debugging log

    if (OtherActor && OtherActor != this)
    {
        if (!OverlappingActors.Contains(OtherActor))
        {
            OverlappingActors.Add(OtherActor);
            HitCount++;
            UE_LOG(LogTemp, Warning, TEXT("HitCount: %d"), HitCount);
        }
    }
}


