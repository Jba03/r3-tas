//
//  script.c
//  r3-tas
//
//  Created by Jba03 on 2022-11-27.
//

#include "script.h"

#pragma mark - Script name tables

static const char * const script_nodetype_table[] = {
    "Keyword",
    "Condition",
    "Operator",
    "Function",
    "Procedure",
    "MetaAction",
    "BeginMacro",
    "EndMacro",
    "EndTree",
    "Field",
    "DsgVar",
    "DsgVarRef",
    "Constant",
    "Real",
    "Button",
    "ConstantVector",
    "Vector",
    "Mask",
    "Module",
    "DsgVarId",
    "String",
    "LipsSynchroRef",
    "FamilyRef",
    "PersoRef",
    "ActionRef",
    "SuperObjectRef",
    "SOLinksRef",
    "WayPointRef",
    "TextRef",
    "ComportRef",
    "ModuleRef",
    "SoundEventRef",
    "ObjectTableRef",
    "GameMaterialRef",
    "VisualMaterial",
    "ParticleGenerator",
    "Color",
    "ModelRef",
    "Light",
    "Caps",
    "Graph",
    "Subroutine",
    "Unknown",
    "CineRef",
    "GraphRef",
};

static const char * const script_keyword_table[] = {
    "If", "IfNot", "If2", "If4", "If8", "If16", "If32", "If64",
    "IfNot2", "IfNot4", "IfNot8", "IfNot16", "IfNot32", "IfNot64",
    "IfDebug ", "IfNotU64 ", "Then", "Else", "EngineGoto", "Me",
    "MainActor", "Nobody", "NoInput", "NoSoundEvent", "NoSoundEvent",
    "NoSuperObject", "Nowhere", "NoSOLinksWord ", "EmptyText", "CapsNull",
    "NoGraph", "NoAction", "NoGMT", "NoVMT", "While", "BeginWhile", "EndWhile",
};

static const char * const script_operator_table[] = {
    "Operator_Plus", "Operator_Minus", "Operator_Mul", "Operator_Div",
    "Operator_UnaryMinus", "Operator_Mod", "Operator_PlusAffect",
    "Operator_MinusAffect", "Operator_MulAffect", "Operator_DivAffect",
    "Operator_PlusPlusAffect", "Operator_MinusMinusAffect",
    "Operator_Affect", "Operator_Dot", ".X", ".Y", ".Z",
    "Operator_VectorPlusVector", "Operator_VectorMinusVector",
    "Operator_VectorUnaryMinus", "Operator_VectorMulScalar",
    "Operator_VectorDivScalar", ".X:=", ".Y:=", ".Z:=", "Operator_Ultra",
    "Operator_ModelCast", "Operator_Array", "Operator_AffectArray"
};

