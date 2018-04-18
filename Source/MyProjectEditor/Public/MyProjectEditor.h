#pragma once

#include "Modules/ModuleManager.h"

/* Precompiled header and custom module class for loading customization when created similar to in plugins.*/
DECLARE_LOG_CATEGORY_EXTERN(MyProjectEditor, All, All);

class FMyProjectEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};