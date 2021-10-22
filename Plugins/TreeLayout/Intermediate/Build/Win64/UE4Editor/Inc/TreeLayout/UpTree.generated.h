// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
struct FUpTreeNode;
struct FUpTreeConnector;
 struct FUpTreeNode;
class UUpTreeLayoutStrategy;
struct FVector2D;
#ifdef TREELAYOUT_UpTree_generated_h
#error "UpTree.generated.h already included, missing '#pragma once' in UpTree.h"
#endif
#define TREELAYOUT_UpTree_generated_h

#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_13_SPARSE_DATA
#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_13_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execBP_GetRoot); \
	DECLARE_FUNCTION(execResetTree); \
	DECLARE_FUNCTION(execMakeNodeConnections); \
	DECLARE_FUNCTION(execGetHierarchy); \
	DECLARE_FUNCTION(execPositionNodes); \
	DECLARE_FUNCTION(execGetNodeById); \
	DECLARE_FUNCTION(execMakeStringFromTree); \
	DECLARE_FUNCTION(execGetMaxDepth);


#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_13_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execBP_GetRoot); \
	DECLARE_FUNCTION(execResetTree); \
	DECLARE_FUNCTION(execMakeNodeConnections); \
	DECLARE_FUNCTION(execGetHierarchy); \
	DECLARE_FUNCTION(execPositionNodes); \
	DECLARE_FUNCTION(execGetNodeById); \
	DECLARE_FUNCTION(execMakeStringFromTree); \
	DECLARE_FUNCTION(execGetMaxDepth);


#if WITH_EDITOR
#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_13_EDITOR_ONLY_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execMakeThreadConnections);


#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_13_EDITOR_ONLY_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execMakeThreadConnections);


#else
#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_13_EDITOR_ONLY_RPC_WRAPPERS
#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_13_EDITOR_ONLY_RPC_WRAPPERS_NO_PURE_DECLS
#endif //WITH_EDITOR
#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_13_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUUpTree(); \
	friend struct Z_Construct_UClass_UUpTree_Statics; \
public: \
	DECLARE_CLASS(UUpTree, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/TreeLayout"), NO_API) \
	DECLARE_SERIALIZER(UUpTree)


#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_13_INCLASS \
private: \
	static void StaticRegisterNativesUUpTree(); \
	friend struct Z_Construct_UClass_UUpTree_Statics; \
public: \
	DECLARE_CLASS(UUpTree, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/TreeLayout"), NO_API) \
	DECLARE_SERIALIZER(UUpTree)


#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_13_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UUpTree(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UUpTree) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UUpTree); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UUpTree); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UUpTree(UUpTree&&); \
	NO_API UUpTree(const UUpTree&); \
public:


#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_13_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UUpTree(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UUpTree(UUpTree&&); \
	NO_API UUpTree(const UUpTree&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UUpTree); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UUpTree); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UUpTree)


#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_13_PRIVATE_PROPERTY_OFFSET
#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_10_PROLOG
#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_13_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_13_PRIVATE_PROPERTY_OFFSET \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_13_SPARSE_DATA \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_13_RPC_WRAPPERS \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_13_EDITOR_ONLY_RPC_WRAPPERS \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_13_INCLASS \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_13_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_13_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_13_PRIVATE_PROPERTY_OFFSET \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_13_SPARSE_DATA \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_13_RPC_WRAPPERS_NO_PURE_DECLS \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_13_EDITOR_ONLY_RPC_WRAPPERS_NO_PURE_DECLS \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_13_INCLASS_NO_PURE_DECLS \
	MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h_13_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> TREELAYOUT_API UClass* StaticClass<class UUpTree>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID MyProject_Plugins_TreeLayout_Source_TreeLayout_Public_UpTree_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
