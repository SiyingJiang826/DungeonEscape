// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"//UMover class must be defined with this line
#include "Math/UnrealMathUtility.h"//to access FMath Class
// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetOwner()->GetActorLocation();
	SetShouldMove(false);
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector CurrentLocation = GetOwner()->GetActorLocation();

	ReachedTarget=CurrentLocation.Equals(TargetLocation);
	if (!ReachedTarget)//ReachedTarget==false
	{
		float Speed = MoveOffset.Length() / MoveTime;
		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
		GetOwner()->SetActorLocation(NewLocation);

		//UE_LOG(LogTemp, Display, TEXT("Mover is moving!"));//
	}
	
}

bool UMover::GetShouldMove()
{
	return ShouldMove;
}

void UMover::SetShouldMove(bool NewShouldMove)
{
	ShouldMove = NewShouldMove;
	if (ShouldMove)
	{
		//Target Location is start location+move offset
		TargetLocation = StartLocation + MoveOffset;
	}
	else
	{
		//Target location is start location
		TargetLocation = StartLocation;
	}
}

