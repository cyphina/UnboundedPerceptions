#include "TreeLayoutWidget.h"

#include "CanvasPanel.h"
#include "CanvasPanelSlot.h"
#include "TreeLayoutFunctionLibrary.h"
#include "UpTree.h"
#include "UpTreeStructs.h"

TArray<UUserWidget*> UTreeLayoutWidget::ShowTree(UUpTree* Tree, float TreeSizeX, float TreeSizeY)
{
   TArray<UUserWidget*> NodeWidgets;

   if(IsValid(Tree))
   {
      Can_Node->ClearChildren();
      Tree->PositionNodes(LayoutStrategy, {TreeSizeX, TreeSizeY});

      if(bShowConnections)
      {
         Branches = Tree->MakeNodeConnections();
      }

      if(bShowThreads)
      {
         Threads = Tree->MakeNodeConnections();
      }

      TArray<UCanvasPanelSlot*> NodeSlots;
      for(const auto& TreeData : Tree->GetHierarchy())
      {
         const FUpTreeNode& Node       = TreeData.Value;
         UUserWidget*       NodeWidget = CreateWidget(this, WidgetClass);
         NodeWidgets.Emplace(NodeWidget);
         UCanvasPanelSlot* NodeSlot = Can_Node->AddChildToCanvas(NodeWidget);
         NodeSlot->SetPosition(UTreeLayoutFunctionLibrary::GetTreeNodePosition(Node));
         NodeSlot->SetAutoSize(true);
         NodeSlots.Emplace(NodeSlot);
      }

      OffsetNodes(NodeSlots);
   }

   return NodeWidgets;
}

void UTreeLayoutWidget::OffsetNodes(TArray<UCanvasPanelSlot*>& NodeSlots)
{
   ForceLayoutPrepass();

   for(auto& NodeSlot : NodeSlots)
   {
      const FVector2D OffsetTowardsCenter = NodeSlot->Content->GetDesiredSize() / 2;
      const FVector2D OffsetSlotPosition  = NodeSlot->GetPosition() - OffsetTowardsCenter;
      NodeSlot->SetPosition(OffsetSlotPosition);
   }
}

int32 UTreeLayoutWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
                                     int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
   for(const FUpTreeConnector& Connector : Branches)
   {
      FSlateDrawElement::MakeCubicBezierSpline(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(), Connector.Control1, Connector.Control2, Connector.Control3,
                                               Connector.Control4, 5.0, ESlateDrawEffect::None, BranchColor);
   }

   for(const FUpTreeConnector& Connector : Threads)
   {
      FSlateDrawElement::MakeCubicBezierSpline(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(), Connector.Control1, Connector.Control2, Connector.Control3,
                                               Connector.Control4, 5.0, ESlateDrawEffect::None, ThreadColor);
   }

   return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}
