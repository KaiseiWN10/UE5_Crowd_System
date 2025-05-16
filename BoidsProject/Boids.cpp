// Fill out your copyright notice in the Description page of Project Settings.


#include "Boids.h"

// Sets default values
ABoids::ABoids()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoids::BeginPlay()
{
	Super::BeginPlay();
    
    BoidsManager = Cast<ABoidsManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoidsManager::StaticClass()));
    HSFM_Manager = Cast<AHSFM_Manager>(UGameplayStatics::GetActorOfClass(GetWorld(), AHSFM_Manager::StaticClass()));
    
    LocalFlockArea = NewObject<USphereComponent>(this, USphereComponent::StaticClass(),TEXT("Sphere"));
    
    if(LocalFlockArea)
    {
        LocalFlockArea->SetupAttachment(RootComponent);
        LocalFlockArea->RegisterComponent();
        LocalFlockArea->CreationMethod = EComponentCreationMethod::Instance;
        LocalFlockArea->SetSphereRadius(BoidsManager->GetLocalFlockRadius());
    }
    
    Velocity = GetActorForwardVector();
    Velocity = Velocity.GetSafeNormal();
    Velocity *= FMath::RandRange(BoidsManager->GetMinSpeed(), BoidsManager->GetMaxSpeed());
	
}

// Called every frame
void ABoids::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    FVector CurrentLocation = GetActorLocation();
    
    FVector VelocityVector = (CurrentLocation - PreviousLocation) / DeltaTime;
    BoidSpeed = VelocityVector.Size();
    
    PreviousLocation = CurrentLocation;
    
    Steer(DeltaTime);
    StayInBoundary();
}

void ABoids::Steer(float DeltaTime)//Update rotation and location
{
    FVector Acceleration = FVector::ZeroVector;
    
    //UE_LOG(LogTemp, Warning, TEXT("Flock Radius: %f"), LocalFlockArea->GetScaledSphereRadius());//Debugging
    
    TArray<AActor*> LocalFlock;
    LocalFlockArea->GetOverlappingActors(LocalFlock, ABoids::StaticClass());
    
    Acceleration += Separate(LocalFlock);
//    UE_LOG(LogTemp, Warning, TEXT("Acceleration: X=%f Y=%f Z=%f"), Acceleration.X, Acceleration.Y, Acceleration.Z);//Debugging
    Acceleration += Alignment(LocalFlock);
    Acceleration += Cohesion(LocalFlock);
    Acceleration += SocialForces(LocalFlock);
    Acceleration += 0.1f * ObstacleForces();
    
    FVector DesiredVelocity = Velocity.GetSafeNormal() * BoidsManager->GetMaxSpeed();
    float RelaxationTime = HSFM_Manager->GetRelaxationTime();
    FVector RelaxationForce = (DesiredVelocity - Velocity) / RelaxationTime;//Acts as air resistance
    
    Acceleration += RelaxationForce;
    
    Acceleration = Acceleration.GetClampedToMaxSize(1000);//Prevent instabilities from external forces
    
    if (Acceleration.Size() > 100)//Scaling acceleration smoothly - damping factor - gradually reduces excessive acceleration
    {
        Acceleration = Acceleration.GetSafeNormal() * (100 + (Acceleration.Size() - 100) * 0.1f);
    }
//    
    if (!Acceleration.ContainsNaN())
    {
        UE_LOG(LogTemp, Warning, TEXT("Acceleration: X=%f Y=%f Z=%f"), Acceleration.X, Acceleration.Y, Acceleration.Z);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Acceleration contains NaN!"));
    }

    Acceleration.Z = 0.0f;
    
    Velocity += Acceleration * DeltaTime;
    Velocity = Velocity.GetClampedToSize(BoidsManager->GetMinSpeed(), BoidsManager->GetMaxSpeed());
    
    FVector NewLocation = GetActorLocation() + (Velocity * DeltaTime);
    NewLocation.Z = PreviousLocation.Z;
    SetActorLocation(NewLocation);
    //SetActorLocation(GetActorLocation() + (Velocity * DeltaTime));
    SetActorRotation(Velocity.ToOrientationQuat());
    
}

