// Copyright Epic Games, Inc. All Rights Reserved.

#include "SuperManager.h"
#include "ContentBrowserModule.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"
#include "DebugHelper.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetToolsModule.h"

#define LOCTEXT_NAMESPACE "FSuperManagerModule"

void FSuperManagerModule::StartupModule()
{
	InItCBMenuExtension();
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}
#pragma region ContentMenuExtension


void FSuperManagerModule::InItCBMenuExtension()
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
	// Add all the extenders in this array
	TArray<FContentBrowserMenuExtender_SelectedPaths>& MenuItems = ContentBrowserModule.GetAllPathViewContextMenuExtenders();

	// Adding our delegate to the list of all existing delegates  
	MenuItems.Add(FContentBrowserMenuExtender_SelectedPaths::CreateRaw(this, &FSuperManagerModule::CustomMenuExtender));
}

// here we define the position of where we want our menu
TSharedRef<FExtender> FSuperManagerModule::CustomMenuExtender(const TArray<FString>& SelectedPaths)
{
	
	TSharedRef<FExtender> MenuExtender(new FExtender);
	if (SelectedPaths.Num() > 0){
		FolderPaths = SelectedPaths;
		MenuExtender->AddMenuExtension(FName("Delete"), // our hook
		EExtensionHook::After, // position relative to hook
		TSharedPtr<FUICommandList>(), // hot keys
		FMenuExtensionDelegate::CreateRaw(this, &FSuperManagerModule::AddCBMenuEntry)); // second delegate where we define name and link actual function
	}
	return MenuExtender;
}

// We define details of our entry and also bind to the actual function 
void FSuperManagerModule::AddCBMenuEntry(FMenuBuilder& MenuBuilder)
{
	// Adding OnDeleteUnusedAssets
	MenuBuilder.AddMenuEntry
	(
		FText::FromString(TEXT("Delete Unused Assets")), // Label/Name
		FText::FromString(TEXT("Safely delete unused assets under the folder.")), // tooltip
		FSlateIcon(), // custom icon
		FExecuteAction::CreateRaw(this, &FSuperManagerModule::OnDeleteUnusedAssetsButtonClicked) // binding to actual function to execute
	);

	// Adding OnDeleteEmptyFolders
	MenuBuilder.AddMenuEntry
	(
		FText::FromString(TEXT("Delete Empty Folders")), // Label/Name
		FText::FromString(TEXT("Safely delete empty folders.")), // tooltip
		FSlateIcon(), // custom icon
		FExecuteAction::CreateRaw(this, &FSuperManagerModule::OnDeleteEmptyFolders) // binding to actual function to execute
	);
}


void FSuperManagerModule::OnDeleteUnusedAssetsButtonClicked()
{
	if(FolderPaths.Num()>1){ 
		DebugHelper::ShowDialogMessage(EAppMsgType::Ok, TEXT("Can only be used on One Folder"), true);
		return;
	}
	TArray<FString> AssetsPaths = UEditorAssetLibrary::ListAssets(FolderPaths[0]);
	if(AssetsPaths.Num()==0) // if empty folder
	{
		DebugHelper::ShowDialogMessage(EAppMsgType::Ok, TEXT("No Assets found under the folder."), true);
		return;
	}
	// dialog to get the confirmation to avoid user accidentally deleting asset
	EAppReturnType::Type ConfirmResult = DebugHelper::ShowDialogMessage(EAppMsgType::YesNo , FString::FromInt(AssetsPaths.Num()) + TEXT(" Assets found.\n Are you sure you want to delete?"), false);
	
	if (ConfirmResult == EAppReturnType::No) return;
	
	FixUpRedirectors();
	
	TArray<FAssetData> UnusedAssetsDataArray;
	for (const FString& AssetPath : AssetsPaths)
	{
		// skipping the Editor folders
		if(AssetPath.Contains(TEXT("Developers")) ||
			AssetPath.Contains(TEXT("Collections")) ||
			AssetPath.Contains(TEXT("__ExternalObjects__")) ||
			AssetPath.Contains(TEXT("__ExternalActors__"))) continue;

		if(!UEditorAssetLibrary::DoesAssetExist(AssetPath)) continue;
		
		TArray<FString> AssetReferencers = UEditorAssetLibrary::FindPackageReferencersForAsset(AssetPath);
		if (AssetReferencers.Num() == 0) // if asset is unused (found no referencers)
		{
			const FAssetData UnusedAssetData = UEditorAssetLibrary::FindAssetData(AssetPath);
			UnusedAssetsDataArray.Add(UnusedAssetData);
		}
	}
	if (UnusedAssetsDataArray.Num() == 0)
    {
    	DebugHelper::ShowDialogMessage(EAppMsgType::Ok, TEXT("No Unused Assets found."), false);
    	return;
    }
    const int32 NumOfAssetsDeleted = ObjectTools::DeleteAssets(UnusedAssetsDataArray);
    
    if (NumOfAssetsDeleted > 0) DebugHelper::ShowNotifyInfo(TEXT("Successfully deleted " + FString::FromInt(NumOfAssetsDeleted)+ " Assets."));
	
}

