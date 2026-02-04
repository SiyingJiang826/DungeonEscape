#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
	if (MoverActor)
	{
		Mover=MoverActor->FindComponentByClass<UMover>();
		if (Mover)
		{
			UE_LOG(LogTemp, Warning, TEXT("Successfully find mover component"));
		
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to find mover component"));
		}
	}
	else
	{
			UE_LOG(LogTemp, Warning, TEXT("Mover Actor nullptr"));
	}
	if (IsPressurePlate)
	{
		OnComponentBeginOverlap.AddDynamic(this,&UTriggerComponent::OnOverlapBegin);
		OnComponentEndOverlap.AddDynamic(this,&UTriggerComponent::OnOverlapEnd);
		UE_LOG(LogTemp, Warning, TEXT("Trigger Component Begin Play"));
	}
	
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTriggerComponent::Trigger(bool NewTriggerValue)
{
	IsTriggered=NewTriggerValue;
	if (Mover)
	{
		Mover->SetShouldMove(IsTriggered);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s doesn't have a mover to trigger"),*GetOwner()->GetActorNameOrLabel());
	}
}

void UTriggerComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("PressurePlateActivator"))
	{
		if (!ActiveActors.Contains(OtherActor))
		{
			ActiveActors.Add(OtherActor);
			UE_LOG(LogTemp, Warning, TEXT("Begin %s  Num=%d"),
				   *GetNameSafe(OtherActor), ActiveActors.Num());

			if (!IsTriggered)
			{
				Trigger(true);
			}
		}
	}

}

void UTriggerComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag("PressurePlateActivator"))
	{
		int32 Removed = ActiveActors.Remove(OtherActor);
		if (Removed > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("End %s  Num=%d"),
				   *GetNameSafe(OtherActor), ActiveActors.Num());

			if (IsTriggered && ActiveActors.Num() == 0)
			{
				Trigger(false);
			}
		}
	}
}
