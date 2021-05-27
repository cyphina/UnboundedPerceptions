// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "TreeLayout/Public/UpTree.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUpTree() {}
// Cross Module References
	TREELAYOUT_API UClass* Z_Construct_UClass_UUpTree_NoRegister();
	TREELAYOUT_API UClass* Z_Construct_UClass_UUpTree();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	UPackage* Z_Construct_UPackage__Script_TreeLayout();
	TREELAYOUT_API UScriptStruct* Z_Construct_UScriptStruct_FUpTreeNode();
	TREELAYOUT_API UScriptStruct* Z_Construct_UScriptStruct_FUpTreeConnector();
	COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
	TREELAYOUT_API UClass* Z_Construct_UClass_UUpTreeLayoutStrategy_NoRegister();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector2D();
// End Cross Module References
	DEFINE_FUNCTION(UUpTree::execBP_GetRoot)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(FUpTreeNode*)Z_Param__Result=P_THIS->BP_GetRoot();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UUpTree::execResetTree)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->ResetTree();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UUpTree::execMakeNodeConnections)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(TArray<FUpTreeConnector>*)Z_Param__Result=P_THIS->MakeNodeConnections();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UUpTree::execGetHierarchy)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(TMap<FName,FUpTreeNode>*)Z_Param__Result=P_THIS->GetHierarchy();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UUpTree::execPositionNodes)
	{
		P_GET_OBJECT(UClass,Z_Param_LayoutStrategy);
		P_GET_STRUCT(FVector2D,Z_Param_TreeSize);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->PositionNodes(Z_Param_LayoutStrategy,Z_Param_TreeSize);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UUpTree::execGetNodeById)
	{
		P_GET_PROPERTY(FNameProperty,Z_Param_Id);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(FUpTreeNode*)Z_Param__Result=P_THIS->GetNodeById(Z_Param_Id);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UUpTree::execMakeStringFromTree)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(FString*)Z_Param__Result=P_THIS->MakeStringFromTree();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UUpTree::execGetMaxDepth)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(int32*)Z_Param__Result=P_THIS->GetMaxDepth();
		P_NATIVE_END;
	}
#if WITH_EDITOR
	DEFINE_FUNCTION(UUpTree::execMakeThreadConnections)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(TArray<FUpTreeConnector>*)Z_Param__Result=P_THIS->MakeThreadConnections();
		P_NATIVE_END;
	}
#endif //WITH_EDITOR
	void UUpTree::StaticRegisterNativesUUpTree()
	{
		UClass* Class = UUpTree::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "BP_GetRoot", &UUpTree::execBP_GetRoot },
			{ "GetHierarchy", &UUpTree::execGetHierarchy },
			{ "GetMaxDepth", &UUpTree::execGetMaxDepth },
			{ "GetNodeById", &UUpTree::execGetNodeById },
			{ "MakeNodeConnections", &UUpTree::execMakeNodeConnections },
			{ "MakeStringFromTree", &UUpTree::execMakeStringFromTree },
#if WITH_EDITOR
			{ "MakeThreadConnections", &UUpTree::execMakeThreadConnections },
