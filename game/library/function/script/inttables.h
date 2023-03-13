struct intcpa_functions
{
    void (*GetPersoAbsolutePosition)(struct intcpa* intr);
    void (*GetMyAbsolutePosition)(struct intcpa* intr);
    void (*GetAngleAroundZToPerso)(struct intcpa* intr);
    void (*DistanceToPerso)(struct intcpa* intr);
    void (*DistanceXToPerso)(struct intcpa* intr);
    void (*DistanceYToPerso)(struct intcpa* intr);
    void (*DistanceZToPerso)(struct intcpa* intr);
    void (*DistanceXYToPerso)(struct intcpa* intr);
    void (*DistanceXZToPerso)(struct intcpa* intr);
    void (*DistanceYZToPerso)(struct intcpa* intr);
    void (*DistanceToPersoCenter)(struct intcpa* intr);
    void (*DistanceXToPersoCenter)(struct intcpa* intr);
    void (*DistanceYToPersoCenter)(struct intcpa* intr);
    void (*DistanceZToPersoCenter)(struct intcpa* intr);
    void (*DistanceXYToPersoCenter)(struct intcpa* intr);
    void (*DistanceXZToPersoCenter)(struct intcpa* intr);
    void (*DistanceYZToPersoCenter)(struct intcpa* intr);
    
    void (*GetRadiusWP)(struct intcpa* intr);
    void (*CircularInterpolationBetween3WP)(struct intcpa* intr);
    void (*BezierBetween3WP)(struct intcpa* intr);
    void (*DistanceToWP)(struct intcpa* intr);
    void (*GetWPAbsolutePosition)(struct intcpa* intr);
    
    void (*Int)(struct intcpa* intr);
    void (*RandomInt)(struct intcpa* intr);
    void (*Real)(struct intcpa* intr);
    void (*Sinus)(struct intcpa* intr);
    void (*Cosinus)(struct intcpa* intr);
    void (*Square)(struct intcpa* intr);
    void (*SquareRoot)(struct intcpa* intr);
    void (*RandomReal)(struct intcpa* intr);
    void (*MinimumReal)(struct intcpa* intr);
    void (*MaximumReal)(struct intcpa* intr);
    void (*DegreeToRadian)(struct intcpa* intr);
    void (*RadianToDegree)(struct intcpa* intr);
    void (*AbsoluteValue)(struct intcpa* intr);
    void (*LimitRealInRange)(struct intcpa* intr);
    void (*Sign)(struct intcpa* intr);
    void (*Cube)(struct intcpa* intr);
    void (*Modulo)(struct intcpa* intr);
    void (*TernInf)(struct intcpa* intr);
    void (*TernSup)(struct intcpa* intr);
    void (*TernEq)(struct intcpa* intr);
    void (*TernInfEq)(struct intcpa* intr);
    void (*TernSupEq)(struct intcpa* intr);
    void (*TernOp)(struct intcpa* intr);
    void (*TemporalRealCombination)(struct intcpa* intr);
    
    /* Hitpoints */
    void (*GetHitPoints)(struct intcpa* intr);
    void (*AddAndGetHitPoints)(struct intcpa* intr);
    void (*SubAndGetHitPoints)(struct intcpa* intr);
    void (*GetHitPointsMax)(struct intcpa* intr);
    void (*AddAndGetHitPointsMax)(struct intcpa* intr);
    void (*SubAndGetHitPointsMax)(struct intcpa* intr);
    
    /* List functions */
    void (*ListSize)(struct intcpa* intr);
    void (*GivePersoInList)(struct intcpa* intr);
    
    /* Vector */
    void (*AbsoluteVector)(struct intcpa* intr);
    void (*RelativeVector)(struct intcpa* intr);
    /* & space transform */
    void (*VecteurLocalToGlobal)(struct intcpa* intr);
    void (*VecteurGlobalToLocal)(struct intcpa* intr);
    
    /* Magnet */
    void (*GetMagnetStrength)(struct intcpa* intr);
    void (*GetMagnetFar)(struct intcpa* intr);
    void (*GetMagnetNear)(struct intcpa* intr);
    void (*GetMagnetDuration)(struct intcpa* intr);
    
    /* Superobject */
    void (*SPO_GetDrawFlag)(struct intcpa* intr);
    void (*GetSPO_GetCollidedSPO)(struct intcpa* intr);
    void (*GetSPO_GetCollidedWallSPO)(struct intcpa* intr);
    void (*GetSPO_GetCollidedCeilingSPO)(struct intcpa* intr);
    
    /* Timer */
    void (*GetTime)(struct intcpa* intr);
    void (*GetElapsedTime)(struct intcpa* intr);
    void (*GetDeltaT)(struct intcpa* intr);
    void (*GetFrameLength)(struct intcpa* intr);
    
    /* Joypad */
    void (*GetFirstActivePad)(struct intcpa* intr);
    void (*InputAnalogicValueX)(struct intcpa* intr);
    void (*InputRealAnalogicValueX)(struct intcpa* intr);
    void (*InputAnalogicValueY)(struct intcpa* intr);
    void (*InputRealAnalogicValueY)(struct intcpa* intr);
    void (*InputAnalogicValue)(struct intcpa* intr);
    void (*InputRealAnalogicValue)(struct intcpa* intr);
    void (*VitessePadAnalogique)(struct intcpa* intr);
    
    void (*GenerateObject)(struct intcpa* intr);
    void (*CountGeneratedObjects)(struct intcpa* intr);
    void (*GetGlobalCounter)(struct intcpa* intr);
    void (*GetSubMapId)(struct intcpa* intr);
    
    /* Color */
    void (*AddColor)(struct intcpa* intr);
    void (*AddRed)(struct intcpa* intr);
    void (*AddGreen)(struct intcpa* intr);
    void (*AddBlue)(struct intcpa* intr);
    void (*AddAlpha)(struct intcpa* intr);
    void (*ColorRedGreenBlueAlpha)(struct intcpa* intr);
    void (*ColorRedGreenBlue)(struct intcpa* intr);
    void (*ColorRed)(struct intcpa* intr);
    void (*ColorGreen)(struct intcpa* intr);
    void (*ColorBlue)(struct intcpa* intr);
    void (*ColorAlpha)(struct intcpa* intr);
    
    /* Visual material */
    void (*GetVMTColor)(struct intcpa* intr);
    void (*GetVMTSpecularCoef)(struct intcpa* intr);
    void (*GetVMTSpecularExponent)(struct intcpa* intr);
    void (*GetVMTDiffuseCoef)(struct intcpa* intr);
    void (*GetVMTAmbientCoef)(struct intcpa* intr);
    void (*GetVMTTextureScrollingCoefU)(struct intcpa* intr);
    void (*GetVMTTextureScrollingCoefV)(struct intcpa* intr);
    void (*GetVMTFrame)(struct intcpa* intr);
    void (*GetVMTNumberOfFrames)(struct intcpa* intr);
    
    /* Multiplayer */
    void (*MP_GetNumberOfPlayers)(struct intcpa* intr);
    void (*MP_GetMaxNumberOfPlayers)(struct intcpa* intr);
    void (*MP_GetViewPortOfPlayer)(struct intcpa* intr);
    void (*MP_GetActiveMainActors)(struct intcpa* intr);
    
    /* Joypad */
    void (*PAD_GetHorizontalInvertion)(struct intcpa* intr);
    void (*PAD_GetVerticalInvertion)(struct intcpa* intr);
    void (*GetRLITableIndex)(struct intcpa* intr);
    void (*PAD2_GetGlobalVector)(struct intcpa* intr);
    void (*PAD2_GetHorizontalAxis)(struct intcpa* intr);
    void (*PAD2_GetVerticalAxis)(struct intcpa* intr);
    void (*PAD2_GetAnalogForce)(struct intcpa* intr);
    void (*PAD2_GetTrueAnalogForce)(struct intcpa* intr);
    void (*PAD2_GetRotationAngle)(struct intcpa* intr);
    void (*PAD2_GetPadSector)(struct intcpa* intr);
    void (*PAD2_GetLongDescValue)(struct intcpa* intr);
    
    /* Text */
    void (*TXT_GetNumberOfLines)(struct intcpa* intr);
    
    /* Input */
    void (*InputAnalogicValue2)(struct intcpa* intr);
    void (*InputAnalogicValueV)(struct intcpa* intr);
    void (*InputRealAnalogicValueV)(struct intcpa* intr);
    void (*InputAnalogicValueZ)(struct intcpa* intr);
    void (*InputRealAnalogicValueZ)(struct intcpa* intr);
    
    /* Save file */
    void (*SAV2_GetValue)(struct intcpa* intr);
    void (*SAV2_GetValueIndex)(struct intcpa* intr);
    void (*SAV2_GetNumberOfExistingSave)(struct intcpa* intr);
    void (*SAV2_GetTimeSaveGame)(struct intcpa* intr);
    void (*SAV2_GetDateSaveGame)(struct intcpa* intr);
    
    void (*GetTransitionSectorID)(struct intcpa* intr);
    void (*FFBPreCreateConstantEffect)(struct intcpa* intr);
    void (*FFBCreateAndLaunchConstantEffect)(struct intcpa* intr);
    void (*FFBStopEffectWithFade)(struct intcpa* intr);
    void (*CreateCheapCharacterList)(struct intcpa* intr);
    void (*TestCheapCharacterList)(struct intcpa* intr);
    void (*GetClosestCheapCharacter)(struct intcpa* intr);
    void (*GetFurthestCheapCharacter)(struct intcpa* intr);
    void (*GetReactivateCheapCharacter)(struct intcpa* intr);
    void (*LitPointsDeMagie)(struct intcpa* intr);
    void (*LitPointsDeMagieMax)(struct intcpa* intr);
    void (*AjouteEtLitPointsDeMagie)(struct intcpa* intr);
    void (*AjouteEtLitPointsDeMagieMax)(struct intcpa* intr);
    void (*EnleveEtLitPointsDeMagie)(struct intcpa* intr);
    void (*EnleveEtLitPointsDeMagieMax)(struct intcpa* intr);
    void (*LitPointsDair)(struct intcpa* intr);
    void (*LitPointsDairMax)(struct intcpa* intr);
    void (*AjouteEtLitPointsDair)(struct intcpa* intr);
    void (*AjouteEtLitPointsDairMax)(struct intcpa* intr);
    void (*EnleveEtLitPointsDair)(struct intcpa* intr);
    void (*EnleveEtLitPointsDairMax)(struct intcpa* intr);
    void (*PersoLePlusProche)(struct intcpa* intr);
    void (*PersoLePlusProcheExtended)(struct intcpa* intr);
    void (*PersoLePlusProcheDansSecteurCourant)(struct intcpa* intr);
    void (*PersoLePlusProcheDansSecteurCourantExtended)(struct intcpa* intr);
    void (*NearerActorInFieldOfVision)(struct intcpa* intr);
    void (*NearerActorInFieldOfVisionExtended)(struct intcpa* intr);
    void (*NearerActorOfFamilyInFieldOfVision)(struct intcpa* intr);
    void (*NearerActorOfFamilyInFieldOfVisionExtended)(struct intcpa* intr);
    void (*NearerActorInFieldOfVisionWithOffset)(struct intcpa* intr);
    void (*NearerActorInFieldOfVisionWithOffsetExtended)(struct intcpa* intr);
    void (*NearerActorOfFamilyInFOVWithOffset)(struct intcpa* intr);
    void (*NearerActorOfFamilyInFOVWithOffsetExtended)(struct intcpa* intr);
    void (*GetNbActivePerso)(struct intcpa* intr);
    void (*CibleLaPlusProche)(struct intcpa* intr);
    void (*CibleLaPlusProcheExtended)(struct intcpa* intr);
    void (*CibleLaPlusProcheAvecAngles)(struct intcpa* intr);
    void (*CibleLaPlusProcheAvecAnglesExtended)(struct intcpa* intr);
    
    /* Graph */
    void (*GetCloserNetwork)(struct intcpa* intr);
    void (*NetWorkCloserCollisionWP)(struct intcpa* intr);
    void (*ReseauWPLePlusProche)(struct intcpa* intr);
    void (*ReseauWPLePlusProcheZPoids)(struct intcpa* intr);
    void (*NetworkCloserWPOfType)(struct intcpa* intr);
    void (*NetworkCloserCollisionWPOfType)(struct intcpa* intr);
    void (*NetworkCloserWPOfExactType)(struct intcpa* intr);
    void (*ReseauWPLePlusDansLAxe)(struct intcpa* intr);
    void (*ReseauWPLePlusDansLAxe2)(struct intcpa* intr);
    void (*ReseauWPLePlusProcheAvecAngle)(struct intcpa* intr);
    void (*NetworkNextWPWithCapa)(struct intcpa* intr);
    void (*NetworkNextWPWithExactCapa)(struct intcpa* intr);
    void (*NetworkNextWPWithCapaOptimalWeight)(struct intcpa* intr);
    void (*NetworkNextWPOfType)(struct intcpa* intr);
    void (*NetworkNextWPOfExactType)(struct intcpa* intr);
    void (*NetworkAffectTypeOfConnectedWP)(struct intcpa* intr);
    void (*NetworkAffectTypeOfConnectedWPWithCapa)(struct intcpa* intr);
    void (*ReseauCheminLePlusCourt)(struct intcpa* intr);
    void (*NetworkBuildOrderedPath)(struct intcpa* intr);
    void (*NetworkBuildOrderedPathCircular)(struct intcpa* intr);
    void (*NetworkAllocateGraphToMSWay)(struct intcpa* intr);
    void (*NetworkAllocateGraphToMSWayCircular)(struct intcpa* intr);
    void (*NetworkBuildRandomPath)(struct intcpa* intr);
    void (*NetworkBuildRandomCircularPath)(struct intcpa* intr);
    void (*ReseauLitIndexCourant)(struct intcpa* intr);
    void (*ReseauForceIndexCourant)(struct intcpa* intr);
    void (*ReseauLitPremierIndex)(struct intcpa* intr);
    void (*ReseauLitDernierIndex)(struct intcpa* intr);
    void (*ReseauIncrementIndex)(struct intcpa* intr);
    void (*ReseauDecrementIndex)(struct intcpa* intr);
    void (*ReseauLitWPAIndex)(struct intcpa* intr);
    void (*ReseauLitCapaciteLiaisonAIndex)(struct intcpa* intr);
    void (*ReseauChangeCapaciteLiaisonAIndex)(struct intcpa* intr);
    void (*ReseauLitPoidsLiaisonAIndex)(struct intcpa* intr);
    void (*ReseauChangePoidsLiaisonAIndex)(struct intcpa* intr);
    void (*NetworkGetIndexOfWPInMSWay)(struct intcpa* intr);
    void (*NetworkForceWPToCurrent)(struct intcpa* intr);
    void (*NetworkTestTheEnds)(struct intcpa* intr);
    void (*ReseauLitCapaciteLiaisonDansGraph)(struct intcpa* intr);
    void (*ReseauChangeCapaciteLiaisonDansGraph)(struct intcpa* intr);
    void (*ReseauLitPoidsLiaisonDansGraph)(struct intcpa* intr);
    void (*ReseauChangePoidsLiaisonDansGraph)(struct intcpa* intr);
    void (*NetworkGetTypeOfWP)(struct intcpa* intr);
    
    /* Capabilities */
    void (*GetCapabilities)(struct intcpa* intr);
    void (*CapabilityAtBitNumber)(struct intcpa* intr);
    
    void (*GetScrollSpeed)(struct intcpa* intr);
    void (*MTGetScrollSpeed)(struct intcpa* intr);
    void (*GetNbFrame)(struct intcpa* intr);
    
    /* Vector function */
    void (*DotProduct)(struct intcpa* intr);
    void (*CrossProduct)(struct intcpa* intr);
    void (*Normalize)(struct intcpa* intr);
    
    /* Superobject */
    void (*GetSPOCoordinates)(struct intcpa* intr);
    void (*GetSPOSighting)(struct intcpa* intr);
    void (*GetSPOHorizon)(struct intcpa* intr);
    void (*GetSPOBanking)(struct intcpa* intr);
    void (*GetTractionFactor)(struct intcpa* intr);
    
    /* Collide object type */
    void (*GetCenterZDEType)(struct intcpa* intr);
    void (*GetCenterZDMType)(struct intcpa* intr);
    void (*GetCenterZDRType)(struct intcpa* intr);
    void (*GetCenterZDDType)(struct intcpa* intr);
    
    void (*TextAffiche)(struct intcpa* intr);
    void (*MJTextAffiche)(struct intcpa* intr);
    void (*GetCPUCounter)(struct intcpa* intr);
    void (*TurnToPosition)(struct intcpa* intr);
    void (*FormatMemCard)(struct intcpa* intr);
    void (*GetDefaultLanguage)(struct intcpa* intr);
    void (*PS2_MagicBox)(struct intcpa* intr);
    void (*SAV2_DoVarsNeedToBeSet)(struct intcpa* intr);
    void (*CINEGetGenerateEvent)(struct intcpa* intr);
    void (*StringCompare)(struct intcpa* intr);
    void (*EmptyString)(struct intcpa* intr);
    
    /* Actor */
    void (*VitesseHorizontaleDuPerso)(struct intcpa* intr);
    void (*VitesseVerticaleDuPerso)(struct intcpa* intr);
    void (*GetPersoZoomFactor)(struct intcpa* intr);
    void (*GetPersoSighting)(struct intcpa* intr);
    void (*GetPersoHorizon)(struct intcpa* intr);
    void (*GetPersoBanking)(struct intcpa* intr);
    
    /* ZDx */
    void (*LitPositionZDM)(struct intcpa* intr);
    void (*LitPositionZDE)(struct intcpa* intr);
    void (*LitPositionZDD)(struct intcpa* intr);
    void (*LitCentreZDM)(struct intcpa* intr);
    void (*LitCentreZDE)(struct intcpa* intr);
    void (*LitCentreZDD)(struct intcpa* intr);
    void (*LitAxeZDM)(struct intcpa* intr);
    void (*LitAxeZDE)(struct intcpa* intr);
    void (*LitAxeZDD)(struct intcpa* intr);
    void (*LitDimensionZDM)(struct intcpa* intr);
    void (*LitDimensionZDE)(struct intcpa* intr);
    void (*LitDimensionZDD)(struct intcpa* intr);
    
    /* Vector */
    void (*VecteurPointAxe)(struct intcpa* intr);
    void (*VecteurPointSegment)(struct intcpa* intr);
    void (*VectorContribution)(struct intcpa* intr);
    void (*VectorCombination)(struct intcpa* intr);
    void (*TemporalVectorCombination)(struct intcpa* intr);
    void (*ScaledVector)(struct intcpa* intr);
    void (*GetVectorNorm)(struct intcpa* intr);
    void (*RotateVector)(struct intcpa* intr);
    void (*VectorAngle)(struct intcpa* intr);
    void (*VectorCos)(struct intcpa* intr);
    void (*VectorSin)(struct intcpa* intr);
    void (*GetNormalCollideVector)(struct intcpa* intr);
    void (*GetNormalSlopeVector)(struct intcpa* intr);
    void (*GetNormalSlopeVectorAndAngle)(struct intcpa* intr);
    void (*GetNormalCollideVector2)(struct intcpa* intr);
    void (*GetCollidePoint)(struct intcpa* intr);
    void (*GetCollidePoint2)(struct intcpa* intr);
    void (*GetHandsCollidePoint)(struct intcpa* intr);
    void (*GetCollideRate)(struct intcpa* intr);
    void (*GetCollideRate2)(struct intcpa* intr);
    void (*GetCollideMaterialType)(struct intcpa* intr);
    void (*GetCollideMaterialType2)(struct intcpa* intr);
    void (*GetCollideMaterialDirection)(struct intcpa* intr);
    void (*GetCollideMaterialCoeff)(struct intcpa* intr);
    void (*GetCollideMaterialTypeForAi)(struct intcpa* intr);
    void (*GetCollisionPoint)(struct intcpa* intr);
    void (*GetCollisionVector)(struct intcpa* intr);
    void (*GetCollisionPerso)(struct intcpa* intr);
    void (*GetCollisionPointMaterial)(struct intcpa* intr);
    void (*GetFirstCollisionInfo)(struct intcpa* intr);
    void (*GetLastTraversedMaterialType)(struct intcpa* intr);
    void (*GetLastTraversedMaterial)(struct intcpa* intr);
    void (*GetCurrentCollidedGMT)(struct intcpa* intr);
    void (*GetColliderType)(struct intcpa* intr);
    void (*GetColliderActor)(struct intcpa* intr);
    void (*GetColliderVector)(struct intcpa* intr);
    void (*GetColliderReal)(struct intcpa* intr);
    void (*GetLastCollisionActor)(struct intcpa* intr);
    void (*ComputeRebondVector)(struct intcpa* intr);
    void (*GetModuleAbsolutePosition)(struct intcpa* intr);
    void (*GetModuleRelativePosition)(struct intcpa* intr);
    void (*GetModuleZoomFactor)(struct intcpa* intr);
    void (*GetModuleSighting)(struct intcpa* intr);
    void (*GetModuleHorizon)(struct intcpa* intr);
    void (*GetModuleBanking)(struct intcpa* intr);
    void (*CastIntegerToChannel)(struct intcpa* intr);
    void (*GetSlotDate)(struct intcpa* intr);
    void (*GetSlotName)(struct intcpa* intr);
    void (*GetSlotScore)(struct intcpa* intr);
    void (*GetStringCharAt)(struct intcpa* intr);
    void (*ScanAsciiKey)(struct intcpa* intr);
    void (*GetFormattedTextInfo)(struct intcpa* intr);
    void (*GetInputEntryName)(struct intcpa* intr);
    void (*GetMechanicGravityFactor)(struct intcpa* intr);
    void (*GetMechanicSlide)(struct intcpa* intr);
    void (*GetMechanicRebound)(struct intcpa* intr);
    void (*GetMechanicSlopeLimit)(struct intcpa* intr);
    void (*GetMechanicInertiaX)(struct intcpa* intr);
    void (*GetMechanicInertiaY)(struct intcpa* intr);
    void (*GetMechanicInertiaZ)(struct intcpa* intr);
    void (*GetMechanicTiltIntensity)(struct intcpa* intr);
    void (*GetMechanicTiltInertia)(struct intcpa* intr);
    void (*GetMechanicTiltOrigin)(struct intcpa* intr);
    void (*GetMechanicMaxSpeed)(struct intcpa* intr);
    void (*GetMechanicStreamPriority)(struct intcpa* intr);
    void (*GetMechanicStreamSpeed)(struct intcpa* intr);
    void (*GetMechanicStreamFactor)(struct intcpa* intr);
    void (*GetSlideFactorX)(struct intcpa* intr);
    void (*GetSlideFactorY)(struct intcpa* intr);
    void (*GetSlideFactorZ)(struct intcpa* intr);
    void (*JumpImpulsion)(struct intcpa* intr);
    void (*GetSpeedAnim)(struct intcpa* intr);
    void (*HierGetFather)(struct intcpa* intr);
    void (*GetActivationZDD)(struct intcpa* intr);
    void (*GetActivationZDM)(struct intcpa* intr);
    void (*GetActivationZDE)(struct intcpa* intr);
    void (*GetActivationZDR)(struct intcpa* intr);
    void (*GetCollisionFrequency)(struct intcpa* intr);
    void (*GetBrainFrequency)(struct intcpa* intr);
    void (*GetLightFrequency)(struct intcpa* intr);
    void (*GetBooleanInArray)(struct intcpa* intr);
    void (*GetNumberOfBooleanInArray)(struct intcpa* intr);
    void (*GetButtonName)(struct intcpa* intr);
    void (*GetOneCustomFloat)(struct intcpa* intr);
    void (*GetOneCustomLong)(struct intcpa* intr);
    void (*GetOneCustomVector)(struct intcpa* intr);
    void (*GetDriversAvailable)(struct intcpa* intr);
    void (*GetCurrentLanguageId)(struct intcpa* intr);
    void (*GetNbLanguages)(struct intcpa* intr);
    void (*GetLanguageText)(struct intcpa* intr);
    void (*TextToInt)(struct intcpa* intr);
    void (*NbAvailableResolution)(struct intcpa* intr);
    void (*CurrentResolution)(struct intcpa* intr);
    void (*SaveResolution)(struct intcpa* intr);
    void (*IsResolutionAvailable)(struct intcpa* intr);
    void (*GetBrightness)(struct intcpa* intr);
    void (*NameResolution)(struct intcpa* intr);
    void (*GetTextureFiltering)(struct intcpa* intr);
    void (*GetAntiAliasing)(struct intcpa* intr);
    void (*GetSaturationDistance)(struct intcpa* intr);
    void (*GetBackgroundDistance)(struct intcpa* intr);
    void (*GetTooFarLimit)(struct intcpa* intr);
    void (*GetTransparencyZoneMin)(struct intcpa* intr);
    void (*GetTransparencyZoneMax)(struct intcpa* intr);
    void (*GetCurrentFrame)(struct intcpa* intr);
    void (*ExecuteVariable)(struct intcpa* intr);
    void (*ComputeProtectKey)(struct intcpa* intr);
    void (*Xor)(struct intcpa* intr);
    void (*And)(struct intcpa* intr);
    void (*Or)(struct intcpa* intr);
    void (*Not)(struct intcpa* intr);
    void (*DivUnsigned)(struct intcpa* intr);
    void (*MulUnsigned)(struct intcpa* intr);
    void (*AddUnsigned)(struct intcpa* intr);
    void (*SubUnsigned)(struct intcpa* intr);
    void (*GetMemoryValue)(struct intcpa* intr);
    void (*GetXBoxRegion)(struct intcpa* intr);
    void (*CreditsBegin)(struct intcpa* intr);
    void (*CreditsGetChapter)(struct intcpa* intr);
    void (*CreditsGetPageFromCurrentChapter)(struct intcpa* intr);
    void (*Cam_GetShiftTarget)(struct intcpa* intr);
    void (*Cam_GetShiftPos)(struct intcpa* intr);
    void (*Cam_GetDistMin)(struct intcpa* intr);
    void (*Cam_GetDistMax)(struct intcpa* intr);
    void (*Cam_GetBoundDistMin)(struct intcpa* intr);
    void (*Cam_GetBoundDistMax)(struct intcpa* intr);
    void (*Cam_GetAngleAlpha)(struct intcpa* intr);
    void (*Cam_GetAngleShiftAlpha)(struct intcpa* intr);
    void (*Cam_GetAngleTheta)(struct intcpa* intr);
    void (*Cam_GetAngleShiftTheta)(struct intcpa* intr);
    void (*Cam_GetLinearSpeed)(struct intcpa* intr);
    void (*Cam_GetLinearIncreaseSpeed)(struct intcpa* intr);
    void (*Cam_GetLinearDecreaseSpeed)(struct intcpa* intr);
    void (*Cam_GetAngularSpeed)(struct intcpa* intr);
    void (*Cam_GetAngularIncreaseSpeed)(struct intcpa* intr);
    void (*Cam_GetAngularDecreaseSpeed)(struct intcpa* intr);
    void (*Cam_GetTargetSpeed)(struct intcpa* intr);
    void (*Cam_GetTargetIncreaseSpeed)(struct intcpa* intr);
    void (*Cam_GetTargetDecreaseSpeed)(struct intcpa* intr);
    void (*Cam_GetFocal)(struct intcpa* intr);
    void (*Cam_GetZMin)(struct intcpa* intr);
    void (*Cam_GetZMax)(struct intcpa* intr);
    void (*Cam_GetTargetedSuperObject)(struct intcpa* intr);
    void (*Cam_GetTypeOfViewport)(struct intcpa* intr);
    void (*Cam_GetCameraOfViewport)(struct intcpa* intr);
    void (*Cam_GetMainCamera)(struct intcpa* intr);
    void (*Cam_ComputeTargetWithTgtPerso)(struct intcpa* intr);
    void (*Cam_GetCurrentTargetPosition)(struct intcpa* intr);
    void (*Cam_GetSectorCameraType)(struct intcpa* intr);
    void (*Cam_GetBestPos)(struct intcpa* intr);
    void (*Cam_GetTransparencyFlag)(struct intcpa* intr);
    void (*Cam_SetMirrorFlag)(struct intcpa* intr);
    void (*Cam_GetMirrorFlag)(struct intcpa* intr);
    void (*CAM_GetViewportCoordinates)(struct intcpa* intr);
    void (*CSHBackMode)(struct intcpa* intr);
    void (*CSHMarioMode)(struct intcpa* intr);
    void (*CSHStrafeLockMode)(struct intcpa* intr);
    void (*GetViewFinderDistance)(struct intcpa* intr);
    void (*LinkInit)(struct intcpa* intr);
    void (*LinkDoBehaviour)(struct intcpa* intr);
    void (*LinkKill)(struct intcpa* intr);
    void (*LinkGetNoCutpoints)(struct intcpa* intr);
    void (*LinkGetCutPoint)(struct intcpa* intr);
    void (*FollowLinkDirection)(struct intcpa* intr);
    void (*SND_GetSoundHandle)(struct intcpa* intr);
    void (*SND_GetSendSoundRequestHandleState)(struct intcpa* intr);
    void (*SND_GetSendVoiceRequestHandleState)(struct intcpa* intr);
    void (*SND_GetSendMusicRequestHandleState)(struct intcpa* intr);
    void (*SND_GetSendAmbianceRequestHandleState)(struct intcpa* intr);
    void (*SND_GetSendMenuSndRequestHandleState)(struct intcpa* intr);
    void (*SND_GetSoundVolume)(struct intcpa* intr);
    void (*SND_GetVoiceVolume)(struct intcpa* intr);
    void (*SND_GetMusicVolume)(struct intcpa* intr);
    void (*SND_GetAmbianceVolume)(struct intcpa* intr);
    void (*SND_GetMenuSndVolume)(struct intcpa* intr);
    void (*IsSoundPlaying)(struct intcpa* intr);
    void (*SND_GetMusicMarker)(struct intcpa* intr);
    void (*MonitoringSetVariable)(struct intcpa* intr);
    void (*R3GetScore)(struct intcpa* intr);
    void (*R3GetNbCage)(struct intcpa* intr);
    void (*PS2DemoGetParameter)(struct intcpa* intr);
    void (*GetScreenDisplayFrequency)(struct intcpa* intr);
    void (*GetLastSaveGameOperationResult)(struct intcpa* intr);
    void (*SinEffect_GetSinusPosition)(struct intcpa* intr);
    void (*SFX_GetLightReflectionStatus)(struct intcpa* intr);
    void (*SFX_GetWaterCirclesStatus)(struct intcpa* intr);
    void (*SFX_IsLightReflectionPossible)(struct intcpa* intr);
    void (*SFX_IsWaterCirclesPossible)(struct intcpa* intr);
    void (*AGO_GetHandle)(struct intcpa* intr);
    void (*IsMcValid)(struct intcpa* intr);
    void (*FormatMc)(struct intcpa* intr);
};