void ABoids::StayInBoundary()
{
    FVector CurrentLocation = GetActorLocation();
    float Boundary = 1500.0f;
    
    if (CurrentLocation.X < -Boundary)
    {
        CurrentLocation.X = Boundary;
    }
    
    else if(CurrentLocation.X > Boundary)
    {
        CurrentLocation.X = -Boundary;
    }
    
    if (CurrentLocation.Y < -Boundary)
    {
        CurrentLocation.Y = Boundary;
    }
    
    else if(CurrentLocation.Y > Boundary)
    {
        CurrentLocation.Y = -Boundary;
    }
    
    CurrentLocation.Z = PreviousLocation.Z;
    
    SetActorLocation(CurrentLocation);
}

FVector ABoids::Separate(TArray<AActor*> LocalFlock) //Implementing boids-SEPARATION
{
    FVector Steering = FVector::ZeroVector;
    FVector SeparationDirection = FVector::ZeroVector;
    int FlockCount = 0;
    
    for (AActor* OtherBoid : LocalFlock)
    {
        if(OtherBoid != nullptr && OtherBoid != this)
        {
            float DistanceToOtherBoid = FVector::Distance(GetActorLocation(), OtherBoid->GetActorLocation());
            
            if(DistanceToOtherBoid < 400.0f)
            {
                FVector StrongPush = (GetActorLocation(), OtherBoid->GetActorLocation()).GetSafeNormal() * 500.0f;
                StrongPush += StrongPush;
                continue;
            }
            
            if(DistanceToOtherBoid > BoidsManager->GetSeparationRadius())
            {
                continue;
            }
            SeparationDirection = GetActorLocation() - OtherBoid->GetActorLocation();
            float DistanceWeight = FMath::Clamp(BoidsManager->GetSeparationRadius() - DistanceToOtherBoid, 0.0f, BoidsManager->GetSeparationRadius());
            SeparationDirection = SeparationDirection.GetSafeNormal() * (DistanceWeight / BoidsManager->GetSeparationRadius());
            //UE_LOG(LogTemp, Warning, TEXT("SEP Direction: %d"), SeparationDirection.FVector());
            Steering += SeparationDirection;
            
            FlockCount++;
        }
    }
    
    if(FlockCount > 0)
    {
        Steering /= FlockCount;
        Steering = Steering.GetSafeNormal();
        Steering -= Velocity.GetSafeNormal();
        Steering *= BoidsManager->GetSeparationStrength();
        
        return Steering;        
    }
    else
    {
        return FVector::ZeroVector;
    }
}

FVector ABoids::Alignment(TArray<AActor*> LocalFlock)
{
    FVector Steering = FVector::ZeroVector;
    int FlockCount = 0;
    
    for (AActor* OtherActor : LocalFlock)
    {
        if(OtherActor != nullptr && OtherActor != this)
        {
            ABoids* OtherBoid = Cast<ABoids>(OtherActor);
            
            float DistanceToOtherBoid = FVector::Distance(GetActorLocation(), OtherActor->GetActorLocation());
            
            if(DistanceToOtherBoid > BoidsManager->GetAlignmentRadius())
            {
                continue;
            }
            
            Steering += OtherBoid->Velocity.GetSafeNormal();
            FlockCount++;
        }
    }
    
    if(FlockCount > 0)
    {
        Steering /= FlockCount;
        Steering = Steering.GetSafeNormal();
        Steering -= Velocity.GetSafeNormal();
        Steering *= BoidsManager->GetAlignmentStrength();
        
        return Steering;
        
    }
    else
    {
        return FVector::ZeroVector;
    }
}

