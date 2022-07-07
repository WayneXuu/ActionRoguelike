// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetCollisionProfileName("PhysicsActor");
	StaticMesh->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::Fire);
	RootComponent = StaticMesh;

	RadialForce = CreateDefaultSubobject<URadialForceComponent>("RadialForce");
	RadialForce->SetAutoActivate(false);
	RadialForce->Radius = 600.0f;
	RadialForce->ImpulseStrength = 2000.0f;
	RadialForce->bImpulseVelChange = true;
	RadialForce->SetupAttachment(StaticMesh);

}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();	

}

void ASExplosiveBarrel::Fire(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForce->FireImpulse();

}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

