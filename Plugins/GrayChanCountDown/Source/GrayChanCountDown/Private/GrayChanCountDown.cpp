// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "GrayChanCountDownPrivatePCH.h"

#include "SlateBasics.h"
#include "SlateExtras.h"

#include "GrayChanCountDownStyle.h"
#include "GrayChanCountDownCommands.h"

#include "LevelEditor.h"

static const FName GrayChanCountDownTabName("GrayChanCountDown");

#define LOCTEXT_NAMESPACE "FGrayChanCountDownModule"

void FGrayChanCountDownModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FGrayChanCountDownStyle::Initialize();
	FGrayChanCountDownStyle::ReloadTextures();

	FGrayChanCountDownCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FGrayChanCountDownCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FGrayChanCountDownModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FGrayChanCountDownModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FGrayChanCountDownModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(GrayChanCountDownTabName, FOnSpawnTab::CreateRaw(this, &FGrayChanCountDownModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FGrayChanCountDownTabTitle", "GrayChanCountDown"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FGrayChanCountDownModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FGrayChanCountDownStyle::Shutdown();

	FGrayChanCountDownCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(GrayChanCountDownTabName);
}

TSharedRef<SDockTab> FGrayChanCountDownModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	// タブを表示した時刻とある時刻との差分を文字で表示する
	int32 Days, Hours24, Minutes;
	const FDateTime Base2016(2016, 1, 1); //比較先時刻：2016/1/1に設定
	const FTimespan TimeDiff = Base2016 - FDateTime::Now(); //時刻差分

	Minutes = TimeDiff.GetMinutes(); //時刻差分：分
	Hours24 = TimeDiff.GetHours(); //時刻差分：時
	Days = TimeDiff.GetDays(); //時刻差分：日

							   // Put your "OnButtonClicked" stuff here
	FText WidgetText = FText::Format(
		LOCTEXT("PluginButtonDialogText",
			"Grayちゃん公開まで\n あと {0}日と{1}時間{2}分"),
		FText::FromString(FString::FromInt(Days)),
		FText::FromString(FString::FromInt(Hours24)),
		FText::FromString(FString::FromInt(Minutes))
		); //表示文字列を設定

	//パッケージビルドNGの警告文
	FText WarningText = FText::FromString(
		TEXT("このプラグインが有効な場合、必ずパッケージビルドに失敗します。\n" 
			"必ずパッケージビルド時には無効にしてください。\n"
			"UE4.11公開時に再度プラグインをテンプレートから作りなおせば解消されるかもしれません。"));

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SOverlay)
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			[
				SNew(SBox)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Font(FSlateFontInfo("Veranda", 32)) //変更箇所：フォントを32に設定
					.Text(WidgetText)
				]
			]//カウントダウン
			+ SOverlay::Slot()
			.VAlign(VAlign_Bottom)
			.HAlign(HAlign_Center)
			[
				SNew(SBox)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(WarningText)
				]
			]//警告文
		];//タブのUI定義
}

void FGrayChanCountDownModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(GrayChanCountDownTabName);
}

void FGrayChanCountDownModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FGrayChanCountDownCommands::Get().OpenPluginWindow);
}

void FGrayChanCountDownModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FGrayChanCountDownCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGrayChanCountDownModule, GrayChanCountDown)