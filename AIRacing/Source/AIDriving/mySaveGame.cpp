// Fill out your copyright notice in the Description page of Project Settings.


#include "mySaveGame.h"
#include "car.h"

void UmySaveGame::setSaveVariables() {
	networkData = Acar::networkData;
	generation = Acar::generation;

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("networkData: %d, generation: %d"), Acar::networkData.Num(), Acar::generation));
}

void UmySaveGame::giveSaveVariables() {
	Acar::networkData = networkData;
	Acar::generation = generation;

	
	//AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("networkData: %d, generation: %d"), Acar::networkData.Num(), Acar::generation));
}