static const char * const script_function_table[] = {
    "GetPersoAbsolutePosition", "GetMyAbsolutePosition", "DistanceFunction",
    "DistanceToPerso", "DistanceFunction", "DistanceFunction", "DistanceFunction",
    "DistanceFunction", "DistanceFunction", "DistanceFunction", "DistanceToPersoCenter",
    "DistanceFunction", "DistanceFunction", "DistanceFunction", "DistanceFunction",
    "DistanceFunction", "DistanceFunction", "GetRadiusWP", "InterpolationBt3WP",
    "BezierBt3WP", "DistanceFunction_DistanceBetweenPersoAndWP", "GetWpAbsolutePosition",
    "MathFunctionInt_ToInt", "MathFunctionInt_RandomInt", "MathFunctionReal_ToReal",
    "MathFunctionReal_Sin", "MathFunctionReal_Cos", "MathFunctionReal_Pow2",
    "MathFunctionReal_SquareRoot", "MathFunctionReal_RandomReal", "MathFunctionReal_Min",
    "MathFunctionReal_Max", "MathFunctionReal_Rad2Deg", "MathFunctionReal_Deg2Rad",
    "MathAbsoluteValue", "MathFunctionReal_Clamp", "MathFunctionReal_Sign",
    "MathFunctionReal_Pow3", "MathFunctionReal_Mod_MapToRange", "Func_TernInf",
    "Func_TernSup", "Func_TernEq", "Func_TernInfEq", "Func_TernSupEq", "Func_TernOp",
    "RealFunction", "HitPointsAndHitPointsMaxFunction_GetHitPoints",
    "HitPointsAndHitPointsMaxFunction_AddHitPoints", "HitPointsAndHitPointsMaxFunction_SubHitPoints",
    "HitPointsAndHitPointsMaxFunction_GetHitPointsMax", "HitPointsAndHitPointsMaxFunction_AddHitPointsMax",
    "HitPointsAndHitPointsMaxFunction_SubHitPointsMax", "ListFunction", "ListFunction",
    "LocalToGlobal", "GlobalToLocal", "LocalToGlobal", "GlobalToLocal", "DummyFunction",
    "DummyFunction", "DummyFunction", "DummyFunction", "SPO_GetDrawFlag", "GetCollidedSPO",
    "GetCollidedSPO", "GetCollidedSPO", "TimeFunction_ElapsedTime", "TimeFunction_ElapsedTimeSince",
    "GetDTFunction", "GetFrameLength", "InputFunction", "InputFunction", "InputFunction",
    "InputFunction", "InputFunction", "InputFunction", "InputFunction", "Code4VitessePadAnalogique",
    "MiscFunction", "MiscFunction", "MiscFunction", "MiscFunction", "ColorFunction", "ColorFunction",
    "ColorFunction", "ColorFunction", "ColorFunction", "ColorFunction", "ColorFunction",
    "ColorFunction", "ColorFunction", "ColorFunction", "ColorFunction", "VisualMaterialFunction",
    "VisualMaterialFunction", "VisualMaterialFunction", "VisualMaterialFunction",
    "VisualMaterialFunction", "VisualMaterialFunction", "VisualMaterialFunction",
    "VisualMaterialFunction", "VisualMaterialFunction", "MultiPlayerFunction", "MultiPlayerFunction",
    "MultiPlayerFunction", "GetActiveMainActors", "GetPadInversions", "GetPadInversions",
    "RLIFunction", "PAD2_Function", "PAD2_Function", "PAD2_Function", "PAD2_Function", "PAD2_Function",
    "PAD2_Function", "PAD2_Function", "InputFunction", "TXT_Function", "InputFunction", "InputFunction",
    "InputFunction", "InputFunction", "InputFunction", "SAV2SaveFunction", "SAV2SaveFunction",
    "SAV2SaveFunction", "SAV2SaveFunction", "SAV2SaveFunction", "GetTransitionSectorIDFunction",
    "FFBFunctions", "FFBFunctions", "FFBFunctions", "CreateCheapCharacterList", "TestCheapCharacterList",
    "GetCheapCharacter", "GetCheapCharacter", "GetCheapCharacter", "Code4PointsDeMagie", "Code4PointsDeMagie",
    "Code4PointsDeMagie", "Code4PointsDeMagie", "Code4PointsDeMagie", "Code4PointsDeMagie", "Code4PointsDair",
    "Code4PointsDair", "Code4PointsDair", "Code4PointsDair", "Code4PointsDair", "Code4PointsDair",
    "Code4PersoLePlusProche", "Code4PersoLePlusProche", "GetNearestActorInCurrentSector",
    "GetNearestActorInCurrentSector", "Code4PersoLePlusProche", "Code4PersoLePlusProche",
    "Code4PersoLePlusProche", "Code4PersoLePlusProche", "Code4PersoLePlusProche", "Code4PersoLePlusProche",
    "Code4PersoLePlusProche", "Code4PersoLePlusProche", "GetNbActivePerso", "Code4PersoLePlusProche",
    "Code4PersoLePlusProche", "Code4PersoLePlusProche", "Code4PersoLePlusProche", "GetCloserNetwork",
    "CloserAndCollisionWPInGraph", "CloserWPInGraph", "CloserWPInGraph", "CloserWPInGraph", "CloserWPInGraph",
    "CloserWPInGraph", "CloserWPInGraph", "CloserWPInGraph", "CloserWPInGraph", "TravelOnAGraph",
    "TravelOnAGraph", "TravelOnAGraph", "TravelOnAGraph", "TravelOnAGraph", "TravelOnAGraph",
    "TravelOnAGraph", "GraphToWayFunction", "GraphToWayFunction", "GraphToWayFunction", "GraphToWayFunction",
    "GraphToWayFunction", "GraphToWayFunction", "GraphToWayFunction", "WayFunction_GetCurrentIndexInPath",
    "WayFunction_SetCurrentIndexInPath", "WayFunction_GetFirstIndexInPath", "WayFunction_GetLastIndexInPath",
    "WayFunction_IncrementCurrentIndexInPath", "WayFunction_IncrementCurrentIndexInPath",
    "WayFunction_GetWayPointOfPath", "WayFunction_GetCapsForNextWayPointOfPath",
    "WayFunction_ChangeCapsForNextWayPointOfPath", "WayFunction_GetWeightForNextWayPointOfPath",
    "WayFunction_ChangeWeightForNextWayPointOfPath", "WayFunction_LookForWPInMSWay",
    "WayFunction_SetCurrentIndexInPathToWP", "GetSituationOnRail", "GraphFunction", "GraphFunction",
    "GraphFunction", "GraphFunction", "MiscFuncOnTypeOfWP", "PersoCapabilityFunction", "CapabilityFunction",
    "GetScrollSpeed", "MTGetScrollSpeed", "GetNbFrame", "DotProduct", "VectorOperations", "VectorOperations",
    "GetSPOCoordinates", "GetSPOCoordinates", "GetSPOCoordinates", "GetSPOCoordinates", "TractionFactorFunction",
    "GetZDxCenter", "GetZDxCenter", "GetZDxCenter", "GetZDxCenter", "Code4TextAffiche", "Code4TextAffiche",
    "GetCPUCounter", "FunctTurnToPosition", "FunctPS2", "FunctPS2", "FunctPS2", "SAV2_DoVarsNeedToBeSet",
    "CINEGetGenerateEvent", "TextComparison", "EmptyText", "Code4VitesseHorizontaleDuPerso",
    "Code4VitesseHorizontaleDuPerso", "3DFunc", "GetPersoSighting", "3DFunc", "3DFunc", "Code4LitPositionZDx",
    "Code4LitPositionZDx", "Code4LitPositionZDx", "Code4LitCentreZDx", "Code4LitCentreZDx", "Code4LitCentreZDx",
    "Code4LitAxeZDx", "Code4LitAxeZDx", "Code4LitAxeZDx", "Code4LitDimensionZDx", "Code4LitDimensionZDx",
    "Code4LitDimensionZDx", "Code4VecteurPointAxe", "Code4VecteurPointAxe", "VectorFunction", "VectorFunction",
    "VectorFunction", "VectorFunction", "VectorFunction", "VectorFunction", "VectorAndAngle", "VectorAndAngle",
    "VectorAndAngle", "GetNormalCollideVector", "GetNormalCollideVector", "GetNormalCollideVector",
    "GetNormalCollideVector", "GetCollidePoint", "GetCollidePoint", "GetCollidePoint", "GetCollidePoint",
    "GetCollidePoint", "GetCollidePoint", "GetCollidePoint", "GetCollidePoint", "GetCollidePoint",
    "GetCollidePoint", "GetCollision", "GetCollision", "GetCollision", "GetCollisionWithCollideMaterial",
    "GetFirstCollisionInfo", "GameMaterialFunction", "GameMaterialFunction", "GameMaterialFunction",
    "CollideCommunicationFunc", "CollideCommunicationFunc", "CollideCommunicationFunc", "CollideCommunicationFunc",
    "GetLastCollisionActor", "ComputeRebondVector", "3DFunc", "3DFunc", "3DFunc", "3DFunc", "3DFunc", "3DFunc",
    "MiscFunction", "NamesFunc", "NamesFunc", "NamesFunc", "NamesFunc", "NamesFunc", "NamesFunc", "NamesFunc",
    "GetMechanicParameter", "GetMechanicParameter", "GetMechanicParameter", "GetMechanicParameter",
    "GetMechanicParameter", "GetMechanicParameter", "GetMechanicParameter", "GetMechanicParameter",
    "GetMechanicParameter", "GetMechanicParameter", "GetMechanicParameter", "GetMechanicParameter",
    "GetMechanicParameter", "GetMechanicParameter", "GetMechanicParameter", "GetMechanicParameter",
    "GetMechanicParameter", "ComputeJumpImpulsion", "GetMechanicParameter", "GetHierarchyLink",
    "GetPriviligedActivationZdx", "GetPriviligedActivationZdx", "GetPriviligedActivationZdx",
    "GetPriviligedActivationZdx", "GetComputationFrequency", "GetComputationFrequency", "GetComputationFrequency",
    "GetBooleanInArray", "GetBooleanInArray", "GetButtonName", "GetCustomValuesFunc", "GetCustomValuesFunc",
    "GetCustomValuesFunc", "GetDriversAvailable", "GetCurrentLanguageId", "GetNbLanguages", "GetLanguageText",
    "TextToInt", "GetNbAvailableResolution", "GetCurrentResolution", "SaveCurrentResolution",
    "IsDisplayModeAvailable", "GetBrightness", "GetNameResolution", "GetVideoOptions", "GetVideoOptions",
    "GetMSSoundValues", "GetMSSoundValues", "GetStdGameLimit", "GetStdGameLimit", "GetStdGameLimit",
    "GetCurrentFrame", "ExecuteVariable", "ComputeProtectKey", "ComputeXor", "ComputeXor", "ComputeXor",
    "ComputeNot", "ComputeXor", "ComputeXor", "ComputeXor", "ComputeXor", "ComputeXor", "GetXBoxRegion",
    "CreditsManagementFunctions", "CreditsManagementFunctions", "CreditsManagementFunctions",
    "CAM_GetVectorParameter", "CAM_GetVectorParameter", "CAM_GetRealParameter", "CAM_GetRealParameter",
    "CAM_GetRealParameter", "CAM_GetRealParameter", "CAM_GetRealParameter", "CAM_GetRealParameter",
    "CAM_GetRealParameter", "CAM_GetRealParameter", "CAM_GetRealParameter", "CAM_GetRealParameter",
    "CAM_GetRealParameter", "CAM_GetRealParameter", "CAM_GetRealParameter", "CAM_GetRealParameter",
    "CAM_GetRealParameter", "CAM_GetRealParameter", "CAM_GetRealParameter", "CAM_GetRealParameter",
    "CAM_GetRealParameter", "CAM_GetRealParameter", "CAM_GetTargetedSuperObject", "CAM_GetTypeOfViewport",
    "CAM_GetCameraOfViewport", "CAM_GetCameraOfViewport", "CAM_ComputeTargetWithTgtPerso",
    "CAM_ComputeCurrentTarget", "CAM_GetSectorCameraType", "CAM_GetBestPos", "CAM_GetSetTransparencyFlag",
    "CAM_GetSetMirrorFlag", "CAM_GetSetMirrorFlag", "CAM_GetViewportCoordinates", "BackMode", "MarioMode",
    "rafeLockMode", "GetViewFinderDistance", "LNK_Init", "LNK_DoBehaviour", "LNK_Kill", "LNK_GetNoCutPoints",
    "LNK_GetCutPoint", "LNK_FollowLinkDirection", "OptionGetHandle", "OptionGetHandleState",
    "OptionGetHandleState", "OptionGetHandleState", "OptionGetHandleState", "OptionGetHandleState",
    "OptionGetTypeVolume", "OptionGetTypeVolume", "OptionGetTypeVolume", "OptionGetTypeVolume",
    "OptionGetTypeVolume", "IsSoundPlaying", "MusicMarkerSlotControlFunction", "MonitoringSetVariable",
    "R3GetValue", "R3GetValue", "FunctPS2", "ScreenFunctions", "MCFunc", "SinusEffect_GetPosition",
    "SFX_GetLightReflectionStatus", "SFX_GetWaterCirclesStatus", "SFX_IsLightReflectionPossible",
    "SFX_IsWaterCirclesPossible", "AGO_GetHandle", "IsMcValid", "FormatMC"
};

