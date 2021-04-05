// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

/**
 * Custom detail view for unit that hides a lot of properties we do not need to change
 */
class MYPROJECTEDITOR_API FUnitDetails : public IDetailCustomization
{
 public:
   static TSharedRef<IDetailCustomization> MakeInstance();

 protected:
   void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
};
