// Copyright (c) 2024 Jiten Ukani

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TraversalMovementComponent.generated.h"

UENUM(BlueprintType)
enum class ETraversalActionType : uint8
{
	ETAT_None UMETA(DisplayName = "None"),
	ETAT_Hurdle UMETA(DisplayName = "Hurdle", Description = "Traverse over a thin object and end on the ground at a similar level (Low fence)"),
	ETAT_Vault UMETA(DisplayName = "Vault", Description = "Traverse over a thin object and end in a falling state (Tall fence, or elevated obstacle with no floor on the other side)"),
	ETAT_Mantle UMETA(DisplayName = "Mantle", Description = "Traverse up and onto an object without passing over it")
};

USTRUCT(BlueprintType)
struct FTraversalCheckResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Result)
	ETraversalActionType ActionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ledge)
	bool bHasFrontLedge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ledge)
	FVector FrontLedgeLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ledge)
	FVector FrontLedgeNormal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ledge)
	bool bHasBackLedge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ledge)
	FVector BackLedgeLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ledge)
	FVector BackLedgeNormal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Floor)
	bool bHasBackFloor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Floor)
	FVector BackFloorLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Obstacle)
	float ObstacleHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Obstacle)
	float ObstacleDepth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ledge)
	float BackLedgeHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Result)
	TObjectPtr<class UPrimitiveComponent> HitComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ChosenMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	float StartTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	float PlayRate;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PORTFOLIO_API UTraversalMovementComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient)
	TArray<class USplineComponent *> Ledges;

	UPROPERTY(Transient)
	TMap<class USplineComponent *, class USplineComponent *> OppositeLedges;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinLedgeWidth = 60.f;

public:
	// Sets default values for this component's properties
	UTraversalMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual class USplineComponent *FindClosestLedge(const FVector &InActorLocation) const;

public:
	UFUNCTION(BlueprintCallable)
	void GetTraversalInformation(const FVector &InHitLocation, const FVector &InActorLocation, class UPrimitiveComponent *HitComponent, UPARAM(ref) FTraversalCheckResult &InOutTraversalCheckResult);
};