void FSuperManagerModule::FixUpRedirectors()
{
	TArray<UObjectRedirector*> RedirectorsToFix;
	
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	FARFilter Filter;
	Filter.bRecursivePaths = true;
	Filter.PackagePaths.Emplace("/Game");
	Filter.ClassPaths.Add(FTopLevelAssetPath(TEXT("/Script/Engine"), TEXT("ObjectRedirector")));
	
	TArray<FAssetData> OutRedirectors;
	AssetRegistryModule.Get().GetAssets(Filter, OutRedirectors);
	
	for(const FAssetData& RedirectorData : OutRedirectors){
		if(UObjectRedirector* RedirectorToFix = Cast<UObjectRedirector>(RedirectorData.GetAsset())){
			RedirectorsToFix.Add(RedirectorToFix);
		}
	}
	
	FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
	AssetToolsModule.Get().FixupReferencers(RedirectorsToFix);
}

void FSuperManagerModule::OnDeleteEmptyFolders()
{
	FixUpRedirectors();
	TArray<FString> FolderPathsArray = UEditorAssetLibrary::ListAssets(FolderPaths[0], true, true);
	uint32 counter =0;
	FString EmptyFolderNames;
	TArray<FString> EmptyFoldersPaths;
	
	for(FString& FolderPath : FolderPathsArray)
	{
		// skipping the Editor folders
		if(FolderPath.Contains(TEXT("Developers")) ||
    			FolderPath.Contains(TEXT("Collections")) ||
    			FolderPath.Contains(TEXT("__ExternalObjects__")) ||
    			FolderPath.Contains(TEXT("__ExternalActors__"))) continue;
		
		if(!UEditorAssetLibrary::DoesDirectoryExist(FolderPath)) continue;

		// removing '/' from end to make the DoesDirectoryHaveAssets work
		if (FolderPath.EndsWith("/"))
		{
			FolderPath.RemoveAt(FolderPath.Len() - 1);
		}
		if(!UEditorAssetLibrary::DoesDirectoryHaveAssets(FolderPath))
		{
			EmptyFolderNames.Append(FolderPath);
			EmptyFolderNames.Append("\n");
			EmptyFoldersPaths.Add(FolderPath);
		}
	}
	if(EmptyFoldersPaths.Num() == 0)
	{
		DebugHelper::ShowDialogMessage(EAppMsgType::Ok, TEXT("No Empty Folders found."), false);
        return;
	}
	EAppReturnType::Type ConfirmResult = DebugHelper::ShowDialogMessage(EAppMsgType::YesNo , FString::FromInt(EmptyFoldersPaths.Num()) + TEXT(" Empty Folders found. \n") + EmptyFolderNames + TEXT("\n Are you sure you want to delete?"), false);
	
	if (ConfirmResult == EAppReturnType::No) return;
	
	for(const FString& Path : EmptyFoldersPaths)
	{
		if (UEditorAssetLibrary::DeleteDirectory(Path)) ++counter;
		else DebugHelper::PrintLog(TEXT("Failed to delete empty folder: ") + Path);
	}
	if (counter>0) DebugHelper::ShowDialogMessage(EAppMsgType::Ok, TEXT("Successfully deleted " + FString::FromInt(counter)+ " Folders."), false);
}

#pragma endregion


void FSuperManagerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSuperManagerModule, SuperManager)