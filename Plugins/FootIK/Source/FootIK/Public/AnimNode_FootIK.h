#pragma once

#include "CoreMinimal.h"
#include "Common.h"
#include "AnimNode_FootIK.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct FOOTIK_API FAnimNode_FootIK : public FAnimNode_SkeletalControlBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "SkeletalControl")
	FPelvis Pelvis;

	UPROPERTY(EditAnywhere, Category = "SkeletalControl")
	FFoot LeftFoot;

	UPROPERTY(EditAnywhere, Category = "SkeletalControl")
	FFoot RightFoot;

	UPROPERTY(EditAnywhere, Category = "Calculations", meta = (ClampMin = "0.0", DisplayName = "Foot IK Apply Distance"))
	float ApplyDistance {0.f};

	UPROPERTY(EditAnywhere, Category = "Calculations", meta = (ClampMin = "0.0", ClampMax = "1.0", DisplayName = "Offset Interpolation Alpha"))
	float OffsetAlpha {0.f};

	UPROPERTY(EditAnywhere, Category = "Calculations", meta = (ClampMin = "0.0", ClampMax = "1.0", DisplayName = "Foot Rotation Interpolation Alpha"))
	float RotationAlpha {0.f};

	//~ Begin FAnimNode_Base interface
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	//~ End FAnimNode_Base interface

	//~ Begin FAnimNode_SkeletalControlBase interface
	virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms) override;
	virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) override;
	virtual void InitializeBoneReferences(const FBoneContainer& RequiredBones) override;
	//~ End FAnimNode_SkeletalControlBase interface

private:
	UPROPERTY()
	class AActor *Actor {nullptr};
	UPROPERTY()
	class USkeletalMeshComponent *Mesh {nullptr};
	UPROPERTY()
	class UCapsuleComponent *CapsuleComponent {nullptr};
	UPROPERTY()
	class UCharacterMovementComponent *CharacterMovement {nullptr};

	// Left chain
	FCompactPoseBoneIndex CachedLowerLimbIndex_L {INDEX_NONE};
	FCompactPoseBoneIndex CachedUpperLimbIndex_L {INDEX_NONE};

	// Right chain
	FCompactPoseBoneIndex CachedLowerLimbIndex_R {INDEX_NONE};
	FCompactPoseBoneIndex CachedUpperLimbIndex_R {INDEX_NONE};

	float PelvisOffsetLeft {0.f};
	float PelvisOffsetRight {0.f};

	// Line Trace
	void LineTraceCalculations(FVector_NetQuantizeNormal& HitNormal, float& PelvisOffset,
		const FVector& ActorLocation, const FVector& FootLocation, float CapsuleHalfHeight);

	void PelvisSkeletalControl(
		FComponentSpacePoseContext& Output,
		TArray<FBoneTransform>& OutBoneTransforms,
		const FBoneReference& BoneReference,
		const FBoneContainer& BoneContainer,
		const FVector& Translation);

	void FootSkeletalControl(
		FComponentSpacePoseContext &Output,
		TArray<FBoneTransform>& OutBoneTransforms,
		const FBoneReference& BoneReference,
		const FBoneContainer& BoneContainer,
		const FCompactPoseBoneIndex& CachedUpperLimbIndex,
		const FCompactPoseBoneIndex& CachedLowerLimbIndex,
		const FBoneSocketTarget& EffectorTarget,
		const FVector& EffectorLocation,
		const FBoneSocketTarget& JointTarget,
		const FVector& JointTargetLocation,
		const FRotator& Rotation);
};