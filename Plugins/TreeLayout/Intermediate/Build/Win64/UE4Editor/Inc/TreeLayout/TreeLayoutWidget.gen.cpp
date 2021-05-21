// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "TreeLayout/Public/TreeLayoutWidget.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTreeLayoutWidget() {}
// Cross Module References
	TREELAYOUT_API UClass* Z_Construct_UClass_UTreeLayoutWidget_NoRegister();
	TREELAYOUT_API UClass* Z_Construct_UClass_UTreeLayoutWidget();
	UMG_API UClass* Z_Construct_UClass_UUserWidget();
	UPackage* Z_Construct_UPackage__Script_TreeLayout();
	TREELAYOUT_API UClass* Z_Construct_UClass_UUpTree_NoRegister();
	UMG_API UClass* Z_Construct_UClass_UUserWidget_NoRegister();
	COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FLinearColor();
	TREELAYOUT_API UClass* Z_Construct_UClass_UUpTreeLayoutStrategy_NoRegister();
	UMG_API UClass* Z_Construct_UClass_UCanvasPanel_NoRegister();
// End Cross Module References
	DEFINE_FUNCTION(UTreeLayoutWidget::execShowTree)
	{
		P_GET_OBJECT(UUpTree,Z_Param_Tree);
		P_GET_PROPERTY(FFloatProperty,Z_Param_TreeSizeX);
		P_GET_PROPERTY(FFloatProperty,Z_Param_TreeSizeY);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(TArray<UUserWidget*>*)Z_Param__Result=P_THIS->ShowTree(Z_Param_Tree,Z_Param_TreeSizeX,Z_Param_TreeSizeY);
		P_NATIVE_END;
	}
	void UTreeLayoutWidget::StaticRegisterNativesUTreeLayoutWidget()
	{
		UClass* Class = UTreeLayoutWidget::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "ShowTree", &UTreeLayoutWidget::execShowTree },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UTreeLayoutWidget_ShowTree_Statics
	{
		struct TreeLayoutWidget_eventShowTree_Parms
		{
			UUpTree* Tree;
			float TreeSizeX;
			float TreeSizeY;
			TArray<UUserWidget*> ReturnValue;
		};
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Tree;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_TreeSizeX;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_TreeSizeY;
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_ReturnValue_Inner;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UTreeLayoutWidget_ShowTree_Statics::NewProp_Tree = { "Tree", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TreeLayoutWidget_eventShowTree_Parms, Tree), Z_Construct_UClass_UUpTree_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UTreeLayoutWidget_ShowTree_Statics::NewProp_TreeSizeX = { "TreeSizeX", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TreeLayoutWidget_eventShowTree_Parms, TreeSizeX), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UTreeLayoutWidget_ShowTree_Statics::NewProp_TreeSizeY = { "TreeSizeY", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TreeLayoutWidget_eventShowTree_Parms, TreeSizeY), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UTreeLayoutWidget_ShowTree_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000000000080008, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UClass_UUserWidget_NoRegister, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTreeLayoutWidget_ShowTree_Statics::NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UTreeLayoutWidget_ShowTree_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010008000000588, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TreeLayoutWidget_eventShowTree_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UFunction_UTreeLayoutWidget_ShowTree_Statics::NewProp_ReturnValue_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutWidget_ShowTree_Statics::NewProp_ReturnValue_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UTreeLayoutWidget_ShowTree_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutWidget_ShowTree_Statics::NewProp_Tree,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutWidget_ShowTree_Statics::NewProp_TreeSizeX,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutWidget_ShowTree_Statics::NewProp_TreeSizeY,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutWidget_ShowTree_Statics::NewProp_ReturnValue_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTreeLayoutWidget_ShowTree_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTreeLayoutWidget_ShowTree_Statics::Function_MetaDataParams[] = {
		{ "Comment", "/**\n    * @brief Creates a visual for this tree which involves generating widgets and positioning them.\n    * @param Tree - Tree used to figure out the relationship between nodes.\n    * @param TreeSizeX - Used to space nodes horizontally.\n    * @param TreeSizeY - Used to space nodes vertically.\n    * @return Returns the generated widgets in case some other system wants to handle them while the canvas in this widget retains ownership of them.\n    */" },
		{ "ModuleRelativePath", "Public/TreeLayoutWidget.h" },
		{ "ToolTip", "@brief Creates a visual for this tree which involves generating widgets and positioning them.\n@param Tree - Tree used to figure out the relationship between nodes.\n@param TreeSizeX - Used to space nodes horizontally.\n@param TreeSizeY - Used to space nodes vertically.\n@return Returns the generated widgets in case some other system wants to handle them while the canvas in this widget retains ownership of them." },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UTreeLayoutWidget_ShowTree_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UTreeLayoutWidget, nullptr, "ShowTree", nullptr, nullptr, sizeof(TreeLayoutWidget_eventShowTree_Parms), Z_Construct_UFunction_UTreeLayoutWidget_ShowTree_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutWidget_ShowTree_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UTreeLayoutWidget_ShowTree_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UTreeLayoutWidget_ShowTree_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UTreeLayoutWidget_ShowTree()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UTreeLayoutWidget_ShowTree_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_UTreeLayoutWidget_NoRegister()
	{
		return UTreeLayoutWidget::StaticClass();
	}
	struct Z_Construct_UClass_UTreeLayoutWidget_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_WidgetClass_MetaData[];
#endif
		static const UE4CodeGen_Private::FClassPropertyParams NewProp_WidgetClass;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_BranchColor_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_BranchColor;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ThreadColor_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_ThreadColor;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bShowConnections_MetaData[];
#endif
		static void NewProp_bShowConnections_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bShowConnections;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bShowThreads_MetaData[];
#endif
		static void NewProp_bShowThreads_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bShowThreads;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LayoutStrategy_MetaData[];
#endif
		static const UE4CodeGen_Private::FClassPropertyParams NewProp_LayoutStrategy;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Can_Node_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Can_Node;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UTreeLayoutWidget_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UUserWidget,
		(UObject* (*)())Z_Construct_UPackage__Script_TreeLayout,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UTreeLayoutWidget_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UTreeLayoutWidget_ShowTree, "ShowTree" }, // 4239916256
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTreeLayoutWidget_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * A widget that generates homogeneous sub widgets laid out on a tree formation given an array of FUpTreeNode's that describe the hierarchical relationship between nodes.\n * It can also draw connections between the widgets.\n * We can change what tree gets shown by this widget at runtime.\n * Does not handle the underlying logic for the widgets it layers out.\n */" },
		{ "IncludePath", "TreeLayoutWidget.h" },
		{ "ModuleRelativePath", "Public/TreeLayoutWidget.h" },
		{ "ToolTip", "A widget that generates homogeneous sub widgets laid out on a tree formation given an array of FUpTreeNode's that describe the hierarchical relationship between nodes.\nIt can also draw connections between the widgets.\nWe can change what tree gets shown by this widget at runtime.\nDoes not handle the underlying logic for the widgets it layers out." },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_WidgetClass_MetaData[] = {
		{ "Category", "TreeLayoutWidget" },
		{ "Comment", "/**\n    * @brief Type of widgets that are generated and laid out in a tree formation.\n    */" },
		{ "ModuleRelativePath", "Public/TreeLayoutWidget.h" },
		{ "ToolTip", "@brief Type of widgets that are generated and laid out in a tree formation." },
	};
#endif
	const UE4CodeGen_Private::FClassPropertyParams Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_WidgetClass = { "WidgetClass", nullptr, (EPropertyFlags)0x0014000000000001, UE4CodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTreeLayoutWidget, WidgetClass), Z_Construct_UClass_UUserWidget_NoRegister, Z_Construct_UClass_UClass, METADATA_PARAMS(Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_WidgetClass_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_WidgetClass_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_BranchColor_MetaData[] = {
		{ "Category", "TreeLayoutWidget" },
		{ "ModuleRelativePath", "Public/TreeLayoutWidget.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_BranchColor = { "BranchColor", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTreeLayoutWidget, BranchColor), Z_Construct_UScriptStruct_FLinearColor, METADATA_PARAMS(Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_BranchColor_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_BranchColor_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_ThreadColor_MetaData[] = {
		{ "Category", "TreeLayoutWidget" },
		{ "ModuleRelativePath", "Public/TreeLayoutWidget.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_ThreadColor = { "ThreadColor", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTreeLayoutWidget, ThreadColor), Z_Construct_UScriptStruct_FLinearColor, METADATA_PARAMS(Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_ThreadColor_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_ThreadColor_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_bShowConnections_MetaData[] = {
		{ "Category", "TreeLayoutWidget" },
		{ "Comment", "/**\n    * @brief Show connections between parent and children nodes\n    */" },
		{ "ModuleRelativePath", "Public/TreeLayoutWidget.h" },
		{ "ToolTip", "@brief Show connections between parent and children nodes" },
	};
#endif
	void Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_bShowConnections_SetBit(void* Obj)
	{
		((UTreeLayoutWidget*)Obj)->bShowConnections = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_bShowConnections = { "bShowConnections", nullptr, (EPropertyFlags)0x0020080000000001, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UTreeLayoutWidget), &Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_bShowConnections_SetBit, METADATA_PARAMS(Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_bShowConnections_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_bShowConnections_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_bShowThreads_MetaData[] = {
		{ "Category", "TreeLayoutWidget" },
		{ "Comment", "/**\n    * @brief Threads are connections made as part to optimize the layout algorithm. You can see enable seeing these to understand how the algorithm works (if it uses threads).\n    */" },
		{ "ModuleRelativePath", "Public/TreeLayoutWidget.h" },
		{ "ToolTip", "@brief Threads are connections made as part to optimize the layout algorithm. You can see enable seeing these to understand how the algorithm works (if it uses threads)." },
	};
#endif
	void Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_bShowThreads_SetBit(void* Obj)
	{
		((UTreeLayoutWidget*)Obj)->bShowThreads = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_bShowThreads = { "bShowThreads", nullptr, (EPropertyFlags)0x0020080000000001, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UTreeLayoutWidget), &Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_bShowThreads_SetBit, METADATA_PARAMS(Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_bShowThreads_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_bShowThreads_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_LayoutStrategy_MetaData[] = {
		{ "Category", "TreeLayoutWidget" },
		{ "Comment", "/**\n    * @brief The strategy used to layout the tree node widgets\n    */" },
		{ "ModuleRelativePath", "Public/TreeLayoutWidget.h" },
		{ "ToolTip", "@brief The strategy used to layout the tree node widgets" },
	};
#endif
	const UE4CodeGen_Private::FClassPropertyParams Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_LayoutStrategy = { "LayoutStrategy", nullptr, (EPropertyFlags)0x0024080000000001, UE4CodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTreeLayoutWidget, LayoutStrategy), Z_Construct_UClass_UUpTreeLayoutStrategy_NoRegister, Z_Construct_UClass_UClass, METADATA_PARAMS(Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_LayoutStrategy_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_LayoutStrategy_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_Can_Node_MetaData[] = {
		{ "BindWidget", "" },
		{ "Category", "TreeLayoutWidget" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/TreeLayoutWidget.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_Can_Node = { "Can_Node", nullptr, (EPropertyFlags)0x002008000008001c, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTreeLayoutWidget, Can_Node), Z_Construct_UClass_UCanvasPanel_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_Can_Node_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_Can_Node_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UTreeLayoutWidget_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_WidgetClass,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_BranchColor,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_ThreadColor,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_bShowConnections,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_bShowThreads,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_LayoutStrategy,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTreeLayoutWidget_Statics::NewProp_Can_Node,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UTreeLayoutWidget_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UTreeLayoutWidget>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UTreeLayoutWidget_Statics::ClassParams = {
		&UTreeLayoutWidget::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_UTreeLayoutWidget_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_UTreeLayoutWidget_Statics::PropPointers),
		0,
		0x00B010A0u,
		METADATA_PARAMS(Z_Construct_UClass_UTreeLayoutWidget_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UTreeLayoutWidget_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UTreeLayoutWidget()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UTreeLayoutWidget_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UTreeLayoutWidget, 4007352627);
	template<> TREELAYOUT_API UClass* StaticClass<UTreeLayoutWidget>()
	{
		return UTreeLayoutWidget::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UTreeLayoutWidget(Z_Construct_UClass_UTreeLayoutWidget, &UTreeLayoutWidget::StaticClass, TEXT("/Script/TreeLayout"), TEXT("UTreeLayoutWidget"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UTreeLayoutWidget);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
