// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UUpTree;
class UUserWidget;
#ifdef TREELAYOUT_TreeLayoutWidget_generated_h
#error "TreeLayoutWidget.generated.h already included, missing '#pragma once' in TreeLayoutWidget.h"
#endif
#define TREELAYOUT_TreeLayoutWidget_generated_h

#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutWidget_h_21_SPARSE_DATA
#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutWidget_h_21_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execShowTree);


#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutWidget_h_21_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execShowTree);


#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutWidget_h_21_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUTreeLayoutWidget(); \
	friend struct Z_Construct_UClass_UTreeLayoutWidget_Statics; \
public: \
	DECLARE_CLASS(UTreeLayoutWidget, UUserWidget, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/TreeLayout"), NO_API) \
	DECLARE_SERIALIZER(UTreeLayoutWidget)


#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutWidget_h_21_INCLASS \
private: \
	static void StaticRegisterNativesUTreeLayoutWidget(); \
	friend struct Z_Construct_UClass_UTreeLayoutWidget_Statics; \
public: \
	DECLARE_CLASS(UTreeLayoutWidget, UUserWidget, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/TreeLayout"), NO_API) \
	DECLARE_SERIALIZER(UTreeLayoutWidget)


#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutWidget_h_21_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UTreeLayoutWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UTreeLayoutWidget) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UTreeLayoutWidget); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UTreeLayoutWidget); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UTreeLayoutWidget(UTreeLayoutWidget&&); \
	NO_API UTreeLayoutWidget(const UTreeLayoutWidget&); \
public:


#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutWidget_h_21_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UTreeLayoutWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UTreeLayoutWidget(UTreeLayoutWidget&&); \
	NO_API UTreeLayoutWidget(const UTreeLayoutWidget&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UTreeLayoutWidget); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UTreeLayoutWidget); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UTreeLayoutWidget)


#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutWidget_h_21_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO__bShowConnections() { return STRUCT_OFFSET(UTreeLayoutWidget, bShowConnections); } \
	FORCEINLINE static uint32 __PPO__bShowThreads() { return STRUCT_OFFSET(UTreeLayoutWidget, bShowThreads); } \
	FORCEINLINE static uint32 __PPO__LayoutStrategy() { return STRUCT_OFFSET(UTreeLayoutWidget, LayoutStrategy); } \
	FORCEINLINE static uint32 __PPO__Can_Node() { return STRUCT_OFFSET(UTreeLayoutWidget, Can_Node); }


#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutWidget_h_18_PROLOG
#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutWidget_h_21_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutWidget_h_21_PRIVATE_PROPERTY_OFFSET \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutWidget_h_21_SPARSE_DATA \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutWidget_h_21_RPC_WRAPPERS \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutWidget_h_21_INCLASS \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutWidget_h_21_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutWidget_h_21_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutWidget_h_21_PRIVATE_PROPERTY_OFFSET \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutWidget_h_21_SPARSE_DATA \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutWidget_h_21_RPC_WRAPPERS_NO_PURE_DECLS \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutWidget_h_21_INCLASS_NO_PURE_DECLS \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutWidget_h_21_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> TREELAYOUT_API UClass* StaticClass<class UTreeLayoutWidget>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutWidget_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
