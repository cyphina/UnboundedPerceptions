#pragma once

#include "MinigameTypes.h"
#include "MinigameManager.generated.h"

class AUserInput;
class UUserWidget;
class AMinigamePawn;

/**
 * Class used as an interface for any other part of the game that has to deal with minigames starting and stopping minigames.  These minigames are so small that we typically
 * store all their data in their respective pawns.  They are also all singleplayer - a multiplayer minigame will probably require a new level.
 */

UCLASS(Blueprintable)
class MYPROJECT_API UMinigameManager : public UObject
{
   GENERATED_BODY()

 public:
   void Init();

   UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Minigame Starter")
   void Auth_StartMiniGame(AUserInput* controllerRef, EMinigameType minigameType);

   //UFUNCTION(BlueprintNativeEvent, Category = "Minigame Starter")
   //void StartNurikabe(FMinigameData& minigameData);

 protected:
   /**
   * Most minigames end in a similar manner.  If the player beat the minigame, then activate some triggers.  If the player fails, activate
	 * other trigger, or destroy the Destroy the pawn, possess the main pawn again 
	 */
   UFUNCTION(BlueprintCallable, Category = "Minigame Helper")
   void EndMiniGame(AUserInput* controllerRef);

   /** Quit the game early on client request */
   UFUNCTION(BlueprintCallable, Category = "Minigame Helper")
   void AbortMinigame(AUserInput* controllerRef);

   UPROPERTY(EditDefaultsOnly, Category = "References")
   TSubclassOf<AMinigamePawn> drawingPawn;

   UPROPERTY(EditDefaultsOnly, Category = "References")
   TSubclassOf<AMinigamePawn> nurikabePawn;

   UPROPERTY()
   class AHUDManager* hudManagerRef;

 private:
   void StartTankGame(AUserInput* controllerRef);
   void StartDrawingGame(AUserInput* controllerRef);

   bool HasAuthority() const;
};
