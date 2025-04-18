#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"

// Console variable for debug
static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(
	TEXT("PULSEFIRE.DebugWeapons"),
	DebugWeaponDrawing,
	TEXT("Draw Debug Lines for Weapons"),
	ECVF_Cheat);

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// Create mesh component
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	// Set default values
	MuzzleSocketName = "MuzzleSocket";
	BaseDamage = 20.0f;
	RateOfFire = 600.0f;
	BulletSpread = 2.0f;
	MaxAmmo = 30;
	CurrentAmmo = MaxAmmo;

	// Calculate time between shots
	TimeBetweenShots = 60.0f / RateOfFire;

	// Set replication
	SetReplicates(true);
	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 33.0f;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	// Calculate time between shots
	TimeBetweenShots = 60.0f / RateOfFire;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::StartFire()
{
	float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);

	GetWorldTimerManager().SetTimer(TimerHandle_AutoFire, this, &AWeapon::Fire, TimeBetweenShots, true, FirstDelay);
}

void AWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_AutoFire);
}

void AWeapon::Reload()
{
	if (CurrentAmmo < MaxAmmo)
	{
		CurrentAmmo = MaxAmmo;
	}
}

int32 AWeapon::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

int32 AWeapon::GetMaxAmmo() const
{
	return MaxAmmo;
}

void AWeapon::Fire()
{
	// Check if we have authority or if we're a client
	if (Role < ROLE_Authority)
	{
		ServerFire();
	}

	// Check if we have ammo
	if (CurrentAmmo <= 0)
	{
		// Play empty sound or animation
		return;
	}

	// Consume ammo
	if (Role == ROLE_Authority)
	{
		CurrentAmmo--;
	}

	// Get owner and socket transform
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		// Calculate direction with random spread
		FVector ShotDirection = EyeRotation.Vector();
		
		// Apply bullet spread
		float HalfRad = FMath::DegreesToRadians(BulletSpread);
		ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);

		// Calculate end point
		FVector TraceEnd = EyeLocation + (ShotDirection * 10000);

		// Set trace parameters
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		// Particle target parameter
		FVector TracerEndPoint = TraceEnd;

		// Hit result
		FHitResult Hit;
		bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParams);
		
		// Process hit
		if (bHit)
		{
			// Get hit actor
			AActor* HitActor = Hit.GetActor();

			// Get surface type
			EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

			// Apply damage
			float ActualDamage = BaseDamage;
			if (SurfaceType == SURFACE_FLESHVULNERABLE)
			{
				ActualDamage *= 4.0f;
			}

			if (Role == ROLE_Authority && HitActor)
			{
				UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection, Hit, MyOwner->GetInstigatorController(), MyOwner, DamageType);
			}

			// Play impact effects
			PlayImpactEffects(SurfaceType, Hit.ImpactPoint);

			// Update tracer endpoint
			TracerEndPoint = Hit.ImpactPoint;
		}

		// Debug
		if (DebugWeaponDrawing > 0)
		{
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f);
		}

		// Play fire effects
		PlayFireEffects(TracerEndPoint);

		// Update hit scan trace for replication
		if (Role == ROLE_Authority)
		{
			HitScanTrace.TraceFrom = EyeLocation;
			HitScanTrace.TraceTo = TracerEndPoint;
			HitScanTrace.SurfaceLocation = bHit ? Hit.ImpactPoint : TracerEndPoint;
			HitScanTrace.SurfaceType = bHit ? UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get()) : 0;
		}

		// Update last fire time
		LastFireTime = GetWorld()->TimeSeconds;
	}
}

void AWeapon::PlayFireEffects(const FVector& TraceEnd)
{
	// Play muzzle effect
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}

	// Play fire sound
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// Play tracer effect
	if (TracerEffect)
	{
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
		if (TracerComp)
		{
			TracerComp->SetVectorParameter("Target", TraceEnd);
		}
	}

	// Play camera shake
	APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner)
	{
		APlayerController* PC = Cast<APlayerController>(MyOwner->GetController());
		if (PC)
		{
			PC->ClientPlayCameraShake(FireAnimation);
		}
	}
}

void AWeapon::PlayImpactEffects(EPhysicalSurface SurfaceType, const FVector& ImpactPoint)
{
	// Select impact effect based on surface type
	UParticleSystem* SelectedEffect = nullptr;
	switch (SurfaceType)
	{
	case SURFACE_FLESHDEFAULT:
	case SURFACE_FLESHVULNERABLE:
		SelectedEffect = FleshImpactEffect;
		break;
	default:
		SelectedEffect = DefaultImpactEffect;
		break;
	}

	// Spawn impact effect
	if (SelectedEffect)
	{
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		FVector ShotDirection = ImpactPoint - MuzzleLocation;
		ShotDirection.Normalize();

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, ImpactPoint, ShotDirection.Rotation());
	}
}

void AWeapon::OnRep_HitScanTrace()
{
	// Play fire effects using replicated data
	PlayFireEffects(HitScanTrace.TraceTo);

	// Play impact effects if we hit something
	if (HitScanTrace.SurfaceType != 0)
	{
		PlayImpactEffects((EPhysicalSurface)HitScanTrace.SurfaceType, HitScanTrace.SurfaceLocation);
	}
}

bool AWeapon::ServerFire_Validate()
{
	return true;
}

void AWeapon::ServerFire_Implementation()
{
	Fire();
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, HitScanTrace);
	DOREPLIFETIME(AWeapon, CurrentAmmo);
}
