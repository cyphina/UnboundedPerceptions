// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "TreeLayout/Public/UpTreeStructs.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUpTreeStructs() {}
// Cross Module References
	TREELAYOUT_API UScriptStruct* Z_Construct_UScriptStruct_FUpTreeConnector();
	UPackage* Z_Construct_UPackage__Script_TreeLayout();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector2D();
	TREELAYOUT_API UScriptStruct* Z_Construct_UScriptStruct_FUpTreeNode();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
// End Cross Module References
class UScriptStruct* FUpTreeConnector::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern TREELAYOUT_API uint32 Get_Z_Construct_UScriptStruct_FUpTreeConnector_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FUpTreeConnector, Z_Construct_UPackage__Script_TreeLayout(), TEXT("UpTreeConnector"), sizeof(FUpTreeConnector), Get_Z_Construct_UScriptStruct_FUpTreeConnector_Hash());
	}
	return Singleton;
}
template<> TREELAYOUT_API UScriptStruct* StaticStruct<FUpTreeConnector>()
{
	return FUpTreeConnector::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FUpTreeConnector(FUpTreeConnector::StaticStruct, TEXT("/Script/TreeLayout"), TEXT("UpTreeConnector"), false, nullptr, nullptr);
static struct FScriptStruct_TreeLayout_StaticRegisterNativesFUpTreeConnector
{
	FScriptStruct_TreeLayout_StaticRegisterNativesFUpTreeConnector()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("UpTreeConnector")),new UScriptStruct::TCppStructOps<FUpTreeConnector>);
	}
} ScriptStruct_TreeLayout_StaticRegisterNativesFUpTreeConnector;
	struct Z_Construct_UScriptStruct_FUpTreeConnector_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Control1_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Control1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Control2_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Control2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Control3_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Control3;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Control4_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Control4;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FUpTreeConnector_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/UpTreeStructs.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FUpTreeConnector_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FUpTreeConnector>();
	}
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FUpTreeConnector_Statics::NewProp_Control1_MetaData[] = {
		{ "Category", "UpTreeConnector" },
		{ "ModuleRelativePath", "Public/UpTreeStructs.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FUpTreeConnector_Statics::NewProp_Control1 = { "Control1", nullptr, (EPropertyFlags)0x0010000000000014, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FUpTreeConnector, Control1), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(Z_Construct_UScriptStruct_FUpTreeConnector_Statics::NewProp_Control1_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FUpTreeConnector_Statics::NewProp_Control1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FUpTreeConnector_Statics::NewProp_Control2_MetaData[] = {
		{ "Category", "UpTreeConnector" },
		{ "ModuleRelativePath", "Public/UpTreeStructs.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FUpTreeConnector_Statics::NewProp_Control2 = { "Control2", nullptr, (EPropertyFlags)0x0010000000000014, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FUpTreeConnector, Control2), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(Z_Construct_UScriptStruct_FUpTreeConnector_Statics::NewProp_Control2_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FUpTreeConnector_Statics::NewProp_Control2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FUpTreeConnector_Statics::NewProp_Control3_MetaData[] = {
		{ "Category", "UpTreeConnector" },
		{ "ModuleRelativePath", "Public/UpTreeStructs.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FUpTreeConnector_Statics::NewProp_Control3 = { "Control3", nullptr, (EPropertyFlags)0x0010000000000014, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FUpTreeConnector, Control3), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(Z_Construct_UScriptStruct_FUpTreeConnector_Statics::NewProp_Control3_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FUpTreeConnector_Statics::NewProp_Control3_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FUpTreeConnector_Statics::NewProp_Control4_MetaData[] = {
		{ "Category", "UpTreeConnector" },
		{ "ModuleRelativePath", "Public/UpTreeStructs.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FUpTreeConnector_Statics::NewProp_Control4 = { "Control4", nullptr, (EPropertyFlags)0x0010000000000014, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FUpTreeConnector, Control4), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(Z_Construct_UScriptStruct_FUpTreeConnector_Statics::NewProp_Control4_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FUpTreeConnector_Statics::NewProp_Control4_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FUpTreeConnector_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FUpTreeConnector_Statics::NewProp_Control1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FUpTreeConnector_Statics::NewProp_Control2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FUpTreeConnector_Statics::NewProp_Control3,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FUpTreeConnector_Statics::NewProp_Control4,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FUpTreeConnector_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_TreeLayout,
		nullptr,
		&NewStructOps,
		"UpTreeConnector",
		sizeof(FUpTreeConnector),
		alignof(FUpTreeConnector),
		Z_Construct_UScriptStruct_FUpTreeConnector_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FUpTreeConnector_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FUpTreeConnector_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FUpTreeConnector_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FUpTreeConnector()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FUpTreeConnector_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_TreeLayout();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("UpTreeConnector"), sizeof(FUpTreeConnector), Get_Z_Construct_UScriptStruct_FUpTreeConnector_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FUpTreeConnector_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FUpTreeConnector_Hash() { return 2782343441U; }
class UScriptStruct* FUpTreeNode::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern TREELAYOUT_API uint32 Get_Z_Construct_UScriptStruct_FUpTreeNode_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FUpTreeNode, Z_Construct_UPackage__Script_TreeLayout(), TEXT("UpTreeNode"), sizeof(FUpTreeNode), Get_Z_Construct_UScriptStruct_FUpTreeNode_Hash());
	}
	return Singleton;
}
template<> TREELAYOUT_API UScriptStruct* StaticStruct<FUpTreeNode>()
{
	return FUpTreeNode::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FUpTreeNode(FUpTreeNode::StaticStruct, TEXT("/Script/TreeLayout"), TEXT("UpTreeNode"), false, nullptr, nullptr);
static struct FScriptStruct_TreeLayout_StaticRegisterNativesFUpTreeNode
{
	FScriptStruct_TreeLayout_StaticRegisterNativesFUpTreeNode()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("UpTreeNode")),new UScriptStruct::TCppStructOps<FUpTreeNode>);
	}
} ScriptStruct_TreeLayout_StaticRegisterNativesFUpTreeNode;
	struct Z_Construct_UScriptStruct_FUpTreeNode_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Id_MetaData[];
