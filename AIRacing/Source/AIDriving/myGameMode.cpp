// Fill out your copyright notice in the Description page of Project Settings.

#include "myGameMode.h"
#include "car.h"

void AmyGameMode::BeginPlay() {
	Super::BeginPlay();

	Acar::networkData.Empty();
	Acar::generation = 0;

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("beginning game for gamemode"));
}