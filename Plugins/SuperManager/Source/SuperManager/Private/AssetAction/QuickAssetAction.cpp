// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetAction/QuickAssetAction.h"
#include "DebugHelper.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetToolsModule.h"

using namespace DebugHelper;

void UQuickAssetAction::DuplicateAsset(int32 DuplicatesNum)
{
	if(DuplicatesNum<=0){
		ShowDialogMessage(EAppMsgType::Ok, TEXT("Invalid Input"), true);
		return;
	}
	
	TArray<FAssetData> SelectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData();
	uint32 Counter= 0;
	for (const FAssetData& SelectedAssetData : SelectedAssetsData){
		for(int32 i =0; i<DuplicatesNum; i++)
		{
			const FString SourceAssetPath = SelectedAssetData.GetObjectPathString();
			const FString NewAssetName = SelectedAssetData.AssetName.ToString() + TEXT("_") + FString::FromInt(i+1);
			const FString NewAssetPathName = FPaths::Combine(SelectedAssetData.PackagePath.ToString(), NewAssetName);

			if (UEditorAssetLibrary::DuplicateAsset(SourceAssetPath, NewAssetPathName))
			{
				UEditorAssetLibrary::SaveAsset(NewAssetPathName, false);
				++Counter;
			}
		}
		if(Counter>0){
			ShowNotifyInfo(TEXT("Successfully Duplicated " + FString::FromInt(Counter) + " files."));
		}
		
		
	}
	
}

void UQuickAssetAction::AddPrefix()
{
	int32 Counter =0;
	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();
	for(UObject* SelectedObject : SelectedObjects)
	{
		if(!SelectedObject) continue;
		FString* FoundPrefix = PrefixMap.Find(SelectedObject->GetClass());
		if(!FoundPrefix || FoundPrefix->IsEmpty())
		{
			ShowDialogMessage(EAppMsgType::Ok, TEXT("No Prefix Found for class: " + SelectedObject->GetClass()->GetName()), false);
			continue;
		}
		FString OldName = SelectedObject->GetName();
		if (OldName.StartsWith(*FoundPrefix)){
			ShowDialogMessage(EAppMsgType::Ok, TEXT("Asset Already has its prefix."), false);
			continue;
		}
		FString NewName = *FoundPrefix + OldName;
		UEditorUtilityLibrary::RenameAsset(SelectedObject, NewName);
		Counter++;
	}
	ShowNotifyInfo(TEXT("Successfully Renamed " + FString::FromInt(Counter) + " assets."));
}

void UQuickAssetAction::DeleteUnusedAsset()
{
	TArray<FAssetData> SelectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData();
	TArray<FAssetData> UnusedAssetsData;
	FixUpRedirectors();
	
	for(const FAssetData& SelectedAssetData : SelectedAssetsData){
		TArray<FString> AssetReference =
		UEditorAssetLibrary::FindPackageReferencersForAsset(SelectedAssetData.GetObjectPathString());
		if(AssetReference.Num()==0){
			UnusedAssetsData.Add(SelectedAssetData);
		}
	}
	if (UnusedAssetsData.Num() == 0)
	{
		ShowDialogMessage(EAppMsgType::Ok, TEXT("No Unused Assets found."), false);
		return;
	}
	const int32 NumOfAssetsDeleted = ObjectTools::DeleteAssets(UnusedAssetsData);
	
	if (NumOfAssetsDeleted > 0) ShowNotifyInfo(TEXT("Successfully deleted " + FString::FromInt(NumOfAssetsDeleted)+ " Assets."));

}
void UQuickAssetAction::RenameBatchAsset(const FString& ReplaceFrom, const FString& ReplaceWith)
{
	int32 Counter =0;
	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();

	for (UObject* SelectedObject : SelectedObjects){
		if(!SelectedObject) continue;
		FString OldName = SelectedObject->GetName();
		if(OldName.Contains(ReplaceFrom)){
			FString NewName = OldName.Replace(*ReplaceFrom, *ReplaceWith, ESearchCase::IgnoreCase);
			UEditorUtilityLibrary::RenameAsset(SelectedObject, NewName);
			Counter++;
		}	
	}
	if (Counter > 0) ShowNotifyInfo(TEXT("Successfully Renamed " + FString::FromInt(Counter)+ " Assets."));


}


void UQuickAssetAction::FixUpRedirectors()
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
