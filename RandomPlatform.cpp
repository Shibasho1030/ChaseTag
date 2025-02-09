

#include "RandomPlatform.h"
#include "CoreMinimal.h"

// Sets default values
ARandomPlatform::ARandomPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MoveSpeed = 200.0f;
	MoveDistance = 500.0f;
	MoveDirection = FVector(1, 0, 0);

}

//i‚ÞŒü‚«‚ð•ÏX‚·‚éŠÖ”
FVector ARandomPlatform::RotateVectorZ(double LocalX, double LocalY)
{
	int32 RandomValue = FMath::Rand() % 3;
	double angle = RandomValue * 60.0 + 120.0;
	double xNew = LocalX * FMath::Cos(FMath::DegreesToRadians(angle)) - LocalY * FMath::Sin(FMath::DegreesToRadians(angle));
	double yNew = LocalX * FMath::Sin(FMath::DegreesToRadians(angle)) + LocalY * FMath::Cos(FMath::DegreesToRadians(angle));

	FVector RotateVector = FVector((float)xNew, (float)yNew, 0.0f);
	return RotateVector;
}

//FVector ARandomPlatform::RotateVector-Z(double LocalX, double LocalY)
//{
//	int32 RandomValue = FMath::Rand() % 2;
//	double angle = RandomValue * 60.0 + 120.0;
//	double xNew = LocalX * FMath::Cos(-FMath::DegreesToRadians(angle)) - LocalY * FMath::Sin(-FMath::DegreesToRadians(angle));
//	double yNew = LocalX * FMath::Sin(-FMath::DegreesToRadians(angle)) + LocalY * FMath::Cos(-FMath::DegreesToRadians(angle));
//
//	FVector RotateVector = FVector((float)xNew, (float)yNew, 0.0f);
//	return RotateVector;
//}



// Called when the game starts or when spawned
void ARandomPlatform::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
}

// Called every frame
void ARandomPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();

	//i‚ÞŒü‚«‚ð•Ï‚¦‚é
	if (FVector::Dist(CurrentLocation, StartLocation) > MoveDistance)
	{
		if (CurrentLocation.X > -3200)
		{
			MoveDirection = FVector(-1, 0, 0);
			StartLocation = CurrentLocation;
		}
		else if (CurrentLocation.X < -4500)
		{
			MoveDirection = FVector(1, 0, 0);
			StartLocation = CurrentLocation;
		}
		else if (CurrentLocation.Y < -2000)
		{
			MoveDirection = FVector(0, 1, 0);
			StartLocation = CurrentLocation;
		}
		else if (CurrentLocation.Y > -500)
		{
			MoveDirection = FVector(0, -1, 0);
			StartLocation = CurrentLocation;
		}
		else
		{
			MoveDirection = RotateVectorZ(MoveDirection.X, MoveDirection.Y);
			StartLocation = CurrentLocation;
		}
	}

	FVector NewLocation = CurrentLocation + (MoveDirection * MoveSpeed * DeltaTime);
	SetActorLocation(NewLocation);




}

