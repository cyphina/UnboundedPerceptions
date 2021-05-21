// Created 5/8/21 11:13 AM

#include "TreeLayoutStrategies.h"

#include "UpTree.h"
#include "UpTreeStructs.h"

void UUpTreeKnuthStrategy::LayoutNodes(UUpTree* Tree) const
{
}

void UUpTreeKnuthWSNaive::LayoutNodes(UUpTree* Tree) const
{
   TArray<int> NextNodeIndex;
   NextNodeIndex.AddDefaulted(Tree->GetMaxDepth() + 1);

   PositionNodesMinimalWidthNoCentering(Tree->GetRoot(), NextNodeIndex);
}

void UUpTreeKnuthWSNaive::PositionNodesMinimalWidthNoCentering(FUpTreeNode* Node, TArray<int>& NextNodeIndex)
{
   if(Node)
   {
      Node->X = NextNodeIndex[Node->Y];
      NextNodeIndex[Node->Y] += 1;

      for(FUpTreeNode* child : Node->Children)
      {
         PositionNodesMinimalWidthNoCentering(child, NextNodeIndex);
      }
   }
}

void UUpTreeKnuthWSCentered::LayoutNodes(UUpTree* Tree) const
{
   TArray<int> NextNodeIndex;
   TArray<int> Offsets;

   NextNodeIndex.AddDefaulted(Tree->GetMaxDepth() + 1);
   Offsets.AddDefaulted(Tree->GetMaxDepth() + 1);

   PositionNodesMinimalWidthCentering(Tree->GetRoot(), NextNodeIndex, Offsets);
}

void UUpTreeKnuthWSCentered::PositionNodesMinimalWidthCentering(FUpTreeNode* Node, TArray<int>& NextNodeIndex, TArray<int>& Offsets)
{
   if(Node)
   {
      for(FUpTreeNode* child : Node->Children)
      {
         PositionNodesMinimalWidthCentering(child, NextNodeIndex, Offsets);
      }
   }

   const int depth = Node->Y;
   float     place = 0;

   // No children? Take the next slot.
   if(!Node->Children.Num())
   {
      place   = NextNodeIndex[depth];
      Node->X = place;
   }
   // One Child? - Offset parent to right by a bit.
   else if(Node->Children.Num() == 1)
   {
      place = Node->Children[0]->X - 1;
   }

   // Two children? - Offset parent to center between them.
   //     o
   //    / \
   //   o    o
   //  /      \
   // o         o
   else
   {
      place = (Node->Children[0]->X + Node->Children[1]->X) / 2;
   }

   // The offset is the space between next slot (e.g., 0, 2, 4, 6, ...) and proposed place.
   // Imagine we have a lot of nodes on the level before the leaf level, and on the leaf we only have a few nodes in the center.
   // We have to offset all those nodes to be where the parent is (in the center).
   Offsets[depth] = FMath::Max<float>(Offsets[depth], NextNodeIndex[depth] - place);

   if(Node->Children.Num())
   {
      // Offset ourselves, but not our children yet.
      Node->X = place + Offsets[depth];
   }

   NextNodeIndex[depth] += 2;
   Node->Mod = Offsets[depth];
}

void UUpTreeReingoldTilfordStrategy::LayoutNodes(UUpTree* Tree) const
{
   Tree->ResetTree();
   AddMods(ReingoldTilfordSetup(Tree->GetRoot()));
}

FUpTreeNode* UUpTreeReingoldTilfordStrategy::ReingoldTilfordSetup(FUpTreeNode* Node)
{
   if(Node)
   {
      if(Node->Children.Num() == 0)
      {
         Node->X = 0;
         return Node;
      }

      if(Node->Children.Num() == 1)
      {
         Node->X = ReingoldTilfordSetup(Node->Children[0])->X;
         return Node;
      }

      FUpTreeNode* LeftNode  = ReingoldTilfordSetup(Node->Children[0]);
      FUpTreeNode* RightNode = ReingoldTilfordSetup(Node->Children[1]);

      Node->X = FixSubtrees(*LeftNode, *RightNode);
      return Node;
   }
   return nullptr;
}

void UUpTreeReingoldTilfordStrategy::AddMods(FUpTreeNode* node, int mod)
{
   if(node)
   {
      node->X += mod;
      for(FUpTreeNode* child : node->Children)
      {
         if(child)
         {
            AddMods(child, mod + node->Mod);
         }
      }
   }
}

