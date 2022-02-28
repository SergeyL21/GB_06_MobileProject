// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor.generated.h"

class UTexture;

UCLASS()
class MOBILEPROJECT_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor();

	UFUNCTION(BlueprintCallable)
	void FlipFlopTexture();

protected:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent *StaticMeshComponent;

	UPROPERTY(EditAnywhere)
	UTexture *DefaultStateTexture;

	UPROPERTY(EditAnywhere)
	UTexture *AltStateTexture;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	class UMaterialInstanceDynamic *MaterialInstanceDynamic;

	bool bIsDefaultStateTexture {true};
};
