// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"

#if PLATFORM_ANDROID
#include "ThirdParty/pugiXML/src/pugixml.hpp"
#endif

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();

#if PLATFORM_ANDROID
	pugi::xml_document xml_doc{};
	pugi::xml_parse_result xml_result {xml_doc.load_file("test.xml")};

	//auto x = xml_result.description();
#endif
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

