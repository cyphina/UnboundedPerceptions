// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProjectEditorVisualizations.h"
#include "Visualizations/PatrolPoints.h"
#include "SceneManagement.h"
#include "WorldObjects/Components/PatrolComponent.h"
#include "RTSHitProxy.h"
#include "Commands.h"
#include "EditorStyle.h"

IMPLEMENT_HIT_PROXY(HRTSVisProxy, HComponentVisProxy)
IMPLEMENT_HIT_PROXY(HPatrolProxy, HRTSVisProxy)

#define LOCTEXT_NAMESPACE "PatrolVisualizerCommands"
class FPatrolVisualizerCommands : public TCommands<FPatrolVisualizerCommands>
{
public:
	FPatrolVisualizerCommands() : TCommands<FPatrolVisualizerCommands>(
		TEXT("Patrol Point Commands"), LOCTEXT("PatrolComponentVisualizer", "Patrol Component Visualizer"),
		NAME_None, FEditorStyle::GetStyleSetName()
		) {}

	virtual void RegisterCommands() override
	{
		/**Command, Text appears on menu, and Long text for command*/
		UI_COMMAND(DuplicateItem, "Duplicate", "Duplicate the current target.", EUserInterfaceActionType::Button, FInputChord());
	}

	TSharedPtr<FUICommandInfo> DuplicateItem;
};
#undef LOCTEXT_NAMESPACE
void FPatrolVisualizer::OnRegister()
{
	/**Bind commands to a function that will be called when selected*/
	PatrolComponentVisualizerActions = MakeShareable(new FUICommandList);
	const auto& commands = FPatrolVisualizerCommands::Get();

	PatrolComponentVisualizerActions->MapAction(commands.DuplicateItem, FExecuteAction::CreateSP(this, &FPatrolVisualizer::OnDuplicateItem), EUIActionRepeatMode::RepeatDisabled);
}


void FPatrolVisualizer::DrawVisualization(const UActorComponent* component, const FSceneView* view,
	FPrimitiveDrawInterface* pdi)
{
	if (const UPatrolComponent* patrolComponent = Cast<const UPatrolComponent>(component))
	{
		const FLinearColor selectedColor = patrolComponent->editorSelectedColor;
		const FLinearColor unselectedColor = patrolComponent->editorUnselectedColor;

		const FVector location = patrolComponent->GetComponentLocation();

		if (patrolComponent->patrolPoints.Num() > 0)
		{
			FLinearColor color = (currentlySelectedPointIndex == 0) ? selectedColor : unselectedColor;
			pdi->SetHitProxy(new HPatrolProxy(component, 0));
			pdi->DrawLine(location, patrolComponent->patrolPoints[0], color, SDPG_Foreground);
			pdi->DrawPoint(patrolComponent->patrolPoints[0], color, 20.f, SDPG_Foreground);
			pdi->SetHitProxy(NULL);
		}

		for (int i = 1; i < patrolComponent->patrolPoints.Num(); ++i)
		{
			FLinearColor color = (i == currentlySelectedPointIndex) ? selectedColor : unselectedColor;
			pdi->SetHitProxy(new HPatrolProxy(component, i));
			pdi->DrawLine(patrolComponent->patrolPoints[i - 1], patrolComponent->patrolPoints[i], color, SDPG_Foreground);
			pdi->DrawPoint(patrolComponent->patrolPoints[i], color, 20.f, SDPG_Foreground);
			pdi->SetHitProxy(NULL);
		}
	}
}

bool FPatrolVisualizer::VisProxyHandleClick(FEditorViewportClient* inViewportClient, HComponentVisProxy* visProxy,
	const FViewportClick& click)
{
	bool bEditing = false;

	if (visProxy && visProxy->Component.IsValid())
	{
		bEditing = true;
		const UPatrolComponent* patrolComponentRef = CastChecked<UPatrolComponent>(visProxy->Component.Get());
		propertyPath = FComponentPropertyPath(patrolComponentRef);
		
		if (visProxy->IsA(HPatrolProxy::StaticGetType()))
		{
			HPatrolProxy* proxy = (HPatrolProxy*)visProxy;
			currentlySelectedPointIndex = proxy->patrolIndex;
		}
	}
	else
	{
		currentlySelectedPointIndex = INDEX_NONE;
	}
	return bEditing;
}

bool FPatrolVisualizer::GetWidgetLocation(const FEditorViewportClient* viewportClient, FVector& outLocation) const
{
	if (IsValid(GetEditedPatrolComponent()) && currentlySelectedPointIndex != INDEX_NONE)
	{
		outLocation = GetEditedPatrolComponent()->patrolPoints[currentlySelectedPointIndex];
		return true;
	}
	return false;
}

bool FPatrolVisualizer::HandleInputDelta(FEditorViewportClient* viewportClient, FViewport* viewport,
	FVector& deltaTranslate, FRotator& deltalRotate, FVector& deltaScale)
{
	bool bHandled = false;
	if (IsValid(GetEditedPatrolComponent()) && currentlySelectedPointIndex != INDEX_NONE)
	{
		GetEditedPatrolComponent()->patrolPoints[currentlySelectedPointIndex] += deltaTranslate;
		bHandled = true;
	}
	return bHandled;
}

bool FPatrolVisualizer::HandleInputKey(FEditorViewportClient* viewportClient, FViewport* viewport, FKey key,
	EInputEvent Event)
{
	bool bHandled = false;
	if (key == EKeys::Delete)
	{
		if (IsValid(GetEditedPatrolComponent()) && currentlySelectedPointIndex != INDEX_NONE)
		{
			GetEditedPatrolComponent()->DeletePatrolPoint(currentlySelectedPointIndex);
			bHandled = true;
		}
	}
	return bHandled;
}

TSharedPtr<SWidget> FPatrolVisualizer::GenerateContextMenu() const
{
	FMenuBuilder menuBuilder(true, PatrolComponentVisualizerActions);
	{
		menuBuilder.BeginSection("Patrol Point Actions");
		{
			menuBuilder.AddMenuEntry(FPatrolVisualizerCommands::Get().DuplicateItem);
		}
		menuBuilder.EndSection();
	}

	TSharedPtr<SWidget> menuWidget = menuBuilder.MakeWidget();
	return menuWidget;
}

UPatrolComponent* FPatrolVisualizer::GetEditedPatrolComponent() const
{
	return Cast<UPatrolComponent>(propertyPath.GetComponent());
}

void FPatrolVisualizer::OnDuplicateItem()
{

}
