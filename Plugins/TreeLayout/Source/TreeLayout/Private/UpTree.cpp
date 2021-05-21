// Created 5/3/21 12:28 AM

#include "UpTree.h"
#include "MaxElement.h"
#include "TreeLayoutFunctionLibrary.h"
#include "TreeLayoutStrategies.h"

int UUpTree::GetMaxDepth()
{
   return MaxDepth;
}

FString UUpTree::MakeStringFromTree() const
{
   FString result = "---Tree Info---\n";
   for(auto const& elem : Hierarchy)
   {
      result += UTreeLayoutFunctionLibrary::NodeToString(elem.Value);
   }
   return result;
}

const FUpTreeNode& UUpTree::GetNodeById(FName Id) const
{
   if(Hierarchy.Contains(Id))
   {
      return Hierarchy[Id];
   }
   return INVALID_NODE;
}

FUpTreeNode* UUpTree::GetRoot() const
{
   return Root;
}

UUpTree* UUpTree::CreateTree(UObject* Outer, TArray<UObject*> DataObjects, const TArray<FName>& Ids, const TArray<FName>& ParentIds)
{
   if(ensure(DataObjects.Num() > 0 && DataObjects.Num() == Ids.Num() && DataObjects.Num() == ParentIds.Num()))
   {
      if(!Outer)
      {
         return nullptr;
      }

      UUpTree* tree = NewObject<UUpTree>(Outer);

      const int numNodes = DataObjects.Num();

      TMap<FName, FUpTreeNode>& NewHierarchy = tree->Hierarchy;
      FUpTreeNode*&             Root         = tree->Root;

      for(int i = 0; i < numNodes; ++i)
      {
         const FName currentNodeId = Ids[i];
         NewHierarchy.Add(currentNodeId, FUpTreeNode());

         FUpTreeNode& node = NewHierarchy[currentNodeId];
         node.Id           = currentNodeId;
         node.Data         = DataObjects[i];
      }

      // Create Children Array and Setup Parents
      for(int i = 0; i < numNodes; ++i)
      {
         const FName currentNodeId   = Ids[i];
         const FName currentParentId = ParentIds[i];

         if(FUpTreeNode* node = &NewHierarchy[currentNodeId]; node)
         {
            if(currentParentId != "")
            {
               if(FUpTreeNode* parent = NewHierarchy.Find(currentParentId); parent)
               {
                  node->ChildIndex = parent->Children.Num();
                  parent->Children.Add(node);
                  node->Parent = parent;
               }
            }
            else
            {
               if(ensureMsgf(!Root, TEXT("Error - More than one root")))
               {
                  Root = node;
               }
            }
         }
      }

      // Setup depths
      if(ensureMsgf(Root, TEXT("Error - No root")))
      {
         PreOrderTraversal(Root,
                           [](FUpTreeNode* node)
                           {
                              if(node->Parent)
                              {
                                 node->Y = node->Parent->Y + 1;
                              }
                           });
      }

      using HierarchyMapType = TRemoveReference<decltype(NewHierarchy)>::Type::ElementType;
      auto proj              = [](HierarchyMapType MapElem)
      {
         return MapElem.Value.Y;
      };

      const int MaxDepth = Algo::MaxElementBy(NewHierarchy, proj)->Value.Y;
      tree->MaxDepth     = MaxDepth;
      return tree;
   }
   return nullptr;
}

void UUpTree::PositionNodes(TSubclassOf<UUpTreeLayoutStrategy> LayoutStrategy, FVector2D TreeSize)
{
   if(IsValid(LayoutStrategy))
   {
      LayoutStrategy.GetDefaultObject()->LayoutNodes(this);
      FUpTreeNode *Left = nullptr, *Right = nullptr, *Bottom = nullptr;
      GetTreeExtents(Left, Right, Bottom);
      OffsetTreeNodes(*Left, *Right, *Bottom, TreeSize);
   }
}

