#pragma once

#include "CoreMinimal.h"
#include "BoneControllers/AnimNode_SkeletalControlBase.h"
#include "Common.generated.h"

USTRUCT()
struct FPelvis
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "SkeletalControl")
	FBoneReference BoneReference;

	float Offset {0.f};
	FVector Translation;
};

USTRUCT()
struct FFoot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "SkeletalControl")
	FBoneSocketTarget EffectorTarget;
	FVector EffectorLocation;

	UPROPERTY(EditAnywhere, Category = "SkeletalControl")
	FName SocketName;

	UPROPERTY(EditAnywhere, Category = "SkeletalControl")
	FBoneSocketTarget JointTarget;

	UPROPERTY(EditAnywhere, Category = "SkeletalControl")
	FVector JointTargetLocation;

	FVector Location;
	float Offset {0.f};
	FRotator Rotation;

	// Line Trace
	bool bHit {false};
	FVector_NetQuantizeNormal HitNormal;

	// Correction
	float HitNormalMax {0.f};
};