static const char * const script_procedure_table[] = {
    "HitPointsProcedure", "HitPointsProcedure", "HitPointsProcedure", "HitPointsProcedure",
    "HitPointsProcedure", "HitPointsProcedure", "HitPointsMaxProcedure", "HitPointsMaxProcedure",
    "HitPointsMaxProcedure", "HitPointsMaxProcedure", "HitPointsMaxProcedure", "TransparenceProcedure",
    "TransparenceProcedure","ACT_SetDrawFlag", "SkinMaterialProcedure", "SkinMaterialProcedure",
    "TransparenceProcedure", "TransparenceProcedure", "TransparenceProcedure", "TransparenceProcedure",
    "ListZDDZDEProcedure", "ListZDDZDEProcedure", "ListZDDZDEProcedure", "ListZDDZDEProcedure",
    "ListZDDZDEProcedure", "ListZDDZDEProcedure", "ListZDDZDEProcedure", "ListProcedure", "ListProcedure",
    "ListProcedure", "ListProcedure", "ListProcedure", "ListProcedure", "ListProcedure", "ListProcedure",
    "ListProcedure", "ListProcedure", "ListProcedure", "ListProcedure", "ListProcedure",
    "ListEnsembleProcedure", "ListEnsembleProcedure", "ListEnsembleProcedure", "ListEnsembleProcedure",
    "FogProcedure", "FogProcedure", "FogProcedure", "FogProcedure", "FogProcedure", "FogProcedure",
    "Dummy", "Dummy", "Dummy", "Dummy", "Dummy", "Dummy", "Dummy", "Dummy", "SinusEffect_SetFreq",
    "SinusEffect_SetAmplitude", "SinusEffect_SetState", "SinusEffect_SetFreq", "SinusEffect_SetFreq",
    "SinusEffect_SetFreq", "SinusEffect_SetRLIParams", "SinusEffect_SetRLIParams", "SPO_SetDrawFlag",
    "SPO_SetEngineDisplayModeFlag", "KeyboardProcedure", "KeyboardProcedure", "NoneProcedure",
    "TraceProcedure", "ChangeComportIntelligenceProcedure", "ChangeComportReflexProcedure",
    "ChangeMyComportIntelligenceProcedure", "ChangeMyComportReflexProcedure",
    "ChangeMyComportIntelligenceAndReflexProcedure", "ChangeActionProcedure", "ChangeActionProcedure",
    "ChangeActionRandomProcedure", "ChangeActionProcedure", "ChangeActionProcedure", "ChangeActionProcedure",
    "ChangeActionRandomProcedure", "ChangeActionProcedure", "ChangeActionProcedure", "ChangeActionProcedure",
    "ChangeActionRandomProcedure", "ChangeActionProcedure", "Proc_Loop", "Proc_EndLoop", "Proc_Break",
    "MiscNoProcedure", "MiscNoProcedure", "MiscNoProcedure", "MiscNoProcedure",
    "MiscProcedure_SetNewMainCharacter", "MiscProcedure_ForceActiveAnObject", "MiscProcedure_DesactiveAnObject",
    "MiscProcedure_AskToChangeLevel_99", "MiscProcedure_AskToChangeLevelWithTransition",
    "MiscProcedure_AskToChangeLevel_101", "MiscProcedure_SetActionReturn", "MiscUltraProcedure",
    "MiscUltraProcedure", "MiscProcedure_InvalidateHierarchy", "MiscUltraProcedure", "MiscMoreProcedure",
    "LinkTableProcedure", "LinkTableProcedure", "LinkTableProcedure", "BuildObjectTableFromTableAndStringProcedure",
    "BuildObjectTableFromTableAndStringProcedure", "ModuleControlProcedure", "ModuleControlProcedure",
    "ModuleControlProcedure", "SaveGameProcedure", "ChangeActivationChannelProcedure",
    "ChangeActivationChannelProcedure", "PersoLightProcedure", "PersoLightProcedure", "PersoLightProcedure",
    "PersoLightProcedure", "PersoLightProcedure", "PersoLightProcedure", "PersoLightProcedure",
    "PersoLightProcedure", "PersoLightProcedure", "PersoLightProcedure", "PersoLightProcedure", "DisplayProcedure",
    "MenuAndVignetteProcedure", "MenuAndVignetteProcedure", "Dummy", "Dummy", "Dummy", "Dummy", "Dummy", "Dummy",
    "Dummy", "Dummy", "Dummy", "Dummy", "Dummy", "Dummy", "TurnProcedure", "TurnProcedure", "TurnPersoProcedure",
    "TurnPersoProcedure", "KillPersoAndClearVariableProcedure", "KillPersoAndClearVariableProcedure",
    "DeletePersoFromGameProcedure", "VisualMaterialProcedure", "VisualMaterialProcedure", "VisualMaterialProcedure",
    "VisualMaterialProcedure", "VisualMaterialProcedure", "VisualMaterialProcedure", "VisualMaterialProcedure",
    "VisualMaterialProcedure", "VisualMaterialProcedure", "VisualMaterialProcedure", "OptionChangerProc",
    "OptionChangerProc", "OptionChangerProc", "OptionChangerProc", "OptionChangerProc", "OptionChangerProc",
    "BatchSetPersoAtModulePosition", "BatchSetPersoOrientation", "BatchSynchroPersoWithPerso", "3DProc", "3DProc",
    "DynamProcedure", "3DProc", "3DProc", "3DProc", "3DProc", "3DProc", "3DProc", "3DProc", "3DProc", "3DProc",
    "3DProc", "3DProc", "3DProc_ForReorientation", "3DProc_ForReorientation", "3DProc", "CustomBitsProc",
    "CustomBitsProc", "CustomBitsProc", "CustomValuesProc", "CustomValuesProc", "CustomValuesProc",
    "SetMechanicSpeedVector", "SetMechanicSpeedReal", "SetMechanicSpeedReal", "SetMechanicSpeedReal",
    "SetMechanicSpeedReal", "SetMechanicSpeedReal", "SetMechanicSpeedVector", "SetMechanicSpeedReal",
    "SetMechanicSpeedReal", "SetMechanicSpeedReal", "SetMechanicSpeedReal", "SetMechanicSpeedReal",
    "SetMechanicSpeedVector", "SetMechanicSpeedReal", "SetMechanicSpeedReal", "SetMechanicSpeedReal",
    "SetMechanicSpeedReal", "SetMechanicSpeedReal", "SetMechanicSpeedVector", "SetMechanicSpeedReal",
    "SetMechanicSpeedReal", "SetMechanicSpeedReal", "SetMechanicSpeedReal", "SetMechanicSpeedReal",
    "FixePositionPerso", "FixePositionPerso", "SetMechanicSpeedVector", "SetMechanicSpeedReal",
    "SetMechanicSpeedReal", "SetMechanicSpeedReal", "SetMechanicSpeedReal", "SetMechanicSpeedReal",
    "SetMechanicSpeedVector", "SetMechanicSpeedReal", "SetMechanicSpeedReal", "SetMechanicSpeedReal",
    "SetMechanicSpeedReal", "SetMechanicSpeedReal", "PrepareMorph", "PrepareMorphMT", "StopMorph", "StopMorph",
    "Morphing", "MorphingMT", "ModuleControlProcedure", "3DProc", "3DProc", "3DProc", "3DProc", "3DProc",
    "3DProc", "3DProc", "3DProc", "3DProc", "3DProc", "3DProc", "3DProc", "3DProc_ForReorientation",
    "3DProc_ForReorientation", "3DProc", "CollideCommunicationProc", "CollideCommunicationProc",
    "CollideCommunicationProc", "CollideCommunicationProc", "CollideReinitCommunicationProc",
    "ResetLastCollisionActor", "ResetLastCollisionActor", "MiscUltraProcedure", "MiscUltraProcedure",
    "ChangeTextDefaultColor", "CreateTextWithFormat", "CreateTextWithFormat", "StringProc", "StringProc",
    "StringProc", "StringProc", "IntToText", "RealToText", "SetMechanicOption", "SetMechanicOption",
    "SetMechanicOption", "SetMechanicOption", "SetMechanicOption", "SetMechanicOption", "SetMechanicOption",
    "SetMechanicOption", "SetMechanicOption", "SetMechanicOption", "SetMechanicOption", "SetMechanicOption",
    "SetMechanicOption", "SetMechanicOption", "SetMechanicOption", "SetMechanicOption", "SetMechanicOption",
    "SetMechanicOption", "SetMechanicOption", "SetMechanicRealParameter", "SetMechanicRealParameter",
    "SetMechanicRealParameter", "SetMechanicRealParameter", "SetMechanicRealParameter", "SetMechanicRealParameter",
    "SetMechanicRealParameter", "SetMechanic3RealParameter", "SetMechanicRealParameter", "SetMechanicRealParameter",
    "SetMechanicRealParameter", "SetMechanicVectorParameter", "SetMechanicRealParameter", "SetMechanicVectorParameter",
    "SetMechanicRealParameter", "SetMechanicVectorParameter", "SetMechanicListParameter", "SetMechanicSpeedVector",
    "SetMechanicSpeedReal", "SetMechanicSpeedReal", "SetMechanicSpeedReal", "SetMechanicSpeedReal",
    "SetMechanicSpeedReal", "SetMechanicSpeedReal", "SetMechanicSpeedReal", "SetMechanicSpeedReal", "PlatformLink",
    "PlatformLink", "SetMechanic3Real", "SetMechanic3Real", "SetMechanicRealParameter", "SetMechanicRealParameter",
    "SetMechanicRealParameter", "SetMechanicClimbSpeedLimit", "SetMechanicRealParameter", "SetMechanicSpeedReal",
    "SetMechanicRealParameter", "SetMechanicRealParameter", "HierFatherLink", "HierFatherLink", "HierFatherLink",
    "MiscProcedure_FreePlatformLinks", "LinkControl", "MiscProcedure_LoadOrSaveEngineClock", "FixePositionZDx",
    "FixePositionZDx", "FixePositionZDx", "ChangeGameMaterialZDx", "ChangeLigthIntensity", "fn_SPO_p_stChangeFlag",
    "fn_SPO_p_stChangeFlag", "ChangeActorSpoFlag", "ChangeScreen", "ChangeScreen", "ChangeScreen", "ChangeScreen",
    "ChangeScreen", "ChangeScreen", "Shadow", "Shadow", "Shadow", "SetPrivilegeActivationZDx",
    "SetPrivilegeActivationZDx", "SetPrivilegeActivationZDx", "SetPrivilegeActivationZDx",
    "SetPrivilegeActivationZDx", "SetPrivilegeActivationZDx", "SetPrivilegeActivationZDx",
    "SetPrivilegeActivationZDx", "SetPrivilegeActivationZDx", "SetPrivilegeActivationZDx",
    "SetPrivilegeActivationZDx", "SetPrivilegeActivationZDx", "ActivationZDR", "SetCollSetCollisionFlag",
    "SetCollSetCollisionFlag", "SetCollSetCollisionFlag", "SetCollSetCollisionFlag", "SetCollSetCollisionFlag",
    "SetCollSetCollisionFlag", "SetComputationFrequency", "SetComputationFrequency", "SetComputationFrequency",
    "WorldMapProc", "WorldMapProc", "WorldMapProc", "ReadAnalogJoystickMario", "SetAnalogJoystickAxes",
    "SetAnalogJoystickAxes", "SetAnalogJoystickAxes", "SetAnalogJoystickAxes", "SetAnalogJoystickAxes2",
    "ReleaseAnalogJoystick", "SetBooleanInArray", "SetDefaultFormatCharacter", "SelectDriver", "Dummy", "Dummy",
    "SelectResolution", "ChangeBrightness", "TextIntegerParameter", "TextRealParameter", "SetVideoOptions",
    "SetVideoOptions", "Dummy", "MiscProtectProcedure", "ShellExecuteProcedure", "TemporalChangeSighting",
    "PAD2_Procedure", "PAD2_Procedure", "PAD2_Procedure", "PAD2_Procedure", "PAD2_Procedure", "PAD2_Procedure",
    "PAD2_Procedure", "InputProcedure", "SHWSetShadowMode", "SHWSetShadowMode", "SHWSetShadowMode", "ValidateMemoryCard",
    "AddMemoryCardString", "MiscProcedure_3dDataSetGlobalAccelerationFactor", "Code4MagicPointsProcedures",
    "Code4MagicPointsProcedures", "Code4MagicPointsProcedures", "Code4MagicPointsProcedures", "Code4MagicPointsProcedures",
    "Code4MagicPointsProcedures", "Code4MagicPointsProcedures", "Code4MagicPointsProcedures", "Code4AirPointsProcedures",
    "Code4AirPointsProcedures", "Code4AirPointsProcedures", "Code4AirPointsProcedures", "Code4AirPointsProcedures",
    "Code4AirPointsProcedures", "Code4AirPointsProcedures", "Code4AirPointsProcedures", "FixePositionFade",
    "SetLocalLight", "SetLocalLightOnOff", "SetLight", "SetLight", "SetLight", "SetLight", "SetLight", "RLIProcedure",
    "RLIProcedure", "RLIProcedure", "SetRadiusWP", "SetCoordWP", "SetCoordWP", "MiscProcOnTypeOfWP", "CapabilityProcedure",
    "CapabilityProcedure", "CapabilityProcedure", "CapabilityProcedure", "WPSetRangeValue", "ReinitGraphProcedure",
    "SetScrollSpeed", "SetScrollSpeed", "SetScrollSpeed", "SetScrollSpeed", "ManageFrame", "ManageFrame", "FreezeAnim",
    "FreezeAnim", "SetDiscreetSpeed", "SPOSuperimpoed", "SPOSuperimpoed", "SPOSuperimpoed", "SPOSuperimpoed",
    "SPOSuperimpoed", "SPOSuperimpoed", "SetOutlineMode", "SetOutlineMode", "SetSPOCoordinates", "SetSPOCoordinates",
    "SetSPOCoordinates", "SetSPOCoordinates", "SetSPOCoordinates", "SetSPOCoordinates", "SetSPOCoordinates",
    "SetSPOCoordinates", "SetSPOCoordinates", "SetSPOCoordinates", "SetSOLCoordinates_SPOParam",
    "SetSOLCoordinates_SPOParam", "SetSOLCoordinates_SPOParam", "SetSOLCoordinates_SPOParam",
    "SetSOLCoordinates_SPOParam", "SetSOLCoordinates_SOLParam", "SetSOLCoordinates_SOLParam",
    "SetSOLCoordinates_SOLParam", "SetSOLCoordinates_SOLParam", "SetSOLCoordinates_SOLParam", "SOLEdit", "SOLEdit",
    "SOLEdit", "SOLEdit", "SOLEdit", "SOLEdit", "SPOSuperimpoed", "SPORotate", "SPORotate", "SPORotate", "SPORotate",
    "SPORotate", "SPORotate", "SPORotate", "SPORotate", "SPORotate", "SPORotate", "SPORotate", "SPORotate", "SPORotate",
    "SPORotate", "SPORotate", "SPORotate", "SPORotate", "SPORotate", "SPORotate", "SPORotate", "SPORotateToIdentity",
    "SPOChangeSighting", "SPOChangeSighting", "SPOChangeSighting", "SPOChangeSighting", "SPOChangeSighting",
    "SPOChangeSighting", "SPOChangeSighting", "SPOChangeSighting", "SPOChangeSighting", "SPOChangeSighting",
    "TractionFactorProcedure", "TurnToPosition", "TurnToPosition", "SortArray", "ActiveJoystickAnal",
    "ActiveJoystickAnal", "SetZDxSize", "SetZDxSize", "Code4ChangeCaractere", "Code4FormateTexte", "Code4FormateTexte",
    "ChangePersoHorizSighting", "PlayDemo", "PlayAutoDemo", "SetCharacterMaskedForResearch", "InitKeyBoardDirections",
    "InitKeyBoardPadButton", "SetCenterPosition", "SetMaximalValues", "SetTooFarLimit", "SetTransparencyZone",
    "BaseFrequenceForMenu", "SaveSinusEffectContext", "SaveSinusEffectContext", "TextSuperImposed", "SubAnimProc",
    "SubAnimProc", "SubAnimProc", "SubAnimProc", "SubAnimProc", "SubAnimProc", "SubAnimProc", "SHWSetDynShadow",
    "SHWSetDynShadow", "SHWSetDynShadow", "PS2SetLanguageProc", "CallSubRoutine", "ACTSetRangeValue", "Code4InternalLights",
    "Code4InternalLights", "ToggleCinemascope", "ToggleCinemascope", "SetCinemascopeRatio", "CAM_stUpdatePosition",
    "CAM_stUpdatePosition2", "CAM_stChangeVectorParameter", "CAM_stChangeVectorParameter", "CAM_stChangeRealParameter",
    "CAM_stChangeRealParameter", "CAM_stChangeRealParameter", "CAM_stChangeRealParameter", "CAM_stChangeRealParameter",
    "CAM_stChangeRealParameter", "CAM_stChangeRealParameter", "CAM_stChangeRealParameter", "CAM_stChangeRealParameter",
    "CAM_stChangeRealParameter", "CAM_stChangeRealParameter", "CAM_stChangeRealParameter", "CAM_stChangeRealParameter",
    "CAM_stChangeRealParameter", "CAM_stChangeRealParameter", "CAM_stChangeRealParameter", "CAM_stChangeRealParameter",
    "CAM_stChangeRealParameter", "CAM_stChangeRealParameter", "CAM_stChangeRealParameter", "CAM_stChangeTgtPerso",
    "CAM_stChangeSecondTgtPerso", "CAM_stChangeChannel", "CAM_stActivate", "CAM_stAssociateViewport", "CAM_stSetIAFlags",
    "CAM_stSetIAFlags", "CAM_stSetIAFlags", "CAM_stSetIAFlags", "CAM_stSetIAFlags", "CAM_stSetIAFlags", "CAM_stSetIAFlags",
    "CAM_stSetIAFlags", "CAM_stSetIAFlags", "CAM_stSetDNMFlags", "CAM_stSetDNMFlags", "CAM_stSetDNMFlags",
    "CAM_stSetDNMFlags", "CAM_stSetDNMFlags", "CAM_stSetDNMFlags", "CAM_stSetDNMFlags", "CAM_stSetDNMFlags",
    "CAM_stSetDNMFlags", "CAM_stSetDNMFlags", "CAM_stChangeConstants", "CAM_stConstants", "CAM_stConstants",
    "CAM_stShowInfo", "CAM_stSetForce", "CAM_stSetForce", "CAM_stSetForce", "CAM_stReset", "CAM_stForceBestPos",
    "CAM_stForceNormalState", "CAM_stForceMovingOnRail", "CAM_stCameraMode", "CAM_stCameraSetTransparencyFlag",
    "InitCamShooter", "RegisterDesignVar", "RegisterDesignVar", "RegisterDesignVar", "RegisterDesignVar",
    "RegisterDesignVar", "GetViewFinderVMT", "GetMainActorComports", "InitBackMode", "InitMarioMode",
    "InitStrafeLockMode", "AddMainActor", "RemoveMainActor", "SetSkin", "SetGCubeVersion", "LNK_InitAll",
    "LNK_SetAgo", "LNK_SetLag", "LNK_SetGaugeSpeed", "LNK_fn_SetSendDataRate", "LNK_fn_SetSendStatusPeriod",
    "LNK_fn_SetLaunchSpeed", "LNK_fn_SetFlagStraight", "LNK_fn_SetUnlaunch", "AGOProcedure", "AGOProcedure",
    "InGameMenuProcedure", "JFFTXTProcedure", "JFFTXTProcedure", "JFFTXTProcedure", "JFFTXTProcedure", "JFFTXTProcedure",
    "JFFTXTProcedure", "JFFTXTProcedure", "JFFTXTProcedure", "JFFTXTProcedure", "JFFTXTProcedure", "JFFTXTProcedure",
    "JFFTXTProcedure", "JFFTXTSettings", "JFFTXTSettings", "JFFTXTSettings", "JFFTXTSettings", "JFFTXTSettings",
    "JFFTXTGetBdBox", "Dummy", "Dummy", "CheatCodeProcedure", "CheatCodeProcedure", "CheatCodeProcedure",
    "CheatCodeProcedure", "MultiPlayerProcedure", "MultiPlayerProcedure", "MultiPlayerProcedure", "MultiPlayerProcedure",
    "AGOProcedure", "AGOProcedure", "AGOProcedure", "AGOProcedure", "JFFTXTProcedure", "JFFTXTProcedure", "JFFTXTProcedure",
    "MultiPlayerProcedure", "MultiPlayerProcedure", "_SinusEffect_SetPreMultiplier", "AGOProcedure", "AGOProcedure",
    "AGOProcedure", "LogWindowProcedure", "RLIProcedure2", "RLIProcedure2", "RLIProcedure2", "VideoProcedure",
    "VideoProcedure", "VideoProcedure", "VideoProcedure", "SetVSyncWait", "BrainActivationProcedure",
    "BrainActivationProcedure", "DisplaySegment", "EndTransitionProcedure", "StartCine", "StartCine", "LoadingLevel",
    "DataPath", "SAV2SaveProcedure", "SAV2SaveProcedure", "SAV2SaveProcedure", "SAV2SaveProcedure", "SAV2SaveProcedure",
    "SAV2SaveProcedure", "SAV2SaveProcedure", "SAV2SaveProcedure", "SAV2SaveProcedure", "SAV2SaveProcedure",
    "SAV2SaveProcedure", "SAV2SaveProcedure", "SAV2SaveProcedure", "SAV2SaveProcedure", "SAV2SaveProcedure",
    "SAV2SaveProcedure", "SAV2SaveProcedure", "SAV2SaveProcedure", "SAV2SaveProcedure", "SCT_UltraProcedure",
    "SCT_UltraProcedure", "PortalActivationProcedure", "PortalActivationProcedure", "LogFileProcedure",
    "LogFileProcedure", "LogFileProcedure", "FFBProcedures", "FFBProcedures", "FFBProcedures", "FFBProcedures",
    "FFBProcedures", "FFBProcedures", "FFBProcedures", "FFBProcedures", "FFBProcedures", "FFBProcedures",
    "SoundProcedure", "SoundProcedure", "SoundProcedure", "SoundProcedure", "SoundProcedureSendRequest",
    "SoundProcedureSendRequest", "SoundProcedureSendRequest", "SoundProcedureSendRequest", "SoundProcedureSendRequest",
    "SoundProcedureSendRequest", "SoundProcedureSendRequest", "SoundProcedureSendRequest", "SoundProcedureSendRequest",
    "SoundProcedureSendRequest", "SoundProcedureSendRequest", "SoundProcedureSendRequest", "SoundProcedureSendRequest",
    "SoundProcedureSendRequest", "SoundProcedureChangeVolumeId", "SoundProcedureChangeVolumeId",
    "SoundProcedureChangeVolumeId", "SoundProcedureStopRequest", "SoundProcedureStopRequest", "SoundProcedureStopRequest",
    "SoundProcedureStopRequest", "SoundProcedureStopRequest", "SoundProcedureStopRequest", "LipsChannelControl",
    "LipsChannelControl", "SoundProcedure", "SoundProcedure", "SoundProcedure", "SoundProcedure",
    "SoundProcedureSetVolume_Anim", "SoundProcedureSetVolume_Voice", "SoundProcedureSetVolume_Music",
    "SoundProcedureSetVolume_Ambiance", "SoundProcedureSetVolume_Menu", "SoundProcedureChangeVolume_Anim",
    "SoundProcedureChangeVolume_Voice", "SoundProcedureChangeVolume_Music", "SoundProcedureChangeVolume_Ambiance",
    "SoundProcedureChangeVolume_Menu", "SoundProcedurePause_Anim", "SoundProcedurePause_Voice",
    "SoundProcedurePause_Music", "SoundProcedurePause_Ambiance", "SoundProcedurePause_Menu", "SoundProcedurePause",
    "SoundProcedureResume_Anim", "SoundProcedureResume_Voice", "SoundProcedureResume_Music",
    "SoundProcedureResume_Ambiance", "SoundProcedureResume_Menu", "SoundProcedureResume", "SetDopplerEffect",
    "SetInStereoMode", "SetPrevMusicFadeOut", "SetRollOffDistances", "SetRollOffDistances",
    "SaveCurrentSoundRequest", "SaveCurrentSoundRequest", "MusicMarkerSlotControlProcedure",
    "SoundProcedurePS2ChangeReverb", "SoundProcedurePS2SetReverbOn", "SoundProcedureSendRequest",
    "SetGameMode", "SetGameMode", "DoNotLoadTextures", "VisualMaterialProcedure", "HandleFamilyDynamicLoad",
    "HandleActorDynamicLoad", "MonitoringStart", "MonitoringStop", "CreateToby", "AddSegmentToToby",
    "TobySetTransparency", "TobyDestroy", "TobySetMaterial", "ScreenProc", "ScreenProc", "ScreenProc", "MCProc",
    "BulletTimeProc", "DemoProc", "SetCurrentValueAsInit", "SetCheapCharacterDistance", "SetCheapCharacterDistance",
    "SetCheapCharacterTimer", "SetCheapCharacterRefObj", "SetCheapCharacterMode", "SetCheapCharacterListAura",
    "SetCheapCharacterPos", "SetCheapCharacterPos", "AddCheapCharacter", "ResetCheapCharacterList", "ActivateCheapCharacter",
    "ActivateCheapCharacter", "SetCheapCharacterOptions", "R3SetValue_R3SetEnvScoreWithRevisit", "R3SetValue_R3SetEnvNbCage",
    "R3SetValue_R3SetCurrentEnvNumber", "R3SetValue_R3SetNbCineOpen", "R3SetValue_R3SetNbBonusOpen",
    "R3SetValue_R3SetDoAutoSaveWhenChangeMap", "R3SetValue_R3SetEndOfGame", "WaterEffect_SetRandomPerturbationGeneration",
    "WaterEffect_GenerateImpulse", "WaterEffect_GenerateSplash", "AGOProcedure", "AGOProcedure", "TakePictureProcedure",
    "TakePictureProcedure", "SFX_ActivateLightReflection", "SFX_ActivateWaterCircles", "GcSetActiveMcSlot", "GcResetToIpl",
    "ChangeActionProcedure", "ChangeActionProcedure", "ChangeActionProcedure", "2DMenuProcedure_ActivateInGameMenu2D",
    "2DMenuProcedure_ActivateInGameMenu2DStart", "2DMenuProcedure_ActivateInGameMenu2DPhoto",
    "2DMenuProcedure_ActivateInGameMenu2DBonus0", "2DMenuProcedure_ActivateInGameMenu2DBonus1", "AllowNormalsRecomputing"
};

