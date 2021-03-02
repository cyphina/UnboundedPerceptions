#pragma once

#include "ComponentVisualizer.h"
#include "UICommandList.h"

class UPatrolComponent;


/**
 * @brief Works if you add this to the blueprint (doesn't seem to work properly if you just add it through the list of actors in the world (at least not until I click on and off the actor)
 */
class FPatrolVisualizer : public FComponentVisualizer
{
 public:
   FPatrolVisualizer();
   virtual ~FPatrolVisualizer();

   virtual void OnRegister() override;
   virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
   //virtual void DrawVisualizationHUD(const UActorComponent* Component, const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override;
   virtual bool VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click) override;
   //virtual void EndEditing() override;
   /**Handles overriding widget location with target instead of actor component*/
   virtual bool GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const override;
   //virtual bool GetCustomInputCoordinateSystem(const FEditorViewportClient* ViewportClient, FMatrix& OutMatrix) const override;
   /**Handles changes made to the widget in the viewport (transformation changes).*/
   virtual bool HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltalRotate,
                                 FVector& DeltaScale) override;
   /**Handles input*/
   virtual bool HandleInputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;
   //virtual bool IsVisualizingArchetype() const override;
   /**Creates right click menu*/
   virtual TSharedPtr<SWidget> GenerateContextMenu() const override;

   UPatrolComponent* GetEditedPatrolComponent() const;

 private:
   int32                  currentlySelectedPointIndex;
   FComponentPropertyPath propertyPath;
   AActor*                patrolComponentOwner;

   /**Output log commands*/
   TSharedPtr<FUICommandList> PatrolComponentVisualizerActions;

   void OnDuplicateItem();
};
