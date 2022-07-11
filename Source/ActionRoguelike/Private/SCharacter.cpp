// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractionComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/PlayerCameraManager.h"
#include "SAttributeComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("Blackhole", IE_Pressed, this, &ASCharacter::Blackhole);

}

void ASCharacter::MoveForward(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), value);

}

void ASCharacter::MoveRight(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector, value);

}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);

}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	if (ensure(ProjectileClass))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

		APlayerCameraManager* CurrentCamera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
		FVector CameraLoc = CurrentCamera->GetCameraLocation();

		FRotator ControlRot = GetWorld()->GetFirstPlayerController()->GetControlRotation();

		FVector Start = CameraLoc;
		FVector End = CameraLoc + ControlRot.Vector() * 5000;

		FHitResult Hit;
		bool bIsHit = GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ObjectQueryParams);

		FVector Impact = bIsHit ? Hit.ImpactPoint : Hit.TraceEnd;

		FRotator FixedRot = UKismetMathLibrary::FindLookAtRotation(HandLocation, Impact);

		FTransform SpawnTM = FTransform(FixedRot, HandLocation);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);

		//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 6.0f, 0, 2.0f);
		//DrawDebugSphere(GetWorld(), Impact, 30.0f, 32, FColor::Green, false, 2.0f);
	}

}

void ASCharacter::Blackhole()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_Blackhole, this, &ASCharacter::Blackhole_TimeElapsed, 0.2f);

}

void ASCharacter::Blackhole_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

	APlayerCameraManager* CurrentCamera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	FVector CameraLoc = CurrentCamera->GetCameraLocation();

	FRotator ControlRot = GetWorld()->GetFirstPlayerController()->GetControlRotation();

	FVector Start = CameraLoc;
	FVector End = CameraLoc + ControlRot.Vector() * 5000;

	FHitResult Hit;
	bool bIsHit = GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ObjectQueryParams);

	FVector Impact = bIsHit ? Hit.ImpactPoint : Hit.TraceEnd;

	FRotator FixedRot = UKismetMathLibrary::FindLookAtRotation(HandLocation, Impact);

	FTransform SpawnTM = FTransform(FixedRot, HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(BlackholeClass, SpawnTM, SpawnParams);

}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}

}