#endif // WITH_EDITOR
			{ "PositionNodes", &UUpTree::execPositionNodes },
			{ "ResetTree", &UUpTree::execResetTree },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UUpTree_BP_GetRoot_Statics
	{
		struct UpTree_eventBP_GetRoot_Parms
		{
			FUpTreeNode ReturnValue;
		};
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UUpTree_BP_GetRoot_Statics::NewProp_ReturnValue_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUpTree_BP_GetRoot_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000008000582, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UpTree_eventBP_GetRoot_Parms, ReturnValue), Z_Construct_UScriptStruct_FUpTreeNode, METADATA_PARAMS(Z_Construct_UFunction_UUpTree_BP_GetRoot_Statics::NewProp_ReturnValue_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UUpTree_BP_GetRoot_Statics::NewProp_ReturnValue_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUpTree_BP_GetRoot_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUpTree_BP_GetRoot_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UUpTree_BP_GetRoot_Statics::Function_MetaDataParams[] = {
		{ "DisplayName", "Get Tree Root" },
		{ "ModuleRelativePath", "Public/UpTree.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UUpTree_BP_GetRoot_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUpTree, nullptr, "BP_GetRoot", nullptr, nullptr, sizeof(UpTree_eventBP_GetRoot_Parms), Z_Construct_UFunction_UUpTree_BP_GetRoot_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUpTree_BP_GetRoot_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54080401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UUpTree_BP_GetRoot_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UUpTree_BP_GetRoot_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UUpTree_BP_GetRoot()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UUpTree_BP_GetRoot_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UUpTree_GetHierarchy_Statics
	{
		struct UpTree_eventGetHierarchy_Parms
		{
			TMap<FName,FUpTreeNode> ReturnValue;
		};
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_ReturnValue_ValueProp;
		static const UE4CodeGen_Private::FNamePropertyParams NewProp_ReturnValue_Key_KeyProp;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[];
#endif
		static const UE4CodeGen_Private::FMapPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUpTree_GetHierarchy_Statics::NewProp_ReturnValue_ValueProp = { "ReturnValue", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 1, Z_Construct_UScriptStruct_FUpTreeNode, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UUpTree_GetHierarchy_Statics::NewProp_ReturnValue_Key_KeyProp = { "ReturnValue_Key", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UUpTree_GetHierarchy_Statics::NewProp_ReturnValue_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FMapPropertyParams Z_Construct_UFunction_UUpTree_GetHierarchy_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000008000582, UE4CodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UpTree_eventGetHierarchy_Parms, ReturnValue), EMapPropertyFlags::None, METADATA_PARAMS(Z_Construct_UFunction_UUpTree_GetHierarchy_Statics::NewProp_ReturnValue_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UUpTree_GetHierarchy_Statics::NewProp_ReturnValue_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUpTree_GetHierarchy_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUpTree_GetHierarchy_Statics::NewProp_ReturnValue_ValueProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUpTree_GetHierarchy_Statics::NewProp_ReturnValue_Key_KeyProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUpTree_GetHierarchy_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UUpTree_GetHierarchy_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UpTree.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UUpTree_GetHierarchy_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUpTree, nullptr, "GetHierarchy", nullptr, nullptr, sizeof(UpTree_eventGetHierarchy_Parms), Z_Construct_UFunction_UUpTree_GetHierarchy_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUpTree_GetHierarchy_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UUpTree_GetHierarchy_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UUpTree_GetHierarchy_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UUpTree_GetHierarchy()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UUpTree_GetHierarchy_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UUpTree_GetMaxDepth_Statics
	{
		struct UpTree_eventGetMaxDepth_Parms
		{
			int32 ReturnValue;
		};
		static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FUnsizedIntPropertyParams Z_Construct_UFunction_UUpTree_GetMaxDepth_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UpTree_eventGetMaxDepth_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUpTree_GetMaxDepth_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUpTree_GetMaxDepth_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UUpTree_GetMaxDepth_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UpTree.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UUpTree_GetMaxDepth_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUpTree, nullptr, "GetMaxDepth", nullptr, nullptr, sizeof(UpTree_eventGetMaxDepth_Parms), Z_Construct_UFunction_UUpTree_GetMaxDepth_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUpTree_GetMaxDepth_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UUpTree_GetMaxDepth_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UUpTree_GetMaxDepth_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UUpTree_GetMaxDepth()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UUpTree_GetMaxDepth_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UUpTree_GetNodeById_Statics
	{
		struct UpTree_eventGetNodeById_Parms
		{
			FName Id;
			FUpTreeNode ReturnValue;
		};
		static const UE4CodeGen_Private::FNamePropertyParams NewProp_Id;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UUpTree_GetNodeById_Statics::NewProp_Id = { "Id", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UpTree_eventGetNodeById_Parms, Id), METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UUpTree_GetNodeById_Statics::NewProp_ReturnValue_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUpTree_GetNodeById_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000008000582, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UpTree_eventGetNodeById_Parms, ReturnValue), Z_Construct_UScriptStruct_FUpTreeNode, METADATA_PARAMS(Z_Construct_UFunction_UUpTree_GetNodeById_Statics::NewProp_ReturnValue_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UUpTree_GetNodeById_Statics::NewProp_ReturnValue_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUpTree_GetNodeById_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUpTree_GetNodeById_Statics::NewProp_Id,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUpTree_GetNodeById_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UUpTree_GetNodeById_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UpTree.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UUpTree_GetNodeById_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUpTree, nullptr, "GetNodeById", nullptr, nullptr, sizeof(UpTree_eventGetNodeById_Parms), Z_Construct_UFunction_UUpTree_GetNodeById_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUpTree_GetNodeById_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UUpTree_GetNodeById_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UUpTree_GetNodeById_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UUpTree_GetNodeById()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UUpTree_GetNodeById_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UUpTree_MakeNodeConnections_Statics
	{
		struct UpTree_eventMakeNodeConnections_Parms
		{
			TArray<FUpTreeConnector> ReturnValue;
		};
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_ReturnValue_Inner;
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUpTree_MakeNodeConnections_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FUpTreeConnector, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UUpTree_MakeNodeConnections_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UpTree_eventMakeNodeConnections_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUpTree_MakeNodeConnections_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUpTree_MakeNodeConnections_Statics::NewProp_ReturnValue_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUpTree_MakeNodeConnections_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UUpTree_MakeNodeConnections_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UpTree.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UUpTree_MakeNodeConnections_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUpTree, nullptr, "MakeNodeConnections", nullptr, nullptr, sizeof(UpTree_eventMakeNodeConnections_Parms), Z_Construct_UFunction_UUpTree_MakeNodeConnections_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUpTree_MakeNodeConnections_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UUpTree_MakeNodeConnections_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UUpTree_MakeNodeConnections_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UUpTree_MakeNodeConnections()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UUpTree_MakeNodeConnections_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UUpTree_MakeStringFromTree_Statics
	{
		struct UpTree_eventMakeStringFromTree_Parms
		{
			FString ReturnValue;
		};
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UUpTree_MakeStringFromTree_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UpTree_eventMakeStringFromTree_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUpTree_MakeStringFromTree_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUpTree_MakeStringFromTree_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UUpTree_MakeStringFromTree_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UpTree.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UUpTree_MakeStringFromTree_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUpTree, nullptr, "MakeStringFromTree", nullptr, nullptr, sizeof(UpTree_eventMakeStringFromTree_Parms), Z_Construct_UFunction_UUpTree_MakeStringFromTree_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUpTree_MakeStringFromTree_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UUpTree_MakeStringFromTree_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UUpTree_MakeStringFromTree_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UUpTree_MakeStringFromTree()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UUpTree_MakeStringFromTree_Statics::FuncParams);
		}
		return ReturnFunction;
	}
#if WITH_EDITOR
	struct Z_Construct_UFunction_UUpTree_MakeThreadConnections_Statics
	{
		struct UpTree_eventMakeThreadConnections_Parms
		{
			TArray<FUpTreeConnector> ReturnValue;
		};
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_ReturnValue_Inner;
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUpTree_MakeThreadConnections_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FUpTreeConnector, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UUpTree_MakeThreadConnections_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UpTree_eventMakeThreadConnections_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUpTree_MakeThreadConnections_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUpTree_MakeThreadConnections_Statics::NewProp_ReturnValue_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUpTree_MakeThreadConnections_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UUpTree_MakeThreadConnections_Statics::Function_MetaDataParams[] = {
		{ "Comment", "/**\n    * @brief Creates connectors between threads (links between nodes that some of the tree layout algorithms generate for optimization purposes)\n    * @return Returns connectors used to build curves between threads.\n    */" },
		{ "ModuleRelativePath", "Public/UpTree.h" },
		{ "ToolTip", "@brief Creates connectors between threads (links between nodes that some of the tree layout algorithms generate for optimization purposes)\n@return Returns connectors used to build curves between threads." },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UUpTree_MakeThreadConnections_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUpTree, nullptr, "MakeThreadConnections", nullptr, nullptr, sizeof(UpTree_eventMakeThreadConnections_Parms), Z_Construct_UFunction_UUpTree_MakeThreadConnections_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUpTree_MakeThreadConnections_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x74020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UUpTree_MakeThreadConnections_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UUpTree_MakeThreadConnections_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UUpTree_MakeThreadConnections()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UUpTree_MakeThreadConnections_Statics::FuncParams);
		}
		return ReturnFunction;
	}
#endif //WITH_EDITOR
	struct Z_Construct_UFunction_UUpTree_PositionNodes_Statics
	{
		struct UpTree_eventPositionNodes_Parms
		{
			TSubclassOf<UUpTreeLayoutStrategy>  LayoutStrategy;
			FVector2D TreeSize;
		};
		static const UE4CodeGen_Private::FClassPropertyParams NewProp_LayoutStrategy;
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_TreeSize;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FClassPropertyParams Z_Construct_UFunction_UUpTree_PositionNodes_Statics::NewProp_LayoutStrategy = { "LayoutStrategy", nullptr, (EPropertyFlags)0x0014000000000080, UE4CodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UpTree_eventPositionNodes_Parms, LayoutStrategy), Z_Construct_UClass_UUpTreeLayoutStrategy_NoRegister, Z_Construct_UClass_UClass, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UUpTree_PositionNodes_Statics::NewProp_TreeSize = { "TreeSize", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UpTree_eventPositionNodes_Parms, TreeSize), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UUpTree_PositionNodes_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUpTree_PositionNodes_Statics::NewProp_LayoutStrategy,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UUpTree_PositionNodes_Statics::NewProp_TreeSize,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UUpTree_PositionNodes_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/UpTree.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UUpTree_PositionNodes_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUpTree, nullptr, "PositionNodes", nullptr, nullptr, sizeof(UpTree_eventPositionNodes_Parms), Z_Construct_UFunction_UUpTree_PositionNodes_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UUpTree_PositionNodes_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UUpTree_PositionNodes_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UUpTree_PositionNodes_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UUpTree_PositionNodes()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UUpTree_PositionNodes_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UUpTree_ResetTree_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UUpTree_ResetTree_Statics::Function_MetaDataParams[] = {
		{ "Comment", "/**\n    * @brief Resets all the node's positions and threads\n    */" },
		{ "ModuleRelativePath", "Public/UpTree.h" },
		{ "ToolTip", "@brief Resets all the node's positions and threads" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UUpTree_ResetTree_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUpTree, nullptr, "ResetTree", nullptr, nullptr, 0, nullptr, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UUpTree_ResetTree_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UUpTree_ResetTree_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UUpTree_ResetTree()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UUpTree_ResetTree_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_UUpTree_NoRegister()
	{
		return UUpTree::StaticClass();
	}
	struct Z_Construct_UClass_UUpTree_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UUpTree_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_TreeLayout,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UUpTree_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UUpTree_BP_GetRoot, "BP_GetRoot" }, // 2016625945
		{ &Z_Construct_UFunction_UUpTree_GetHierarchy, "GetHierarchy" }, // 2078620472
		{ &Z_Construct_UFunction_UUpTree_GetMaxDepth, "GetMaxDepth" }, // 744582731
		{ &Z_Construct_UFunction_UUpTree_GetNodeById, "GetNodeById" }, // 3731700227
		{ &Z_Construct_UFunction_UUpTree_MakeNodeConnections, "MakeNodeConnections" }, // 394224592
		{ &Z_Construct_UFunction_UUpTree_MakeStringFromTree, "MakeStringFromTree" }, // 194143610
#if WITH_EDITOR
		{ &Z_Construct_UFunction_UUpTree_MakeThreadConnections, "MakeThreadConnections" }, // 4148440282
#endif //WITH_EDITOR
		{ &Z_Construct_UFunction_UUpTree_PositionNodes, "PositionNodes" }, // 3380147354
		{ &Z_Construct_UFunction_UUpTree_ResetTree, "ResetTree" }, // 1560260017
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UUpTree_Statics::Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Comment", "/**\n * @brief Represents data with a hierarchical relationship in a tree-like format.\n */" },
		{ "IncludePath", "UpTree.h" },
		{ "ModuleRelativePath", "Public/UpTree.h" },
		{ "ToolTip", "@brief Represents data with a hierarchical relationship in a tree-like format." },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UUpTree_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUpTree>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UUpTree_Statics::ClassParams = {
		&UUpTree::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		0,
		0,
		0x000000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UUpTree_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UUpTree_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UUpTree()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UUpTree_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UUpTree, 4116620929);
	template<> TREELAYOUT_API UClass* StaticClass<UUpTree>()
	{
		return UUpTree::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UUpTree(Z_Construct_UClass_UUpTree, &UUpTree::StaticClass, TEXT("/Script/TreeLayout"), TEXT("UUpTree"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UUpTree);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