static const char * const script_condition_table[] = {
    "Cond_And",
                    "Cond_Or",
                    "Cond_Not",
                    "Cond_XOR",
                    "Cond_Equal",
                    "Cond_Different",
                    "Cond_Lesser",
                    "Cond_Greater",
                    "Cond_LesserOrEqual",
                    "Cond_GreaterOrEqual",
                    "CollidePersoZDDNoWithPerso",
                    "CollideModuleZDDNoWithPerso",
                    "CollidePersoAllZDDWithPerso",
                    "CollidePersoZDDWithAnyPerso",
                    "CollideModuleZDDWithAnyPerso",
                    "CollidePersoZDENoWithPersoZDENo",
                    "CollideModuleZDENoWithPersoZDENo",
                    "CollidePersoZDENoWithModuleZDENo",
                    "CollideModuleZDENoWithModuleZDENo",
                    "CollidePersoZDENoWithPersoTypeZDE",
                    "CollideModuleZDENoWithPersoTypeZDE",
                    "CollidePersoTypeZDEWithPersoTypeZDE",
                    "CollidePersoAllZDEWithPersoAllZDE",
                    "CollidePersoTypeZDEWithPersoAllZDE",
                    "CollidePersoAllZDEWithPersoTypeZDE",
                    "CollidePersoZDENoWithTypeZDE",
                    "CollideModuleZDENoWithTypeZDE",
                    "CollideWithGround",
                    "CollideWithWall",
                    "CollideWithNothing",
                    "CollideWithCeiling",
                    "CollideWithPerso",
                    "CollideWithWater",
                    "CollideWithThisPerso",
                    "ZDMCollideWithGround",
                    "ZDMCollideWithWall",
                    "ZDMCollideWithNothing",
                    "ZDMCollideWithCeiling",
                    "IsPersoInList",
                    "IsModelInList",
                    "IsFamilyInList",
                    "ListEmptyTest",
                    "UserEvent_IsSet",
                    "UserEvent_IsSet2",
                    "UserEvent_IsSet3",
                    "PressedBut",
                    "JustPressedBut",
                    "ReleasedBut",
                    "JustReleasedBut",
                    "IsTimeElapsed",
                    "IsValidObject",
                    "IsValidWayPoint",
                    "IsValidGMT",
                    "IsValidVMT",
                    "IsValidAction",
                    "IsValidText",
                    "IsValidSPO",
                    "IsValidGraph",
                    "SeePerso",
                    "IsActivable",
                    "IsAlreadyHandled",
                    "Alw_IsMine",
                    "IsPersoLightOn",
                    "IsPersoLightPulseOn",
                    "IsPersoLightGyroPhareOn",
                    "IsRLITransitionInProgress",
                    "IsInAlwaysActiveList",
                    "IsAlwaysActive",
                    "IsAnActivePad",
                    "IsMultitap",
                    "SAV2_IsValid",
                    "IsWidescreen",
                    "EngineIsInPAL",
                    "IsZDMCollideWithObstacle",
                    "IsZDMCollideWithWall",
                    "IsZDMCollideWithGround",
                    "IsZDMCollideWithCeiling",
                    "CmtIdentifierContainsMask",
                    "HitByCollider",
                    "IsTypeOfGMTCollide",
                    "IsInComport",
                    "IsInReflexComport",
                    "IsInAction",
                    "ChangeActionEnable",
                    "EngineReinitRequested",
                    "IsThereMechEvent",
                    "CollisionWP",
                    "IsCustomBitSet",
                    "IsPersoActive",
                    "CheckActionEnd",
                    "IsCurrentStateCustomBitSet",
                    "IsGiBlock",
                    "IsMechanicBlock",
                    "IsMechanicAnimation",
                    "IsMechanicCollide",
                    "IsMechanicGravity",
                    "IsMechanicTilt",
                    "IsMechanicGi",
                    "IsMechanicClimb",
                    "IsMechanicOnGround",
                    "IsMechanicSpider",
                    "IsMechanicShoot",
                    "IsMechanicSwim",
                    "IsMechanicNeverFall",
                    "IsMechanicCollisionControl",
                    "IsMechanicKeepSpeedZ",
                    "IsMechanicSpeedLimit",
                    "IsMechanicInertia",
                    "IsMechanicStream",
                    "IsMechanicStickOnPlatform",
                    "IsMechanicPatformCrash",
                    "IsMechanicScale",
                    "IsMechanicExec",
                    "CanFall",
                    "IsMechanicCrash",
                    "IsNullVector",
                    "HierIsSonOfActor",
                    "IsMorphing",
                    "CheckAnimEnd",
                    "CheckAnimSmooth",
                    "HasTheCapability",
                    "HasOneOfTheCapabilities",
                    "HasTheCapabilityNumber",
                    "PersoHasTheCapability",
                    "PersoHasOneOfTheCapabilities",
                    "PersoHasTheCapabilityNumber",
                    "MagnetIsActivated",
                    "NEstPasEnCollisionAvecMap",
                    "NEstPasEnCollisionAvecProjectile",
                    "NEstPasEnCollisionAvecSecondCharact",
                    "NEstPasEnCollisionAvecMainCharact",
                    "NEstPasEnCollisionAvecAutresSecteurs",
                    "IsInFamily",
                    "IsInModel",
                    "AJoypadIsConnected",
                    "AKeyJustPressed",
                    "AButtonPadJustPressed",
                    "IsInDemoMode",
                    "IsShapnessMax",
                    "IsTooFar",
                    "IsSubAnimPlaying",
                    "TestCBSubAnim",
                    "IsInSubAnim",
                    "IsSubAnimNearEnd",
                    "IsSameSPO",
                    "PressedPadBut",
                    "JustPressedPadBut",
                    "ReleasedPadBut",
                    "JustReleasedPadBut",
                    "IsCinePlaying",
                    "LoadInProgress",
                    "SAV2LastError",
                    "CheckMCStatus",
                    "Cam_IsActive",
                    "Cam_IsViewportOwner",
                    "Cam_IsFlagNoDynamicTarget",
                    "Cam_IsFlagNoAverageMoveTgtPerso",
                    "Cam_IsFlagNoParseCutAngle",
                    "Cam_IsFlagNoVisibility",
                    "Cam_IsFlagNoVisibilityWithDynHie",
                    "Cam_IsFlagNoDynChangeTheta",
                    "Cam_IsFlagNoShiftUntilPosReached",
                    "Cam_IsFlagNoDynSpeed",
                    "Cam_IsFlagNoLinearParsing",
                    "Cam_IsFlagNoLinearInertia",
                    "Cam_IsFlagNoAngularParsing",
                    "Cam_IsFlagNoAngularInertia",
                    "Cam_IsFlagNoTargetParsing",
                    "Cam_IsFlagNoTargetInertia",
                    "Cam_IsFlagNoObstacle",
                    "Cam_IsFlagFixedOrientation",
                    "Cam_IsFlagForcedPosition",
                    "Cam_IsFlagForcedTarget",
                    "Cam_IsFlagForcedAxis",
                    "SND_IsInStereoMode",
                    "SND_IsMusicPlaying",
                    "SND_IsVoicePlaying",
                    "SND_IsEventValid"
};

