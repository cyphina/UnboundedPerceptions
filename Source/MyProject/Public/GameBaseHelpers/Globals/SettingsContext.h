#pragma once
#include "GameInstanceSubsystem.h"
#include "SettingsContext.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKeybindsChanged, FInputActionKeyMapping, NewKeyMap);

UCLASS()
class MYPROJECT_API USettingsContext : public UGameInstanceSubsystem
{
   GENERATED_BODY()

public:
   FOnKeybindsChanged OnKeybindsChanged() { return OnKeybindsChangedEvent; }

protected:
   UPROPERTY(BlueprintAssignable)
   FOnKeybindsChanged OnKeybindsChangedEvent;
};
