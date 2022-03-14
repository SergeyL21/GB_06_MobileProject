#include "AnimNode_FootIK.h"

#include "TwoBoneIK.h"
#include "Animation/AnimInstanceProxy.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// --------------------------------------------------------------------------------------
void FAnimNode_FootIK::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	Super::Initialize_AnyThread(Context);

	// Foot
	LeftFoot.EffectorTarget.Initialize(Context.AnimInstanceProxy);
	LeftFoot.JointTarget.Initialize(Context.AnimInstanceProxy);

	RightFoot.EffectorTarget.Initialize(Context.AnimInstanceProxy);
	RightFoot.JointTarget.Initialize(Context.AnimInstanceProxy);

	// References
	Mesh = Context.AnimInstanceProxy->GetSkelMeshComponent();
	Actor = Mesh->GetOwner();

	CapsuleComponent = Actor->FindComponentByClass<UCapsuleComponent>();
	CharacterMovement = Actor->FindComponentByClass<UCharacterMovementComponent>();

	int x = 0;
}

// --------------------------------------------------------------------------------------
void FAnimNode_FootIK::EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output,
	TArray<FBoneTransform>& OutBoneTransforms)
{
	Super::EvaluateSkeletalControl_AnyThread(Output, OutBoneTransforms);

	const FBoneContainer &BoneContainer {Output.Pose.GetPose().GetBoneContainer()};
	const float CapsuleHalfHeight {CapsuleComponent ? CapsuleComponent->GetUnscaledCapsuleHalfHeight() : 0.f};
	TEnumAsByte<enum EMovementMode> MovementMode {CharacterMovement ? CharacterMovement->MovementMode : EMovementMode::MOVE_None};
	FVector ActorLocation {Actor->GetActorLocation()};
	LeftFoot.Location = Mesh->GetSocketLocation(LeftFoot.SocketName);
	RightFoot.Location = Mesh->GetSocketLocation(RightFoot.SocketName);

	if (MovementMode != MOVE_Walking)
	{
		return;
	}
	
	LineTraceCalculations(LeftFoot.HitNormal, PelvisOffsetLeft, ActorLocation, LeftFoot.Location, CapsuleHalfHeight);
	LineTraceCalculations(RightFoot.HitNormal, PelvisOffsetRight, ActorLocation, RightFoot.Location, CapsuleHalfHeight);

	auto PelvisOffsetNew {PelvisOffsetLeft < PelvisOffsetRight ? PelvisOffsetLeft : PelvisOffsetRight};
	Pelvis.Offset += OffsetAlpha * (PelvisOffsetNew - Pelvis.Offset);
	Pelvis.Translation = {0.f, 0.f, Pelvis.Offset};

	if (PelvisOffsetLeft > PelvisOffsetRight)
	{
		LeftFoot.Offset += OffsetAlpha * (-PelvisOffsetRight + PelvisOffsetLeft - LeftFoot.Offset);
		RightFoot.Offset += OffsetAlpha * (0.f - RightFoot.Offset);
	}
	else
	{
		LeftFoot.Offset += OffsetAlpha * (0.f - LeftFoot.Offset);
		RightFoot.Offset += OffsetAlpha * (PelvisOffsetLeft - PelvisOffsetRight - RightFoot.Offset);
	}

	LeftFoot.EffectorLocation = {LeftFoot.Offset, 0.f, 0.f};
	RightFoot.EffectorLocation = {RightFoot.Offset, 0.f, 0.f};

	// Foot rotation
	auto LeftFootPitch {-180.f / PI * FGenericPlatformMath::Atan2(LeftFoot.HitNormal.X, LeftFoot.HitNormal.Z)};
	auto LeftFootYaw {0.f};
	auto LeftFootRoll {180.f / PI * FGenericPlatformMath::Atan2(LeftFoot.HitNormal.Y, LeftFoot.HitNormal.Z)};
	LeftFoot.Rotation += RotationAlpha * (FRotator{LeftFootPitch, LeftFootYaw, LeftFootRoll} - LeftFoot.Rotation);

	auto RightFootPitch {-180.f / PI * FGenericPlatformMath::Atan2(RightFoot.HitNormal.X, RightFoot.HitNormal.Z)};
	auto RightFootYaw {0.f};
	auto RightFootRoll {180.f / PI * FGenericPlatformMath::Atan2(RightFoot.HitNormal.Y, RightFoot.HitNormal.Z)};
	RightFoot.Rotation += RotationAlpha * (FRotator{RightFootPitch, RightFootYaw, RightFootRoll} - RightFoot.Rotation);

	// Pelvis translation
	PelvisSkeletalControl(Output, OutBoneTransforms, Pelvis.BoneReference, BoneContainer, Pelvis.Translation);

	// Foot IK and rotation
	FootSkeletalControl(
		Output,
		OutBoneTransforms,
		LeftFoot.EffectorTarget.BoneReference,
		BoneContainer,
		CachedUpperLimbIndex_L,
		CachedLowerLimbIndex_L,
		LeftFoot.EffectorTarget,
		LeftFoot.EffectorLocation,
		LeftFoot.JointTarget,
		LeftFoot.JointTargetLocation,
		LeftFoot.Rotation);

	FootSkeletalControl(
		Output,
		OutBoneTransforms,
		RightFoot.EffectorTarget.BoneReference,
		BoneContainer,
		CachedUpperLimbIndex_R,
		CachedLowerLimbIndex_R,
		RightFoot.EffectorTarget,
		RightFoot.EffectorLocation,
		RightFoot.JointTarget,
		RightFoot.JointTargetLocation,
		RightFoot.Rotation);
}