FVector ABoids::Cohesion(TArray<AActor*> LocalFlock)
{
    FVector Steering = FVector::ZeroVector;
    FVector AveragePosition;
    int FlockCount = 0;
    
    for (AActor* OtherActor : LocalFlock)
    {
        if(OtherActor != nullptr && OtherActor != this)
        {
            
            float DistanceToOtherBoid = FVector::Distance(GetActorLocation(), OtherActor->GetActorLocation());
            
            if(DistanceToOtherBoid > BoidsManager->GetAlignmentRadius())
            {
                continue;
            }
            
            AveragePosition += OtherActor->GetActorLocation();

            FlockCount++;
        }
    }
    
    if(FlockCount > 0)
    {
        AveragePosition /= FlockCount;
        Steering = AveragePosition - GetActorLocation();
        Steering = Steering.GetSafeNormal();
        Steering -= Velocity.GetSafeNormal();
        Steering *= BoidsManager->GetCohesionStrength();
        
        return Steering;
        
    }
    
    else
    {
        return FVector::ZeroVector;
    }
}

FVector ABoids::SocialForces(TArray<AActor*> LocalFlock)
{
    FVector SocialForce = FVector::ZeroVector;
    
    for (AActor* OtherActor : LocalFlock)
    {
        if(OtherActor != nullptr && OtherActor != this)
        {
            float DistanceToOtherBoid = FVector::Distance(GetActorLocation(), OtherActor->GetActorLocation());
            DistanceToOtherBoid = FMath::Max(DistanceToOtherBoid,1.0f);//prevent extreme values for exponential force calc.
            
            if(DistanceToOtherBoid > BoidsManager->GetSeparationRadius())
            {
                continue;
            }
            
            FVector SFDirection = GetActorLocation() - OtherActor->GetActorLocation();
            if (!SFDirection.IsNearlyZero())
            {
                SFDirection = SFDirection.GetSafeNormal();
            }
            else
            {
                SFDirection = FVector::ZeroVector;
            }
            
            float RepulsionForce = HSFM_Manager->GetRepulsionStrength() * FMath::Exp((BoidsManager->GetSeparationRadius() - DistanceToOtherBoid) / HSFM_Manager->GetDistanceFalloff());
            
            if (!FMath::IsFinite(RepulsionForce))//Handle invalid force values
            {
                RepulsionForce = 0;
            }
            
            SocialForce += RepulsionForce * SFDirection;
        }
    }
    return SocialForce;
}

FVector ABoids::ObstacleForces()
{
    FVector ObstacleForce = FVector::ZeroVector;
    
    TArray<FOverlapResult> OverlapResults; //Stores the results of the overlap query
    //Each entry represents an actor detected within the overlap
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    
    GetWorld()->OverlapMultiByChannel(
                                      OverlapResults,
                                      GetActorLocation(),
                                      FQuat::Identity,
                                      ECC_WorldStatic,//Unreal's collision channel for static objects
                                      FCollisionShape::MakeSphere(HSFM_Manager->GetObstacleRadius()),
                                      QueryParams
                                      );
    
    for(const FOverlapResult& Result : OverlapResults)
    {
        AActor* Obstacle = Result.GetActor();
        if(!Obstacle)
        {
            continue;
        }
        
        float DistanceToObstacle = FVector::Distance(GetActorLocation(), Obstacle->GetActorLocation());
        if(DistanceToObstacle > HSFM_Manager->GetObstacleRadius())
        {
            continue;
        }
        FVector ObstacleDirection = (GetActorLocation() - Obstacle->GetActorLocation());
        ObstacleDirection = ObstacleDirection.GetSafeNormal();
        
        float RepulsionForce = HSFM_Manager->GetObstacleRepulsionStrength() * FMath::Exp((HSFM_Manager->GetObstacleRadius() - DistanceToObstacle) / HSFM_Manager->GetObstacleDistanceFalloff());
        
        ObstacleForce += RepulsionForce * ObstacleDirection;
    }
    
    return ObstacleForce;

}

