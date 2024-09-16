// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APortal::APortal()
{
    PrimaryActorTick.bCanEverTick = true;

    // Crear y configurar el componente de colisión
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetBoxExtent(FVector(50.0f, 50.0f, 100.0f));
    RootComponent = CollisionBox;

    // Crear y configurar el componente de malla
    PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
    PortalMesh->SetupAttachment(RootComponent);

    // Configurar la detección de colisiones
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapBegin);

    // Asignar la malla estática
    static ConstructorHelpers::FObjectFinder<UStaticMesh> PortalMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
    if (PortalMeshAsset.Succeeded())
    {
        PortalMesh->SetStaticMesh(PortalMeshAsset.Object);
        PortalMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
        PortalMesh->SetWorldScale3D(FVector(1.0f, 1.0f, 0.1f));
        PortalMesh->SetCollisionProfileName(TEXT("PortalCollision"));
    }
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
    Super::BeginPlay();

    // Asignar el destino programáticamente
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        if (Actor->GetName() == TEXT("NombreDelActorDestino"))
        {
            Destino = Actor;
            break;
        }
    }
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this) && OtherComp && Destino)
    {
        ACharacter* Character = Cast<ACharacter>(OtherActor);
        if (Character)
        {
            Character->SetActorLocation(Destino->GetActorLocation());
        }
    }
}

