#pragma once
#include "framework.h"

class StructMaker
{
public:
	void* structPtr = nullptr;
private:
	size_t totalSize = 0;
	size_t padding = 0;

	template <typename T>
	void add(T Src)
	{
		constexpr auto Size = sizeof(T);

		totalSize += Size;

		structPtr = realloc(structPtr, totalSize);
		if (structPtr) memcpy((void*)(reinterpret_cast<uintptr_t>(structPtr) + padding), &Src, Size);

		//printf("Size: %d, totalSize: %d, paddint: %d\n", Size, totalSize, padding);

		padding += Size;
	}

	template <class none = void>
	constexpr void* Create() const
	{
		return (void*)((uintptr_t)structPtr + padding);
	}

public:
	template <typename T, typename... Rest>
	void* Create(T retValue, Rest ... params)
	{
		add(retValue);

		return Create<>(params...);
	}
};

template <class T>
struct TArray
{
	friend struct FString;

public:
	T* Data;
	int32_t Count;
	int32_t Max;

	inline TArray()
	{
		Data = nullptr;
		Count = Max = 0;
	};

	inline int Num() const
	{
		return Count;
	};

	inline T& operator[](int i)
	{
		return Data[i];
	};

	inline const T& operator[](int i) const
	{
		return Data[i];
	};

	inline bool IsValidIndex(int i) const
	{
		return i < Num();
	}

	inline void Add(T InputData)
	{
		Data = (T*)realloc(Data, sizeof(T) * (Count + 1));
		Data[Count++] = InputData;
		Max = Count;
	};
};

struct FString : private TArray<wchar_t>
{
	FString()
	{
	};

	FString(const wchar_t* other)
	{
		Max = Count = *other ? std::wcslen(other) + 1 : 0;

		if (Count)
		{
			Data = const_cast<wchar_t*>(other);
		}
	}

	bool IsValid() const
	{
		return Data != nullptr;
	}

	const wchar_t* ToWString() const
	{
		return Data;
	}

	std::string ToString() const
	{
		auto length = std::wcslen(Data);

		std::string str(length, '\0');

		std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + length, '?', &str[0]);

		return str;
	}
};

struct FGuid
{
	int A;
	int B;
	int C;
	int D;
};

struct FName;


inline void (*FreeInternal)(void*);
void (*FNameToString)(FName* pThis, FString& out);

struct FName
{
	uint32_t ComparisonIndex;
	uint32_t DisplayIndex;

	FName() = default;

	explicit FName(int64_t name)
	{
		DisplayIndex = (name & 0xFFFFFFFF00000000LL) >> 32;
		ComparisonIndex = (name & 0xFFFFFFFFLL);
	};

	FName(uint32_t comparisonIndex, uint32_t displayIndex) : ComparisonIndex(comparisonIndex),
		DisplayIndex(displayIndex)
	{
	}

	auto ToString()
	{
		FString temp;
		FNameToString(this, temp);

		std::wstring ret(temp.ToWString());

		FreeInternal((void*)temp.ToWString());

		return ret;
	}
};

void* (*ProcessEvent)(void* Object, void* Function, void* Params);

struct UObject
{
	void** VTableObject;
	DWORD ObjectFlags;
	DWORD InternalIndex;
	UObject* Class;
	FName NamePrivate;
	UObject* Outer;

	bool IsA(UObject* cmp) const
	{
		if (this->Class == cmp)
		{
			return true;
		}
		return false;
	}

	std::wstring GetName()
	{
		return NamePrivate.ToString();
	}

	std::string GetNameA()
	{
		auto NameW = NamePrivate.ToString();
		return std::string(NameW.begin(), NameW.end());
	}

	std::wstring GetFullName()
	{
		std::wstring temp;

		for (auto outer = Outer; outer; outer = outer->Outer)
		{
			temp = outer->GetName() + L"." + temp;
		}

		temp = reinterpret_cast<UObject*>(Class)->GetName() + L" " + temp + this->GetName();
		return temp;
	}

	FName GetFName() const
	{
		return *reinterpret_cast<const FName*>(this + 0x18);
	}

	bool isValid() const
	{
		return !Util::IsBadReadPtr((void*)this);
	}


