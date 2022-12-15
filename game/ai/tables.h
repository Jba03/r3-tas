//
//  tables.h
//  r3-tas
//
//  Created by Jba03 on 2022-12-01.
//

#ifndef tables_h
#define tables_h

static const char * const script_nodetype_table[] = {
    "Keyword", "Condition", "Operator", "Function",
    "Procedure", "MetaAction", "BeginMacro", "EndMacro",
    "EndTree", "Field",
    "DsgVar", // 10
    "DsgVarRef", // 11
    "Constant", // 12
    "Real", "Button", "ConstantVector", "Vector", "Mask",
    "Module", "DsgVarId", "String", "LipsSynchroRef",
    "FamilyRef", "PersoRef", "ActionRef", "SuperObjectRef",
    "SOLinksRef", "WayPointRef", "TextRef", "ComportRef",
    "ModuleRef", "SoundEventRef", "ObjectTableRef",
    "GameMaterialRef", "VisualMaterial", "ParticleGenerator",
    "Color", "ModelRef", "Light", "Caps", "Graph",
    "Subroutine", "Unknown", "CineRef", "GraphRef",
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
    "Operator_Plus",
    "Operator_Minus",
    "Operator_Mul",
    "Operator_Div",
    "Operator_UnaryMinus",
    "Operator_Mod",
    
    "Operator_PlusAffect", // 6
    "Operator_MinusAffect",
    "Operator_MulAffect",
    "Operator_DivAffect",
    "Operator_PlusPlusAffect"
    "Operator_MinusMinusAffect",
    "Operator_Affect", // 12
    
    "Operator_Dot", ".X", ".Y", ".Z",
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
    "Cond_And", // 0
    "Cond_Or",
    "Cond_Not",
    "Cond_XOR", // 3
    
    "Cond_Equal", // 4
    "Cond_Different",
    "Cond_Lesser",
    "Cond_Greater",
    "Cond_LesserOrEqual",
    "Cond_GreaterOrEqual", // 9
    
    "CollidePersoZDDNoWithPerso", // 10
    "CollideModuleZDDNoWithPerso",
    "CollidePersoAllZDDWithPerso",
    "CollidePersoZDDWithAnyPerso",
    "CollideModuleZDDWithAnyPerso", // 14
    
    "CollidePersoZDENoWithPersoZDENo", // 15
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
    "CollideModuleZDENoWithTypeZDE", // 26
    
    "CollideWithGround", // 27
    "CollideWithWall",
    "CollideWithNothing",
    "CollideWithCeiling",
    "CollideWithPerso",
    "CollideWithWater",
    "CollideWithThisPerso", // 33
    
    "ZDMCollideWithGround", // 34
    "ZDMCollideWithWall",
    "ZDMCollideWithNothing",
    "ZDMCollideWithCeiling", // 37
    
    "IsPersoInList", // 38
    "IsModelInList",
    "IsFamilyInList",
    "ListEmptyTest", // 41
    
    "UserEvent_IsSet", // 42
    "UserEvent_IsSet2",
    "UserEvent_IsSet3", // 44
    
    "PressedBut", // 45
    "JustPressedBut",
    "ReleasedBut",
    "JustReleasedBut", // 48
    
    "IsTimeElapsed", // 49
    
    "IsValidObject", // 50
    "IsValidWayPoint",
    "IsValidGMT",
    "IsValidVMT",
    "IsValidAction",
    "IsValidText",
    "IsValidSPO",
    "IsValidGraph", // 57
    
    "SeePerso", // 58
    "IsActivable", // 59
    "IsAlreadyHandled", // 60
    "Alw_IsMine", // 61
    
    "IsPersoLightOn", // 62
    "IsPersoLightPulseOn",
    "IsPersoLightGyroPhareOn", // 64
    
    "IsRLITransitionInProgress", // 65
    "IsInAlwaysActiveList", // 66
    "IsAlwaysActive", // 67
    "IsAnActivePad", // 68
    "IsMultitap", // 69
    "SAV2_IsValid", // 70
    "IsWidescreen", // 71
    "EngineIsInPAL", // 72
    
    "IsZDMCollideWithObstacle", // 73
    "IsZDMCollideWithWall",
    "IsZDMCollideWithGround",
    "IsZDMCollideWithCeiling", // 76
    
    "CmtIdentifierContainsMask", // 77
    "HitByCollider", // 78
    "IsTypeOfGMTCollide",
    "IsInComport",
    "IsInReflexComport",
    "IsInAction",
    "ChangeActionEnable", // 83
    
    "EngineReinitRequested", // 84
    "IsThereMechEvent", // 85
    "CollisionWP", // 86
    "IsCustomBitSet", "IsPersoActive",
    "CheckActionEnd", "IsCurrentStateCustomBitSet", "IsGiBlock", "IsMechanicBlock", "IsMechanicAnimation",
    "IsMechanicCollide", "IsMechanicGravity", "IsMechanicTilt", "IsMechanicGi", "IsMechanicClimb", "IsMechanicOnGround",
    "IsMechanicSpider", "IsMechanicShoot", "IsMechanicSwim", "IsMechanicNeverFall", "IsMechanicCollisionControl",
    "IsMechanicKeepSpeedZ", "IsMechanicSpeedLimit", "IsMechanicInertia", "IsMechanicStream", "IsMechanicStickOnPlatform",
    "IsMechanicPatformCrash", "IsMechanicScale", "IsMechanicExec", "CanFall", "IsMechanicCrash", "IsNullVector",
    "HierIsSonOfActor", "IsMorphing", "CheckAnimEnd", "CheckAnimSmooth", "HasTheCapability", "HasOneOfTheCapabilities",
    "HasTheCapabilityNumber", "PersoHasTheCapability", "PersoHasOneOfTheCapabilities", "PersoHasTheCapabilityNumber",
    "MagnetIsActivated", "NEstPasEnCollisionAvecMap", "NEstPasEnCollisionAvecProjectile", "NEstPasEnCollisionAvecSecondCharact",
    "NEstPasEnCollisionAvecMainCharact", "NEstPasEnCollisionAvecAutresSecteurs", "IsInFamily", "IsInModel", "AJoypadIsConnected",
    "AKeyJustPressed", "AButtonPadJustPressed", "IsInDemoMode", "IsShapnessMax", "IsTooFar", "IsSubAnimPlaying", "TestCBSubAnim",
    "IsInSubAnim", "IsSubAnimNearEnd", "IsSameSPO", "PressedPadBut", "JustPressedPadBut", "ReleasedPadBut", "JustReleasedPadBut",
    "IsCinePlaying", "LoadInProgress", "SAV2LastError", "CheckMCStatus", "Cam_IsActive", "Cam_IsViewportOwner",
    "Cam_IsFlagNoDynamicTarget", "Cam_IsFlagNoAverageMoveTgtPerso", "Cam_IsFlagNoParseCutAngle", "Cam_IsFlagNoVisibility",
    "Cam_IsFlagNoVisibilityWithDynHie", "Cam_IsFlagNoDynChangeTheta", "Cam_IsFlagNoShiftUntilPosReached", "Cam_IsFlagNoDynSpeed",
    "Cam_IsFlagNoLinearParsing", "Cam_IsFlagNoLinearInertia", "Cam_IsFlagNoAngularParsing", "Cam_IsFlagNoAngularInertia",
    "Cam_IsFlagNoTargetParsing", "Cam_IsFlagNoTargetInertia", "Cam_IsFlagNoObstacle", "Cam_IsFlagFixedOrientation",
    "Cam_IsFlagForcedPosition", "Cam_IsFlagForcedTarget", "Cam_IsFlagForcedAxis", "SND_IsInStereoMode",
    "SND_IsMusicPlaying", "SND_IsVoicePlaying", "SND_IsEventValid"
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

#define L(T) static const uint64_t T##_length = sizeof T / sizeof *T;
L(script_nodetype_table)
L(script_keyword_table)
L(script_operator_table)
L(script_function_table)
L(script_procedure_table)
L(script_condition_table)
L(script_field_table)
L(script_meta_action_table)
#undef L

#endif /* tables_h */
