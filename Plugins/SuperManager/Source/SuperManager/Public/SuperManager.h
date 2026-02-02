// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FSuperManagerModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

#pragma region AdvanceDeletionTab
	bool DeleteSingleAsset(const FAssetData& AssetDataToDelete);
#pragma endregion 
	
private:

#pragma region ContentMenuExtension
	void InItCBMenuExtension();
	TSharedRef<FExtender> CustomMenuExtender(const TArray<FString>& SelectedPaths);
	void AddCBMenuEntry(FMenuBuilder& MenuBuilder);
	void OnDeleteUnusedAssetsButtonClicked();
	void OnDeleteEmptyFolders();
	void OnAdvancedDeleteButtonClicked();
	
	void FixUpRedirectors();
	void DeleteEmptyFolders(const FString& FolderPathName);
	TArray<FString> FolderPaths;
#pragma endregion
#pragma region CustomEditorTab
	void RegisterAdvancedDeleteTab();
	
	TSharedRef<SDockTab> OnSpawnAdvancedDeletionTab(const FSpawnTabArgs& SpawnTabArgs);
	
	TArray<TSharedPtr<FAssetData>> GetAllAssetDataUnderSelectedFolder();
#pragma endregion
};

