// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UGrabber : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//
	//Function called from BP, when leftMousebutton is released object is released
	//
	UFUNCTION(BlueprintCallable)
	void Release();
	//
	//Function called from BP, when leftMousebutton is presed object is grabed
	//
	UFUNCTION(BlueprintCallable)
	void Grab();
private:

	UPhysicsHandleComponent* GetPhysicsHandle() const;
	bool GetGrabbbleInReach(FHitResult& OutHitResult) const;
		
	//
	//From how far object can be grabbed
	//	
	UPROPERTY(EditAnywhere)
	int MaxGrabDistance = 100;
	//
	//Radius of collison sphere
	//
	UPROPERTY(EditAnywhere)
	float GrabRadius = 100;
	//
	//Distance Between player and held object
	//
	UPROPERTY(EditAnywhere)
	float HoldDistance = 200;



};
