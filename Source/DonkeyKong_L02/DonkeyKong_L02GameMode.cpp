// Copyright Epic Games, Inc. All Rights Reserved.

#include "DonkeyKong_L02GameMode.h"
#include "DonkeyKong_L02Character.h"
#include "UObject/ConstructorHelpers.h"
#include "ObstaculoMuro.h"
#include "Capsula.h"
#include "componentePlataforma.h"
#include "Barril.h"
#include "Esfera.h"
#include "Portal.h"
#include "Kismet/GameplayStatics.h"

ADonkeyKong_L02GameMode::ADonkeyKong_L02GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ADonkeyKong_L02GameMode::BeginPlay()
{
    Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, TEXT("Creando plataforma"));
	// Spawn an instance of the AMyFirstActor class at the
	//default location.

	FTransform SpawnLocation;
	SpawnLocation.SetLocation(FVector(1160.0f, -210.0f, 140.0f));
	SpawnLocation.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
	obstaculo01 = GetWorld()->SpawnActor<AObstaculoMuro>(AObstaculoMuro::StaticClass(), SpawnLocation);
	/*if (player01 != nullptr)
	{
		player01->SetObstaculo(obstaculo01);
	}*/
	//comienza la parte de la creación de la componete de  plataforma
	FVector posicionInicial = FVector(1160.0f, -1100.0f, 400.f);
	FRotator rotacionInicial = FRotator(0.0f, 0.0f, 10.0f);
	FTransform SpawnLocationCP;
	float anchoComponentePlataforma = 600.0f;
	float incrementoAltoComponentePlataforma = -105.0f;
	float incrementoAltoEntrePisos = 800.0f;
	float incrementoInicioPiso = 100.0f;

	for (int npp = 0; npp < 5; npp++) {
		rotacionInicial.Roll = rotacionInicial.Roll * -1;
		incrementoInicioPiso = incrementoInicioPiso * -1;
		incrementoAltoComponentePlataforma = incrementoAltoComponentePlataforma * -1;
		SpawnLocationCP.SetRotation(FQuat(rotacionInicial));

		for (int ncp = 0; ncp < 5; ncp++) {
			if (ncp == 0 || ncp == 1 || ncp == 4) {
				float ajusteY = anchoComponentePlataforma * ncp;
				SpawnLocationCP.SetLocation(FVector(posicionInicial.X, posicionInicial.Y + ajusteY, posicionInicial.Z));
				AcomponentePlataforma* plataforma = GetWorld()->SpawnActor<AcomponentePlataforma>(AcomponentePlataforma::StaticClass(), SpawnLocationCP);
				componentesPlataforma.Add(plataforma);

				if (ncp == 1) {
					FVector posicionObjetivo = FVector(posicionInicial.X, posicionInicial.Y + anchoComponentePlataforma * 3, posicionInicial.Z);
					plataforma->IniciarMovimiento(posicionObjetivo);
				}

				if (ncp < 4) {
					posicionInicial.Z = posicionInicial.Z + incrementoAltoComponentePlataforma;
				}
			}
		}

		posicionInicial.Z = posicionInicial.Z + incrementoAltoEntrePisos;
		posicionInicial.Y = posicionInicial.Y + incrementoInicioPiso;
	}

	//FVector posicionInicial = FVector(1160.0f, -1100.0f, 400.f);
	//FRotator rotacionInicial = FRotator(0.0f, 0.0f, 0.0f); // Iniciamos sin inclinación
	//FTransform SpawnLocationCP;
	//float anchoComponentePlataforma = 600.0f;
	//float incrementoAltoComponentePlataforma = -105.0f;
	//float incrementoAltoEntrePisos = 800.0f;
	//float incrementoInicioPiso = 100.0f;

	//// Bucle para crear los pisos de plataformas
	//for (int npp = 0; npp < 5; npp++) {
	//	// Si no es el primer piso, comenzamos con la inclinación alternada
	//	if (npp != 0) {
	//		rotacionInicial.Roll = rotacionInicial.Roll * -1; // Alternamos la inclinación
	//		incrementoInicioPiso = incrementoInicioPiso * -1; // Alternamos el desplazamiento
	//		incrementoAltoComponentePlataforma = incrementoAltoComponentePlataforma * -1; // Cambiamos la altura de la plataforma
	//	}

	//	SpawnLocationCP.SetRotation(FQuat(rotacionInicial));

	//	// Bucle para crear las plataformas dentro de un piso
	//	for (int ncp = 0; ncp < 5; ncp++) {
	//		SpawnLocationCP.SetLocation(FVector(posicionInicial.X, posicionInicial.Y + anchoComponentePlataforma * ncp, posicionInicial.Z));
	//		componentesPlataforma.Add(GetWorld()->SpawnActor<AcomponentePlataforma>(AcomponentePlataforma::StaticClass(), SpawnLocationCP));

	//		if (ncp < 4) {
	//			posicionInicial.Z = posicionInicial.Z + incrementoAltoComponentePlataforma; // Ajustamos la altura para la siguiente plataforma
	//		}
	//	}

	//	// Preparamos la posición inicial para el siguiente piso
	//	posicionInicial.Z = posicionInicial.Z + incrementoAltoEntrePisos; // Subimos de piso
	//	posicionInicial.Y = posicionInicial.Y + incrementoInicioPiso; // Ajustamos el desplazamiento horizontal
	//}

	GetWorld()->GetTimerManager().SetTimer(SpawnBarrilTimerHandle, this, &ADonkeyKong_L02GameMode::SpawnBarril, 3.0f, true);
	
	//Crear una esfera en la parte superior del primer piso que baje rebotando hasta llegar al suelo
	
	for (int nes = 0; nes < 5; nes++) {
		FTransform SpawnLocationEsfera;
		SpawnLocationEsfera.SetLocation(FVector(1300.0f, -200.0f, 1060.0f));
		SpawnLocationEsfera.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
		esferasMap.Add(nes, GetWorld()->SpawnActor<AEsfera>(AEsfera::StaticClass(), SpawnLocationEsfera));
	}

	// Inicializar el portal
	InitializePortal();
}

