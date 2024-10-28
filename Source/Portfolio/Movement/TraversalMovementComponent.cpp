// Copyright (c) 2024 Jiten Ukani

#include "TraversalMovementComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetMathLibrary.h"

UTraversalMovementComponent::UTraversalMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTraversalMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	const AActor *Owner = GetOwner();
	if (!Owner)
		return;

	// Find ledge splines
	Owner->GetComponents(USplineComponent::StaticClass(), Ledges);

	// create a map of opposite facing splines
	// TODO: Implement opposite finding algorithm.
	if (Ledges.Num() >= 4)
	{
		OppositeLedges.Reset();
		OppositeLedges.Add(Ledges[0], Ledges[1]);
		OppositeLedges.Add(Ledges[1], Ledges[0]);
		OppositeLedges.Add(Ledges[2], Ledges[3]);
		OppositeLedges.Add(Ledges[3], Ledges[2]);
	}
}

USplineComponent *UTraversalMovementComponent::FindClosestLedge(const FVector &InActorLocation) const
{
	constexpr float OffsetForCorners = 10.f;
	float ClosestDistanceSqrd = 0.f;
	USplineComponent *ClosestLedge = nullptr;

	for (USplineComponent *Ledge : Ledges)
	{
		const FVector Location = Ledge->FindLocationClosestToWorldLocation(InActorLocation, ESplineCoordinateSpace::World);
		const FVector UpVector = Ledge->FindUpVectorClosestToWorldLocation(InActorLocation, ESplineCoordinateSpace::World);
		const FVector OffsetVector = Location + (UpVector * OffsetForCorners);

		const float DistanceSqrd = FVector::DistSquared(InActorLocation, OffsetVector);
		if (DistanceSqrd < ClosestDistanceSqrd || ClosestLedge == nullptr)
		{
			ClosestLedge = Ledge;
			ClosestDistanceSqrd = DistanceSqrd;
		}
	}

	return ClosestLedge;
}

void UTraversalMovementComponent::GetTraversalInformation(const FVector &InHitLocation, const FVector &InActorLocation, class UPrimitiveComponent *HitComponent, FTraversalCheckResult &InOutTraversalCheckResult)
{
	InOutTraversalCheckResult.HitComponent = HitComponent;

	const USplineComponent *ClosestLedge = FindClosestLedge(InActorLocation);

	// Early out if we can't find a valid ledge or it's not wide enough
	if (ClosestLedge == nullptr || ClosestLedge->GetSplineLength() < MinLedgeWidth)
	{
		InOutTraversalCheckResult.bHasFrontLedge = false;
		return;
	}

	const FVector HitLocationOnSpline = ClosestLedge->FindLocationClosestToWorldLocation(InHitLocation, ESplineCoordinateSpace::Local);
	const float DistanceAlongSpline = ClosestLedge->GetDistanceAlongSplineAtLocation(HitLocationOnSpline, ESplineCoordinateSpace::Local);
	const float ClampedDistanceAlongSpline = FMath::Clamp(DistanceAlongSpline, MinLedgeWidth / 2, ClosestLedge->GetSplineLength() - (MinLedgeWidth / 2));
	const FTransform LedgeTransform = ClosestLedge->GetTransformAtDistanceAlongSpline(ClampedDistanceAlongSpline, ESplineCoordinateSpace::World);

	// Update Struct with Front Ledge information
	InOutTraversalCheckResult.bHasFrontLedge = true;
	InOutTraversalCheckResult.FrontLedgeLocation = LedgeTransform.GetLocation();
	InOutTraversalCheckResult.FrontLedgeNormal = UKismetMathLibrary::GetUpVector(LedgeTransform.GetRotation().Rotator());

	// Check for valid Back Ledge
	USplineComponent **pOppositeLedge = OppositeLedges.Find(ClosestLedge);
	if (!pOppositeLedge || !*pOppositeLedge)
	{
		InOutTraversalCheckResult.bHasBackLedge = false;
		return;
	}

	const USplineComponent *OppositeLedge = *pOppositeLedge;
	const FTransform OppositeLedgeTranform = OppositeLedge->FindTransformClosestToWorldLocation(LedgeTransform.GetLocation(), ESplineCoordinateSpace::World);

	// Update Struct with Back Ledge information
	InOutTraversalCheckResult.bHasBackLedge = true;
	InOutTraversalCheckResult.BackLedgeLocation = OppositeLedgeTranform.GetLocation();
	InOutTraversalCheckResult.BackLedgeNormal = UKismetMathLibrary::GetUpVector(OppositeLedgeTranform.GetRotation().Rotator());
}