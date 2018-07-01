// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//abstract class
//minimalAPI
class MYPROJECT_API IWorldObject
{
protected:
	bool isSelected = false; //everything can be selected, but selection of non hero worldobjects will do different things like just display information
	int objectID = 0;

public:	
	/**Get name as referred to by game UI.  May be unique (if I'm lazy it will be, else we can use gameplayTags as another unique ID)*/
	virtual void					SetGameName(FText value) = 0;
	virtual FText					GetGameName() const = 0 ;

	/**Get image that is shown when a WorldObject is selected*/
	virtual void					SetImage(UTexture2D* value) = 0;
	virtual UTexture2D*				GetImage() const = 0;

	/**Select a WorldObject*/
	virtual void					SetSelected(bool value ) = 0;
	virtual bool					GetSelected() const = 0;

	virtual void					SetObjectID(int value) = 0;
	virtual int						GetObjectID() const = 0;

	/**Field to see if a WorldObject is selectable*/
	virtual void					SetCanTarget(bool value) = 0;
	virtual bool					GetCanTarget() const = 0;

	virtual ~IWorldObject() {}
};
