// Fill out your copyright notice in the Description page of Project Settings.


#include "explosion.h"

// Sets default values
Aexplosion::Aexplosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Aexplosion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Aexplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UMediaPlayer* Aexplosion::createMediaPlayer() {
	mediaPlayer = NewObject<UMediaPlayer>();
	return mediaPlayer;
}

UMediaTexture* Aexplosion::createMediaTexture() {
	mediaTexture = NewObject<UMediaTexture>(
		GetTransientPackage(),
		UMediaTexture::StaticClass(),
		NAME_None,
		RF_Transient | RF_Public
	);

	// Recommended settings
	mediaTexture->UpdateResource();
	mediaTexture->AddToRoot(); // Prevent GC if needed
	mediaTexture->Filter = TF_Bilinear;
	mediaTexture->ClearColor = FLinearColor::Black;
	mediaTexture->AddressX = TA_Clamp;
	mediaTexture->AddressY = TA_Clamp;

	return mediaTexture;
}

