// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidgets/AdvanceDeletionWidget.h"
#include "SlateBasics.h"
#include "ClassIconFinder.h"
#include "SuperManager.h"
#include "DebugHelper.h"

#define LIST_ALL TEXT("List All Available Assets")
#define LIST_UNUSED TEXT("List All Unused Assets")
#define LIST_SAME_NAME TEXT("List Duplicate Name Assets")


void SAdvanceDeletionTab::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;
	AvailableAssetDataArray = InArgs._AssetsDataArray;
	CurrentSelectedFolderPath = InArgs._CurrentSelectedFolder;
	
	DisplayedAssetDataArray = AvailableAssetDataArray;
	
	CheckboxesArray.Empty();
	AssetsToDeleteArray.Empty();
	ComboBoxOptionsArray.Empty();
	
	ComboBoxOptionsArray.Add(MakeShared<FString>(LIST_ALL));
	ComboBoxOptionsArray.Add(MakeShared<FString>(LIST_UNUSED));
	ComboBoxOptionsArray.Add(MakeShared<FString>(LIST_SAME_NAME));
	
	FSlateFontInfo TitleTextFont = GetFontInfo();
	TitleTextFont.Size = 30;

	ChildSlot
	[
		// Main widget
		SNew(SVerticalBox)

		// first slot for title text
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBorder)
			.Padding(FMargin(3.f))
			.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
			[
				SNew(STextBlock)
				.Text(FText::FromString("Advanced Deletion Tool"))
				.Font(FAppStyle::GetFontStyle("HeadingExtraSmall")) 
				.Justification(ETextJustify::Center)
				.ColorAndOpacity(FColor::White)
			]
		]
		
		// second slot for the condition options
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			.HAlign(HAlign_Left)
			[
				ConstructComboBox()
			]
			
			+SHorizontalBox::Slot()
			.FillWidth(1.f)
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Bottom)
			[
				ConstructTextForRowWidget("Current Folder: " +CurrentSelectedFolderPath, GetFontInfo())

			]
		]
		
		// third slot for the asset list
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Fill)
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
			[
				ConstructAssetList()
			]
			
		]

		// fourth slot for the buttons
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.FillWidth(10.f)
			.Padding(5.f)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				ConstructSelectAllButton()
			]
			+SHorizontalBox::Slot()
			.FillWidth(10.f)
            .Padding(5.f)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				ConstructDeselectAllButton()
			]
			+SHorizontalBox::Slot()
			.FillWidth(10.f)
            .Padding(5.f)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
            [
            	ConstructDeleteAllButton()
	            
            ]
		]
		
	];
}

TSharedRef<ITableRow> SAdvanceDeletionTab::OnGenerateRowForList(TSharedPtr<FAssetData> AssetDataToDisplay, const TSharedRef<STableViewBase>& OwnerTable)
{
	if(!AssetDataToDisplay.IsValid()) return SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable);
	const FSlateBrush* ClassIcon = FClassIconFinder::FindThumbnailForClass(AssetDataToDisplay->GetClass());
	if (!ClassIcon)
	{
		// if icon not found
		ClassIcon = FAppStyle::GetBrush("ClassIcon.Object");
	}
	const FString AssetName = AssetDataToDisplay->AssetName.ToString();

	FSlateFontInfo AssetNameFont =  GetFontInfo();
    AssetNameFont.Size = 10;
	
	TSharedRef<STableRow <TSharedPtr <FAssetData>>> AssetRowWidget =
	SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable)
	[
		SNew(SHorizontalBox)

		// 1. Checkbox
		+SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.AutoWidth()
		.Padding(FMargin(5.f)) 
		[
			ConstructCheckbox(AssetDataToDisplay)
		]

		// 2. Class Icon
		+SHorizontalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.AutoWidth()
		.Padding(FMargin(5.f))
		[
			SNew(SImage)
			.Image(ClassIcon)
		]

		// 3. Asset Name
		+SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.FillWidth(1.f)
		.Padding(FMargin(5.f))
		[
			ConstructTextForRowWidget(AssetName, AssetNameFont)
		]

		// 4. Sync Button
		+SHorizontalBox::Slot()
	   .HAlign(HAlign_Right)
	   .VAlign(VAlign_Center)
	   .AutoWidth()
	   .Padding(FMargin(5.f))
		[
			ConstructSyncToContentButton(AssetDataToDisplay)
		]

		// 5. Delete Button
		+SHorizontalBox::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Center)
		.AutoWidth()
		.Padding(FMargin(5.f))
		[
			ConstructDeleteButtonForRowWidget(AssetDataToDisplay)
		]
	];

	return AssetRowWidget;
}

TSharedRef<SListView<TSharedPtr<FAssetData>>> SAdvanceDeletionTab::ConstructAssetList()
{
	ConstructedAssetListView = SNew(SListView<TSharedPtr<FAssetData>>)
								.ListItemsSource(&DisplayedAssetDataArray)
								.OnGenerateRow(this, &SAdvanceDeletionTab::OnGenerateRowForList);
	return ConstructedAssetListView.ToSharedRef();
}

