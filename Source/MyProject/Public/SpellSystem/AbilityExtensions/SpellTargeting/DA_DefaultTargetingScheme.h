// Created 1/3/21 3:56 PM

#pragma once
#include "Engine/DataAsset.h"
#include "DA_DefaultTargetingScheme.generated.h"

class UEnvQuery;

/**
 * @brief Holds default target generation strategies for the most basic of targeting schemes.
 * For more complicated interactions you should try and setup your own custom schemes in the CastSpell BTTask nodes.
 */
UCLASS(Abstract)
class MYPROJECT_API UDA_DefaultTargetingScheme : public UDataAsset
{
   GENERATED_BODY()

 public:
   virtual UEnvQuery* GetSingleUnitTargetQuery() const PURE_VIRTUAL(UDA_DefaultTargetingScheme::GetSingleUnitTargetQuery, return nullptr; );

   virtual UEnvQuery* GetRadiusTargetQuery() const PURE_VIRTUAL(UDA_DefaultTargetingScheme::GetRadiusTargetQuery, return nullptr; );;

   virtual UEnvQuery* GetLineTargetQuery() const PURE_VIRTUAL(UDA_DefaultTargetingScheme::GetLineTargetQuery, return nullptr; );;
};

UCLASS()
class MYPROJECT_API UDA_DefaultOffensiveTargetingScheme : public UDA_DefaultTargetingScheme
{
   GENERATED_BODY()

 public:
   UEnvQuery* GetSingleUnitTargetQuery() const override { return DefaultOffensiveSingleUnitTargetQuery; }

   UEnvQuery* GetRadiusTargetQuery() const override { return DefaultOffensiveAOETargetQuery; }

   UEnvQuery* GetLineTargetQuery() const override { return DefaultOffensiveLineTargetQuery; }

 protected:
   UPROPERTY(EditDefaultsOnly)
   UEnvQuery* DefaultOffensiveSingleUnitTargetQuery;

   UPROPERTY(EditDefaultsOnly)
   UEnvQuery* DefaultOffensiveAOETargetQuery;

   UPROPERTY(EditDefaultsOnly)
   UEnvQuery* DefaultOffensiveLineTargetQuery;
};

UCLASS()
class MYPROJECT_API UDA_DefaultSupportTargetingScheme : public UDA_DefaultTargetingScheme
{
   GENERATED_BODY()

 public:
   UEnvQuery* GetSingleUnitTargetQuery() const override { return DefaultSupportSingleUnitTargetQuery; }

   UEnvQuery* GetRadiusTargetQuery() const override { return DefaultSupportAOETargetQuery; }

   UEnvQuery* GetLineTargetQuery() const override { return DefaultSupportLineTargetQuery; }

 protected:
   UPROPERTY(EditDefaultsOnly)
   UEnvQuery* DefaultSupportSingleUnitTargetQuery;

   UPROPERTY(EditDefaultsOnly)
   UEnvQuery* DefaultSupportAOETargetQuery;

   UPROPERTY(EditDefaultsOnly)
   UEnvQuery* DefaultSupportLineTargetQuery;
};

UCLASS()
class MYPROJECT_API UDA_DefaultPurgeTargetingScheme : public UDA_DefaultTargetingScheme
{
   GENERATED_BODY()

 public:
   UEnvQuery* GetSingleUnitTargetQuery() const override { return DefaultPurgeSingleUnitTargetQuery; }

   UEnvQuery* GetRadiusTargetQuery() const override { return DefaultPurgeAOETargetQuery; }

   UEnvQuery* GetLineTargetQuery() const override { return DefaultPurgeLineTargetQuery; }

 protected:
   UPROPERTY(EditDefaultsOnly)
   UEnvQuery* DefaultPurgeSingleUnitTargetQuery;

   UPROPERTY(EditDefaultsOnly)
   UEnvQuery* DefaultPurgeAOETargetQuery;

   UPROPERTY(EditDefaultsOnly)
   UEnvQuery* DefaultPurgeLineTargetQuery;
};

UCLASS()
class MYPROJECT_API UDA_DefaultDisableTargetingScheme : public UDA_DefaultTargetingScheme
{
   GENERATED_BODY()

 public:
   UEnvQuery* GetSingleUnitTargetQuery() const override { return DefaultDisableSingleUnitTargetQuery; }

   UEnvQuery* GetRadiusTargetQuery() const override { return DefaultDisableAOETargetQuery; }

   UEnvQuery* GetLineTargetQuery() const override { return DefaultDisableLineTargetQuery; }

 protected:
   UPROPERTY(EditDefaultsOnly)
   UEnvQuery* DefaultDisableSingleUnitTargetQuery;

   UPROPERTY(EditDefaultsOnly)
   UEnvQuery* DefaultDisableAOETargetQuery;

   UPROPERTY(EditDefaultsOnly)
   UEnvQuery* DefaultDisableLineTargetQuery;
};
