// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "TreeLayout/Public/TreeLayoutFunctionLibrary.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTreeLayoutFunctionLibrary() {}
// Cross Module References
	TREELAYOUT_API UClass* Z_Construct_UClass_UTreeLayoutFunctionLibrary_NoRegister();
	TREELAYOUT_API UClass* Z_Construct_UClass_UTreeLayoutFunctionLibrary();
	ENGINE_API UClass* Z_Construct_UClass_UBlueprintFunctionLibrary();
	UPackage* Z_Construct_UPackage__Script_TreeLayout();
	TREELAYOUT_API UScriptStruct* Z_Construct_UScriptStruct_FUpTreeConnector();
	TREELAYOUT_API UScriptStruct* Z_Construct_UScriptStruct_FUpTreeNode();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector2D();
	UMG_API UClass* Z_Construct_UClass_UUserWidget_NoRegister();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
	TREELAYOUT_API UClass* Z_Construct_UClass_UUpTree_NoRegister();
// End Cross Module References
	DEFINE_FUNCTION(UTreeLayoutFunctionLibrary::execUpTreeToString)
	{
		P_GET_OBJECT(UUpTree,Z_Param_Tree);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(FString*)Z_Param__Result=UTreeLayoutFunctionLibrary::UpTreeToString(Z_Param_Tree);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UTreeLayoutFunctionLibrary::execNodeToString)
	{
		P_GET_STRUCT_REF(FUpTreeNode,Z_Param_Out_Node);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(FString*)Z_Param__Result=UTreeLayoutFunctionLibrary::NodeToString(Z_Param_Out_Node);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UTreeLayoutFunctionLibrary::execConnectorToString)
	{
		P_GET_STRUCT_REF(FUpTreeConnector,Z_Param_Out_Connector);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(FString*)Z_Param__Result=UTreeLayoutFunctionLibrary::ConnectorToString(Z_Param_Out_Connector);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UTreeLayoutFunctionLibrary::execGetParent)
	{
		P_GET_STRUCT_REF(FUpTreeNode,Z_Param_Out_Node);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(FUpTreeNode*)Z_Param__Result=UTreeLayoutFunctionLibrary::GetParent(Z_Param_Out_Node);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UTreeLayoutFunctionLibrary::execIsNodeValid)
	{
		P_GET_STRUCT_REF(FUpTreeNode,Z_Param_Out_Node);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(bool*)Z_Param__Result=UTreeLayoutFunctionLibrary::IsNodeValid(Z_Param_Out_Node);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UTreeLayoutFunctionLibrary::execMakeConnectionBetweenWidgets)
	{
		P_GET_OBJECT(UUserWidget,Z_Param_ContainingWidget);
		P_GET_OBJECT(UUserWidget,Z_Param_Parent);
		P_GET_OBJECT(UUserWidget,Z_Param_Child);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(FUpTreeConnector*)Z_Param__Result=UTreeLayoutFunctionLibrary::MakeConnectionBetweenWidgets(Z_Param_ContainingWidget,Z_Param_Parent,Z_Param_Child);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UTreeLayoutFunctionLibrary::execGetTreeNodePosition)
	{
		P_GET_STRUCT_REF(FUpTreeNode,Z_Param_Out_Node);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(FVector2D*)Z_Param__Result=UTreeLayoutFunctionLibrary::GetTreeNodePosition(Z_Param_Out_Node);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UTreeLayoutFunctionLibrary::execStratifyData)
	{
		P_GET_OBJECT(UObject,Z_Param_Outer);
		P_GET_TARRAY(UObject*,Z_Param_DataObjects);
		P_GET_TARRAY_REF(FName,Z_Param_Out_Ids);
		P_GET_TARRAY_REF(FName,Z_Param_Out_ParentIds);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(UUpTree**)Z_Param__Result=UTreeLayoutFunctionLibrary::StratifyData(Z_Param_Outer,Z_Param_DataObjects,Z_Param_Out_Ids,Z_Param_Out_ParentIds);
		P_NATIVE_END;
	}
	void UTreeLayoutFunctionLibrary::StaticRegisterNativesUTreeLayoutFunctionLibrary()
	{
		UClass* Class = UTreeLayoutFunctionLibrary::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "ConnectorToString", &UTreeLayoutFunctionLibrary::execConnectorToString },
			{ "GetParent", &UTreeLayoutFunctionLibrary::execGetParent },
			{ "GetTreeNodePosition", &UTreeLayoutFunctionLibrary::execGetTreeNodePosition },
			{ "IsNodeValid", &UTreeLayoutFunctionLibrary::execIsNodeValid },
			{ "MakeConnectionBetweenWidgets", &UTreeLayoutFunctionLibrary::execMakeConnectionBetweenWidgets },
			{ "NodeToString", &UTreeLayoutFunctionLibrary::execNodeToString },
			{ "StratifyData", &UTreeLayoutFunctionLibrary::execStratifyData },
			{ "UpTreeToString", &UTreeLayoutFunctionLibrary::execUpTreeToString },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UTreeLayoutFunctionLibrary_ConnectorToString_Statics
	{
		struct TreeLayoutFunctionLibrary_eventConnectorToString_Parms
		{
			FUpTreeConnector Connector;
			FString ReturnValue;
		};
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Connector_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Connector;
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTreeLayoutFunctionLibrary_ConnectorToString_Statics::NewProp_Connector_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_ConnectorToString_Statics::NewProp_Connector = { "Connector", nullptr, (EPropertyFlags)0x0010000008000182, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TreeLayoutFunctionLibrary_eventConnectorToString_Parms, Connector), Z_Construct_UScriptStruct_FUpTreeConnector, METADATA_PARAMS(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_ConnectorToString_Statics::NewProp_Connector_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_ConnectorToString_Statics::NewProp_Connector_MetaData)) };
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_ConnectorToString_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TreeLayoutFunctionLibrary_eventConnectorToString_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UTreeLayoutFunctionLibrary_ConnectorToString_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutFunctionLibrary_ConnectorToString_Statics::NewProp_Connector,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutFunctionLibrary_ConnectorToString_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTreeLayoutFunctionLibrary_ConnectorToString_Statics::Function_MetaDataParams[] = {
		{ "BlueprintAutocast", "" },
		{ "Comment", "/** Can use this to debug connectors in BPs */" },
		{ "ModuleRelativePath", "Public/TreeLayoutFunctionLibrary.h" },
		{ "ToolTip", "Can use this to debug connectors in BPs" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_ConnectorToString_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UTreeLayoutFunctionLibrary, nullptr, "ConnectorToString", nullptr, nullptr, sizeof(TreeLayoutFunctionLibrary_eventConnectorToString_Parms), Z_Construct_UFunction_UTreeLayoutFunctionLibrary_ConnectorToString_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_ConnectorToString_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14422401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_ConnectorToString_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_ConnectorToString_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UTreeLayoutFunctionLibrary_ConnectorToString()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UTreeLayoutFunctionLibrary_ConnectorToString_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetParent_Statics
	{
		struct TreeLayoutFunctionLibrary_eventGetParent_Parms
		{
			FUpTreeNode Node;
			FUpTreeNode ReturnValue;
		};
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Node_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Node;
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
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetParent_Statics::NewProp_Node_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetParent_Statics::NewProp_Node = { "Node", nullptr, (EPropertyFlags)0x0010000008000182, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TreeLayoutFunctionLibrary_eventGetParent_Parms, Node), Z_Construct_UScriptStruct_FUpTreeNode, METADATA_PARAMS(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetParent_Statics::NewProp_Node_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetParent_Statics::NewProp_Node_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetParent_Statics::NewProp_ReturnValue_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetParent_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000008000582, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TreeLayoutFunctionLibrary_eventGetParent_Parms, ReturnValue), Z_Construct_UScriptStruct_FUpTreeNode, METADATA_PARAMS(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetParent_Statics::NewProp_ReturnValue_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetParent_Statics::NewProp_ReturnValue_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetParent_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetParent_Statics::NewProp_Node,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetParent_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetParent_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/TreeLayoutFunctionLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetParent_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UTreeLayoutFunctionLibrary, nullptr, "GetParent", nullptr, nullptr, sizeof(TreeLayoutFunctionLibrary_eventGetParent_Parms), Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetParent_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetParent_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14422401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetParent_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetParent_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetParent()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetParent_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetTreeNodePosition_Statics
	{
		struct TreeLayoutFunctionLibrary_eventGetTreeNodePosition_Parms
		{
			FUpTreeNode Node;
			FVector2D ReturnValue;
		};
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Node_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Node;
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetTreeNodePosition_Statics::NewProp_Node_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetTreeNodePosition_Statics::NewProp_Node = { "Node", nullptr, (EPropertyFlags)0x0010000008000182, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TreeLayoutFunctionLibrary_eventGetTreeNodePosition_Parms, Node), Z_Construct_UScriptStruct_FUpTreeNode, METADATA_PARAMS(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetTreeNodePosition_Statics::NewProp_Node_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetTreeNodePosition_Statics::NewProp_Node_MetaData)) };
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetTreeNodePosition_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TreeLayoutFunctionLibrary_eventGetTreeNodePosition_Parms, ReturnValue), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetTreeNodePosition_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetTreeNodePosition_Statics::NewProp_Node,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetTreeNodePosition_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetTreeNodePosition_Statics::Function_MetaDataParams[] = {
		{ "Comment", "/**\n    * @brief Grabs the position of a node\n    * @param Node - Node to get the position of\n    * @return Returns the node's position\n    */" },
		{ "ModuleRelativePath", "Public/TreeLayoutFunctionLibrary.h" },
		{ "ToolTip", "@brief Grabs the position of a node\n@param Node - Node to get the position of\n@return Returns the node's position" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetTreeNodePosition_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UTreeLayoutFunctionLibrary, nullptr, "GetTreeNodePosition", nullptr, nullptr, sizeof(TreeLayoutFunctionLibrary_eventGetTreeNodePosition_Parms), Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetTreeNodePosition_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetTreeNodePosition_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14C22401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetTreeNodePosition_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetTreeNodePosition_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetTreeNodePosition()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetTreeNodePosition_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UTreeLayoutFunctionLibrary_IsNodeValid_Statics
	{
		struct TreeLayoutFunctionLibrary_eventIsNodeValid_Parms
		{
			FUpTreeNode Node;
			bool ReturnValue;
		};
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Node_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Node;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[];
#endif
		static void NewProp_ReturnValue_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTreeLayoutFunctionLibrary_IsNodeValid_Statics::NewProp_Node_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_IsNodeValid_Statics::NewProp_Node = { "Node", nullptr, (EPropertyFlags)0x0010000008000182, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TreeLayoutFunctionLibrary_eventIsNodeValid_Parms, Node), Z_Construct_UScriptStruct_FUpTreeNode, METADATA_PARAMS(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_IsNodeValid_Statics::NewProp_Node_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_IsNodeValid_Statics::NewProp_Node_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTreeLayoutFunctionLibrary_IsNodeValid_Statics::NewProp_ReturnValue_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	void Z_Construct_UFunction_UTreeLayoutFunctionLibrary_IsNodeValid_Statics::NewProp_ReturnValue_SetBit(void* Obj)
	{
		((TreeLayoutFunctionLibrary_eventIsNodeValid_Parms*)Obj)->ReturnValue = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_IsNodeValid_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000582, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(TreeLayoutFunctionLibrary_eventIsNodeValid_Parms), &Z_Construct_UFunction_UTreeLayoutFunctionLibrary_IsNodeValid_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_IsNodeValid_Statics::NewProp_ReturnValue_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_IsNodeValid_Statics::NewProp_ReturnValue_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UTreeLayoutFunctionLibrary_IsNodeValid_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutFunctionLibrary_IsNodeValid_Statics::NewProp_Node,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutFunctionLibrary_IsNodeValid_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTreeLayoutFunctionLibrary_IsNodeValid_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/TreeLayoutFunctionLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_IsNodeValid_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UTreeLayoutFunctionLibrary, nullptr, "IsNodeValid", nullptr, nullptr, sizeof(TreeLayoutFunctionLibrary_eventIsNodeValid_Parms), Z_Construct_UFunction_UTreeLayoutFunctionLibrary_IsNodeValid_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_IsNodeValid_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14422401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_IsNodeValid_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_IsNodeValid_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UTreeLayoutFunctionLibrary_IsNodeValid()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UTreeLayoutFunctionLibrary_IsNodeValid_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets_Statics
	{
		struct TreeLayoutFunctionLibrary_eventMakeConnectionBetweenWidgets_Parms
		{
			const UUserWidget* ContainingWidget;
			const UUserWidget* Parent;
			const UUserWidget* Child;
			FUpTreeConnector ReturnValue;
		};
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ContainingWidget_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_ContainingWidget;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Parent_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Parent;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Child_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Child;
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets_Statics::NewProp_ContainingWidget_MetaData[] = {
		{ "EditInline", "true" },
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets_Statics::NewProp_ContainingWidget = { "ContainingWidget", nullptr, (EPropertyFlags)0x0010000000080082, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TreeLayoutFunctionLibrary_eventMakeConnectionBetweenWidgets_Parms, ContainingWidget), Z_Construct_UClass_UUserWidget_NoRegister, METADATA_PARAMS(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets_Statics::NewProp_ContainingWidget_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets_Statics::NewProp_ContainingWidget_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets_Statics::NewProp_Parent_MetaData[] = {
		{ "EditInline", "true" },
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets_Statics::NewProp_Parent = { "Parent", nullptr, (EPropertyFlags)0x0010000000080082, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TreeLayoutFunctionLibrary_eventMakeConnectionBetweenWidgets_Parms, Parent), Z_Construct_UClass_UUserWidget_NoRegister, METADATA_PARAMS(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets_Statics::NewProp_Parent_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets_Statics::NewProp_Parent_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets_Statics::NewProp_Child_MetaData[] = {
		{ "EditInline", "true" },
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets_Statics::NewProp_Child = { "Child", nullptr, (EPropertyFlags)0x0010000000080082, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TreeLayoutFunctionLibrary_eventMakeConnectionBetweenWidgets_Parms, Child), Z_Construct_UClass_UUserWidget_NoRegister, METADATA_PARAMS(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets_Statics::NewProp_Child_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets_Statics::NewProp_Child_MetaData)) };
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TreeLayoutFunctionLibrary_eventMakeConnectionBetweenWidgets_Parms, ReturnValue), Z_Construct_UScriptStruct_FUpTreeConnector, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets_Statics::NewProp_ContainingWidget,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets_Statics::NewProp_Parent,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets_Statics::NewProp_Child,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets_Statics::Function_MetaDataParams[] = {
		{ "Comment", "/**\n    * @brief Used to make connections between the center of parent and child node widgets given only the node widget data and not the node data.\n    * ! Invariant - For accurate results, both widgets must be on screen. You can also try using UUpTree::MakeNodeConnections which works when the widgets are not all on screen at once.\n    * @param ContainingWidget - Widget that contains both parent and child (most likely a canvas)\n    * @param Parent - Parent Widget\n    * @param Child  - Child Widget\n    * @return Returns a connector connecting between the center of the parent and center of the child widget.\n    */" },
		{ "ModuleRelativePath", "Public/TreeLayoutFunctionLibrary.h" },
		{ "ToolTip", "@brief Used to make connections between the center of parent and child node widgets given only the node widget data and not the node data.\n! Invariant - For accurate results, both widgets must be on screen. You can also try using UUpTree::MakeNodeConnections which works when the widgets are not all on screen at once.\n@param ContainingWidget - Widget that contains both parent and child (most likely a canvas)\n@param Parent - Parent Widget\n@param Child  - Child Widget\n@return Returns a connector connecting between the center of the parent and center of the child widget." },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UTreeLayoutFunctionLibrary, nullptr, "MakeConnectionBetweenWidgets", nullptr, nullptr, sizeof(TreeLayoutFunctionLibrary_eventMakeConnectionBetweenWidgets_Parms), Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UTreeLayoutFunctionLibrary_NodeToString_Statics
	{
		struct TreeLayoutFunctionLibrary_eventNodeToString_Parms
		{
			FUpTreeNode Node;
			FString ReturnValue;
		};
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Node_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Node;
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTreeLayoutFunctionLibrary_NodeToString_Statics::NewProp_Node_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_NodeToString_Statics::NewProp_Node = { "Node", nullptr, (EPropertyFlags)0x0010000008000182, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TreeLayoutFunctionLibrary_eventNodeToString_Parms, Node), Z_Construct_UScriptStruct_FUpTreeNode, METADATA_PARAMS(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_NodeToString_Statics::NewProp_Node_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_NodeToString_Statics::NewProp_Node_MetaData)) };
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_NodeToString_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TreeLayoutFunctionLibrary_eventNodeToString_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UTreeLayoutFunctionLibrary_NodeToString_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutFunctionLibrary_NodeToString_Statics::NewProp_Node,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutFunctionLibrary_NodeToString_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTreeLayoutFunctionLibrary_NodeToString_Statics::Function_MetaDataParams[] = {
		{ "BlueprintAutocast", "" },
		{ "Comment", "/** Can use this to debug nodes in BPs */" },
		{ "ModuleRelativePath", "Public/TreeLayoutFunctionLibrary.h" },
		{ "ToolTip", "Can use this to debug nodes in BPs" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_NodeToString_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UTreeLayoutFunctionLibrary, nullptr, "NodeToString", nullptr, nullptr, sizeof(TreeLayoutFunctionLibrary_eventNodeToString_Parms), Z_Construct_UFunction_UTreeLayoutFunctionLibrary_NodeToString_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_NodeToString_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14422401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_NodeToString_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_NodeToString_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UTreeLayoutFunctionLibrary_NodeToString()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UTreeLayoutFunctionLibrary_NodeToString_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics
	{
		struct TreeLayoutFunctionLibrary_eventStratifyData_Parms
		{
			UObject* Outer;
			TArray<UObject*> DataObjects;
			TArray<FName> Ids;
			TArray<FName> ParentIds;
			UUpTree* ReturnValue;
		};
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Outer;
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_DataObjects_Inner;
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_DataObjects;
		static const UE4CodeGen_Private::FNamePropertyParams NewProp_Ids_Inner;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Ids_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_Ids;
		static const UE4CodeGen_Private::FNamePropertyParams NewProp_ParentIds_Inner;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ParentIds_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_ParentIds;
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::NewProp_Outer = { "Outer", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TreeLayoutFunctionLibrary_eventStratifyData_Parms, Outer), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::NewProp_DataObjects_Inner = { "DataObjects", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::NewProp_DataObjects = { "DataObjects", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TreeLayoutFunctionLibrary_eventStratifyData_Parms, DataObjects), EArrayPropertyFlags::None, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::NewProp_Ids_Inner = { "Ids", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::NewProp_Ids_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::NewProp_Ids = { "Ids", nullptr, (EPropertyFlags)0x0010000008000182, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TreeLayoutFunctionLibrary_eventStratifyData_Parms, Ids), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::NewProp_Ids_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::NewProp_Ids_MetaData)) };
	const UE4CodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::NewProp_ParentIds_Inner = { "ParentIds", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::NewProp_ParentIds_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::NewProp_ParentIds = { "ParentIds", nullptr, (EPropertyFlags)0x0010000008000182, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TreeLayoutFunctionLibrary_eventStratifyData_Parms, ParentIds), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::NewProp_ParentIds_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::NewProp_ParentIds_MetaData)) };
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TreeLayoutFunctionLibrary_eventStratifyData_Parms, ReturnValue), Z_Construct_UClass_UUpTree_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::NewProp_Outer,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::NewProp_DataObjects_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::NewProp_DataObjects,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::NewProp_Ids_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::NewProp_Ids,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::NewProp_ParentIds_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::NewProp_ParentIds,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::Function_MetaDataParams[] = {
		{ "Comment", "/**\n    * @brief Generates a hierarchy from a relational table (e.g. {a=>b, b=>c, ...})\n    * @param Outer - Object which owns the newly created tree (tied lifetimes).\n    * @param DataObjects - List of UObjects with UPROPERTY's for the ID and Parent ID references.\n    * @param Ids - List of ids for each of the nodes.\n    * @param ParentIds - Each ParentId should be a name inside Ids. The ParentId identifies the current node's parent. Root has a ParentId of \"\".\n    * @return - Returns a UObject with data that holds node data generated from the relational table.\n    */" },
		{ "DefaultToSelf", "Outer" },
		{ "HidePin", "Outer" },
		{ "ModuleRelativePath", "Public/TreeLayoutFunctionLibrary.h" },
		{ "ToolTip", "@brief Generates a hierarchy from a relational table (e.g. {a=>b, b=>c, ...})\n@param Outer - Object which owns the newly created tree (tied lifetimes).\n@param DataObjects - List of UObjects with UPROPERTY's for the ID and Parent ID references.\n@param Ids - List of ids for each of the nodes.\n@param ParentIds - Each ParentId should be a name inside Ids. The ParentId identifies the current node's parent. Root has a ParentId of \"\".\n@return - Returns a UObject with data that holds node data generated from the relational table." },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UTreeLayoutFunctionLibrary, nullptr, "StratifyData", nullptr, nullptr, sizeof(TreeLayoutFunctionLibrary_eventStratifyData_Parms), Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UTreeLayoutFunctionLibrary_UpTreeToString_Statics
	{
		struct TreeLayoutFunctionLibrary_eventUpTreeToString_Parms
		{
			UUpTree* Tree;
			FString ReturnValue;
		};
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Tree;
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_UpTreeToString_Statics::NewProp_Tree = { "Tree", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TreeLayoutFunctionLibrary_eventUpTreeToString_Parms, Tree), Z_Construct_UClass_UUpTree_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_UpTreeToString_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TreeLayoutFunctionLibrary_eventUpTreeToString_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UTreeLayoutFunctionLibrary_UpTreeToString_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutFunctionLibrary_UpTreeToString_Statics::NewProp_Tree,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutFunctionLibrary_UpTreeToString_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTreeLayoutFunctionLibrary_UpTreeToString_Statics::Function_MetaDataParams[] = {
		{ "BlueprintAutocast", "" },
		{ "ModuleRelativePath", "Public/TreeLayoutFunctionLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UTreeLayoutFunctionLibrary_UpTreeToString_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UTreeLayoutFunctionLibrary, nullptr, "UpTreeToString", nullptr, nullptr, sizeof(TreeLayoutFunctionLibrary_eventUpTreeToString_Parms), Z_Construct_UFunction_UTreeLayoutFunctionLibrary_UpTreeToString_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_UpTreeToString_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_UpTreeToString_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutFunctionLibrary_UpTreeToString_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UTreeLayoutFunctionLibrary_UpTreeToString()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UTreeLayoutFunctionLibrary_UpTreeToString_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_UTreeLayoutFunctionLibrary_NoRegister()
	{
		return UTreeLayoutFunctionLibrary::StaticClass();
	}
	struct Z_Construct_UClass_UTreeLayoutFunctionLibrary_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UTreeLayoutFunctionLibrary_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UBlueprintFunctionLibrary,
		(UObject* (*)())Z_Construct_UPackage__Script_TreeLayout,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UTreeLayoutFunctionLibrary_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UTreeLayoutFunctionLibrary_ConnectorToString, "ConnectorToString" }, // 3543739730
		{ &Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetParent, "GetParent" }, // 1083681683
		{ &Z_Construct_UFunction_UTreeLayoutFunctionLibrary_GetTreeNodePosition, "GetTreeNodePosition" }, // 2942979046
		{ &Z_Construct_UFunction_UTreeLayoutFunctionLibrary_IsNodeValid, "IsNodeValid" }, // 911979442
		{ &Z_Construct_UFunction_UTreeLayoutFunctionLibrary_MakeConnectionBetweenWidgets, "MakeConnectionBetweenWidgets" }, // 449766124
		{ &Z_Construct_UFunction_UTreeLayoutFunctionLibrary_NodeToString, "NodeToString" }, // 649075266
		{ &Z_Construct_UFunction_UTreeLayoutFunctionLibrary_StratifyData, "StratifyData" }, // 2867003114
		{ &Z_Construct_UFunction_UTreeLayoutFunctionLibrary_UpTreeToString, "UpTreeToString" }, // 1717730270
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTreeLayoutFunctionLibrary_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "TreeLayoutFunctionLibrary.h" },
		{ "ModuleRelativePath", "Public/TreeLayoutFunctionLibrary.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UTreeLayoutFunctionLibrary_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UTreeLayoutFunctionLibrary>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UTreeLayoutFunctionLibrary_Statics::ClassParams = {
		&UTreeLayoutFunctionLibrary::StaticClass,
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
		0x001000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UTreeLayoutFunctionLibrary_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UTreeLayoutFunctionLibrary_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UTreeLayoutFunctionLibrary()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UTreeLayoutFunctionLibrary_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UTreeLayoutFunctionLibrary, 673191230);
	template<> TREELAYOUT_API UClass* StaticClass<UTreeLayoutFunctionLibrary>()
	{
		return UTreeLayoutFunctionLibrary::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UTreeLayoutFunctionLibrary(Z_Construct_UClass_UTreeLayoutFunctionLibrary, &UTreeLayoutFunctionLibrary::StaticClass, TEXT("/Script/TreeLayout"), TEXT("UTreeLayoutFunctionLibrary"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UTreeLayoutFunctionLibrary);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