int UUpTreeReingoldTilfordStrategy::FixSubtrees(FUpTreeNode& Left, FUpTreeNode& Right)
{
   FUpTreeNode* LeftInner   = &Left;
   FUpTreeNode* RightInner  = &Right;
   float        MaxOffset   = 0.f;
   float        LeftOffset  = 0.f;
   float        RightOffset = 0.f;
   FUpTreeNode* LeftOuter   = nullptr;
   FUpTreeNode* RightOuter  = nullptr;

   Contour(LeftInner, RightInner, MaxOffset, LeftOffset, RightOffset, LeftOuter, RightOuter);
   MaxOffset += 1;                                                  // Add 1 to distance between left and right trees to ensure they do not conflict.
   MaxOffset += static_cast<int>(Right.X + MaxOffset + Left.X) % 2; // Even out midpoint

   // Shift node in right subtree and its children
   Right.Mod = MaxOffset;
   Right.X += MaxOffset;

   // Add the shift to the current mod sum of the right subtree
   if(Right.Children.Num() > 0)
   {
      RightOffset += MaxOffset;
   }

   // Build threads (see below how threads marked (T) help us find left and right contours from root). 0 marks overlapped nodes (2 on top of each other)
   // Threads are built when we the subtrees joined at the current node at different heights and are not empty. They are used so that in later calculations we can easier traverse the contours.
   // Threads can only be made at leaf nodes (since if the next node on a contour is not the son of the current node, it must be a leaf)
   // Also threads can only be made on the leftmost and rightmost nodes on a subtree.
   //             o
   //          /     \
   //         o        o
   //       /  \     /  \ 
   //      o    o   o    o
   //    /    /    / \  T
   //   o   o     o   o
   //    \ T         T <--- This thread isn't used unless this whole tree is a left subtree of another (then it will help us walk over this tree's right contour).
   //      o - - - -

   // If right subtree deeper, then insert thread from leftmost node on left subtree to leftmost node on right subtree.
   if(RightInner && !LeftInner)
   {
      LeftOuter->Thread = RightInner;
      LeftOuter->Mod    = RightOffset - LeftOffset;
   }
   // If left subtree deeper, insert thread from rightmost node on right subtree to rightmost node on left subtree.
   else if(LeftInner && !RightInner)
   {
      RightOuter->Thread = LeftInner;
      RightOuter->Mod    = LeftOffset - RightOffset;
   }

   return (Left.X + Right.X) / 2;
}

void UUpTreeReingoldTilfordStrategy::Contour(FUpTreeNode*& Left, FUpTreeNode*& Right, float& MaxOffset, float& LeftOffset, float& RightOffset, FUpTreeNode*& LeftOuter,
                                             FUpTreeNode*& RightOuter)
{
   // Get difference in shifted positions
   const float CurrentOffset = Left->X + LeftOffset - (Right->X + RightOffset);

   // Record largest overlap of left tree's right contour into right tree's left contour  (if left is bigger than right, we're overlapping).
   if(CurrentOffset > MaxOffset)
   {
      MaxOffset = CurrentOffset;
   }

   // Move along the contour
   if(!LeftOuter)
   {
      LeftOuter = Left;
   }

   if(!RightOuter)
   {
      RightOuter = Right;
   }

   FUpTreeNode* NewLeftOuter  = Left->NextLeft();
   FUpTreeNode* NewRightOuter = Right->NextRight();
   Left                       = Left->NextRight();
   Right                      = Right->NextLeft();

   if(Left && Right)
   {
      LeftOffset += Left->Mod;
      RightOffset += Right->Mod;
      Contour(Left, Right, MaxOffset, LeftOffset, RightOffset, NewLeftOuter, NewRightOuter);
   }
}

void UUpTreeBucheimLayoutStrategy::LayoutNodes(UUpTree* Tree) const
{
   Tree->ResetTree();

   FirstWalk(Tree->GetRoot());
   SecondWalk(Tree->GetRoot());
}

void UUpTreeBucheimLayoutStrategy::FirstWalk(FUpTreeNode* Node, int Distance)
{
   if(Node->IsLeaf())
   {
      if(FUpTreeNode* LeftSibling = Node->GetLeftSibling())
      {
         Node->X = LeftSibling->X + Distance;
      }
   }
   else
   {
      FUpTreeNode* DefaultAncestor = Node->Children[0];
      for(FUpTreeNode* Child : Node->Children)
      {
         FirstWalk(Child);
         Apportion(Child, DefaultAncestor, Distance);
      }

      ExecuteShifts(Node);
      const float Midpoint = (Node->Children[0]->X + Node->Children[Node->Children.Num() - 1]->X) / 2;

      if(FUpTreeNode* LeftSibling = Node->GetLeftSibling())
      {
         Node->X   = LeftSibling->X + Distance;
         Node->Mod = Node->X - Midpoint;
      }
      else
      {
         Node->X = Midpoint;
      }
   }
}