void ADonkeyKong_L02GameMode::SpawnBarril()
{
	//if (barriles.Num() >= numeroMaximoBarriles) {
	if (contadorBarriles < numeroMaximoBarriles) {
		//Spawn de un objeto barril en la escena sobre la primera plataforma
		FTransform SpawnLocationBarril;
		SpawnLocationBarril.SetLocation(FVector(1160.0f, 900.0f, 860.0f));
		SpawnLocationBarril.SetRotation(FQuat(FRotator(90.0f, 0.0f, 0.0f)));
		//ABarril* barril01 = GetWorld()->SpawnActor<ABarril>(ABarril::StaticClass(), SpawnLocationBarril);
		barriles.Add(GetWorld()->SpawnActor<ABarril>(ABarril::StaticClass(), SpawnLocationBarril));
		contadorBarriles++;

		if (contadorBarriles >= numeroMaximoBarriles) {
			GetWorld()->GetTimerManager().ClearTimer(SpawnBarrilTimerHandle);
		}
	}
}
void ADonkeyKong_L02GameMode::InitializePortal()
{ 
	// Posición del portal (puedes ajustarla según sea necesario)
	FVector PortalLocation = FVector(1220.0f, -110.0f, 220.0f);
	FRotator PortalRotation = FRotator::ZeroRotator;
	FActorSpawnParameters SpawnParams;

	// Crear el portal en la ubicación especificada
	Portal = GetWorld()->SpawnActor<APortal>(APortal::StaticClass(), PortalLocation, PortalRotation, SpawnParams);
	if (Portal)
	{
		UE_LOG(LogTemp, Warning, TEXT("Portal creado en la ubicación: %s"), *PortalLocation.ToString());

		// Asignar el destino del portal
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

		for (AActor* Actor : FoundActors)
		{
			if (Actor->GetName() == TEXT("componentePlataforma_13"))
			{
				Portal->Destino = Actor;
				UE_LOG(LogTemp, Warning, TEXT("Destino del portal asignado a: %s"), *Actor->GetName());
				break;
			}
		}
	}
}