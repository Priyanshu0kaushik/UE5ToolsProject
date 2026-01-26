// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Particles/ParticleSystem.h"	
#include "Sound/SoundCue.h"
#include "Sound/SoundWave.h"
#include "Engine/Texture.h"
#include "Blueprint/UserWidget.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/Texture2D.h"
#include "Engine/Blueprint.h"
#include "QuickAssetAction.generated.h"

/**
 * 
 */
UCLASS()
class SUPERMANAGER_API UQuickAssetAction : public UAssetActionUtility
{
	GENERATED_BODY()
public:
	UFUNCTION(CallInEditor)
	void DuplicateAsset(int32 DuplicatesNum);

	UFUNCTION(CallInEditor)
	void AddPrefix();
	
	UFUNCTION(CallInEditor)
	void DeleteUnusedAsset();
	
	UFUNCTION(CallInEditor)
	void RenameBatchAsset(const FString& ReplaceFrom, const FString& ReplaceWith);
protected:
	TMap<UClass*,FString> PrefixMap{
			{UBlueprint::StaticClass(),TEXT("BP_")},
			{UStaticMesh::StaticClass(),TEXT("SM_")},
			{UMaterial::StaticClass(), TEXT("M_")},
			{UMaterialInstanceConstant::StaticClass(),TEXT("MI_")},
			{UMaterialFunctionInterface::StaticClass(), TEXT("MF_")},
			{UParticleSystem::StaticClass(), TEXT("PS_")},
			{USoundCue::StaticClass(), TEXT("SC_")},
			{USoundWave::StaticClass(), TEXT("SW_")},
			{UTexture::StaticClass(), TEXT("T_")},
			{UTexture2D::StaticClass(), TEXT("T_")},
			{UUserWidget::StaticClass(), TEXT("WBP_")},
			{USkeletalMeshComponent::StaticClass(), TEXT("SK_")}
		
	};
	void FixUpRedirectors();
};
