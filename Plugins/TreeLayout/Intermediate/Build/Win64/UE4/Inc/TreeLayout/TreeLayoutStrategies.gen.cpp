// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "TreeLayout/Public/TreeLayoutStrategies.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTreeLayoutStrategies() {}
// Cross Module References
	TREELAYOUT_API UClass* Z_Construct_UClass_UUpTreeLayoutStrategy_NoRegister();
	TREELAYOUT_API UClass* Z_Construct_UClass_UUpTreeLayoutStrategy();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	UPackage* Z_Construct_UPackage__Script_TreeLayout();
	TREELAYOUT_API UClass* Z_Construct_UClass_UUpTreeKnuthStrategy_NoRegister();
	TREELAYOUT_API UClass* Z_Construct_UClass_UUpTreeKnuthStrategy();
	TREELAYOUT_API UClass* Z_Construct_UClass_UUpTreeKnuthWSNaive_NoRegister();
	TREELAYOUT_API UClass* Z_Construct_UClass_UUpTreeKnuthWSNaive();
	TREELAYOUT_API UClass* Z_Construct_UClass_UUpTreeKnuthWSCentered_NoRegister();
	TREELAYOUT_API UClass* Z_Construct_UClass_UUpTreeKnuthWSCentered();
	TREELAYOUT_API UClass* Z_Construct_UClass_UUpTreeReingoldTilfordStrategy_NoRegister();
	TREELAYOUT_API UClass* Z_Construct_UClass_UUpTreeReingoldTilfordStrategy();
	TREELAYOUT_API UClass* Z_Construct_UClass_UUpTreeBucheimLayoutStrategy_NoRegister();
	TREELAYOUT_API UClass* Z_Construct_UClass_UUpTreeBucheimLayoutStrategy();