TArray<FUpTreeConnector> UUpTree::MakeNodeConnections() const
{
   TArray<FUpTreeConnector> connections;
   connections.Reserve(Hierarchy.Num() - 1);

   for(auto& pair : Hierarchy)
   {
      if(FUpTreeNode* Parent = pair.Value.Parent)
      {
         const FVector2D parentPos    = {Parent->X, Parent->Y};
         const FVector2D nodePos      = {pair.Value.X, pair.Value.Y};
         const float     halfwayPoint = (nodePos.Y + parentPos.Y) / 2;

         FVector2D C1 = parentPos;
         FVector2D C2 = {parentPos.X, halfwayPoint};
         FVector2D C3 = {nodePos.X, halfwayPoint};
         FVector2D C4 = nodePos;

         connections.Emplace(C1, C2, C3, C4);
      }
   }

   return connections;
}

void UUpTree::ResetTree()
{
   PostOrderTraversal(GetRoot(),
                      [](FUpTreeNode* Node)
                      {
                         Node->Mod    = 0;
                         Node->Thread = nullptr;
                      });
}

#if WITH_EDITORONLY_DATA
TArray<FUpTreeConnector> UUpTree::MakeThreadConnections() const
{
   TArray<FUpTreeConnector> connections;
   connections.Reserve(Hierarchy.Num() - 1);

   for(auto& pair : Hierarchy)
   {
      if(FUpTreeNode* Thread = pair.Value.Thread)
      {
         const FVector2D threadPos    = {Thread->X, Thread->Y};
         const FVector2D nodePos      = {pair.Value.X, pair.Value.Y};
         const float     halfwayPoint = (nodePos.Y + threadPos.Y) / 2;

         FVector2D C1 = threadPos;
         FVector2D C2 = {threadPos.X, halfwayPoint};
         FVector2D C3 = {nodePos.X, halfwayPoint};
         FVector2D C4 = nodePos;

         connections.Emplace(C1, C2, C3, C4);
      }
   }
   return connections;
}
#endif

void UUpTree::GetTreeExtents(FUpTreeNode*& Left, FUpTreeNode*& Right, FUpTreeNode*& Bottom) const
{
   if(!GetRoot())
   {
      return;
   }

   Left   = GetRoot();
   Right  = GetRoot();
   Bottom = GetRoot();

   PreOrderTraversal(GetRoot(),
                     [&Left, &Right, &Bottom](FUpTreeNode* Node)
                     {
                        if(Node->X < Left->X)
                        {
                           Left = Node;
                        }

                        if(Node->X > Right->X)
                        {
                           Right = Node;
                        }

                        if(Node->Y > Bottom->Y)
                        {
                           Bottom = Node;
                        }
                     });
}

void UUpTree::OffsetTreeNodes(const FUpTreeNode& Left, const FUpTreeNode& Right, const FUpTreeNode& Bottom, const FVector2D TreeSize)
{
   if(!GetRoot())
   {
      return;
   }

   if(Root->Children.Num() == 0)
   {
      Root->X = 1;
   }

   // Nodes between parents get more separation.
   const float Separation     = Left.Parent == Right.Parent ? 1 : 2;
   float       LocalOffsetX   = TreeSize.X / (Right.X + Separation);
   const int   AdjustedBottom = Bottom.Y ? Bottom.Y : 1;
   float       LocalOffsetY   = TreeSize.Y / AdjustedBottom;

   PreOrderTraversal(GetRoot(),
                     [LocalOffsetX, LocalOffsetY](FUpTreeNode* Node)
                     {
                        Node->X = Node->X * LocalOffsetX;
                        Node->Y = Node->Y * LocalOffsetY;
                     });
}

void UUpTree::PostOrderTraversal(FUpTreeNode* node, const TFunction<void(FUpTreeNode*)>& f)
{
   if(node)
   {
      for(FUpTreeNode* child : node->Children)
      {
         PostOrderTraversal(child, f);
      }
      f(node);
   }
}

void UUpTree::PreOrderTraversal(FUpTreeNode* node, const TFunction<void(FUpTreeNode*)>& f)
{
   if(node)
   {
      f(node);
      for(FUpTreeNode* child : node->Children)
      {
         PreOrderTraversal(child, f);
      }
   }
}
