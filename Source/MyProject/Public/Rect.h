// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 *
 */
USTRUCT(BlueprintType, NoExport)
struct FRect {
   float screenX, screenY, width, length;
   FRect(float screenX = 0, float screenY = 0, float width = 0, float length = 0) : screenX(screenX), screenY(screenY), width(width), length(length) {}
   ~FRect() {}
};
