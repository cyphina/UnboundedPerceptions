#pragma once

#include "Framework/Commands/Commands.h"
#include "SlateStyle.h"

class FUpEditorStyle : public FSlateStyleSet
{
 public:
   FUpEditorStyle(); 

   ~FUpEditorStyle();

   static FUpEditorStyle& Get()
   {
      if(!Singleton.IsSet())
      {
         Singleton.Emplace();
      }
      return Singleton.GetValue();
   }

   static void Destroy() { Singleton.Reset(); }

	static FName GetEditorDesignSubMenuIconStyleName() { return "UpEditor.DesignSubMenuIcon"; }
	static FName GetEditorTriggerManagerIconStyleName() { return "UpEditor.TriggerMenuIcon"; }
	static FName GetEditorUpVisIconStyleName() { return "UpEditor.UpVisIcon"; }


 private:
   static TOptional<FUpEditorStyle> Singleton;
};

class FUpCommonCommands : public TCommands<FUpCommonCommands>
{
 public:
   FUpCommonCommands();

   /** Initialize commands */
   virtual void RegisterCommands() override;

   TSharedPtr<FUICommandInfo> OpenLevelTriggerList;
};
