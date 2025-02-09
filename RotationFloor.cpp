


#include "RotationFloor.h"

// Sets default values
ARotationFloor::ARotationFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RotationSpeed = FRotator(10, 0, 0);


}

// Called when the game starts or when spawned
void ARotationFloor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARotationFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldRotation(RotationSpeed * DeltaTime);

}