static const char * const script_field_table[] = {
    "Position", "Orientation", "Speed", "NormSpeed",
    "AbsoluteAxisX", "AbsoluteAxisY", "AbsoluteAxisZ",
    "PrevComportIntell", "PrevComportReflex", "ShadowScaleX",
    "ShadowScaleY", "PadGlobalVector", "PadHorizontalAxis",
    "PadVerticalAxis", "PadAnalogForce", "PadTrueAnalogForce",
    "PadRotationAngle", "PadSector", "SystemDate", "SystemTime",
};

static const char * const script_meta_action_table[] = {
    "TIME_FrozenWait", "ACTION_ExecuteAction",
    "ACTION_WaitEndOfAction", "ACTION_WaitEndOfAnim",
    "CAM_CineMoveAToBTgtC", "CAM_CineMoveAToBTgtAC",
    "CAM_CinePosATgtB", "CAM_CinePosAMoveTgtBToC",
    "CAM_CinePosATgtBTurnPosH", "CAM_CinePosATgtBTurnTgtH",
    "CAM_CinePosATgtBTurnPosV", "CAM_CinePosATgtBTurnTgtV",
};

#define L(T) const uint64_t T##_length = sizeof T / sizeof *T;
L(script_nodetype_table)
L(script_keyword_table)
L(script_operator_table)
L(script_function_table)
L(script_procedure_table)
L(script_condition_table)
L(script_field_table)
L(script_meta_action_table)
#undef L