// End Cross Module References
	void UUpTreeLayoutStrategy::StaticRegisterNativesUUpTreeLayoutStrategy()
	{
	}
	UClass* Z_Construct_UClass_UUpTreeLayoutStrategy_NoRegister()
	{
		return UUpTreeLayoutStrategy::StaticClass();
	}
	struct Z_Construct_UClass_UUpTreeLayoutStrategy_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UUpTreeLayoutStrategy_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_TreeLayout,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UUpTreeLayoutStrategy_Statics::Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "TreeLayoutStrategies.h" },
		{ "ModuleRelativePath", "Public/TreeLayoutStrategies.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UUpTreeLayoutStrategy_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUpTreeLayoutStrategy>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UUpTreeLayoutStrategy_Statics::ClassParams = {
		&UUpTreeLayoutStrategy::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x000000A1u,
		METADATA_PARAMS(Z_Construct_UClass_UUpTreeLayoutStrategy_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UUpTreeLayoutStrategy_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UUpTreeLayoutStrategy()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UUpTreeLayoutStrategy_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UUpTreeLayoutStrategy, 2372872969);
	template<> TREELAYOUT_API UClass* StaticClass<UUpTreeLayoutStrategy>()
	{
		return UUpTreeLayoutStrategy::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UUpTreeLayoutStrategy(Z_Construct_UClass_UUpTreeLayoutStrategy, &UUpTreeLayoutStrategy::StaticClass, TEXT("/Script/TreeLayout"), TEXT("UUpTreeLayoutStrategy"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UUpTreeLayoutStrategy);
	void UUpTreeKnuthStrategy::StaticRegisterNativesUUpTreeKnuthStrategy()
	{
	}
	UClass* Z_Construct_UClass_UUpTreeKnuthStrategy_NoRegister()
	{
		return UUpTreeKnuthStrategy::StaticClass();
	}
	struct Z_Construct_UClass_UUpTreeKnuthStrategy_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UUpTreeKnuthStrategy_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UUpTreeLayoutStrategy,
		(UObject* (*)())Z_Construct_UPackage__Script_TreeLayout,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UUpTreeKnuthStrategy_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "TreeLayoutStrategies.h" },
		{ "ModuleRelativePath", "Public/TreeLayoutStrategies.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UUpTreeKnuthStrategy_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUpTreeKnuthStrategy>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UUpTreeKnuthStrategy_Statics::ClassParams = {
		&UUpTreeKnuthStrategy::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x000000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UUpTreeKnuthStrategy_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UUpTreeKnuthStrategy_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UUpTreeKnuthStrategy()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UUpTreeKnuthStrategy_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UUpTreeKnuthStrategy, 1278422809);
	template<> TREELAYOUT_API UClass* StaticClass<UUpTreeKnuthStrategy>()
	{
		return UUpTreeKnuthStrategy::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UUpTreeKnuthStrategy(Z_Construct_UClass_UUpTreeKnuthStrategy, &UUpTreeKnuthStrategy::StaticClass, TEXT("/Script/TreeLayout"), TEXT("UUpTreeKnuthStrategy"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UUpTreeKnuthStrategy);
	void UUpTreeKnuthWSNaive::StaticRegisterNativesUUpTreeKnuthWSNaive()
	{
	}
	UClass* Z_Construct_UClass_UUpTreeKnuthWSNaive_NoRegister()
	{
		return UUpTreeKnuthWSNaive::StaticClass();
	}
	struct Z_Construct_UClass_UUpTreeKnuthWSNaive_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UUpTreeKnuthWSNaive_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UUpTreeLayoutStrategy,
		(UObject* (*)())Z_Construct_UPackage__Script_TreeLayout,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UUpTreeKnuthWSNaive_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "TreeLayoutStrategies.h" },
		{ "ModuleRelativePath", "Public/TreeLayoutStrategies.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UUpTreeKnuthWSNaive_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUpTreeKnuthWSNaive>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UUpTreeKnuthWSNaive_Statics::ClassParams = {
		&UUpTreeKnuthWSNaive::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x000000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UUpTreeKnuthWSNaive_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UUpTreeKnuthWSNaive_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UUpTreeKnuthWSNaive()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UUpTreeKnuthWSNaive_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UUpTreeKnuthWSNaive, 111047183);
	template<> TREELAYOUT_API UClass* StaticClass<UUpTreeKnuthWSNaive>()
	{
		return UUpTreeKnuthWSNaive::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UUpTreeKnuthWSNaive(Z_Construct_UClass_UUpTreeKnuthWSNaive, &UUpTreeKnuthWSNaive::StaticClass, TEXT("/Script/TreeLayout"), TEXT("UUpTreeKnuthWSNaive"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UUpTreeKnuthWSNaive);
	void UUpTreeKnuthWSCentered::StaticRegisterNativesUUpTreeKnuthWSCentered()
	{
	}
	UClass* Z_Construct_UClass_UUpTreeKnuthWSCentered_NoRegister()
	{
		return UUpTreeKnuthWSCentered::StaticClass();
	}
	struct Z_Construct_UClass_UUpTreeKnuthWSCentered_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UUpTreeKnuthWSCentered_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UUpTreeLayoutStrategy,
		(UObject* (*)())Z_Construct_UPackage__Script_TreeLayout,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UUpTreeKnuthWSCentered_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "TreeLayoutStrategies.h" },
		{ "ModuleRelativePath", "Public/TreeLayoutStrategies.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UUpTreeKnuthWSCentered_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUpTreeKnuthWSCentered>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UUpTreeKnuthWSCentered_Statics::ClassParams = {
		&UUpTreeKnuthWSCentered::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x000000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UUpTreeKnuthWSCentered_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UUpTreeKnuthWSCentered_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UUpTreeKnuthWSCentered()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UUpTreeKnuthWSCentered_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UUpTreeKnuthWSCentered, 4161499399);
	template<> TREELAYOUT_API UClass* StaticClass<UUpTreeKnuthWSCentered>()
	{
		return UUpTreeKnuthWSCentered::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UUpTreeKnuthWSCentered(Z_Construct_UClass_UUpTreeKnuthWSCentered, &UUpTreeKnuthWSCentered::StaticClass, TEXT("/Script/TreeLayout"), TEXT("UUpTreeKnuthWSCentered"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UUpTreeKnuthWSCentered);
	void UUpTreeReingoldTilfordStrategy::StaticRegisterNativesUUpTreeReingoldTilfordStrategy()
	{
	}
	UClass* Z_Construct_UClass_UUpTreeReingoldTilfordStrategy_NoRegister()
	{
		return UUpTreeReingoldTilfordStrategy::StaticClass();
	}
	struct Z_Construct_UClass_UUpTreeReingoldTilfordStrategy_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UUpTreeReingoldTilfordStrategy_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UUpTreeLayoutStrategy,
		(UObject* (*)())Z_Construct_UPackage__Script_TreeLayout,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UUpTreeReingoldTilfordStrategy_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "TreeLayoutStrategies.h" },
		{ "ModuleRelativePath", "Public/TreeLayoutStrategies.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UUpTreeReingoldTilfordStrategy_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUpTreeReingoldTilfordStrategy>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UUpTreeReingoldTilfordStrategy_Statics::ClassParams = {
		&UUpTreeReingoldTilfordStrategy::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x000000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UUpTreeReingoldTilfordStrategy_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UUpTreeReingoldTilfordStrategy_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UUpTreeReingoldTilfordStrategy()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UUpTreeReingoldTilfordStrategy_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UUpTreeReingoldTilfordStrategy, 3537645600);
	template<> TREELAYOUT_API UClass* StaticClass<UUpTreeReingoldTilfordStrategy>()
	{
		return UUpTreeReingoldTilfordStrategy::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UUpTreeReingoldTilfordStrategy(Z_Construct_UClass_UUpTreeReingoldTilfordStrategy, &UUpTreeReingoldTilfordStrategy::StaticClass, TEXT("/Script/TreeLayout"), TEXT("UUpTreeReingoldTilfordStrategy"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UUpTreeReingoldTilfordStrategy);
	void UUpTreeBucheimLayoutStrategy::StaticRegisterNativesUUpTreeBucheimLayoutStrategy()
	{
	}
	UClass* Z_Construct_UClass_UUpTreeBucheimLayoutStrategy_NoRegister()
	{
		return UUpTreeBucheimLayoutStrategy::StaticClass();
	}
	struct Z_Construct_UClass_UUpTreeBucheimLayoutStrategy_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UUpTreeBucheimLayoutStrategy_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UUpTreeLayoutStrategy,
		(UObject* (*)())Z_Construct_UPackage__Script_TreeLayout,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UUpTreeBucheimLayoutStrategy_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "TreeLayoutStrategies.h" },
		{ "ModuleRelativePath", "Public/TreeLayoutStrategies.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UUpTreeBucheimLayoutStrategy_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUpTreeBucheimLayoutStrategy>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UUpTreeBucheimLayoutStrategy_Statics::ClassParams = {
		&UUpTreeBucheimLayoutStrategy::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x000000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UUpTreeBucheimLayoutStrategy_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UUpTreeBucheimLayoutStrategy_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UUpTreeBucheimLayoutStrategy()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UUpTreeBucheimLayoutStrategy_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UUpTreeBucheimLayoutStrategy, 1187380137);
	template<> TREELAYOUT_API UClass* StaticClass<UUpTreeBucheimLayoutStrategy>()
	{
		return UUpTreeBucheimLayoutStrategy::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UUpTreeBucheimLayoutStrategy(Z_Construct_UClass_UUpTreeBucheimLayoutStrategy, &UUpTreeBucheimLayoutStrategy::StaticClass, TEXT("/Script/TreeLayout"), TEXT("UUpTreeBucheimLayoutStrategy"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UUpTreeBucheimLayoutStrategy);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
