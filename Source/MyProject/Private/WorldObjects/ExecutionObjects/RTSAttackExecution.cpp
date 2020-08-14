   // If we're not stunned and our attack rate is filled
   if(!IsStunned() && GetTargetUnit()) {
      // Create a gameplay effect for this
      FGameplayEffectContextHandle context = GetAbilitySystemComponent()->MakeEffectContext();
      context.AddInstigator(this, this);
      FGameplayEffectSpecHandle damageEffectHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(URTSDamageEffect::StaticClass(), 1, context);

      // Set all the effect's custom magnitude values else error is triggered
      UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Strength"), 0);
      UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Intelligence"), 0);
      UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Agility"), 0);
      UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Understanding"), 0);
      UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Health"), 0);

      // Stat used to determine damage depends on attack style
      switch(combatParams.combatStyle) {
         case ECombatType::Melee:
            UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Strength"), 100);
            break;
         case ECombatType::Magic:
            UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Intelligence"), 100);
         case ECombatType::Ranged:
            UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Stats.Agility"), 100);
            break;
         default: UE_LOG(LogTemp, Warning, TEXT("Error, combatstyle tag is not what it should be!")); return;
      }

      // TODO: Should add weapon element here
      UAbilitySystemBlueprintLibrary::AddAssetTag(damageEffectHandle, FGameplayTag::RequestGameplayTag("Combat.Element.Force"));
      GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*damageEffectHandle.Data.Get(), GetTargetUnit()->GetAbilitySystemComponent());
   }

   // Remove invisibility if you attack somebody
   GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.Effect.Invisibility")));