#pragma mark - Translation

#define BUFFER_SIZE 8192
#define SCRIPT_MAX_DEPTH 256

struct Translation {
    char* buf;
    char* ptr;
    struct ScriptNode* current;
    int indent;
    int line;
    int col;
};

#define append(...) t->ptr += sprintf(t->ptr, __VA_ARGS__)
#define cmp(a, b, ...) if (!strcmp(a,b)) { t->ptr += sprintf(t->ptr, __VA_ARGS__); return; }
#define first_child ((struct ScriptNode*)array_get(t->current->children, 0))
#define second_child ((struct ScriptNode*)array_get(t->current->children, 1))
#define third_child ((struct ScriptNode*)array_get(t->current->children, 2))

static const char* tostring(struct ScriptNode* node);
static const char* tostring_all(struct ScriptNode* node, bool separate_arguments);

static int indent = 0;

static void keyword(struct Translation* t)
{
    if (t->current->param >= script_keyword_table_length)
    {
        error("keyword index out of table range\n");
        return;
    }
    
    const char* keyword = script_keyword_table[t->current->param];
    
    int imod = -1, nmod = -1;
    
    cmp(keyword, "If",          "if %s",              tostring(first_child))
    cmp(keyword, "IfNot",       "if !(%s)",           tostring(first_child))
    cmp(keyword, "Then",        "{\n%s%*s}",            tostring_all(t->current, false), indent * 4, "")
    cmp(keyword, "Else",        "else\n%*s{\n%s%*s}\n", indent * 4, "", tostring_all(t->current, false), indent * 4, "")
    cmp(keyword, "IfDebug",     "if (debug && %s)",     tostring(first_child))
    cmp(keyword, "IfNotU64",    "if (!u64) {\n%s\n}\n", tostring_all(t->current, false))
    if (sscanf(keyword, "If%d", &imod)    > 0) append("if (randomizer %% %d == 0 && (%s))", imod, tostring(first_child));
    if (sscanf(keyword, "IfNot%d", &nmod) > 0) append("if (randomizer %% %d != 0 && (%s))", nmod, tostring(first_child));
        t->ptr += sprintf(t->ptr, "%s", keyword);
}

