// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TestInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class MOBILEPROJECT_API UTestInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOnImageSelected, TArray<uint8>);

class ITestInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	FOnImageSelected OnImageSelected;
	
	virtual void OpenImageFromGallery() = 0;
};
