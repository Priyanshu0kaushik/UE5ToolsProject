// Fill out your copyright notice in the Description page of Project Settings.


#include "ImGUIActor.h"
#include <imgui.h>

// Sets default values
AImGUIActor::AImGUIActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AImGUIActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AImGUIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