static void condition(struct Translation* t)
{
    const char* condition = script_condition_table[t->current->param];
    
    cmp(condition, "Cond_And",              "%s && %s", tostring(first_child), tostring(second_child))
    cmp(condition, "Cond_Or",               "%s || %s", tostring(first_child), tostring(second_child))
    cmp(condition, "Cond_Not",              "!(%s)",    tostring(first_child)                        )
    cmp(condition, "Cond_XOR",              "%s ^ %s",  tostring(first_child), tostring(second_child))
    cmp(condition, "Cond_Equal",            "%s == %s", tostring(first_child), tostring(second_child))
    cmp(condition, "Cond_Different",        "%s != %s", tostring(first_child), tostring(second_child))
    cmp(condition, "Cond_Lesser",           "%s < %s",  tostring(first_child), tostring(second_child))
    cmp(condition, "Cond_Greater",          "%s > %s",  tostring(first_child), tostring(second_child))
    cmp(condition, "Cond_LesserOrEqual",    "%s <= %s", tostring(first_child), tostring(second_child))
    cmp(condition, "Cond_GreaterOrEqual",   "%s >= %s", tostring(first_child), tostring(second_child))
    
    t->ptr += sprintf(t->ptr, "%s(%s)", condition, tostring(first_child));
    
//    cmp(condition, "Cond_PressedBut",       "button_pressed(%s)",       tostring(first_child))
//    cmp(condition, "Cond_JustPressedBut",   "button_just_pressed(%s)",  tostring(first_child))
//    cmp(condition, "Cond_ReleasedBut",      "button_released(%s)",      tostring(first_child))
//    cmp(condition, "Cond_JustReleasedBut",  "button_just_released(%s)", tostring(first_child))
//
//    cmp(condition, "_fn_p_stValidityCondition", "valid_object(%s)", tostring(first_child))
}

static void constant(struct Translation* t)
{
    t->ptr += sprintf(t->ptr, "%i", t->current->param);
}

static void real(struct Translation* t)
{
    t->ptr += sprintf(t->ptr, "%f", (float)t->current->param);
}

static void vector(struct Translation* t)
{
    /* do all script vectors have 3 components? */
    t->ptr += sprintf(t->ptr, "(%s, %s, %s)", tostring(first_child), tostring(second_child), tostring(third_child));
}

static void operator(struct Translation* t)
{
    const char* operator = script_operator_table[t->current->param];
    
    cmp(operator, "Operator_Plus",              "(%s %c %s)",   tostring(first_child), first_child->param >= 0 ? '+' : ' ', tostring(second_child)) /* add */
    cmp(operator, "Operator_Minus",             "(%s - %s)",    tostring(first_child), tostring(second_child)) /* subtract */
    cmp(operator, "Operator_UnaryMinus",        "-%s",          tostring(first_child)                        ) /* negate */
    cmp(operator, "Operator_Mul",               "(%s * %s)",    tostring(first_child), tostring(second_child)) /* multiply */
    cmp(operator, "Operator_Div",               "(%s / %s)",    tostring(first_child), tostring(second_child)) /* divide */
    cmp(operator, "Operator_Mod",               "(%s %% %s)",   tostring(first_child), tostring(second_child)) /* modulo */
    cmp(operator, "Operator_Affect",            "%s = %s;",     tostring(first_child), tostring(second_child)) /* assign */
    cmp(operator, "Operator_PlusAffect",        "%s += %s;",    tostring(first_child), tostring(second_child)) /* add and assign */
    cmp(operator, "Operator_PlusPlusAffect",    "%s++;",        tostring(first_child)                        ) /* increment */
    cmp(operator, "Operator_MinusAffect",       "%s -= %s",     tostring(first_child), tostring(second_child)) /* subtract and assign */
    cmp(operator, "Operator_MinusMinusAffect",  "%s--;",        tostring(first_child)                        ) /* decrement */
    cmp(operator, "Operator_Dot",               "%s.%s",        tostring(first_child), tostring(second_child)) /* field access */
    cmp(operator, ".X",                         "%s.x",         tostring(first_child)                        ) /* vector component X */
    cmp(operator, ".Y",                         "%s.y",         tostring(first_child)                        ) /* vector component Y */
    cmp(operator, ".Z",                         "%s.z",         tostring(first_child)                        ) /* vector component Z */
    cmp(operator, ".X:=",                       "%s.x = %s;",   tostring(first_child), tostring(second_child)) /* assign to vector component X */
    cmp(operator, ".Y:=",                       "%s.y = %s;",   tostring(first_child), tostring(second_child)) /* assign to vector component Y */
    cmp(operator, ".Z:=",                       "%s.z = %s;",   tostring(first_child), tostring(second_child)) /* assign to vector component Z */
    cmp(operator, "Operator_VectorPlusVector",  "%s + %s",      tostring(first_child), tostring(second_child))
    cmp(operator, "Operator_VectorMinusVector", "%s - %s",      tostring(first_child), tostring(second_child))
    cmp(operator, "Operator_VectorMulScalar",   "%s * %s",      tostring(first_child), tostring(second_child))
    cmp(operator, "Operator_VectorDivScalar",   "%s / %s",      tostring(first_child), tostring(second_child))
    cmp(operator, "Operator_VectorUnaryMinus",  "- %s",         tostring(first_child)                        )
    cmp(operator, "Operator_Ultra",             "%s.%s",        tostring(first_child), tostring(second_child))
    cmp(operator, "Operator_Array",             "%s[%s]",       tostring(first_child), tostring(second_child))
    /* type cast - maybe emit this as two consecutive tokens, so that the original type can also be viewed */
    cmp(operator, "Operator_ModelCast",         "((%s)(%s))",   tostring(first_child), tostring(second_child))
    
    append("(%X)(", t->current->param);
    unsigned N = array_element_count(t->current->children);
    for (unsigned i = 0; i < N; i++)
    {
        struct ScriptNode* childnode = array_get(t->current->children, i);
        append("%s", tostring(childnode));
        if (i != N - 1) append("%s", ", ");
    }
    append(");");
}

static void field(struct Translation* t)
{
    const char* field = script_field_table[t->current->param];
    
    if (first_child == NULL)
    {
        append("%s", field);
        return;
    }
    
    append("%s(", field); /* open parameter list */
    
    unsigned N = array_element_count(t->current->children);
    for (unsigned i = 0; i < N; i++)
    {
        struct ScriptNode* childnode = array_get(t->current->children, i);
        append("%s", tostring(childnode));
        if (i != N - 1) append("%s", ", ");
    }
    
    append(")"); /* close parameter list */
}

static void actor_reference(struct Translation* t)
{
    t->ptr += sprintf(t->ptr, "actor(@%X)", t->current->param);
}

static void dsgvar_reference(struct Translation* t)
{
    t->ptr += sprintf(t->ptr, "DsgVar_%d", t->current->param);
}

static void superobject_reference(struct Translation* t)
{
    t->ptr += sprintf(t->ptr, "SuperObject(@%X)", t->current->param);
}

static void function(struct Translation* t)
{
    const char* function = script_function_table[t->current->param];
    
    const unsigned N = array_element_count(t->current->children);
    t->ptr += sprintf(t->ptr, "%s(%s)", function, N > 0 ? tostring_all(t->current, true) : "");
}

