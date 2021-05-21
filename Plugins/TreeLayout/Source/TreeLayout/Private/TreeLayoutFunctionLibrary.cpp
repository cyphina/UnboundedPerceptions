#include "TreeLayoutFunctionLibrary.h"
#include "PanelWidget.h"
#include "SlateBlueprintLibrary.h"
#include "UpTree.h"
#include "UserWidget.h"

UUpTree* UTreeLayoutFunctionLibrary::StratifyData(UObject* Outer, TArray<UObject*> DataObjects, const TArray<FName>& Ids, const TArray<FName>& ParentIds)
{
   return UUpTree::CreateTree(Outer, DataObjects, Ids, ParentIds);
}

FVector2D UTreeLayoutFunctionLibrary::GetTreeNodePosition(const FUpTreeNode& Node)
{
   return FVector2D(Node.X, Node.Y);
}

FUpTreeConnector UTreeLayoutFunctionLibrary::MakeConnectionBetweenWidgets(const UUserWidget* ContainingWidget, const UUserWidget* Parent, const UUserWidget* Child)
{
   if(ContainingWidget && Parent && Child)
   {
      const FGeometry& ContainingGeometry = ContainingWidget->GetPaintSpaceGeometry();

      const FGeometry& ParentGeometry       = Parent->GetPaintSpaceGeometry();
      const FVector2D  ParentSize           = ParentGeometry.GetAbsoluteSize();
      const FVector2D  CenterOfParentWidget = ContainingGeometry.AbsoluteToLocal(ParentGeometry.GetAbsolutePosition() + (ParentSize * 0.5f));

      const FGeometry& ChildGeometry       = Child->GetPaintSpaceGeometry();
      const FVector2D  ChildSize           = ChildGeometry.GetAbsoluteSize();
      const FVector2D  CenterOfChildWidget = ContainingGeometry.AbsoluteToLocal(ChildGeometry.GetAbsolutePosition() + (ChildSize * 0.5f));

      const float HalfwayPoint = (CenterOfChildWidget.Y + CenterOfParentWidget.Y) / 2;

      const FVector2D C1 = CenterOfParentWidget;
      const FVector2D C2 = {CenterOfParentWidget.X, HalfwayPoint};
      const FVector2D C3 = {CenterOfChildWidget.X, HalfwayPoint};
      const FVector2D C4 = CenterOfChildWidget;

      return FUpTreeConnector(C1, C2, C3, C4);
   }
   return FUpTreeConnector();
}

const bool UTreeLayoutFunctionLibrary::IsNodeValid(const FUpTreeNode& Node)
{
   return Node.IsValid();
}

const FUpTreeNode& UTreeLayoutFunctionLibrary::GetParent(const FUpTreeNode& Node)
{
   if(Node.Parent)
   {
      return *Node.Parent;
   }
   return UUpTree::INVALID_NODE;
}

FString UTreeLayoutFunctionLibrary::ConnectorToString(const FUpTreeConnector& Connector)
{
   return FString::Join(TArray<FString>{Connector.Control1.ToString(), Connector.Control2.ToString(), Connector.Control3.ToString(), Connector.Control4.ToString()},
                        TEXT(" "));
}

FString UTreeLayoutFunctionLibrary::NodeToString(const FUpTreeNode& Node)
{
   FString result = "";

   result += FString::Printf(TEXT("Node Name: %s \n"), *Node.Id.ToString());
   result += FString::Printf(TEXT("Position: (%f,%f) \n"), Node.X, Node.Y);
   result += FString::Printf(TEXT("Num Children: %d \n"), Node.Children.Num());
   result += FString::Printf(TEXT("Mod: %d \n"), Node.Mod);

   if(Node.Thread)
   {
      result += FString::Printf(TEXT("Thread: %s \n"), *Node.Thread->Id.ToString());
   }

   return result;
}

FString UTreeLayoutFunctionLibrary::UpTreeToString(UUpTree* Tree)
{
   if(!Tree)
   {
      return FString();
   }

   return Tree->MakeStringFromTree();
}
