// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "ShootingEnemy.h"
#include "RTSProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "SpellSystem/GameplayEffects/DamageEffect.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"

void AShootingEnemy::BeginPlay()
{
   Super::BeginPlay();
}

void AShootingEnemy::Attack_Implementation()
{
   if (!IsStunned()) // If we're not stunned and our attack rate is filled
   {
      FTransform      transform  = FTransform{FVector(GetActorLocation().X, GetActorLocation().Y, targetData.targetUnit->GetActorLocation().Z)};
      ARTSProjectile* projectile = GetWorld()->SpawnActorDeferred<ARTSProjectile>(projectileType, transform, this);

      FGameplayEffectContextHandle context = GetAbilitySystemComponent()->MakeEffectContext();
      context.AddInstigator(this, this);
      FGameplayEffectSpecHandle damageEffectHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(UDamageEffect::StaticClass(), 1, context);

      // set all the effect's custom magnitude values else it complains
      UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Strength"), 0);
      UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Intelligence"), 0);
      UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Understanding"), 0);
      UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Agility"), 100);

      UAbilitySystemBlueprintLibrary::AddAssetTag(damageEffectHandle, initialStats.element);
      projectile->hitEffects.Add(damageEffectHandle);
      projectile->canGoThroughWalls = false;
      projectile->targetting        = EBulletTargettingScheme::Bullet_Ally;

      projectile->FinishSpawning(transform);
      projectile->FireInDirection((targetData.targetUnit->GetActorLocation() - FVector(GetActorLocation().X, GetActorLocation().Y, 0)).GetSafeNormal());

   }
}