static void procedure(struct Translation* t)
{
    const char* proc = script_procedure_table[t->current->param];
    cmp(proc, "Proc_Loop", "for (int i = 0; i < %s; i++) {\n", tostring_all(first_child, false))
    cmp(proc, "Proc_EndLoop", "}\n")
    cmp(proc, "Proc_Break", "break;\n")
    
    if (t->current->children)
    {
        append("%s(", proc); /* open parameter list */
        
        unsigned N = array_element_count(t->current->children);
        for (unsigned i = 0; i < N; i++)
        {
            struct ScriptNode* childnode = array_get(t->current->children, i);
            append("%s", tostring(childnode));
            if (i != N - 1) append("%s", ", ");
        }
        
        append(")"); /* close parameter list */
    }
}

static void macro(struct Translation* t)
{
    t->ptr += sprintf(t->ptr, "macro(@%X)", t->current->param);
}

static const char* tostring(struct ScriptNode* node)
{
    if (!node) return "NULL";
    
    struct Translation t;
    t.current = node;
    t.buf = malloc(BUFFER_SIZE * 10);
    t.ptr = t.buf;
    
    //info("Node: %s, %d children\n", script_nodetype_table[node->type], array_element_count(node->children));
    
    memset(t.buf, 0, BUFFER_SIZE * 10);
    
    switch (node->type)
    {
        case NODE_TYPE_KEYWORD: keyword(&t); break;
        case NODE_TYPE_CONDITION: condition(&t); break;
        case NODE_TYPE_OPERATOR: operator(&t); break;
        case NODE_TYPE_FUNCTION: function(&t); break;
        case NODE_TYPE_PROCEDURE: procedure(&t); break;
        case NODE_TYPE_META_ACTION: return "MetaAction";
        case NODE_TYPE_BEGIN_MACRO: return "Macro";
        case NODE_TYPE_BEGIN_MACRO2: return "Macro2";
        case NODE_TYPE_END_MACRO: return "EndMacro";
        case NODE_TYPE_FIELD: field(&t); break;
        case NODE_TYPE_DSGVARREF: dsgvar_reference(&t); break;
        case NODE_TYPE_DSGVARREF2: dsgvar_reference(&t); break;
        case NODE_TYPE_CONSTANT: constant(&t); break;
        case NODE_TYPE_REAL: real(&t); break;
        case NODE_TYPE_BUTTON: return "Button";
        case NODE_TYPE_CONSTANT_VECTOR:
        case NODE_TYPE_VECTOR: vector(&t); break;
        case NODE_TYPE_MASK: return "Mask";
        case NODE_TYPE_MODULEREF: return "ModuleRef";
        case NODE_TYPE_DSGVARID: return "DsgVarID";
        case NODE_TYPE_STRING: return "String";
        case NODE_TYPE_LIPSSYNCHROREF: return "LipsSynchroRef";
        case NODE_TYPE_FAMILYREF: return "FamilyRef";
        case NODE_TYPE_ACTORREF: actor_reference(&t); break;
        case NODE_TYPE_ACTIONREF: return "ActionRef";
        case NODE_TYPE_SUPEROBJECTREF: superobject_reference(&t); break;
        case NODE_TYPE_UNKNOWN:return "Unk";
        case NODE_TYPE_WAYPOINTREF: return "WaypointRef";
        case NODE_TYPE_TEXTREF: return "TextRef";
        case NODE_TYPE_COMPORTREF: return "ComportRef";
        case NODE_TYPE_MODULEREF2:return "ModuleRef2";
        case NODE_TYPE_SOUNDEVENTREF:return "SoundEventRef";
        case NODE_TYPE_OBJECTTABLEREF:return "ObjectTableRef";
        case NODE_TYPE_GAMEMATERIALREF: return "GameMaterialRef";
        case NODE_TYPE_VISUALMATERIAL: return "VisualMaterial";
        case NODE_TYPE_PARTICLEGENERATOR:return "ParticleGenerator";
        case NODE_TYPE_MODELREF:return "ModelRef";
        case NODE_TYPE_MODELREF2: return "ModelRef2";
        case NODE_TYPE_CUSTOMBITS: return "CustomBits";
        case NODE_TYPE_CAPS: return "Caps";
        case NODE_TYPE_UNKNOWN2: return "Unk2";
        case NODE_TYPE_SUBROUTINE: macro(&t); break;
        case NODE_TYPE_NULL: return "NULL";
        case NODE_TYPE_NULL2: return "NULL.2";
        case NODE_TYPE_GRAPHREF:return "GraphRef";
 
            
        default:
            //warning("[script-translation] invalid node type %X\n", node->type);
            t.ptr += sprintf(t.ptr, "Not yet read.");
            break;
    }
    
    return t.buf;
}

static const char* tostring_all(struct ScriptNode* parent, bool sep)
{
    char buf[BUFFER_SIZE * 10];
    char* ptr = &buf[0];
    
    if (!sep) indent++;
    
    unsigned count = array_element_count(parent->children);
    for (unsigned n = 0; n < count; n++)
    {
        struct ScriptNode* childnode = array_get(parent->children, n);
        if (childnode->depth == parent->depth + 1)
        {
            if (sep)
            {
                ptr += sprintf(ptr, "%s", tostring(childnode));
                if (n != count - 1) ptr += sprintf(ptr, "%s", ", ");
            }
            else
            {
                for (int i = 0; i < indent * 4; i++)
                    ptr += sprintf(ptr, " ");
                
                ptr += sprintf(ptr, "%s\n", tostring(childnode));
            }
        }
    }
    
    if (!sep) indent--;
    
    return strdup(buf);
}

SCRIPT const char* script_translate(struct Script* scpt)
{
    if (!scpt->nodes) return "error: invalid script nodes";
    
    int saved = log_indent;
    log_indent = 0;
    
    indent = 0;
    
    int n = 0;
    int last_depth = 1;
    struct ScriptNode* tree_depth[SCRIPT_MAX_DEPTH];
    memset(tree_depth, 0, sizeof(struct ScriptNode*) * SCRIPT_MAX_DEPTH);
    
    while (1)
    {
        //info("array_get (%d): %d\n", array_element_count(scpt->nodes), n);
        struct ScriptNode* node = array_get(scpt->nodes, n);
        
        node->children = array_create();
        
        for(int i = 0; i < (node->depth - 1) * 4; i++) printf(" ");
            info(BOLD COLOR_GREEN "%s (%d)\n", script_nodetype_table[node->type], node->depth);

        if (node->depth > last_depth && node->depth != 0) /* New child node */
            tree_depth[last_depth] = array_get(scpt->nodes, n - 1);
        else if (node->depth == last_depth) /* Parameter or component of parent */
            last_depth--;
        else if (node->depth < last_depth)
            last_depth = node->depth - 1;

        if (node->depth <= 0) /* End of file. */
            break;

        struct ScriptNode* parent = tree_depth[last_depth];
        if (parent)
            array_push(parent->children, node);

        last_depth = node->depth;
        
        n++;
    }
    
    /* Iterate over the top nodes only - the rest of
     * the node tree will be translated recursively. */
    for (unsigned n = 0; n < array_element_count(scpt->nodes); n++)
    {
        struct ScriptNode* node = array_get(scpt->nodes, n);
        if (node)
            if (node->depth == 1)
                //printf("%s\n", script_nodetype_table[node->type]);
                info(BOLD COLOR_PINK "%s\n", tostring(node));
    }
    
    log_indent = saved;
    
    return NULL;
}

#pragma mark - Script

SCRIPT static struct ScriptNode* scriptnode_read(struct Stream* stream)
{
    struct ScriptNode* node = malloc(sizeof *node);
    node->param = read32();
    advance(3); /* TODO: read */
    node->type = read8();
    advance(2);
    node->depth = read8();
    advance(1);
    
    //if (node->type < script_nodetype_table_length)
        //info("Script node: %s\n", script_nodetype_table[node->type]);
    
    node->children = NULL;
    node->line = -1;
    node->col = -1;
    
    return node;
}

SCRIPT struct Script* script_read(const address addr)
{
    if (addr == 0x00) return NULL;
    
    struct Script* script = malloc(sizeof *script);
    script->offset = addr;
    script->ptr_scriptnodes_offset = memory.read_32(addr) & 0xFFFFFFF;
    script->nodes = array_create();
    
    info("Script @ %X\n", addr);
    
    info("scriptnodes offset: %X\n", script->ptr_scriptnodes_offset);
    
    struct Stream* stream = stream_open(script->ptr_scriptnodes_offset);
    while (1)
    {
        struct ScriptNode* node = scriptnode_read(stream);
        array_push(script->nodes, node);
        if (node->depth == 0) break;
    }
    stream_close(stream);
    
    script_translate(script);
    
    return script;
}