	//TODO: fix return values!
	template <typename ReturnType = void*, typename First, typename ... Rest>
	inline ReturnType Call(UObject* function, First&& firstParam, Rest&&... params)
	{
		ReturnType RetInstance{};

		auto caller = new StructMaker();

		auto ret = *(ReturnType*)caller->Create(std::forward<First>(firstParam), std::forward<Rest>(params)...,
		                                        RetInstance);

		ProcessEvent(this, function, caller->structPtr);

		return ret;
	}
};

struct UField : UObject
{
    UField* Next;
};

struct UStruct : UField
{
    UStruct* Super; // 0x30
    struct UProperty* Children; // 0x38
    uint32_t Size; // 0x40
    char pad_44[0x88 - 0x30 - 0x14];
};
struct UFunction : UStruct
{
    uint32_t FunctionFlags;
    char pad[28]; // 0x8C
    void* Func; // 0xB0
};

struct FRotator
{
	float Pitch;
	float Yaw;
	float Roll;
};

struct FVector
{
	float X;
	float Y;
	float Z;
};

struct FVector2D
{
	float X;
	float Y;

	inline FVector2D()
		: X(0), Y(0)
	{
	}

	inline FVector2D(float x, float y)
		: X(x),
		Y(y)
	{
	}
};

struct FUObjectItem
{
	UObject* Object;
	DWORD Flags;
	DWORD ClusterIndex;
	DWORD SerialNumber;
	DWORD SerialNumber2;
};

struct TUObjectArray
{
	uint8_t* Objects;
	uint32_t MaxElements;
	uint32_t NumElements;
};

struct FKey {
	struct FName KeyName; // 0x00(0x08)
	char UnknownData_8[0x10]; // 0x08(0x10)
};

struct FActorSpawnParameters
{
	FActorSpawnParameters() : Name(), Template(nullptr), Owner(nullptr), Instigator(nullptr), OverrideLevel(nullptr),
		SpawnCollisionHandlingOverride(), bRemoteOwned(0), bNoFail(0),
		bDeferConstruction(0),
		bAllowDuringConstructionScript(0),
		NameMode(),
		ObjectFlags()
	{
	}
	;

	FName Name;
	UObject* Template;
	UObject* Owner;
	UObject* Instigator;
	UObject* OverrideLevel;
	ESpawnActorCollisionHandlingMethod SpawnCollisionHandlingOverride;

private:
	uint8_t bRemoteOwned : 1;

public:
	bool IsRemoteOwned() const { return bRemoteOwned; }

	uint8_t bNoFail : 1;
	uint8_t bDeferConstruction : 1;
	uint8_t bAllowDuringConstructionScript : 1;
	ESpawnActorNameMode NameMode;
	EObjectFlags ObjectFlags;
};

template<class TEnum>
class TEnumAsByte
{
public:
	inline TEnumAsByte()
	{
	}

	inline TEnumAsByte(TEnum _value)
		: value(static_cast<uint8_t>(_value))
	{
	}

	explicit inline TEnumAsByte(int32_t _value)
		: value(static_cast<uint8_t>(_value))
	{
	}

	explicit inline TEnumAsByte(uint8_t _value)
		: value(_value)
	{
	}

	inline operator TEnum() const
	{
		return (TEnum)value;
	}

	inline TEnum GetValue() const
	{
		return (TEnum)value;
	}

private:
	uint8_t value;
};

// CRAP FOR MISSLE START

struct UCurveTable : UObject
{
	char UnknownData_28[0x58]; // 0x28(0x58)
};

struct FCurveTableRowHandle
{
	struct UCurveTable* CurveTable; // 0x00(0x08)
	struct FName RowName; // 0x08(0x08)
};

struct FScalableFloat
{
	float Value; // 0x00(0x04)
	char UnknownData_4[0x4]; // 0x04(0x04)
	struct FCurveTableRowHandle Curve; // 0x08(0x10)
	char UnknownData_18[0x10]; // 0x18(0x10)
};

