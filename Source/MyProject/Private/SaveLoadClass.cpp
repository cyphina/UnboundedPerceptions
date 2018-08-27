// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "SaveLoadClass.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "WorldObjects/Ally.h"
#include "WorldObjects/Summon.h"
#include "SpellSystem/MySpell.h"
#include "Stats/BaseCharacter.h"
#include "WorldObjects/BaseHero.h"
#include "AssetRegistryModule.h"
#include "ResourceManager.h"
#include "RTSGameMode.h"

USaveLoadClass::~USaveLoadClass()
{
}

void USaveLoadClass::Init()
{
	controllerRef = Cast<AUserInput>(GetOuter()->GetWorld()->GetFirstPlayerController());
}

//TODO: Make struct with collection of level data (data on each level), struct with collection of quest data, struct with collection of trigger information

void USaveLoadClass::SetupSaveData()
{
	//TODO: Let player input id
	//saveGameData->id = 

	//Append SaveGameInfo at the beginning of the save file, or read it when being loaded first.  Should be read when startup to show times of all the saves and loads
	gameSaveSaveData.timestamp = FDateTime::Now();

	SetupSaveControllerData();
	SetupSavePlayerData();
	SetupSaveHeroData();
	SetupSaveSummonData();
	SetupNPCEscortData();
}

void USaveLoadClass::SetupSaveControllerData() 
{
	//Save Scene
	sceneSaveData.levelName = *controllerRef->GetGameMode()->GetCurLevelName();
	//TODO: Save scene interactables
	//Save Camera
	cameraSaveData.cameraTransform = controllerRef->GetPawn()->GetTransform();
	cameraSaveData.cameraSpeed = controllerRef->camMoveSpeedMultiplier;
	cameraSaveData.isCamNavDisabled = controllerRef->isCamNavDisabled;

}

void USaveLoadClass::SetupSavePlayerData()
{
	ABasePlayer* basePlayer = controllerRef->GetBasePlayer();
	playerSaveData.money = basePlayer->money;
	playerSaveData.heroNum = basePlayer->heroes.Num();
	playerSaveData.summonNum = basePlayer->summons.Num();
	playerSaveData.npcNum = basePlayer->npcs.Num();

	for(auto it = basePlayer->GetDialogTopics().CreateConstIterator(); *it == basePlayer->GetDialogTopics().Last(); ++it)
	{
		playerSaveData.dialogTopics.Add(it->GetTagName());
	}
}

void USaveLoadClass::SetupSaveBaseCharacterData(const FBaseCharacter& baseChar, FBaseCharacterSaveInfo& saveInfo)
{
	for(FGameplayAttributeData* attData : baseChar.GetAttributes())
	{
		saveInfo.attributes.Add(attData->GetBaseValue());
	}
	for(Stat stat : baseChar.GetSkills())
	{
		saveInfo.skills.Add(stat.GetBaseValue());
	}
	for(Vital vital : baseChar.GetVitals())
	{
		saveInfo.currentVitals.Add(vital.GetCurrValue());
		saveInfo.vitals.Add(vital.GetBaseValue());
	}
	for(FGameplayAttributeData* attData : baseChar.GetMechanics())
	{
		saveInfo.mechanics.Add(attData->GetBaseValue());
	}
	saveInfo.level = baseChar.GetLevel();
}

void USaveLoadClass::SetupSaveAllyData(AAlly& ally, FAllySaveInfo& allyInfo)
{
	allyInfo.name = ally.GetGameName();
	allyInfo.actorTransform = ally.GetTransform();
	//TODO: Move image data to DataAsset so we can reconfigure it when we load character

	SetupSaveBaseCharacterData(ally.GetBaseCharacter(), allyInfo.baseCSaveInfo);
}

void USaveLoadClass::SetupSaveSummonData()
{
	//Save data for every summon
	int i = 0;
	for(ASummon* summonRef : controllerRef->GetBasePlayer()->summons)
	{
		summonsSaveData.Emplace();
		SetupSaveAllyData(*summonRef, summonsSaveData[i].allyInfo);
		summonsSaveData[i].duration = summonRef->timeLeft;
		++i;
	}
}

