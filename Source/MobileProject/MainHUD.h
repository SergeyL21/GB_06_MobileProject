// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

class UTexture2D;

/**
 * 
 */
UCLASS()
class MOBILEPROJECT_API AMainHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMainHUD();
	
	UFUNCTION(BlueprintCallable)
	virtual void OpenImage();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnImageOpened(UTexture2D* SelectedPhoto);

private:
	void OnImageOpened(TArray<uint8> ImageBytes);

	UTexture2D* LoadImageFromBytes(const TArray<uint8>& ImageBytes);
	
	class ITestInterface* TestInterface;
};
