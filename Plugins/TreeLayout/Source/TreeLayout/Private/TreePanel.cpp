// Created 4/29/21 10:39 AM

#include "TreePanel.h"

FChildren* STreePanel::GetChildren()
{
   return &Children;
}

void STreePanel::Construct(const FArguments& InArgs)
{
}

void STreePanel::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
   if(Children.Num() > 0)
   {
   }
}

void STreePanel::CacheDesiredSize(float InLayoutScaleMultiplier)
{
   // Handles performing the Tidy Algorithm to give every node a local index
   for(int32 ChildIndex = 0; ChildIndex < Children.Num(); ++ChildIndex)
   {

   }
}

FVector2D STreePanel::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
   // Compute Left-Most, Right-Most, and Depth-Most Nodes to find extents.
   return TotalDesiredSizes;
}

STreePanel::FSlot& STreePanel::AddSlot(int32 ParentIndex)
{
   FSlot& NewSlot = *(new FSlot(ParentIndex));
   Children.Add(&NewSlot);
   return NewSlot;
}
