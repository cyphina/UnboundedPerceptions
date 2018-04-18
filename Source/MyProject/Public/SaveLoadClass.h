#pragma once
#include "Archive.h"
#include "SaveLoadClass.generated.h"

/**
 * Class that deals with saving and loading
 * Saving requires us to store our information in a binary array, and then write that array to disk
 * When writing to the binary array, we have to pass information by reference, which is why we want to copy our information to a struct.
 * For certain data, we can only grab some copies of information.  We can workaround this using friend classes, however
 * we'd have to modify the source code to get certain informatoin like getting the transformation in scene component.
 */

class AUserInput;
class ABasePlayer;
struct FBaseCharacter;

class AAlly;
class ASummon;
class ABaseHero;

/*Definitions of all the structs which we first copy the data we want to save to 
 The structs are declared here since we want them to be class members since we need to setup a callback without any parameters
to FWorldDelegate::OnPostWorldInitialization to properly call the loading setup at the right time since OpenLevel is Async.*/

#pragma region savestructs
//Data about the save itself
struct FSaveGameDataInfo
{
	FName id; //Player can name a save
	FDateTime timestamp; //time when game is saved

	friend FArchive&	operator<<(FArchive& ar, FSaveGameDataInfo& saveDataInfo)
	{
		ar << saveDataInfo.id;
		ar << saveDataInfo.timestamp;
		return ar;
	}
};

//LevelData - Will contain information about level state 
struct FSceneSaveInfo 
{
	FName					levelName;
	TArray<FName>			pickedUpInteractables; //what items have been picked up already? - This may be saved in a struct with all the level data  

	friend FArchive&							operator<<(FArchive& ar, FSceneSaveInfo& saveData)
	{
		ar << saveData.levelName;
		ar << saveData.pickedUpInteractables;
		return ar;
	}
};

struct FCameraSaveInfo 
{
	FTransform				cameraTransform;
	int						cameraSpeed;
	bool					isCamNavDisabled;

	friend FArchive&							operator<<(FArchive& ar, FCameraSaveInfo& saveData)
	{
		ar << saveData.cameraTransform;
		ar << saveData.cameraSpeed;
		ar << saveData.isCamNavDisabled;
		return ar;
	}

	friend void									operator<<(FMemoryReader& ar, FCameraSaveInfo& saveData)
	{
		ar << saveData.cameraTransform;
		ar << saveData.cameraSpeed;
		ar << saveData.isCamNavDisabled;
		
	}
};

struct FBasePlayerSaveInfo
{
	int					money;
	TArray<FName>		dialogTopics;
	int					heroNum;
	int					summonNum;
	int					npcNum;

	friend FArchive&							operator<<(FArchive& ar, FBasePlayerSaveInfo& saveData)
	{
		ar << saveData.money;
		ar << saveData.dialogTopics;
		ar << saveData.heroNum << saveData.summonNum << saveData.npcNum;
		return ar;
	}
};

struct FBaseCharacterSaveInfo
{
	TArray<int>				attributes;
	TArray<int>				skills;
	TArray<int>				vitals;
	TArray<int>				currentVitals;
	TArray<int>				mechanics;
	int						level;

	friend FArchive&		operator<<(FArchive& ar, FBaseCharacterSaveInfo& saveData)
	{
		ar << saveData.attributes;
		ar << saveData.skills;
		ar << saveData.vitals;
		ar << saveData.mechanics;
		ar << saveData.level;
		return ar;
	}
};

struct FAllySaveInfo 
{
	FText name;
	FTransform actorTransform;
	FBaseCharacterSaveInfo baseCSaveInfo;
	//TODO: Save buffs and debuffs

	friend FArchive&							operator<<(FArchive& ar, FAllySaveInfo& saveData)
	{
		ar << saveData.name;
		ar << saveData.actorTransform;
		ar << saveData.baseCSaveInfo;
		return ar;
	}
};

struct FSummonSaveInfo
{
	FAllySaveInfo		allyInfo;
	int					duration;