void USaveLoadClass::SetupSaveHeroData()
{
	for(int i = 0; i < playerSaveData.heroNum; ++i)
	{
		if(ABaseHero* heroRef = controllerRef->GetBasePlayer()->heroes[i]) // NULL check because heroes is always length 4 and can have some empty entries				
		{
			heroesSaveData.Emplace();	
			SetupSaveAllyData(*heroRef, heroesSaveData[i].allyInfo);
			heroesSaveData[i].currentExp = heroRef->GetCurrentExp();
			heroesSaveData[i].expToNextLevel = heroRef->GetExpToLevel();
			heroesSaveData[i].attPoints = heroRef->attPoints;
			heroesSaveData[i].skillPoints = heroRef->skillPoints;

			//save our spells	 
			for(TSubclassOf<UMySpell> spell : heroRef->abilities)
			{	
				//TODO: Properly setup namespace and keys for each spell in table
				if(spell.GetDefaultObject())
					heroesSaveData[i].spellIDs.Add(spell.GetDefaultObject()->spellDefaults.id);
			}

			heroRef->backpack->SaveBackpack(heroesSaveData[i].backpackInfo);
		}
	}

	//TODO: Save these things as well!
	//save inventory
	//save equipment
	//save spellbook
}

void USaveLoadClass::SetupNPCEscortData()
{
	int i = 0;
	for(AAlly* npcRef : controllerRef->GetBasePlayer()->npcs)
	{
		npcsSaveData.Emplace();
		SetupSaveAllyData(*npcRef, npcsSaveData[i]);
		++i;
	}
}

void USaveLoadClass::SetupLoad()
{
	SetupController();
	SetupPlayer();
	SetupAlliedUnits();
}

void USaveLoadClass::SetupController()
{
	GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Red, TEXT("Testing to see if code can be called after level load!"));
	controllerRef = Cast<AUserInput>(GetOuter()->GetWorld()->GetFirstPlayerController());
	controllerRef->GetPawn()->SetActorTransform(cameraSaveData.cameraTransform);
	controllerRef->camMoveSpeedMultiplier = cameraSaveData.cameraSpeed;
	controllerRef->isCamNavDisabled = cameraSaveData.isCamNavDisabled;
}

void USaveLoadClass::SetupPlayer()
{
	for(FName dialogTopic : playerSaveData.dialogTopics)
	{
		controllerRef->GetBasePlayer()->LearnDialogTopic(FGameplayTag::RequestGameplayTag(dialogTopic));
	}

	Cast<ABasePlayer>(GetOuter()->GetWorld()->GetGameState()->PlayerArray[0])->money = playerSaveData.money;
}