struct intcpa_conditions
{
    void (*CollidePersoZDDNoWithPerso)(struct intcpa* intr); // 10
    void (*CollideModuleZDDNoWithPerso)(struct intcpa* intr);
    void (*CollidePersoAllZDDWithPerso)(struct intcpa* intr);
    void (*CollidePersoZDDWithAnyPerso)(struct intcpa* intr);
    void (*CollideModuleZDDWithAnyPerso)(struct intcpa* intr); // 14
    
    void (*CollidePersoZDENoWithPersoZDENo)(struct intcpa* intr); // 15
    void (*CollideModuleZDENoWithPersoZDENo)(struct intcpa* intr);
    void (*CollidePersoZDENoWithModuleZDENo)(struct intcpa* intr);
    void (*CollideModuleZDENoWithModuleZDENo)(struct intcpa* intr);
    void (*CollidePersoZDENoWithPersoTypeZDE)(struct intcpa* intr);
    void (*CollideModuleZDENoWithPersoTypeZDE)(struct intcpa* intr);
    void (*CollidePersoTypeZDEWithPersoTypeZDE)(struct intcpa* intr);
    void (*CollidePersoAllZDEWithPersoAllZDE)(struct intcpa* intr);
    void (*CollidePersoTypeZDEWithPersoAllZDE)(struct intcpa* intr);
    void (*CollidePersoAllZDEWithPersoTypeZDE)(struct intcpa* intr);
    void (*CollidePersoZDENoWithTypeZDE)(struct intcpa* intr);
    void (*CollideModuleZDENoWithTypeZDE)(struct intcpa* intr); // 26
    