struct FFortTargetFilter
{
	char ActorTypeFilter; // 0x00(0x01)
	char UnknownData_1[0x7]; // 0x01(0x07)
	UObject* ActorClassFilter; // 0x08(0x08)
	bool bExcludeInstigator; // 0x10(0x01)
	bool bExcludeRequester; // 0x11(0x01)
	bool bExcludeAllAttachedToInstigator; // 0x12(0x01)
	bool bExcludeAllAttachedToRequester; // 0x13(0x01)
	bool bExcludePawnFriends; // 0x14(0x01)
	bool bExcludeFriendlyAI; // 0x15(0x01)
	bool bExcludePawnEnemies; // 0x16(0x01)
	bool bExcludeNonPawnFriends; // 0x17(0x01)
	bool bExcludeNonPawnEnemies; // 0x18(0x01)
	bool bExcludeDBNOPawns; // 0x19(0x01)
	bool bExcludeWithoutNavigationCorridor; // 0x1a(0x01)
	bool bExcludeNonPlayerBuiltPieces; // 0x1b(0x01)
	bool bExcludePlayerBuiltPieces; // 0x1c(0x01)
	bool bExcludeNonBGABuildings; // 0x1d(0x01)
	bool bExcludeNonBlockingHits; // 0x1e(0x01)
	bool bTraceComplexCollision; // 0x1f(0x01)
};

struct FFortAbilityTargetSelection
{
	char Shape; // 0x00(0x01)
	char UnknownData_1[0x7]; // 0x01(0x07)
	struct FString CustomShapeComponentName; // 0x08(0x10)
	uint8_t TestType; // 0x18(0x01)
	uint8_t PrimarySource; // 0x19(0x01)
	uint8_t SecondarySource; // 0x1a(0x01)
	char UnknownData_1B[0x5]; // 0x1b(0x05)
	struct FScalableFloat Range; // 0x20(0x28)
	struct FVector HalfExtents; // 0x48(0x0c)
	char UnknownData_54[0x4]; // 0x54(0x04)
	struct FScalableFloat ConeYawAngle; // 0x58(0x28)
	struct FScalableFloat ConePitchAngle; // 0x80(0x28)
	struct FScalableFloat ConeMinRadius; // 0xa8(0x28)
	struct FFortTargetFilter TargetFilter; // 0xd0(0x20)
	bool bExcludeObstructedByWorld; // 0xf0(0x01)
	bool bCreateHitResultWhenNoTargetsFound; // 0xf1(0x01)
	bool bUseProjectileRotationForDamageZones; // 0xf2(0x01)
	uint8_t TargetSelectionUsage; // 0xf3(0x01)
	int MaxTargets; // 0xf4(0x04)
};

struct FFortAbilityTargetSelectionList
{
	struct TArray<struct FFortAbilityTargetSelection> List; // 0x00(0x10)
	char bStopAtFirstSuccess : 1; // 0x10(0x01)
	char bKeepCheckingListOnIndestructibleHit : 1; // 0x10(0x01)
	char bUseWeaponRanges : 1; // 0x10(0x01)
	char bUseMaxYawAngleToTarget : 1; // 0x10(0x01)
	char UnknownData_10_4 : 4; // 0x10(0x01)
	char UnknownData_11[0x3]; // 0x11(0x03)
	float MaxYawAngleToTarget; // 0x14(0x04)
	char UnknownData_18[0x8]; // 0x18(0x08)
};

struct FGameplayEffectSpecHandle
{
	char UnknownData_0[0x18]; // 0x00(0x18)
};

struct FGameplayTag
{
	struct FName TagName; // 0x00(0x08)
};

// Size: 0x20 (Inherited: 0x00)
struct FGameplayTagContainer
{
	struct TArray<struct FGameplayTag> GameplayTags; // 0x00(0x10)
	struct TArray<struct FGameplayTag> ParentTags; // 0x10(0x10)
};

struct FFortGameplayEffectContainerSpec
{
	struct FFortAbilityTargetSelectionList TargetSelection; // 0x00(0x20)
	struct TArray<struct FGameplayEffectSpecHandle> TargetGameplayEffectSpecs; // 0x20(0x10)
	struct TArray<struct FGameplayEffectSpecHandle> OwnerGameplayEffectSpecs; // 0x30(0x10)
	struct FGameplayTagContainer ActivationCues; // 0x40(0x20)
	struct FGameplayTagContainer ImpactCues; // 0x60(0x20)
};

//CRAP FOR MISSLE END