void USaveLoadClass::SetupAlliedUnits()
{
	FActorSpawnParameters spawnParams;

	for(FAllySaveInfo npc : npcsSaveData)
	{
		if(AAlly* spawnedNPCAlly = ResourceManager::FindTriggerObjectInWorld<AAlly>(*npc.name.ToString(), controllerRef->GetWorld()))
		{
			spawnedNPCAlly->SetActorTransform(npc.actorTransform);
			SetupBaseCharacter(spawnedNPCAlly, npc.baseCSaveInfo);
			spawnedNPCAlly->Stop();
		}
		else
		{
			FAssetRegistryModule& assetReg = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
			//TArray<FAssetData> allyAssets; 
			//checkf(assetReg.Get().GetAssetsByPath("/Game/RTS_Tutorial/Blueprints/Actors/WorldObjects",allyAssets,false), TEXT("Cannot find ally assets to load, check validity of installation!"));
			//allyAssets.FindByPredicate([&](FAssetData assetData){ return *npc.name.ToString() == assetData.AssetName ;})->GetAsset();
			//!!!---INVARIANT: Names of files of characters must be the same name as their name in the game--!!!
			FAssetData npcAllyAsset = assetReg.Get().GetAssetByObjectPath(*(FString("/Game/RTS_Tutorial/Blueprints/Actors/WorldObjects/") + npc.name.ToString()));
			spawnedNPCAlly = controllerRef->GetWorld()->SpawnActorDeferred<AAlly>(npcAllyAsset.GetAsset()->GetClass(), npc.actorTransform);
			SetupBaseCharacter(spawnedNPCAlly, npc.baseCSaveInfo);
			spawnedNPCAlly->FinishSpawning(npc.actorTransform);
		}
	}

	for(FHeroSaveInfo hero : heroesSaveData)
	{
		if(ABaseHero* spawnedHero = ResourceManager::FindTriggerObjectInWorld<ABaseHero>(*hero.allyInfo.name.ToString(), controllerRef->GetWorld()))
		{
			spawnedHero->SetActorTransform(hero.allyInfo.actorTransform);
			SetupBaseCharacter(spawnedHero, hero.allyInfo.baseCSaveInfo);	
			for(int i = 0; i < hero.spellIDs.Num(); ++i)
			{
#if UE_EDITOR
				//conditionally compiled because in the real game we should make sure these spell classes exist before hand and shouldn't need to check
				//technically indexer checks but it crashes when it can't find key
				if(USpellManager::Get().spellClasses.Contains(hero.spellIDs[i]))
#endif
					spawnedHero->abilities[i] = USpellManager::Get().spellClasses[hero.spellIDs[i]];
			}
			spawnedHero->attPoints = hero.attPoints;
			spawnedHero->SetCurrentExp(hero.currentExp);
			spawnedHero->expForLevel = hero.expToNextLevel;
			spawnedHero->Stop();

			for(int i = 0; i < hero.backpackInfo.itemIDs.Num(); ++i)
			{
				spawnedHero->backpack->AddItemToSlot(FMyItem(hero.backpackInfo.itemIDs[i], hero.backpackInfo.itemCounts[i]), hero.backpackInfo.itemSlots[i]);
			}
		}
		else
		{
			FAssetRegistryModule& assetReg = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
			FAssetData heroAsset = assetReg.Get().GetAssetByObjectPath(*(FString("/Game/RTS_Tutorial/Blueprints/Actors/WorldObjects/Allies") + hero.allyInfo.name.ToString()));
			spawnedHero = controllerRef->GetWorld()->SpawnActorDeferred<ABaseHero>(heroAsset.GetAsset()->GetClass(), hero.allyInfo.actorTransform);
			SetupBaseCharacter(spawnedHero, hero.allyInfo.baseCSaveInfo);
			for(int i = 0; i < hero.spellIDs.Num(); ++i)
			{
				spawnedHero->abilities[i] = USpellManager::Get().spellClasses[hero.spellIDs[i]];
			}
			spawnedHero->attPoints = hero.attPoints;
			spawnedHero->SetCurrentExp(hero.currentExp);
			spawnedHero->expForLevel = hero.expToNextLevel;
			spawnedHero->FinishSpawning(hero.allyInfo.actorTransform);

			for(int i = 0; i < hero.backpackInfo.itemIDs.Num(); ++i)
			{
				spawnedHero->backpack->AddItemToSlot(FMyItem(hero.backpackInfo.itemIDs[i], hero.backpackInfo.itemCounts[i]), hero.backpackInfo.itemSlots[i]);
			}
		}
	}
	
	for(FSummonSaveInfo summon : summonsSaveData)
	{
		if(ASummon* spawnedSummon = ResourceManager::FindTriggerObjectInWorld<ASummon>(*summon.allyInfo.name.ToString(), controllerRef->GetWorld()))
		{
			spawnedSummon->SetActorTransform(summon.allyInfo.actorTransform);
			SetupBaseCharacter(spawnedSummon, summon.allyInfo.baseCSaveInfo);	
			spawnedSummon->timeLeft = summon.duration;
			spawnedSummon->Stop();
		}
		else
		{
			FAssetRegistryModule& assetReg = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
			FAssetData summonAsset = assetReg.Get().GetAssetByObjectPath(*(FString("/Game/RTS_Tutorial/Blueprints/Actors/WorldObjects/") + summon.allyInfo.name.ToString()));
			spawnedSummon = controllerRef->GetWorld()->SpawnActorDeferred<ASummon>(summonAsset.GetAsset()->GetClass(), summon.allyInfo.actorTransform);
			spawnedSummon->SetActorTransform(summon.allyInfo.actorTransform);
			SetupBaseCharacter(spawnedSummon, summon.allyInfo.baseCSaveInfo);	
			spawnedSummon->timeLeft = summon.duration;
			spawnedSummon->FinishSpawning(summon.allyInfo.actorTransform);
		}
	}
}

