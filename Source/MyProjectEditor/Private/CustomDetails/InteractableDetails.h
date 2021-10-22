// Created 6/15/21 1:36 AM

#pragma once
#include "IDetailCustomization.h"

class MYPROJECTEDITOR_API FInteractableDetails : public IDetailCustomization
{
 public:
   static TSharedRef<IDetailCustomization> MakeInstance();

 protected:
   void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
};
