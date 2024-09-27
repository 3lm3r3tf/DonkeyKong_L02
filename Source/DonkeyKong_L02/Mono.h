// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mono.generated.h"

UCLASS()
class DONKEYKONG_L02_API AMono : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMono();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MonoMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector MovimientoDireccion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovimientoVelocidad;
	void Move(float DeltaTime);
	void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit);
};
