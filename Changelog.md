# Fixed Development Build
* Fixed several warnings about custom node being in an editor module (moved it to a UncookedOnly module)
    * Fixed all the spells that using the old custom node.
* Renamed several assets to follow Up convention
* Removed several unused assets
* Changed RTSAbilitySystemComponent to load default abilities asynchronously when a Unit is spawned in.
* Changed several conditionally compiled functions to WITH_EDITOR.
* Fixed colors in character menu browser.