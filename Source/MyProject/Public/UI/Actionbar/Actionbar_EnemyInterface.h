// Created 1/15/21 9:0 PM

#pragma once
#include "Actionbar_UnitInterface.h"
#include "Actionbar_EnemyInterface.generated.h"

class UTextBlock;
class AUnit;

/** This UI shows up on our actionbar when we select an enemy */
UCLASS()
class MYPROJECT_API UActionbar_EnemyInterface : public UActionbar_UnitInterface
{
   GENERATED_BODY()
   
protected:
   UPROPERTY(Meta=(BindWidget))
   UTextBlock* Text_EnemyDescription;

};
