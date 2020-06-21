#Script to fill default values Spell Classes and Entries in the Spell DataTable as opposed to having to do it manually

import unreal_engine as ue
from unreal_engine.classes import DataTable
from unreal_engine.classes import Blueprint
from unreal_engine.classes import BlueprintGeneratedClass

asset_path = '/Game/RTS_Tutorial/Tables/SpellList.SpellList'
spell_asset_path = '/Game/RTS_Tutorial/Blueprints/SpellSystem/Spells'

table = ue.load_object(DataTable, asset_path)
spellAssets = ue.get_assets(spell_asset_path, True)

tableDict = table.data_table_as_dict()

ue.begin_transaction('SpellTableConfigure')

for key in tableDict.keys():
    row = tableDict[key]
    rowName = tableDict[key].get_field('name')
    rowName = rowName.replace(" ", "")
    rowName = rowName.replace("'", "")
    
    rowSpellAsset = None    

    for spell in spellAssets:
        if spell.get_name().casefold() == rowName.casefold():
            rowSpellAsset = spell

    if rowSpellAsset:
        print('Class: ' + str(rowSpellAsset.get_class())) #Ensure the object is an actual blueprint, not a redirector
        print('Name: ' + str(rowSpellAsset.get_name()))
    else:
        print("Cant find asset for spell table entry named " + rowName)
    
    spellDefault = rowSpellAsset.GeneratedClass.get_cdo()
    spellStruct = spellDefault.get_property('spellDefaults')
    spellDefault.spellDefaults.set_field('id', int(key))

    #classPath = rowSpellAsset.get_path_name() + '_C'
    #bGenClass = ue.load_object(BlueprintGeneratedClass, classPath)
    spellTableRow = table.data_table_find_row(key)
    spellTableRow.spellClass = rowSpellAsset.GeneratedClass
    table.data_table_remove_row(key)
    table.data_table_add_row(key, spellTableRow)

ue.end_transaction()
    