    void (*CollideWithGround)(struct intcpa* intr); // 27
    void (*CollideWithWall)(struct intcpa* intr);
    void (*CollideWithNothing)(struct intcpa* intr);
    void (*CollideWithCeiling)(struct intcpa* intr);
    void (*CollideWithPerso)(struct intcpa* intr);
    void (*CollideWithWater)(struct intcpa* intr);
    void (*CollideWithThisPerso)(struct intcpa* intr); // 33
    
    void (*ZDMCollideWithGround)(struct intcpa* intr); // 34
    void (*ZDMCollideWithWall)(struct intcpa* intr);
    void (*ZDMCollideWithNothing)(struct intcpa* intr);
    void (*ZDMCollideWithCeiling)(struct intcpa* intr); // 37
    
    void (*IsPersoInList)(struct intcpa* intr); // 38
    void (*IsModelInList)(struct intcpa* intr);
    void (*IsFamilyInList)(struct intcpa* intr);
    void (*ListEmptyTest)(struct intcpa* intr); // 41
    
    void (*UserEvent_IsSet)(struct intcpa* intr); // 42
    void (*UserEvent_IsSet2)(struct intcpa* intr);
    void (*UserEvent_IsSet3)(struct intcpa* intr); // 44
    
    void (*PressedBut)(struct intcpa* intr); // 45
    void (*JustPressedBut)(struct intcpa* intr);
    void (*ReleasedBut)(struct intcpa* intr);
    void (*JustReleasedBut)(struct intcpa* intr); // 48
    
