#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

//
//Scene component atached to player pawn camera
//Is used to grab and move objects from one place to another
//Uses PhysicsHandle to Atach Objects
//
UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//
	//Setting pointer to PhysicsHandle
	//
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	//
	//chceck if PhysicHandle is created and some object is grabbed
	//
	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		//
		//Calculation of the distance of the held object
		//
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		//
		//Set grabbed object location
		//
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation,GetComponentRotation());
	}

}
//
//Function to release Grabbed Object
//
void UGrabber::Release(){

	//
	//Setting pointer to PhysicsHandle
	//
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	//
	//Checking if PhysicHandle and grabed component is not null
	//
	if(PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{	
		//
		//Setting pointer to GrabbedActor
		//
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		//
		//Removing Grabbed tag from grabbed actor. If tag is removed object can stick to object that trigger action when placing grabbed object
		//
		GrabbedActor->Tags.Remove("Grabbed");
		//
		//Waking up rigidbodies if they fell asleep from inactivity
		//
		PhysicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();
		//
		//Release currently held item
		//
		PhysicsHandle->ReleaseComponent();
	}


}
//
//Function to grab objects
//
void UGrabber::Grab(){
	//
	//Pointer to PhysicsHandle
	//
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	//
	//Setting FHitResult variable to use as Outparameter and store information about hit trace
	//
	FHitResult HitResult;
	//
	//Checking if physics handle is not null and object can be grabbed
	//
	if(PhysicsHandle && GetGrabbbleInReach(HitResult)){
		//
		//Setting grabbed component to variable
		//
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		//
		//setting simulated physics on grabbed
		//
		HitComponent->SetSimulatePhysics(true);
		//
		//Waking up RigidBodies if objec was inactive
		//
		HitComponent-> WakeAllRigidBodies();
		//
		//Saving actor to variable
		//
		AActor* HitActor = HitResult.GetActor();
		//
		//Adding tag to grabbed object to prevent snapping object to trigger when held
		//
		HitActor->Tags.Add("Grabbed");
		//
		//Deataching if object is in trigger component
		//
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		//
		//Grabbing object
		//
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
			);
	}
}
//
//Function to find PhysicsHandleComponent
//
UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	//
	//Searching for UPhysicHandleComponent
	//
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	//
	//Checking if Component exists
	//
	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Grabber requires a UPhyssicsHandleComponent"));
	}
	return Result;
}
//
//Function to Get Grabbable object. Returning bool if some object is in reach and FHitResult
//
bool UGrabber::GetGrabbbleInReach(FHitResult& OutHitResult) const
{
	//
	//Vector holding grabber location
	//
	FVector LineStart = GetComponentLocation();
	//
	//Vector holding end of the traceline
	//
	FVector LineEnd = LineStart + GetForwardVector() * MaxGrabDistance;
	//
	//Drawing traceline
	//
	DrawDebugLine(GetWorld(),LineStart,LineEnd,FColor::Red);
	//
	//Drawing collisionSphere on hit
	//
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	return GetWorld()->SweepSingleByChannel(
			OutHitResult,
			LineStart,
			LineEnd,
			FQuat::Identity,
			ECC_GameTraceChannel2,
			Sphere
			);
}