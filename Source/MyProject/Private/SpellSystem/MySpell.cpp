// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "MySpell.h"
#include "GameplayEffects/CoolDownEffect.h"
#include "GameplayEffects/DamageEffect.h"
#include "SpellManager.h"
#include "WorldObjects/Unit.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"


UMySpell::UMySpell() : UGameplayAbility()
{

}

void UMySpell::PostInitProperties()
{
	Super::PostInitProperties();
	CooldownGameplayEffectClass = UCoolDownEffect::StaticClass();
}

void UMySpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData); //Will trigger blueprint activate ability
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Ability Activated from Myspell");
}

bool UMySpell::CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	const FGameplayTagContainer* CooldownTags = GetCooldownTags();
	if (CooldownTags)
	{
		check(ActorInfo->AbilitySystemComponent.IsValid());
		/*
		FGameplayTagContainer g;
		ActorInfo->AbilitySystemComponent->GetOwnedGameplayTags(g);
		TArray<FGameplayTag> tagArray;
		g.GetGameplayTagArray(tagArray);
		for(FGameplayTag t : tagArray)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple,t.GetTagName().ToString());
		}*/
		//CooldownTags->Num() > 0 && ActorInfo->AbilitySystemComponent->HasAnyMatchingGameplayTags(*CooldownTags) ||
		if (ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(AbilityTags.GetByIndex(0)))
		{
			const FGameplayTag& CooldownTag = UAbilitySystemGlobals::Get().ActivateFailCooldownTag;

			if (OptionalRelevantTags && CooldownTag.IsValid())
			{
				OptionalRelevantTags->AddTag(CooldownTag);
			}
			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Cooldown checked");
			return false;
		}
	}
	return true;
}

FGameplayEffectContextHandle UMySpell::MakeEffectContext(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo) const
{
	check(ActorInfo);
	FGameplayEffectContextHandle Context = FGameplayEffectContextHandle(UAbilitySystemGlobals::Get().AllocGameplayEffectContext());
	// By default use the owner and avatar as the instigator and causer
	Context.AddInstigator(ActorInfo->AvatarActor.Get(), ActorInfo->AvatarActor.Get());

	// add in the ability tracking here.
	Context.SetAbility(this);

	return Context;
}

void UMySpell::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	//ApplyCooldown(Handle, ActorInfo, ActivationInfo);
	///---Our version of applying the cooldown---
	FGameplayEffectSpecHandle sH = MakeOutgoingGameplayEffectSpec(CooldownGameplayEffectClass, 1);
	UAbilitySystemBlueprintLibrary::SetDuration(sH, GetCDDuration(ActorInfo->AbilitySystemComponent.Get()));
	UAbilitySystemBlueprintLibrary::AddGrantedTag(sH, AbilityTags.GetByIndex(0));
	//ActorInfo->AbilitySystemComponent.Get()->AddLooseGameplayTag(AbilityTags.GetByIndex(0));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FromInt(GetCDDuration(ActorInfo->AbilitySystemComponent.Get())));
	K2_ApplyGameplayEffectSpecToOwner(sH);

	unitRef = Cast<AUnit>(ActorInfo->AvatarActor.Get());
	if (unitRef)
	{
		unitRef->CommitCast(this);
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString("Committing ability for unit:") + unitRef->GetName());
	}
}

void UMySpell::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	unitRef = Cast<AUnit>(ActorInfo->AvatarActor.Get());
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString("Giving Ability") + unitRef->GetName());
}

float UMySpell::GetCooldownTimeRemaining(const FGameplayAbilityActorInfo* ActorInfo) const
{
	if (ActorInfo->AbilitySystemComponent.IsValid())
	{
		const FGameplayTag cdTag = AbilityTags.GetByIndex(0);
		FGameplayTagContainer c = FGameplayTagContainer(cdTag);
		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(c);
		TArray< float > Durations = ActorInfo->AbilitySystemComponent->GetActiveEffectsTimeRemaining(Query);
#if UE_EDITOR
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, cdTag.GetTagName().ToString());
		//GEngine->AddOnScreenDebugMessage(45, 5.f, FColor::Green, FString::FromInt(Durations.Num()));
#endif
		
		if(Durations.Num()>0)
			return Durations[Durations.Num()-1];
	}

	return 0.f;
}