    void (*IsTimeElapsed)(struct intcpa* intr); // 49
    
    void (*IsValidObject)(struct intcpa* intr); // 50
    void (*IsValidWayPoint)(struct intcpa* intr);
    void (*IsValidGMT)(struct intcpa* intr);
    void (*IsValidVMT)(struct intcpa* intr);
    void (*IsValidAction)(struct intcpa* intr);
    void (*IsValidText)(struct intcpa* intr);
    void (*IsValidSPO)(struct intcpa* intr);
    void (*IsValidGraph)(struct intcpa* intr); // 57
    
    void (*SeePerso)(struct intcpa* intr); // 58
    void (*IsActivable)(struct intcpa* intr); // 59
    void (*IsAlreadyHandled)(struct intcpa* intr); // 60
    void (*Alw_IsMine)(struct intcpa* intr); // 61
    
    void (*IsPersoLightOn)(struct intcpa* intr); // 62
    void (*IsPersoLightPulseOn)(struct intcpa* intr);
    void (*IsPersoLightGyroPhareOn)(struct intcpa* intr); // 64
    
    void (*IsRLITransitionInProgress)(struct intcpa* intr); // 65
    void (*IsInAlwaysActiveList)(struct intcpa* intr); // 66
    void (*IsAlwaysActive)(struct intcpa* intr); // 67
    void (*IsAnActivePad)(struct intcpa* intr); // 68
    void (*IsMultitap)(struct intcpa* intr); // 69
    void (*SAV2_IsValid)(struct intcpa* intr); // 70
    void (*IsWidescreen)(struct intcpa* intr); // 71
    void (*EngineIsInPAL)(struct intcpa* intr); // 72
    
