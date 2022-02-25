// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestInterface.h"

class MOBILEPROJECT_API AndroidTestInterface : public ITestInterface
{

#if PLATFORM_ANDROID
public:
	static struct _JNIEnv *ENV;
#endif

public:
	AndroidTestInterface();
	
	virtual void OpenImageFromGallery() override;
};