bool UMySpell::isOnCD(UAbilitySystemComponent* abilityComponent) const
{
	if (abilityComponent->HasMatchingGameplayTag(AbilityTags.GetByIndex(0)))
		return true;
	return false;
}

//called a lot might want to save in some local variable when called in timeline
float UMySpell::GetCDDuration(UAbilitySystemComponent* abilityComponent) const
{
	FGameplayAbilitySpec* abilitySpec = abilityComponent->FindAbilitySpecFromClass(GetClass());
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, GetName().ToString());
	//checkf(abilitySpec != nullptr, TEXT("Ability hasn't been registered to unit"));
	if (!abilitySpec)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *(FString::FromInt(abilityComponent->GetActivatableAbilities().Num()) + GetName().ToString()));
		return -1;
	}

	int numUpgrades = USpellManager::Get().GetSpellInfo(spellDefaults.id)->cdDuration.Num();
	return USpellManager::Get().GetSpellInfo(spellDefaults.id)->cdDuration[GetIndex(abilitySpec->Level, numUpgrades, GetMaxLevel())];
}

int UMySpell::GetRange(UAbilitySystemComponent* abilityComponent) const
{
	FGameplayAbilitySpec* abilitySpec = abilityComponent->FindAbilitySpecFromClass(GetClass());
	checkf(abilitySpec, TEXT("Ability hasn't been registered to unit"));
	int numUpgrades = USpellManager::Get().GetSpellInfo(spellDefaults.id)->range.Num();
	if(numUpgrades > 0)
		return USpellManager::Get().GetSpellInfo(spellDefaults.id)->range[GetIndex(abilitySpec->Level, numUpgrades, GetMaxLevel())];
	return 0;
}

int UMySpell::GetReqLevel(UAbilitySystemComponent* abilityComponent) const
{
	FGameplayAbilitySpec* abilitySpec = abilityComponent->FindAbilitySpecFromClass(GetClass());
	checkf(abilitySpec, TEXT("Ability hasn't been registered to unit"));
	int numUpgrades = USpellManager::Get().GetSpellInfo(spellDefaults.id)->reqLevel.Num();
	return USpellManager::Get().GetSpellInfo(spellDefaults.id)->reqLevel[GetIndex(abilitySpec->Level, numUpgrades, GetMaxLevel())];
}

int UMySpell::GetCost(UAbilitySystemComponent* abilityComponent) const
{
	FGameplayAbilitySpec* abilitySpec = abilityComponent->FindAbilitySpecFromClass(GetClass());
	checkf(abilitySpec, TEXT("Ability hasn't been registered to unit"));
	int numUpgrades = USpellManager::Get().GetSpellInfo(spellDefaults.id)->cost.Num();
	if(numUpgrades > 0)
		return USpellManager::Get().GetSpellInfo(spellDefaults.id)->cost[GetIndex(abilitySpec->Level, numUpgrades, GetMaxLevel())];
	return 0;
}

float UMySpell::GetSpellDuration(UAbilitySystemComponent* abilityComponent) const
{
	FGameplayAbilitySpec* abilitySpec = abilityComponent->FindAbilitySpecFromClass(GetClass());
	checkf(abilitySpec, TEXT("Ability hasn't been registered to unit"));
	int numUpgrades = USpellManager::Get().GetSpellInfo(spellDefaults.id)->duration.Num();
	if(numUpgrades > 0)
		return USpellManager::Get().GetSpellInfo(spellDefaults.id)->duration[GetIndex(abilitySpec->Level, numUpgrades, GetMaxLevel())];
	return 0;
}

FDamageScalarStruct UMySpell::GetDamage(UAbilitySystemComponent* abilityComponent) const
{
	FGameplayAbilitySpec* abilitySpec = abilityComponent->FindAbilitySpecFromClass(GetClass());
	checkf(abilitySpec, TEXT("Ability hasn't been registered to unit"));
	int numUpgrades = USpellManager::Get().GetSpellInfo(spellDefaults.id)->damage.Num() / 4;
	FDamageScalarStruct damageScalerScalars = FDamageScalarStruct();
	if(numUpgrades > 0)
	{
		int damageOffsetIndex = GetIndex(abilitySpec->Level, numUpgrades, GetMaxLevel());
		damageScalerScalars.strength = (USpellManager::Get().GetSpellInfo(spellDefaults.id)->damage[NUM_SCALING_DAMAGE_ATT * damageOffsetIndex]);
		damageScalerScalars.intelligence = (USpellManager::Get().GetSpellInfo(spellDefaults.id)->damage[NUM_SCALING_DAMAGE_ATT * damageOffsetIndex + 1]);
		damageScalerScalars.agility = (USpellManager::Get().GetSpellInfo(spellDefaults.id)->damage[NUM_SCALING_DAMAGE_ATT * damageOffsetIndex + 2]);
		damageScalerScalars.understanding = (USpellManager::Get().GetSpellInfo(spellDefaults.id)->damage[NUM_SCALING_DAMAGE_ATT * damageOffsetIndex +3]);
	}
	return damageScalerScalars;
}