	friend FArchive&							operator<<(FArchive& ar, FSummonSaveInfo& saveData)
	{
		ar << saveData.allyInfo;
		ar << saveData.duration;
		return ar;
	}
};

struct FHeroSaveInfo
{
	FAllySaveInfo			allyInfo;
	int						currentExp;
	int						expToNextLevel; //we could technically recalculate this...
	int						attPoints;
	int						skillPoints;
	TArray<int>				spellIDs;

	friend FArchive&		operator<<(FArchive& ar, FHeroSaveInfo& saveData)
	{
		ar << saveData.allyInfo;
		ar << saveData.currentExp;
		ar << saveData.expToNextLevel;
		ar << saveData.attPoints;
		ar << saveData.skillPoints;
		ar << saveData.spellIDs;
		return ar; 
	}
};
#pragma endregion

UCLASS()
class MYPROJECT_API USaveLoadClass : public UObject
{
	GENERATED_BODY()

public:

	~USaveLoadClass();

	/**@param filePath: The absolute filepath of the folder we should save the data to*/
	bool							SaveToFilePath(const FString& filePath);
	/**@param filePath: The absolute filepath of the folder we should save the data to*/
	bool							LoadFromFilePath(const FString& filePath);

	AUserInput*						controllerRef;	//Used to print message to client log seen with (~) key
	bool							currentlyLoading; //Check if a load was initiated so level callback knows to deal with it

private:

	FSaveGameDataInfo				gameSaveSaveData;
	FSceneSaveInfo					sceneSaveData;
	FCameraSaveInfo					cameraSaveData;
	FBasePlayerSaveInfo				playerSaveData;
	TArray<FAllySaveInfo>			npcsSaveData;
	TArray<FSummonSaveInfo>			summonsSaveData;
	TArray<FHeroSaveInfo>			heroesSaveData;

	bool							loadSuccess; //Did we sucessfully load data from binary array?  Used to make sure callback triggers only when level is loaded

	//Remember: Passing Information by reference doesn't require storage and doesn't require indirection 
	/**Function used to save and load data from binary array by using the << operator which saves/loads depending on FArchive derived type
	 *@param ar: Archive used to save/load data from the disk to binary array and vice versa*/
	void							SaveLoadFunction(FArchive& ar, bool isSaving);

#pragma region SaveSetup
	
	/**Setup structs used to save data*/
	//TODO: Save settings and load them at game start.  Could be seperate from this system
	void							SetupSaveData();
	/**Setup structs with controller information*/
	void							SetupSaveControllerData();
	/**Save data about player*/
	void							SetupSavePlayerData();
	/**Save data about unit's stats*/
	void							SetupSaveBaseCharacterData(const FBaseCharacter& baseChar, FBaseCharacterSaveInfo& saveInfo);
	/**Save Ally data inside heroes, summons, and NPC esorts*/
	void							SetupSaveAllyData(AAlly& ally, FAllySaveInfo& allyInfo);
	/**Save data about summons in party*/
	void							SetupSaveSummonData();
	/**Save data about heroes in party*/
	void							SetupSaveHeroData();
	/**Save data about npc escorts in party*/
	void							SetupNPCEscortData();

	/**Actually set the actor's data values according to what is loaded*/
	
#pragma endregion

#pragma region LoadedDataSetup
public:
	void								SetupLoadedData();
	inline void							SetupController();
	inline void							SetupPlayer();
	inline void							SetupAlliedUnits();
	inline void							SetupBaseCharacter(AAlly* spawnedAlly, FBaseCharacterSaveInfo& baseCSaveInfo);
	
	/**This function can be called when a level loads, but it will only trigger after a player-initiated load*/
	void								SetupLoad();
#pragma endregion
};

/**Used so we can serialize objects and prefix binary data with a string so it won't become corrupted if fields are added or removed*/
struct FSaveGameArchive : public FObjectAndNameAsStringProxyArchive
{
	FSaveGameArchive(FArchive& inInnerArchive) : FObjectAndNameAsStringProxyArchive(inInnerArchive, true)
	{
		ArIsSaveGame = true;
	}
};