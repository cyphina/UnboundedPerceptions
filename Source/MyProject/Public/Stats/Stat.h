// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Stats/Attribute.h"
#include "AttributeSet.h"
namespace CombatInfo
{
	 //function pointer to a function that maps the attribute to how much it corresponds to the modified stat
	using EFF = int(*)(int);

	//Modifying attribute represents relationship between stats and attribute
	struct ModifyingAttribute
	{
		FGameplayAttributeData* attribute; //Attribute the stat with this modifyingattribute attachment will scale off of
		EFF effectRatio;

		/// <param name="att"> BaseAttribute </param>
		/// <param name="eff"> Effect Ratio </param>

		ModifyingAttribute(FGameplayAttributeData* att) : attribute(att)
		{
			effectRatio = nullptr;
		}

		ModifyingAttribute(FGameplayAttributeData* att, EFF eff) : attribute(att)
		{
			effectRatio = eff;
		}
	};

	//Stats are skills that increase with attributes.  From the outside, they look like attributes
	class MYPROJECT_API Stat
	{
	public:
		ModifyingAttribute attMod;
		FGameplayAttributeData& bA; //Since a stat can be directly modified by things like equipment and buffs, we need it to have features of an attribute as well

		Stat(FGameplayAttributeData& attData);
		/// <param name="attData"> Link to AttSet  </param>
		/// <param name="baseV"> Initial Base Value </param>
		/// <param name="mod"> Mod Value </param>
		Stat(FGameplayAttributeData& attData, int baseV, ModifyingAttribute mod);
		~Stat();

		//allowing us to change what modifies what  
		void ChangeModifier(ModifyingAttribute arr);
		void CalculateModValue(); //sets up base value for this stat
		float GetBaseValue() const; 
		void SetBaseValue(float value);
		float GetBuffValue() const; //Can be negative if we're debuffed
		void SetBuffValue(float value);
		float GetAdjustedValue() const;

		void Update();
	};
}