float UMySpell::GetPeriod(UAbilitySystemComponent* abilityComponent) const
{
	FGameplayAbilitySpec* abilitySpec = abilityComponent->FindAbilitySpecFromClass(GetClass());
	checkf(abilitySpec, TEXT("Ability hasn't been registered to unit"));
	int numUpgrades = USpellManager::Get().GetSpellInfo(spellDefaults.id)->period.Num();
	if(numUpgrades > 0)
		return USpellManager::Get().GetSpellInfo(spellDefaults.id)->period[GetIndex(abilitySpec->Level, numUpgrades, GetMaxLevel())];
	return 0;
}

float UMySpell::GetCastTime(UAbilitySystemComponent* abilityComponent) const
{
	FGameplayAbilitySpec* abilitySpec = abilityComponent->FindAbilitySpecFromClass(GetClass());
	checkf(abilitySpec, TEXT("Ability hasn't been registered to unit"));
	return USpellManager::Get().GetSpellInfo(spellDefaults.id)->casttime;
}

float UMySpell::GetAOE(UAbilitySystemComponent* abilityComponent) const
{
	FGameplayAbilitySpec* abilitySpec = abilityComponent->FindAbilitySpecFromClass(GetClass());
	checkf(abilitySpec, TEXT("Ability hasn't been registered to unit"));
	int numUpgrades = USpellManager::Get().GetSpellInfo(spellDefaults.id)->AOE.Num();
	if(numUpgrades > 0)
		return USpellManager::Get().GetSpellInfo(spellDefaults.id)->AOE[GetIndex(abilitySpec->Level, numUpgrades, GetMaxLevel())];
	return 0;
}

int UMySpell::GetLevel(UAbilitySystemComponent* abilityComponent) const
{
	return abilityComponent->FindAbilitySpecFromClass(GetClass())->Level;
}

FGameplayEffectSpecHandle UMySpell::SetScaling(FGameplayEffectSpecHandle specHandle)
{
	FDamageScalarStruct damageScalings = GetDamage(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo()));
	specHandle = UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(specHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Strength"), damageScalings.strength);
	specHandle = UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(specHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Intelligence"), damageScalings.intelligence);
	specHandle = UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(specHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Agility"), damageScalings.agility);
	specHandle = UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(specHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Understanding"), damageScalings.understanding);
	return specHandle;
}

FGameplayEffectSpecHandle UMySpell::SetPeriod(FGameplayEffectSpecHandle specHandle, float period)
{
	FGameplayEffectSpec* Spec = specHandle.Data.Get();
	if (Spec)
	{
		Spec->Period = period;
	}
	else
	{
		ABILITY_LOG(Warning, TEXT("UMySpell::SetPeriod called with invalid SpecHandle"));
	}
	return specHandle;
}

FGameplayEffectSpecHandle UMySpell::CreateGameplayEffectFromTableValues(TSubclassOf<UGameplayEffect> effectClass, FGameplayTag name, FGameplayTagContainer assetTags)
{
	UAbilitySystemComponent* abilityComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetActorInfo().AvatarActor.Get());
	FGameplayEffectSpecHandle effect = USpellFunctionLibrary::MakeGameplayEffect(this, effectClass, GetLevel(abilityComp), GetSpellDuration(abilityComp), GetPeriod(abilityComp), USpellManager::Get().GetSpellInfo(spellDefaults.id)->elem, name, assetTags);
	if(effectClass->IsChildOf(UDamageEffect::StaticClass()))
		SetScaling(effect);
	return effect;
}

FORCEINLINE UAbilitySystemComponent* UMySpell::GetOwnerAbilitySystemComponent() const
{
	return UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningActorFromActorInfo());
}