TSharedRef<SCheckBox> SAdvanceDeletionTab::ConstructCheckbox(const TSharedPtr<FAssetData>& AssetDataToDisplay)
{
	TSharedRef<SCheckBox> checkBox = SNew(SCheckBox)
		.Type(ESlateCheckBoxType::CheckBox)
		.OnCheckStateChanged(this, &SAdvanceDeletionTab::OnCheckBoxStateChanged, AssetDataToDisplay)
		.Visibility(EVisibility::Visible);
	CheckboxesArray.Add(checkBox);
	return checkBox;
}

TSharedRef<STextBlock> SAdvanceDeletionTab::ConstructTextForRowWidget(const FString& TextContent, const FSlateFontInfo& FontInfo)
{
	TSharedRef<STextBlock> ConstructedTextBlock = SNew(STextBlock)
	.Text(FText::FromString(TextContent))
	.Font(FontInfo)
	.ColorAndOpacity(FColor::White);

	return ConstructedTextBlock;
}

TSharedRef<SButton> SAdvanceDeletionTab::ConstructSyncToContentButton(TSharedPtr<FAssetData>& AssetData)
{
	TSharedRef<SButton> ConstructedButton = SNew(SButton)
		  .ToolTipText(FText::FromString("Find in Content Browser"))
		  .ButtonStyle(FAppStyle::Get(), "SimpleButton")
		  .ContentPadding(0)
		  .OnClicked(this, &SAdvanceDeletionTab::OnSyncToContentButtonClicked, AssetData)
		[
			SNew(SImage)
			  .Image(FAppStyle::Get().GetBrush("SystemWideCommands.FindInContentBrowser"))
			  .ColorAndOpacity(FSlateColor::UseForeground())
		];
	return ConstructedButton;
}

TSharedRef<SButton> SAdvanceDeletionTab::ConstructDeleteButtonForRowWidget(const TSharedPtr<FAssetData>& AssetDataToDisplay)
{
	TSharedRef<SButton> ConstructedButton =
		SNew(SButton)
		.ContentPadding(2.0f)
		.ButtonStyle(FAppStyle::Get(), "SimpleButton")
		.ToolTipText(FText::FromString(TEXT("Delete this specific asset permanently")))
		.OnClicked(this, &SAdvanceDeletionTab::OnDeleteClicked, AssetDataToDisplay)
		[
			SNew(SImage)
			.Image(FAppStyle::Get().GetBrush("Icons.Delete"))
			.ColorAndOpacity(FSlateColor::UseForeground())
		];
		
	return ConstructedButton;
}

TSharedRef<SButton> SAdvanceDeletionTab::ConstructSelectAllButton()
{
	TSharedRef<SButton> ConstructedButton =
		SNew(SButton)
		.ContentPadding(2.0f)
		.ButtonStyle(FAppStyle::Get(), "SimpleButton")
		.ToolTipText(FText::FromString(TEXT("Select All Assets")))
		.OnClicked(this, &SAdvanceDeletionTab::OnSelectAllClicked)
		[
			ConstructTextForRowWidget("Select All", GetFontInfo())
		];
		
	return ConstructedButton;
}

TSharedRef<SButton> SAdvanceDeletionTab::ConstructDeselectAllButton()
{
	TSharedRef<SButton> ConstructedButton =
		SNew(SButton)
		.ContentPadding(2.0f)
		.ButtonStyle(FAppStyle::Get(), "SimpleButton")
		.ToolTipText(FText::FromString(TEXT("Deselect All Assets")))
		.OnClicked(this, &SAdvanceDeletionTab::OnDeselectAllClicked)
		[
			ConstructTextForRowWidget("Deselect All", GetFontInfo())
		];
		
	return ConstructedButton;
}

TSharedRef<SButton> SAdvanceDeletionTab::ConstructDeleteAllButton()
{
	TSharedRef<SButton> ConstructedButton =
    	SNew(SButton)
    	.ContentPadding(2.0f)
    	.ButtonStyle(FAppStyle::Get(), "PrimaryButton")
    	.ToolTipText(FText::FromString(TEXT("Delete All Assets")))
    	.OnClicked(this, &SAdvanceDeletionTab::OnDeleteAllClicked)
    	[
    		ConstructTextForRowWidget("Delete All", GetFontInfo())
    	];
    	
    return ConstructedButton;
}

#pragma region ComboBox

TSharedRef<SComboBox<TSharedPtr<FString>>> SAdvanceDeletionTab::ConstructComboBox()
{
	TSharedRef<SComboBox<TSharedPtr<FString>>> ConstructedCombobox=
		SNew(SComboBox<TSharedPtr<FString>>)
		.OptionsSource(&ComboBoxOptionsArray)
		.OnGenerateWidget_Lambda([this](TSharedPtr<FString> Item)
		{
			return ConstructTextForRowWidget(*Item, GetFontInfo());
		})
		.OnSelectionChanged(this, &SAdvanceDeletionTab::OnSelectionChanged)
		[
			SAssignNew(ComboBoxDisplayText, STextBlock)
				.Text(FText::FromString("Select Option"))
		];
	return ConstructedCombobox;
}

