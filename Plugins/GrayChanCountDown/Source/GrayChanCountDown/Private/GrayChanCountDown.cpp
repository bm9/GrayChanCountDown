// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

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
	// �^�u��\�����������Ƃ��鎞���Ƃ̍����𕶎��ŕ\������
	int32 Days, Hours24, Minutes;
	const FDateTime Base2016(2016, 1, 1); //��r�掞���F2016/1/1�ɐݒ�
	const FTimespan TimeDiff = Base2016 - FDateTime::Now(); //��������

	Minutes = TimeDiff.GetMinutes(); //���������F��
	Hours24 = TimeDiff.GetHours(); //���������F��
	Days = TimeDiff.GetDays(); //���������F��

							   // Put your "OnButtonClicked" stuff here
	FText WidgetText = FText::Format(
		LOCTEXT("PluginButtonDialogText",
			"Gray�������J�܂�\n ���� {0}����{1}����{2}��"),
		FText::FromString(FString::FromInt(Days)),
		FText::FromString(FString::FromInt(Hours24)),
		FText::FromString(FString::FromInt(Minutes))
		); //�\���������ݒ�

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
					.Font(FSlateFontInfo("Veranda", 32)) //�ύX�ӏ��F�t�H���g��32�ɐݒ�
					.Text(WidgetText)
				]
			]//�J�E���g�_�E��
		];//�^�u��UI��`
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