void USaveLoadClass::SetupBaseCharacter(AAlly* spawnedAlly, FBaseCharacterSaveInfo& baseCSaveInfo)
{
	for(int i = 0; i < baseCSaveInfo.attributes.Num(); ++i)
	{
		spawnedAlly->baseC->GetAttribute(i)->SetBaseValue(baseCSaveInfo.attributes[i]);
	}
	for(int i = 0; i < baseCSaveInfo.skills.Num(); ++i)
	{
		spawnedAlly->baseC->GetSkill(i)->SetBaseValue(baseCSaveInfo.skills[i]);
	}
	for(int i = 0; i < baseCSaveInfo.vitals.Num(); ++i)
	{
		spawnedAlly->baseC->GetVital(i)->SetBaseValue(baseCSaveInfo.vitals[i]);
	}
	for(int i = 0; i < baseCSaveInfo.mechanics.Num(); ++i)
	{
		spawnedAlly->baseC->GetMechanic(i)->SetBaseValue(baseCSaveInfo.mechanics[i]);
	}
}

void USaveLoadClass::SaveLoadFunction(FArchive& ar, bool isSaving)
{
	/*Slow since we have to construct and assign an empty copy, but this way is safer than the alternatives which could involve looping with manually inputted indices
	*and having to redelete information 
	*TODO: Possible Optimizations
	*Save defaults and just copy the default values so we don't have to reconstruct 
	*Use arrays of preset sizes that will never overflow and keep overwriting and reading to a section of the data to prevent reconstruction*/

	gameSaveSaveData = FSaveGameDataInfo();
	sceneSaveData = FSceneSaveInfo();
	cameraSaveData = FCameraSaveInfo();
	playerSaveData = FBasePlayerSaveInfo();
	heroesSaveData = TArray<FHeroSaveInfo>();
	summonsSaveData = TArray<FSummonSaveInfo>();
	npcsSaveData = TArray<FAllySaveInfo>();

	if(isSaving)
		SetupSaveData();
	else
		SetupLoad();

	ar << gameSaveSaveData;
	ar << sceneSaveData;
	ar << cameraSaveData;
	ar << playerSaveData;
	ar << heroesSaveData;
	ar << summonsSaveData;
	ar << npcsSaveData;
}

bool USaveLoadClass::SaveToFilePath(const FString& filePath)
{
	FBufferArchive binaryArray;

	SaveLoadFunction(binaryArray,true);

	if(binaryArray.Num() <= 0 ) return false;

	if(FFileHelper::SaveArrayToFile(binaryArray, *filePath))
	{
		binaryArray.FlushCache(); 
		binaryArray.Empty();
		//Client message from Controller
		controllerRef->ClientMessage("Save Success!", NAME_None, 2.f);
		return true;
	}

	//Free Binary ARray
	binaryArray.FlushCache();
	binaryArray.Empty();
	controllerRef->ClientMessage("File Could Not Be Saved!", NAME_None, 2.f);
	return false;
}

bool USaveLoadClass::LoadFromFilePath(const FString& filePath)
{
	TArray<uint8> binaryArray;
	if(!FFileHelper::LoadFileToArray(binaryArray, *filePath))
	{
		controllerRef->ClientMessage("FFILEHELPER:>> Invalid File");
		return false;
	}

	//Testing
	controllerRef->ClientMessage("Loaded file size", NAME_None, 2.f);
	controllerRef->ClientMessage(FString::FromInt(binaryArray.Num()), NAME_None, 2.f);

	if(binaryArray.Num() <= 0) return false;
	FMemoryReader fromBinary = FMemoryReader(binaryArray, true); //free data after done
	fromBinary.Seek(0); 

	SaveLoadFunction(fromBinary,false);

	fromBinary.FlushCache();
	binaryArray.Empty();
	fromBinary.Close();

	controllerRef->GetGameMode()->StreamLevelAsync(sceneSaveData.levelName);
	return true;
}
