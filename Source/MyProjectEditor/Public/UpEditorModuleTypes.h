#pragma once
#include "ModuleManager.h"

/** Allows us to split up our module's logic into separate parts */
class IUpEditorSubmodule
{
 public:
   virtual void OnStartupModule() {}
   virtual void OnShutdownModule() {}
};

/** Holds all the logic for the editor module. Consists of several submodules for organization (as opposed to creating several editor modules */
class IUpEditorModule : public IModuleInterface
{
 public:
   void StartupModule() override
   {
      if(!IsRunningCommandlet())
      {
         AddModuleListeners();
         for(auto Submodule : submodules)
         {
            Submodule->OnStartupModule();
         }
      }
   }

   void ShutdownModule() override
   {
      if(!IsRunningCommandlet())
      {
         for(auto Submodule : submodules)
         {
            Submodule->OnShutdownModule();
         }
      }
   }

   virtual void AddModuleListeners() {}

 protected:
   TArray<TSharedRef<IUpEditorSubmodule>> submodules;
};