    void (*IsZDMCollideWithObstacle)(struct intcpa* intr); // 73
    void (*IsZDMCollideWithWall)(struct intcpa* intr);
    void (*IsZDMCollideWithGround)(struct intcpa* intr);
    void (*IsZDMCollideWithCeiling)(struct intcpa* intr); // 76
    
    void (*CmtIdentifierContainsMask)(struct intcpa* intr); // 77
    void (*HitByCollider)(struct intcpa* intr); // 78
    void (*IsTypeOfGMTCollide)(struct intcpa* intr);
    void (*IsInComport)(struct intcpa* intr);
    void (*IsInReflexComport)(struct intcpa* intr);
    void (*IsInAction)(struct intcpa* intr);
    void (*ChangeActionEnable)(struct intcpa* intr); // 83
    
    void (*EngineReinitRequested)(struct intcpa* intr); // 84
    void (*IsThereMechEvent)(struct intcpa* intr); // 85
    void (*CollisionWP)(struct intcpa* intr); // 86
    void (*IsCustomBitSet)(struct intcpa* intr);
    void (*IsPersoActive)(struct intcpa* intr);
    void (*CheckActionEnd)(struct intcpa* intr);
    void (*IsCurrentStateCustomBitSet)(struct intcpa* intr);
    void (*IsGiBlock)(struct intcpa* intr);
    
