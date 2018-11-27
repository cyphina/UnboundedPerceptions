// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ESkillContainer.generated.h"

class USkillSlot;
class UMySpell;
class AAlly;

/* Container widget for skillslots.  Represents an interface between a group of skillslots and spell usage.
 * A ESkillContainer can be linked to any spell array inside a unit, meaning we can use this for
 * our heroes' class specific ability list too.
 */

UCLASS()
class MYPROJECT_API UESkillContainer : public UUserWidget
{
   GENERATED_BODY()

   UPROPERTY(BlueprintSetter = SetAllyRef)
   AAlly* allyRef;

   /**Shouldn't take too much space/performance to copy of we just copy the pointer
    * Represents the container in the unit which the set of skills in this container comes from.  So far only use for this is having two sets of containers
    * in heroes (skill and class abilities)
    * May one day replace this with polymorphic SkillContainerSkillSet classes which allow us to get the right skill container polymorphically
    
   UPROPERTY(BlueprintReadOnly, Category = "Properties", Meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
   TArray<TSubclassOf<UMySpell>> skills;*/

 public:
   void NativeConstruct() override;

   FText manaHelpText;

   UPROPERTY(BlueprintReadWrite, Category = "Properties")
   TArray<USkillSlot*> skillSlots;

   /**Get a skillslot from this container*/
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   USkillSlot* GetSkillSlot(int index) const;

   FORCEINLINE UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors") AAlly* GetAllyRef() const { return allyRef; }

   UFUNCTION(BlueprintSetter, Category = "Accessors")
   void SetAllyRef(AAlly* ally) { allyRef = ally; }

   UFUNCTION(BlueprintCallable)
   void UseSkill(int skillToUseIndex);
};
