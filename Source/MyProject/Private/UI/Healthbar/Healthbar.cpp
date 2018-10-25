// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Healthbar.h"

void UHealthbar::SetHealth(float value)
{
   healthPercent = value;
}

float UHealthbar::GetHealth() const
{
   return healthPercent;
}