    void (*IsMechanicBlock)(struct intcpa* intr);
    void (*IsMechanicAnimation)(struct intcpa* intr);
    void (*IsMechanicCollide)(struct intcpa* intr);
    void (*IsMechanicGravity)(struct intcpa* intr);
    void (*IsMechanicTilt)(struct intcpa* intr);
    void (*IsMechanicGi)(struct intcpa* intr);
    void (*IsMechanicClimb)(struct intcpa* intr);
    void (*IsMechanicOnGround)(struct intcpa* intr);
    void (*IsMechanicSpider)(struct intcpa* intr);
    void (*IsMechanicShoot)(struct intcpa* intr);
    void (*IsMechanicSwim)(struct intcpa* intr);
    void (*IsMechanicNeverFall)(struct intcpa* intr);
    void (*IsMechanicCollisionControl)(struct intcpa* intr);
    void (*IsMechanicKeepSpeedZ)(struct intcpa* intr);
    void (*IsMechanicSpeedLimit)(struct intcpa* intr);
    void (*IsMechanicInertia)(struct intcpa* intr);
    void (*IsMechanicStream)(struct intcpa* intr);
    void (*IsMechanicStickOnPlatform)(struct intcpa* intr);
    void (*IsMechanicPatformCrash)(struct intcpa* intr);
    void (*IsMechanicScale)(struct intcpa* intr);
    void (*IsMechanicExec)(struct intcpa* intr);
    
