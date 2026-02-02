// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidgets/AdvanceDeletionWidget.h"
#include "SlateBasics.h"
#include "SuperManager.h"
#include "DebugHelper.h"

void SAdvanceDeletionTab::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;
	AvailableAssetDataArray = InArgs._AssetsDataArray;
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
			SNew(STextBlock)
			.Text(FText::FromString("Advance Deletion"))
			.Font(TitleTextFont)
			.Justification(ETextJustify::Center)
			.ColorAndOpacity(FColor::White)
		]
		
		// second slot for the condition options
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
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
		]
		
	];
}

TSharedRef<ITableRow> SAdvanceDeletionTab::OnGenerateRowForList(TSharedPtr<FAssetData> AssetDataToDisplay, const TSharedRef<STableViewBase>& OwnerTable)
{
	if(!AssetDataToDisplay.IsValid()) return SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable);
	const FString AssetName = AssetDataToDisplay->AssetName.ToString();
	const FString AssetClassName = AssetDataToDisplay->AssetClassPath.GetAssetName().ToString();

	FSlateFontInfo AssetClassFont =  GetFontInfo();
	AssetClassFont.Size = 10;

	FSlateFontInfo AssetNameFont =  GetFontInfo();
    AssetNameFont.Size = 10;
	
	TSharedRef<STableRow <TSharedPtr <FAssetData>>> AssetRowWidget =
		 SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable)
		[
			SNew(SHorizontalBox)
			// 1st slot Checkbox
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.FillWidth(0.05f)
			[
				ConstructCheckbox(AssetDataToDisplay)
			]
			// 2nd slot Asset Class Name

			+SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.FillWidth(0.5f)
			[
				ConstructTextForRowWidget(AssetClassName,AssetClassFont)
			]
			// 3rd slot Asset Name

			+SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			[
				ConstructTextForRowWidget(AssetName,AssetNameFont)
			]
			// 4th slot direct delete button

			+SHorizontalBox::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Center)
			[
				ConstructButtonForRowWidget(AssetDataToDisplay)
			]
			
		];
	return AssetRowWidget;
}

TSharedRef<SListView<TSharedPtr<FAssetData>>> SAdvanceDeletionTab::ConstructAssetList()
{
	ConstructedAssetListView = SNew(SListView<TSharedPtr<FAssetData>>)
								.ItemHeight(24.f)
								.ListItemsSource(&AvailableAssetDataArray)
								.OnGenerateRow(this, &SAdvanceDeletionTab::OnGenerateRowForList);
	return ConstructedAssetListView.ToSharedRef();
}

TSharedRef<SCheckBox> SAdvanceDeletionTab::ConstructCheckbox(const TSharedPtr<FAssetData>& AssetDataToDisplay)
{
	TSharedRef<SCheckBox> checkBox = SNew(SCheckBox)
		.Type(ESlateCheckBoxType::CheckBox)
		.OnCheckStateChanged(this, &SAdvanceDeletionTab::OnCheckBoxStateChanged, AssetDataToDisplay)
		.Visibility(EVisibility::Visible);
	return checkBox;
}

TSharedRef<STextBlock> SAdvanceDeletionTab::ConstructTextForRowWidget(const FString& TextContent,
	const FSlateFontInfo& FontInfo)
{
	TSharedRef<STextBlock> ConstructedTextBlock = SNew(STextBlock)
	.Text(FText::FromString(TextContent))
	.Font(FontInfo)
	.ColorAndOpacity(FColor::White);

	return ConstructedTextBlock;
}

TSharedRef<SButton> SAdvanceDeletionTab::ConstructButtonForRowWidget(const TSharedPtr<FAssetData>& AssetDataToDisplay)
{
	TSharedRef<SButton> ConstructedButton =
		SNew(SButton)
		.Text(FText::FromString(TEXT("Delete")))
		.OnClicked(this, &SAdvanceDeletionTab::OnDeleteClicked, AssetDataToDisplay);
		
	return ConstructedButton;
}

FReply SAdvanceDeletionTab::OnDeleteClicked(TSharedPtr<FAssetData> ClickedAssetData)
{
	FSuperManagerModule& SuperManagerModule = FModuleManager::LoadModuleChecked<FSuperManagerModule>(TEXT("SuperManager"));
	const bool bAssetDeleted =  SuperManagerModule.DeleteSingleAsset(*ClickedAssetData.Get());
	if (bAssetDeleted)
	{
		//Refresh
		if (AvailableAssetDataArray.Contains(ClickedAssetData))
		{
			AvailableAssetDataArray.Remove(ClickedAssetData);
		}
		RefreshAssetList();
	}
	return FReply::Handled();
}

void SAdvanceDeletionTab::RefreshAssetList()
{
	if (ConstructedAssetListView.IsValid()) ConstructedAssetListView->RebuildList();
}

void SAdvanceDeletionTab::OnCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData)
{
	switch(NewState)
	{
	case ECheckBoxState::Checked:
		break;

	case ECheckBoxState::Unchecked:
		break;

	case ECheckBoxState::Undetermined:
		break;
		
	default:
		break;
	}
	DebugHelper::PrintOnScreen ("Working.....");
}
