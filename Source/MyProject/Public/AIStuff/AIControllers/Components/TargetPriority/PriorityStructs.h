#pragma once
<<<<<<< HEAD
=======
#include "GameplayTags.h"
>>>>>>> componentrefactor

class UEnvQueryTest;

USTRUCT(BlueprintType, NoExport)
struct FTargetTest {
   UPROPERTY(EditAnywhere, meta = (DisplayThumbnail))
   UEnvQueryTest* test;

   UPROPERTY(EditAnywhere)
   float scoreMultiplier;
};

USTRUCT(BlueprintType, NoExport)
struct FSpellTargetCriteria {
   /** Targeting best for damage spells like fire bullet and fire storm*/
   UPROPERTY(EditDefaultsOnly, meta = (AllowedClasses = "UOffensiveTests"))
   TArray<FTargetTest> offensiveTests;

   /** Targeting best for defensive skills like stone skin and absorption wall*/
   UPROPERTY(EditDefaultsOnly, meta = (AllowedClasses = "UDefensiveTests"))
   TArray<FTargetTest> defensiveTests;

   /** Targeting best for healing skills like cure and heal circle */
   UPROPERTY(EditDefaultsOnly, meta = (AllowedClasses = "USupportTests"))
   TArray<FTargetTest> supportTests;

   /** Targeting best for skills to remove buffs/debuffs like purgatory flame and Purity's Answer */
   UPROPERTY(EditDefaultsOnly, meta = (AllowedClasses = "UPurgeTests"))
   TArray<FTargetTest> purgeTests;

   /** Targeting best for enemies that should be stunned/silence/weakened/tenderized like Iron Maiden and Demon's Breath */
   UPROPERTY(EditDefaultsOnly, meta = (AllowedClasses = "UDisableTests"))
   TArray<FTargetTest> disableTests;

   /** Targeting best for allies that should have their attacking capability strengthened like using Enrage */
   UPROPERTY(EditDefaultsOnly, meta = (AllowedClasses = "UBuffTests"))
   TArray<FTargetTest> buffTests;

   FORCEINLINE const TArray<FTargetTest>& GetCriteriaFromSpellDescTag(FGameplayTag descTag) const { return descTagToTargetTestsMap[descTag]; }

 private:
   const TMap<FGameplayTag, const TArray<FTargetTest>> descTagToTargetTestsMap = {{FGameplayTag::RequestGameplayTag("Skill.Category.Offensive"), offensiveTests}};
};
