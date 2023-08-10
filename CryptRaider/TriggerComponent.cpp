// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"


UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}
// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
	
    UE_LOG(LogTemp, Display, TEXT("Component alive"));
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    AActor* Actor = GetAcceptableActor();
    if(Mover)
    {
        if(Actor)
        {
            UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
            if(Component !=nullptr){
            Component->SetSimulatePhysics(false);
            Actor->AttachToComponent(this,FAttachmentTransformRules::KeepWorldTransform);
            UE_LOG(LogTemp, Display, TEXT("Unlocking!"));
            Mover->SetShouldMove(true);
            }
        }
        else
        {
            Mover->SetShouldMove(false);
        }
    
    }
}

AActor* UTriggerComponent::GetAcceptableActor() const{
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);

    for(AActor* Actor:Actors )
    {
        bool HasAcceptaableTag = Actor->ActorHasTag(TagToUnlockDoor);
        bool IsGrabbed = Actor->ActorHasTag("Grabbed");
        if(HasAcceptaableTag && !IsGrabbed){
            return Actor;
        }
        
    }
    return nullptr;
}
	void UTriggerComponent::SetMover(UMover* NewMover){
        Mover = NewMover;
    }

