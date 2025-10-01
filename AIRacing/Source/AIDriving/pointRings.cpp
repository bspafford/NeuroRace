// Fill out your copyright notice in the Description page of Project Settings.


#include "pointRings.h"
#include "car.h"

// Sets default values
ApointRings::ApointRings()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ringMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ring Mesh"));
	SetRootComponent(ringMesh);

    ringMesh->SetGenerateOverlapEvents(true);
    ringMesh->OnComponentBeginOverlap.AddDynamic(this, &ApointRings::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ApointRings::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ApointRings::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ApointRings::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult) {
    if (OtherActor && OtherActor != this)
    {
        float dot = FVector::DotProduct(GetActorForwardVector(), OtherActor->GetActorForwardVector());
        if (dot < 0) {// facing in correct direction
            // add points to the other object
            Acar* car = Cast<Acar>(OtherActor);
            if (car && !car->pointRingsAlreadyHit.Contains(this)) {
                car->points++;
                car->pointRingsAlreadyHit.Add(this);
                car->deathTime = 0.f;

            }
        }
    }
}