// --------------------------------------------------------------------------------------
bool FAnimNode_FootIK::IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones)
{
	// Pelvis
	if (!Pelvis.BoneReference.IsValidToEvaluate(RequiredBones))
	{
		return false;
	}

	// Left foot
	if ((CachedUpperLimbIndex_L == INDEX_NONE) ||
		(CachedLowerLimbIndex_L == INDEX_NONE) ||
		(!LeftFoot.EffectorTarget.IsValidToEvaluate(RequiredBones)) ||
		(!LeftFoot.JointTarget.IsValidToEvaluate(RequiredBones)))
	{
		return false;
	}

	// Right foot
	if ((CachedUpperLimbIndex_R == INDEX_NONE) ||
		(CachedLowerLimbIndex_R == INDEX_NONE) ||
		(!RightFoot.EffectorTarget.IsValidToEvaluate(RequiredBones)) ||
		(!RightFoot.JointTarget.IsValidToEvaluate(RequiredBones)))
	{
		return false;
	}
	
	return true;
}

// --------------------------------------------------------------------------------------
void FAnimNode_FootIK::InitializeBoneReferences(const FBoneContainer& RequiredBones)
{
	Super::InitializeBoneReferences(RequiredBones);

	// Pelvis
	Pelvis.BoneReference.Initialize(RequiredBones);

	// Left foot
	LeftFoot.EffectorTarget.InitializeBoneReferences(RequiredBones);
	LeftFoot.JointTarget.InitializeBoneReferences(RequiredBones);

	// Right foot
	RightFoot.EffectorTarget.InitializeBoneReferences(RequiredBones);
	RightFoot.JointTarget.InitializeBoneReferences(RequiredBones);

	// Get bone index of the Effector
	auto IKBoneCompactPoseIndex_L {LeftFoot.EffectorTarget.BoneReference.GetCompactPoseIndex(RequiredBones)};
	if (IKBoneCompactPoseIndex_L != INDEX_NONE)
	{
		// Cache bone index up from Effector
		CachedLowerLimbIndex_L = RequiredBones.GetParentBoneIndex(IKBoneCompactPoseIndex_L);
		if (CachedLowerLimbIndex_L != INDEX_NONE)
		{
			// Cache bone index up from pelvis
			CachedUpperLimbIndex_L = RequiredBones.GetParentBoneIndex(CachedLowerLimbIndex_L);
		}
	}

	auto IKBoneCompactPoseIndex_R {RightFoot.EffectorTarget.BoneReference.GetCompactPoseIndex(RequiredBones)};
	if (IKBoneCompactPoseIndex_R != INDEX_NONE)
	{
		// Cache bone index up from Effector
		CachedLowerLimbIndex_R = RequiredBones.GetParentBoneIndex(IKBoneCompactPoseIndex_R);
		if (CachedLowerLimbIndex_R != INDEX_NONE)
		{
			// Cache bone index up from pelvis
			CachedUpperLimbIndex_R = RequiredBones.GetParentBoneIndex(CachedLowerLimbIndex_R);
		}
	}
}

// --------------------------------------------------------------------------------------
void FAnimNode_FootIK::LineTraceCalculations(FVector_NetQuantizeNormal& HitNormal, float& PelvisOffset,
	const FVector& ActorLocation, const FVector& FootLocation, float CapsuleHalfHeight)
{
	FHitResult HitResult;
	//FCollisionQueryParams TraceParams;

	auto Start {FVector{FootLocation.X, FootLocation.Y, ActorLocation.Z}};
	auto End {FVector{FootLocation.X, FootLocation.Y, (ActorLocation.Z - CapsuleHalfHeight - ApplyDistance)}};

	UKismetSystemLibrary::LineTraceSingle(Mesh, Start, End, TraceTypeQuery1, false,
		{Actor}, EDrawDebugTrace::None, HitResult, true);

	HitNormal = HitResult.Normal;

	// Pelvis offset
	if (HitResult.bBlockingHit)
	{
		PelvisOffset = CapsuleHalfHeight - HitResult.Distance;
	}
	else
	{
		PelvisOffset = 0.f;
	}
}