    void (*CanFall)(struct intcpa* intr);
    void (*IsMechanicCrash)(struct intcpa* intr);
    void (*IsNullVector)(struct intcpa* intr);
    void (*HierIsSonOfActor)(struct intcpa* intr);
    void (*IsMorphing)(struct intcpa* intr);
    void (*CheckAnimEnd)(struct intcpa* intr);
    void (*CheckAnimSmooth)(struct intcpa* intr);
    void (*HasTheCapability)(struct intcpa* intr);
    void (*HasOneOfTheCapabilities)(struct intcpa* intr);
    void (*HasTheCapabilityNumber)(struct intcpa* intr);
    void (*PersoHasTheCapability)(struct intcpa* intr);
    void (*PersoHasOneOfTheCapabilities)(struct intcpa* intr);
    void (*PersoHasTheCapabilityNumber)(struct intcpa* intr);
    void (*MagnetIsActivated)(struct intcpa* intr);
    void (*NEstPasEnCollisionAvecMap)(struct intcpa* intr);
    void (*NEstPasEnCollisionAvecProjectile)(struct intcpa* intr);
    void (*NEstPasEnCollisionAvecSecondCharact)(struct intcpa* intr);
    void (*NEstPasEnCollisionAvecMainCharact)(struct intcpa* intr);
    void (*NEstPasEnCollisionAvecAutresSecteurs)(struct intcpa* intr);
    