#endif
		static const UE4CodeGen_Private::FNamePropertyParams NewProp_Id;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Data_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Data;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_X_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_X;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Y_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Y;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FUpTreeNode_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/UpTreeStructs.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FUpTreeNode_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FUpTreeNode>();
	}
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FUpTreeNode_Statics::NewProp_Id_MetaData[] = {
		{ "Category", "UpTreeNode" },
		{ "ModuleRelativePath", "Public/UpTreeStructs.h" },
	};
#endif
	const UE4CodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FUpTreeNode_Statics::NewProp_Id = { "Id", nullptr, (EPropertyFlags)0x0010000000000015, UE4CodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FUpTreeNode, Id), METADATA_PARAMS(Z_Construct_UScriptStruct_FUpTreeNode_Statics::NewProp_Id_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FUpTreeNode_Statics::NewProp_Id_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FUpTreeNode_Statics::NewProp_Data_MetaData[] = {
		{ "Category", "UpTreeNode" },
		{ "ModuleRelativePath", "Public/UpTreeStructs.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FUpTreeNode_Statics::NewProp_Data = { "Data", nullptr, (EPropertyFlags)0x0010000000000015, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FUpTreeNode, Data), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(Z_Construct_UScriptStruct_FUpTreeNode_Statics::NewProp_Data_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FUpTreeNode_Statics::NewProp_Data_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FUpTreeNode_Statics::NewProp_X_MetaData[] = {
		{ "Category", "UpTreeNode" },
		{ "ModuleRelativePath", "Public/UpTreeStructs.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FUpTreeNode_Statics::NewProp_X = { "X", nullptr, (EPropertyFlags)0x0010000000000014, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FUpTreeNode, X), METADATA_PARAMS(Z_Construct_UScriptStruct_FUpTreeNode_Statics::NewProp_X_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FUpTreeNode_Statics::NewProp_X_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FUpTreeNode_Statics::NewProp_Y_MetaData[] = {
		{ "Category", "UpTreeNode" },
		{ "ModuleRelativePath", "Public/UpTreeStructs.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FUpTreeNode_Statics::NewProp_Y = { "Y", nullptr, (EPropertyFlags)0x0010000000000014, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FUpTreeNode, Y), METADATA_PARAMS(Z_Construct_UScriptStruct_FUpTreeNode_Statics::NewProp_Y_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FUpTreeNode_Statics::NewProp_Y_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FUpTreeNode_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FUpTreeNode_Statics::NewProp_Id,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FUpTreeNode_Statics::NewProp_Data,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FUpTreeNode_Statics::NewProp_X,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FUpTreeNode_Statics::NewProp_Y,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FUpTreeNode_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_TreeLayout,
		nullptr,
		&NewStructOps,
		"UpTreeNode",
		sizeof(FUpTreeNode),
		alignof(FUpTreeNode),
		Z_Construct_UScriptStruct_FUpTreeNode_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FUpTreeNode_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FUpTreeNode_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FUpTreeNode_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FUpTreeNode()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FUpTreeNode_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_TreeLayout();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("UpTreeNode"), sizeof(FUpTreeNode), Get_Z_Construct_UScriptStruct_FUpTreeNode_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FUpTreeNode_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FUpTreeNode_Hash() { return 1938503627U; }
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