void SAdvanceDeletionTab::OnSelectionChanged(TSharedPtr<FString> SelectedOption, ESelectInfo::Type SelectInfo)
{
	ComboBoxDisplayText->SetText(FText::FromString(*SelectedOption));

	FSuperManagerModule& SuperManagerModule = FModuleManager::LoadModuleChecked<FSuperManagerModule>(TEXT("SuperManager"));
	
	if(*SelectedOption == LIST_ALL)
	{
		DisplayedAssetDataArray = AvailableAssetDataArray;
	}
	else if(*SelectedOption == LIST_UNUSED)
	{
		SuperManagerModule.ListAllUnusedAsset(AvailableAssetDataArray, DisplayedAssetDataArray);
	}
	else if (*SelectedOption == LIST_SAME_NAME)
	{
		SuperManagerModule.ListALlDuplicateNameAsset(AvailableAssetDataArray, DisplayedAssetDataArray);
	}
	else{}

	RefreshAssetList();

}

#pragma endregion

FReply SAdvanceDeletionTab::OnDeleteClicked(TSharedPtr<FAssetData> ClickedAssetData)
{
	FSuperManagerModule& SuperManagerModule = FModuleManager::LoadModuleChecked<FSuperManagerModule>(TEXT("SuperManager"));
	const bool bAssetDeleted =  SuperManagerModule.DeleteSingleAsset(*ClickedAssetData.Get());
	if (bAssetDeleted)
	{
		//Refresh
		if (AvailableAssetDataArray.Contains(ClickedAssetData))
			AvailableAssetDataArray.Remove(ClickedAssetData);
		
		if (DisplayedAssetDataArray.Contains(ClickedAssetData)) DisplayedAssetDataArray.Remove(ClickedAssetData);

		RefreshAssetList();
	}
	return FReply::Handled();
}

FReply SAdvanceDeletionTab::OnDeleteAllClicked()
{
	if (AssetsToDeleteArray.Num() == 0)
	{
		DebugHelper::ShowDialogMessage(EAppMsgType::Ok, "No Assets Selected.", false);
		return FReply::Handled();
	}
	FSuperManagerModule& SuperManagerModule = FModuleManager::LoadModuleChecked<FSuperManagerModule>(TEXT("SuperManager"));

	TArray<FAssetData> AssetsToDelete;
	for (const TSharedPtr<FAssetData>& DataToDelete : AssetsToDeleteArray)
	{
		AssetsToDelete.Add(*DataToDelete.Get());
	}
	const bool bAssetsDeleted =  SuperManagerModule.DeleteAssets(AssetsToDelete);
	if (bAssetsDeleted)
	{
		for (const TSharedPtr<FAssetData>& DataToDelete : AssetsToDeleteArray)
		{
			if (AvailableAssetDataArray.Contains(DataToDelete)) AvailableAssetDataArray.Remove(DataToDelete);
			if (DisplayedAssetDataArray.Contains(DataToDelete)) DisplayedAssetDataArray.Remove(DataToDelete);
		}
		RefreshAssetList();

	}
	return FReply::Handled();
}

FReply SAdvanceDeletionTab::OnSelectAllClicked()
{
	if(CheckboxesArray.Num() == 0) return FReply::Handled();

	for (const TSharedPtr<SCheckBox>& CheckBox : CheckboxesArray)
	{
		if (!CheckBox->IsChecked()) CheckBox->ToggleCheckedState();
	}
	return FReply::Handled();
}

FReply SAdvanceDeletionTab::OnDeselectAllClicked()
{
	if(CheckboxesArray.Num() == 0) return FReply::Handled();

	for (const TSharedPtr<SCheckBox>& CheckBox : CheckboxesArray)
	{
		if (CheckBox->IsChecked()) CheckBox->ToggleCheckedState();
	}
	return FReply::Handled();
}

void SAdvanceDeletionTab::RefreshAssetList()
{
	AssetsToDeleteArray.Empty();
	CheckboxesArray.Empty();

	if (ConstructedAssetListView.IsValid()) ConstructedAssetListView->RebuildList();
}

void SAdvanceDeletionTab::OnCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData)
{
	switch(NewState)
	{
	case ECheckBoxState::Checked:
		AssetsToDeleteArray.AddUnique(AssetData);
		break;

	case ECheckBoxState::Unchecked:
		if (AssetsToDeleteArray.Contains(AssetData)) AssetsToDeleteArray.Remove(AssetData);
		break;

	case ECheckBoxState::Undetermined:
		break;
		
	default:
		break;
	}
	DebugHelper::PrintOnScreen ("Working.....");
}

FReply SAdvanceDeletionTab::OnSyncToContentButtonClicked(TSharedPtr<FAssetData> ClickedAssetData)
{
	FSuperManagerModule& SuperMangerModule = FModuleManager::LoadModuleChecked<FSuperManagerModule>(TEXT("SuperManager"));
	SuperMangerModule.SyncContentBrowserToAsset(ClickedAssetData->GetSoftObjectPath().ToString());
	return FReply::Handled();
}
