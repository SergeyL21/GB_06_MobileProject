// Fill out your copyright notice in the Description page of Project Settings.

#include "TestActor.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/Texture.h"
#include "Materials/MaterialInstanceDynamic.h"

// --------------------------------------------------------------------------------------------------------------------
// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);
}

// --------------------------------------------------------------------------------------------------------------------
void ATestActor::FlipFlopTexture()
{
	if (!IsValid(MaterialInstanceDynamic))
	{
		return;
	}
	
	if (bIsDefaultStateTexture && IsValid(AltStateTexture))
	{
		MaterialInstanceDynamic->SetTextureParameterValue("Texture", AltStateTexture);
		MaterialInstanceDynamic->SetScalarParameterValue("Scalar", 0.5f);
		bIsDefaultStateTexture = false;
	}
	else if (IsValid(DefaultStateTexture))
	{
		MaterialInstanceDynamic->SetTextureParameterValue("Texture", DefaultStateTexture);
		MaterialInstanceDynamic->SetScalarParameterValue("Scalar", 0.25f);
		bIsDefaultStateTexture = true;
	}
}

// --------------------------------------------------------------------------------------------------------------------
// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();

	auto Material {StaticMeshComponent->GetMaterial(0)};
	if (Material)
	{
		MaterialInstanceDynamic = UMaterialInstanceDynamic::Create(Material, this);
		StaticMeshComponent->SetMaterial(0, MaterialInstanceDynamic);
	}
}

// --------------------------------------------------------------------------------------------------------------------
// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}