void UUpTreeBucheimLayoutStrategy::Apportion(FUpTreeNode* Node, FUpTreeNode*& DefaultAncestor, int Distance)
{
   if(FUpTreeNode* LeftSibling = Node->GetLeftSibling())
   {
      // RST = RightSubtree, LST = LeftSubtree
      FUpTreeNode *NodeInsideRST = Node, *NodeOutsideRST = Node;
      FUpTreeNode* NodeInsideLST  = LeftSibling;
      FUpTreeNode* NodeOutsideLST = NodeInsideRST->Parent ? NodeInsideRST->Parent->Children[0] : nullptr;

      int ModSumInsideRST  = NodeInsideRST->Mod;
      int ModSumOutsideRST = NodeOutsideRST->Mod;
      int ModSumInsideLST  = NodeInsideLST->Mod;
      int ModSumOutsideLST = NodeOutsideLST->Mod;
      int Shift            = 0;

      while(NodeInsideLST = NodeInsideLST->NextRight(), NodeInsideRST = NodeInsideRST->NextLeft(), NodeInsideLST && NodeInsideRST)
      {
         NodeOutsideLST           = NodeOutsideLST->NextLeft();
         NodeOutsideRST           = NodeOutsideRST->NextRight();
         NodeOutsideRST->Ancestor = Node;
         Shift                    = NodeInsideLST->X + ModSumInsideLST - (NodeInsideRST->X + ModSumInsideRST) + Distance;

         if(Shift > 0)
         {
            MoveSubtree(Ancestor(NodeInsideLST, Node, DefaultAncestor), Node, Shift);
            ModSumInsideRST += Shift;
            ModSumOutsideRST += Shift;
         }

         ModSumInsideLST += NodeInsideLST->Mod;
         ModSumInsideRST += NodeInsideRST->Mod;
         ModSumOutsideLST += NodeOutsideLST->Mod;
         ModSumOutsideRST += NodeOutsideRST->Mod;
      }

      if(NodeInsideLST && !NodeOutsideRST->NextRight())
      {
         NodeOutsideRST->Thread = NodeInsideLST;
         NodeOutsideRST->Mod += ModSumInsideLST - ModSumOutsideRST;
      }
      if(NodeInsideRST && !NodeOutsideLST->NextLeft())
      {
         NodeOutsideLST->Thread = NodeInsideRST;
         NodeOutsideLST->Mod += ModSumInsideRST - ModSumOutsideLST;
         DefaultAncestor = Node;
      }
   }
}

void UUpTreeBucheimLayoutStrategy::MoveSubtree(FUpTreeNode* GreatestLeftAncestor, FUpTreeNode* GreatestRightAncestor, int Shift)
{
   const float Change = Shift / (GreatestRightAncestor->ChildIndex - GreatestLeftAncestor->ChildIndex);
   GreatestRightAncestor->Change -= Change;
   GreatestRightAncestor->Shift += Shift;
   GreatestLeftAncestor->Change += Change;
   GreatestRightAncestor->X += Shift;
   GreatestRightAncestor->Mod += Shift;
}

void UUpTreeBucheimLayoutStrategy::ExecuteShifts(FUpTreeNode* Node)
{
   float Shift = 0, Change = 0;

   for(int i = Node->Children.Num() - 1; i >= 0; --i)
   {
      FUpTreeNode* Child = Node->Children[i];
      Child->X += Shift;
      Child->Mod += Shift;
      Change += Child->Change;
      Shift += Child->Shift + Change;
   }
}

FUpTreeNode* UUpTreeBucheimLayoutStrategy::Ancestor(FUpTreeNode* NodeInsideLST, FUpTreeNode* Node, FUpTreeNode* DefaultAncestor)
{
   if(NodeInsideLST->Ancestor->IsSibling(Node))
   {
      return NodeInsideLST->Ancestor;
   }
   else
   {
      return DefaultAncestor;
   }
}

void UUpTreeBucheimLayoutStrategy::SecondWalk(FUpTreeNode* Node, int Mod)
{
   Node->X += Mod;
   for(FUpTreeNode* Child : Node->Children)
   {
      SecondWalk(Child, Mod + Node->Mod);
   }
}
