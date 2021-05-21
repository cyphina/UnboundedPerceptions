#pragma once
#include "SPanel.h"

/**
 * @brief A panel which allows us to arrange elements in a node tree structure.
 */
class TREELAYOUT_API STreePanel : public SPanel
{
 public:
   struct FSlot : public TSlotBase<FSlot>
   {
      FSlot(int32 ParentIndex) : TSlotBase<FSlot>(), ParentIndex(ParentIndex) {}
      int32 ParentIndex;
   };

   SLATE_BEGIN_ARGS(STreePanel) {}
   SLATE_ATTRIBUTE(int, MaxNodes)
   SLATE_EVENT(FOnClicked, OnClicked)
   SLATE_END_ARGS()

   void   Construct(const FArguments& InArgs);
   FSlot& AddSlot(int32 ParentIndex);

   virtual void       OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const override;
   virtual FVector2D  ComputeDesiredSize(float LayoutScaleMultiplier) const override;
   virtual void       CacheDesiredSize(float InLayoutScaleMultiplier) override;
   virtual FChildren* GetChildren() override;

 private:
   TPanelChildren<FSlot> Children;

   FVector2D     TotalDesiredSizes;
   mutable int32 Depth;
   mutable int32 Width;
};