    void (*IsInFamily)(struct intcpa* intr);
    void (*IsInModel)(struct intcpa* intr);
    void (*AJoypadIsConnected)(struct intcpa* intr);
    void (*AKeyJustPressed)(struct intcpa* intr);
    void (*AButtonPadJustPressed)(struct intcpa* intr);
    void (*IsInDemoMode)(struct intcpa* intr);
    void (*IsShapnessMax)(struct intcpa* intr);
    void (*IsTooFar)(struct intcpa* intr);
    void (*IsSubAnimPlaying)(struct intcpa* intr);
    void (*TestCBSubAnim)(struct intcpa* intr);
    void (*IsInSubAnim)(struct intcpa* intr);
    void (*IsSubAnimNearEnd)(struct intcpa* intr);
    void (*IsSameSPO)(struct intcpa* intr);
    void (*PressedPadBut)(struct intcpa* intr);
    void (*JustPressedPadBut)(struct intcpa* intr);
    void (*ReleasedPadBut)(struct intcpa* intr);
    void (*JustReleasedPadBut)(struct intcpa* intr);
    void (*IsCinePlaying)(struct intcpa* intr);
    void (*LoadInProgress)(struct intcpa* intr);
    void (*SAV2LastError)(struct intcpa* intr);
    void (*CheckMCStatus)(struct intcpa* intr);
    
    void (*Cam_IsActive)(struct intcpa* intr);
    void (*Cam_IsViewportOwner)(struct intcpa* intr);
    void (*Cam_IsFlagNoDynamicTarget)(struct intcpa* intr);
    void (*Cam_IsFlagNoAverageMoveTgtPerso)(struct intcpa* intr);
    void (*Cam_IsFlagNoParseCutAngle)(struct intcpa* intr);
    void (*Cam_IsFlagNoVisibility)(struct intcpa* intr);
    void (*Cam_IsFlagNoVisibilityWithDynHie)(struct intcpa* intr);
    void (*Cam_IsFlagNoDynChangeTheta)(struct intcpa* intr);
    void (*Cam_IsFlagNoShiftUntilPosReached)(struct intcpa* intr);
    void (*Cam_IsFlagNoDynSpeed)(struct intcpa* intr);
    void (*Cam_IsFlagNoLinearParsing)(struct intcpa* intr);
    void (*Cam_IsFlagNoLinearInertia)(struct intcpa* intr);
    void (*Cam_IsFlagNoAngularParsing)(struct intcpa* intr);
    void (*Cam_IsFlagNoAngularInertia)(struct intcpa* intr);
    void (*Cam_IsFlagNoTargetParsing)(struct intcpa* intr);
    void (*Cam_IsFlagNoTargetInertia)(struct intcpa* intr);
    void (*Cam_IsFlagNoObstacle)(struct intcpa* intr);
    void (*Cam_IsFlagFixedOrientation)(struct intcpa* intr);
    void (*Cam_IsFlagForcedPosition)(struct intcpa* intr);
    void (*Cam_IsFlagForcedTarget)(struct intcpa* intr);
    void (*Cam_IsFlagForcedAxis)(struct intcpa* intr);
};

struct intcpa_dsg
{
    void (*Read)(struct intcpa* intr, pointer actor, unsigned var, uint32_t* type, void** result);
    void (*Write)(struct intcpa* intr, pointer actor, unsigned var, uint32_t type, void* store);
};
