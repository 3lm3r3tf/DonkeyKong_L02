// Fill out your copyright notice in the Description page of Project Settings.


#include "Mono.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/Actor.h"

// Sets default values
AMono::AMono()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    MonoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MonoMesh"));
    RootComponent = MonoMesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MonoMeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone'"));
    if (MonoMeshAsset.Succeeded())
    {
        MonoMesh->SetStaticMesh(MonoMeshAsset.Object);
    }

    MonoMesh->SetSimulatePhysics(true); // Activar simulación de físicas
    MonoMesh->SetMassOverrideInKg(NAME_None, 100.0f); // Ajustar la masa del objeto

    MovimientoVelocidad = 100.0f; // Reducir la velocidad de movimiento
    MovimientoDireccion = FVector(0, -1, 0); // Movimiento en el eje Y

}

// Called when the game starts or when spawned
void AMono::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMono::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    Move(DeltaTime);

}

void AMono::Move(float DeltaTime)
{
    FVector CurrentLocation = GetActorLocation();
    FVector NewLocation = CurrentLocation + (MovimientoDireccion * MovimientoVelocidad * DeltaTime);

    SetActorLocation(NewLocation);
}

void AMono::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    // Cambiar la dirección del movimiento en el eje Y
    MovimientoDireccion.Y *= -1;
}
