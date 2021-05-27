#pragma once

#include "Modules/ModuleManager.h"

/* Precompiled header and custom module class for loading customization when created similar to in plugins.
 * Some hot reloading errors.  Here's what I found on the issue, and why we have to run run.bat else we get the GIsHotReload error:
 *
 * Since the editor module is not hot-reloaded, it is not considered to be compatible with the latest (hot-reloaded) game modules.
 *
 * Hot-reload only effects classes derived from UCLASS types.
 * If your custom module uses classes that are not UCLASS, these classes are not being updated with the hot-reload.
 * As you mentioned, compiling without a hot-reload will update everything at the same time and avoid the error.
 * Though it is not a plugin module, an example of this can be found here: https://issues.unrealengine.com/issue/UE-25350 .
 */

class FMyProjectDeveloperHelpersModule : public IModuleInterface
{
 public:
   virtual void StartupModule() override;
   virtual void ShutdownModule() override;
};
