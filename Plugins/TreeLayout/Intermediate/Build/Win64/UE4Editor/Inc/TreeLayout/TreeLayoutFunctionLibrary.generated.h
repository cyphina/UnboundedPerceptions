// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UUpTree;
struct FUpTreeNode;
struct FUpTreeConnector;
class UUserWidget;
struct FVector2D;
class UObject;
#ifdef TREELAYOUT_TreeLayoutFunctionLibrary_generated_h
#error "TreeLayoutFunctionLibrary.generated.h already included, missing '#pragma once' in TreeLayoutFunctionLibrary.h"
#endif
#define TREELAYOUT_TreeLayoutFunctionLibrary_generated_h

#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutFunctionLibrary_h_17_SPARSE_DATA
#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutFunctionLibrary_h_17_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execUpTreeToString); \
	DECLARE_FUNCTION(execNodeToString); \
	DECLARE_FUNCTION(execConnectorToString); \
	DECLARE_FUNCTION(execGetParent); \
	DECLARE_FUNCTION(execIsNodeValid); \
	DECLARE_FUNCTION(execMakeConnectionBetweenWidgets); \
	DECLARE_FUNCTION(execGetTreeNodePosition); \
	DECLARE_FUNCTION(execStratifyData);


#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutFunctionLibrary_h_17_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execUpTreeToString); \
	DECLARE_FUNCTION(execNodeToString); \
	DECLARE_FUNCTION(execConnectorToString); \
	DECLARE_FUNCTION(execGetParent); \
	DECLARE_FUNCTION(execIsNodeValid); \
	DECLARE_FUNCTION(execMakeConnectionBetweenWidgets); \
	DECLARE_FUNCTION(execGetTreeNodePosition); \
	DECLARE_FUNCTION(execStratifyData);


#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutFunctionLibrary_h_17_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUTreeLayoutFunctionLibrary(); \
	friend struct Z_Construct_UClass_UTreeLayoutFunctionLibrary_Statics; \
public: \
	DECLARE_CLASS(UTreeLayoutFunctionLibrary, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/TreeLayout"), NO_API) \
	DECLARE_SERIALIZER(UTreeLayoutFunctionLibrary)


#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutFunctionLibrary_h_17_INCLASS \
private: \
	static void StaticRegisterNativesUTreeLayoutFunctionLibrary(); \
	friend struct Z_Construct_UClass_UTreeLayoutFunctionLibrary_Statics; \
public: \
	DECLARE_CLASS(UTreeLayoutFunctionLibrary, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/TreeLayout"), NO_API) \
	DECLARE_SERIALIZER(UTreeLayoutFunctionLibrary)


#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutFunctionLibrary_h_17_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UTreeLayoutFunctionLibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UTreeLayoutFunctionLibrary) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UTreeLayoutFunctionLibrary); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UTreeLayoutFunctionLibrary); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UTreeLayoutFunctionLibrary(UTreeLayoutFunctionLibrary&&); \
	NO_API UTreeLayoutFunctionLibrary(const UTreeLayoutFunctionLibrary&); \
public:


#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutFunctionLibrary_h_17_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UTreeLayoutFunctionLibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UTreeLayoutFunctionLibrary(UTreeLayoutFunctionLibrary&&); \
	NO_API UTreeLayoutFunctionLibrary(const UTreeLayoutFunctionLibrary&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UTreeLayoutFunctionLibrary); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UTreeLayoutFunctionLibrary); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UTreeLayoutFunctionLibrary)


#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutFunctionLibrary_h_17_PRIVATE_PROPERTY_OFFSET
#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutFunctionLibrary_h_14_PROLOG
#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutFunctionLibrary_h_17_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutFunctionLibrary_h_17_PRIVATE_PROPERTY_OFFSET \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutFunctionLibrary_h_17_SPARSE_DATA \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutFunctionLibrary_h_17_RPC_WRAPPERS \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutFunctionLibrary_h_17_INCLASS \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutFunctionLibrary_h_17_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutFunctionLibrary_h_17_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutFunctionLibrary_h_17_PRIVATE_PROPERTY_OFFSET \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutFunctionLibrary_h_17_SPARSE_DATA \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutFunctionLibrary_h_17_RPC_WRAPPERS_NO_PURE_DECLS \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutFunctionLibrary_h_17_INCLASS_NO_PURE_DECLS \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutFunctionLibrary_h_17_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> TREELAYOUT_API UClass* StaticClass<class UTreeLayoutFunctionLibrary>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_TreeLayoutFunctionLibrary_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