// --------------------------------------------------------------------------------------
void FAnimNode_FootIK::PelvisSkeletalControl(FComponentSpacePoseContext& Output,
	TArray<FBoneTransform>& OutBoneTransforms, const FBoneReference& BoneReference, const FBoneContainer& BoneContainer,
	const FVector& Translation)
{
	const auto CompactPoseBoneReference {BoneReference.GetCompactPoseIndex(BoneContainer)};
	auto NewBoneTM {Output.Pose.GetComponentSpaceTransform(CompactPoseBoneReference)};
	const auto ComponentTransform {Output.AnimInstanceProxy->GetComponentTransform()};

	FAnimationRuntime::ConvertCSTransformToBoneSpace(
		ComponentTransform,
		Output.Pose,
		NewBoneTM,
		CompactPoseBoneReference,
		BCS_ComponentSpace);

	NewBoneTM.AddToTranslation(Translation);

	FAnimationRuntime::ConvertBoneSpaceTransformToCS(
		ComponentTransform,
		Output.Pose,
		NewBoneTM,
		CompactPoseBoneReference,
		BCS_ComponentSpace);

	OutBoneTransforms.Add(FBoneTransform{BoneReference.GetCompactPoseIndex(BoneContainer), NewBoneTM});
}

// --------------------------------------------------------------------------------------
void FAnimNode_FootIK::FootSkeletalControl(FComponentSpacePoseContext& Output,
	TArray<FBoneTransform>& OutBoneTransforms, const FBoneReference& BoneReference, const FBoneContainer& BoneContainer,
	const FCompactPoseBoneIndex& CachedUpperLimbIndex, const FCompactPoseBoneIndex& CachedLowerLimbIndex,
	const FBoneSocketTarget& EffectorTarget, const FVector& EffectorLocation, const FBoneSocketTarget& JointTarget,
	const FVector& JointTargetLocation, const FRotator& Rotation)
{
	auto BoneReferenceCompactPoseIndex {BoneReference.GetCompactPoseIndex(BoneContainer)};

	// get component space transforms for our bones
	auto UpperLimbCSTransform {Output.Pose.GetComponentSpaceTransform(CachedUpperLimbIndex)};
	auto LowerLimbCSTransform {Output.Pose.GetComponentSpaceTransform(CachedLowerLimbIndex)};
	auto EndBoneCSTransform {Output.Pose.GetComponentSpaceTransform(BoneReferenceCompactPoseIndex)};

	auto DesiredPos {EffectorTarget.GetTargetTransform(
		EffectorLocation, Output.Pose, Output.AnimInstanceProxy->GetComponentTransform()).GetTranslation() + Pelvis.Translation};
	auto JointTargetPos {JointTarget.GetTargetTransform(
		JointTargetLocation, Output.Pose, Output.AnimInstanceProxy->GetComponentTransform()).GetTranslation()};

	UpperLimbCSTransform.SetLocation(UpperLimbCSTransform.GetTranslation() + Pelvis.Translation);
	LowerLimbCSTransform.SetLocation(LowerLimbCSTransform.GetTranslation() + Pelvis.Translation);
	EndBoneCSTransform.SetLocation(EndBoneCSTransform.GetTranslation() + Pelvis.Translation);

	AnimationCore::SolveTwoBoneIK(
		UpperLimbCSTransform,
		LowerLimbCSTransform,
		EndBoneCSTransform,
		JointTargetPos,
		DesiredPos,
		false,
		1.f,
		1.2f);

	// Rotation
	const auto ComponentTransform {Output.AnimInstanceProxy->GetComponentTransform()};
	FAnimationRuntime::ConvertCSTransformToBoneSpace(
		ComponentTransform,
		Output.Pose,
		EndBoneCSTransform,
		BoneReferenceCompactPoseIndex,
		BCS_WorldSpace);

	const FQuat BoneQuat {Rotation};
	EndBoneCSTransform.SetRotation(BoneQuat * EndBoneCSTransform.GetRotation());
	FAnimationRuntime::ConvertBoneSpaceTransformToCS(
		ComponentTransform,
		Output.Pose,
		EndBoneCSTransform,
		BoneReferenceCompactPoseIndex,
		BCS_WorldSpace);

	// Update transforms. Order is very important!
	OutBoneTransforms.Add(FBoneTransform{CachedUpperLimbIndex, UpperLimbCSTransform});
	OutBoneTransforms.Add(FBoneTransform{CachedLowerLimbIndex, LowerLimbCSTransform});
	OutBoneTransforms.Add(FBoneTransform{BoneReferenceCompactPoseIndex, EndBoneCSTransform});
}


