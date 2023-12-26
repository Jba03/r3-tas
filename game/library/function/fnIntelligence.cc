//
//  intelligence.c
//  r3-tas
//
//  Created by Jba03 on 2023-03-06.
//

#include "stIntelligence.hh"
#include "stSuperObject.hh"
#include "stMind.hh"
#include "stTreeInterpret.hh"

#include "memory.hh"

#define current() (c->frame->current)
#define op() fnTreeInterpret(c)

#define lval(nd)    *((int32*)&nd->param)
#define ulval(nd)   *((uint32*)&nd->param)
#define rval(nd)    *((float*)&nd->param)

#define pval(nd) *((uint64**)(&nd->param + 4))

#pragma mark - Functions

stNodeInterpret *fn_AIFunction_GetPersoAbsolutePosition(stTreeInterpretContext *c)
{
    stNodeInterpret *function = current();
    /* Only modify the node if the actor is valid */
    if ((c->isUltra && c->actorReference) || (!c->isUltra && c->owner))
    {
        stVector3D position = fnActorGetPosition(c->isUltra ? c->actorReference : c->owner);
        /* Allocate a new vector */
        
        /* Use the padding fields of the node to assign a system address */
        //pval(function) = (uint64*)position;
    }
    
//    if (c->isUltra)
//    {
//        stActor *actor = c->
//    }
//    else
//    {
//        stSuperObject *actorSO = fnSuperobe
//        fnSuperobjectGetGlobalMatrix(<#const stSuperObject *so#>)
//    }
//    
//    if (c->isUltra)
//    {
//        stNodeInterpret *function = current();
//        fnSuperobjectGetGlobalMatrix(<#const stSuperObject *so#>)
//        fnActorGetPosition(<#const stEngineObject *actor#>)
//    }
//    
    return function;
}

stNodeInterpret *GetMyAbsolutePosition(stTreeInterpretContext *c);
stNodeInterpret *GetAngleAroundZToPerso(stTreeInterpretContext *c);

/* Used */
stNodeInterpret *DistanceToPerso(stTreeInterpretContext *c);

stNodeInterpret *DistanceXToPerso(stTreeInterpretContext *c);
stNodeInterpret *DistanceYToPerso(stTreeInterpretContext *c);
stNodeInterpret *DistanceZToPerso(stTreeInterpretContext *c);
stNodeInterpret *DistanceXYToPerso(stTreeInterpretContext *c);
stNodeInterpret *DistanceXZToPerso(stTreeInterpretContext *c);
stNodeInterpret *DistanceYZToPerso(stTreeInterpretContext *c);
stNodeInterpret *DistanceToPersoCenter(stTreeInterpretContext *c);
stNodeInterpret *DistanceXToPersoCenter(stTreeInterpretContext *c);
stNodeInterpret *DistanceYToPersoCenter(stTreeInterpretContext *c);
stNodeInterpret *DistanceZToPersoCenter(stTreeInterpretContext *c);
stNodeInterpret *DistanceXYToPersoCenter(stTreeInterpretContext *c);
stNodeInterpret *DistanceXZToPersoCenter(stTreeInterpretContext *c);
stNodeInterpret *DistanceYZToPersoCenter(stTreeInterpretContext *c);

stNodeInterpret *GetRadiusWP(stTreeInterpretContext *c);
stNodeInterpret *CircularInterpolationBetween3WP(stTreeInterpretContext *c);
stNodeInterpret *BezierBetween3WP(stTreeInterpretContext *c);
stNodeInterpret *DistanceToWP(stTreeInterpretContext *c);
stNodeInterpret *GetWPAbsolutePosition(stTreeInterpretContext *c);

stNodeInterpret *Int(stTreeInterpretContext *c)
{
    stNodeInterpret *function = current();
    stNodeInterpret *expression = op();
    lval(function) = lval(expression);
    return function;
}

stNodeInterpret *RandomInt(stTreeInterpretContext *c)
{
    stNodeInterpret *function = current();
    stNodeInterpret *min = op();
    stNodeInterpret *max = op();
    
//    int32 v = fnRNDCall(c->globals->rnd, 1, 0, lval(min), lval(max));
//    lval(function) = v;
    
    return function;
}

stNodeInterpret *Real(stTreeInterpretContext *c);
stNodeInterpret *Sinus(stTreeInterpretContext *c);
stNodeInterpret *Cosinus(stTreeInterpretContext *c);
stNodeInterpret *Square(stTreeInterpretContext *c);
stNodeInterpret *SquareRoot(stTreeInterpretContext *c);
stNodeInterpret *RandomReal(stTreeInterpretContext *c);
stNodeInterpret *MinimumReal(stTreeInterpretContext *c);
stNodeInterpret *MaximumReal(stTreeInterpretContext *c);
stNodeInterpret *DegreeToRadian(stTreeInterpretContext *c);
stNodeInterpret *RadianToDegree(stTreeInterpretContext *c);
stNodeInterpret *AbsoluteValue(stTreeInterpretContext *c);
stNodeInterpret *LimitRealInRange(stTreeInterpretContext *c);
stNodeInterpret *Sign(stTreeInterpretContext *c);
stNodeInterpret *Cube(stTreeInterpretContext *c);
stNodeInterpret *Modulo(stTreeInterpretContext *c);
stNodeInterpret *TernInf(stTreeInterpretContext *c);
stNodeInterpret *TernSup(stTreeInterpretContext *c);
stNodeInterpret *TernEq(stTreeInterpretContext *c);
stNodeInterpret *TernInfEq(stTreeInterpretContext *c);
stNodeInterpret *TernSupEq(stTreeInterpretContext *c);
stNodeInterpret *TernOp(stTreeInterpretContext *c);
stNodeInterpret *TemporalRealCombination(stTreeInterpretContext *c);

stNodeInterpret *fn_AIFunction_HitPoints(stTreeInterpretContext *c)
{
    stNodeInterpret *function = current();
    stNodeInterpret *object = op();
    stActor *actor = pointer(host_byteorder_32(object->param));
    stStandardGameInfo *stdGame = fnActorGetStdGame(actor);
    
    if (stdGame)
    {
        switch (function->type)
        {
            case 46: /* GetHitPoints */
            {
                lval(function) = (int32)stdGame->hitPoints;
                break;
            }
                
            case 47: /* AddAndGetHitPoints */
            {
                if (c->memoryAccessMode & INTERPRETER_WRITE)
                {
                    stNodeInterpret *add = op();
                    if (stdGame->hitPoints + (uint8)lval(add) <= stdGame->maxHitPoints)
                        stdGame->hitPoints += (uint8)lval(add);
                    else
                        stdGame->hitPoints = stdGame->maxHitPoints;
                }
                
                lval(function) = (int32)stdGame->hitPoints;
                break;
            }
                
            case 48: /* SubAndGetHitPoints */
            {
                if (c->memoryAccessMode & INTERPRETER_WRITE)
                {
                    stNodeInterpret *add = op();
                    if ((int32)stdGame->hitPoints - lval(add) > 0)
                        stdGame->hitPoints -= lval(add);
                    else
                        stdGame->hitPoints = 0;
                }
                
                lval(function) = (int32)stdGame->hitPoints;
                break;
            }
                
            case 49: /* GetHitPointsMax */
            {
                
                
                break;
            }
                
            case 50: /* AddAndGetHitPointsMax */
            {
                break;
            }
                
            case 51: /* SubAndGetHitPointsMax */
            {
                break;
            }
        }
    }
    
    return function;
}

/* List functions */
stNodeInterpret *ListSize(stTreeInterpretContext *c);
stNodeInterpret *GivePersoInList(stTreeInterpretContext *c);

/* Vector */
stNodeInterpret *AbsoluteVector(stTreeInterpretContext *c);
stNodeInterpret *RelativeVector(stTreeInterpretContext *c);
/* & space transform */
stNodeInterpret *VecteurLocalToGlobal(stTreeInterpretContext *c);
stNodeInterpret *VecteurGlobalToLocal(stTreeInterpretContext *c);

/* Magnet */
stNodeInterpret *GetMagnetStrength(stTreeInterpretContext *c);
stNodeInterpret *GetMagnetFar(stTreeInterpretContext *c);
stNodeInterpret *GetMagnetNear(stTreeInterpretContext *c);
stNodeInterpret *GetMagnetDuration(stTreeInterpretContext *c);

/* Superobject */
stNodeInterpret *SPO_GetDrawFlag(stTreeInterpretContext *c);
stNodeInterpret *GetSPO_GetCollidedSPO(stTreeInterpretContext *c);
stNodeInterpret *GetSPO_GetCollidedWallSPO(stTreeInterpretContext *c);
stNodeInterpret *GetSPO_GetCollidedCeilingSPO(stTreeInterpretContext *c);

/* Timer */
stNodeInterpret *GetTime(stTreeInterpretContext *c);
stNodeInterpret *GetElapsedTime(stTreeInterpretContext *c);
stNodeInterpret *GetDeltaT(stTreeInterpretContext *c);
stNodeInterpret *GetFrameLength(stTreeInterpretContext *c);

/* Joypad */
stNodeInterpret *GetFirstActivePad(stTreeInterpretContext *c);
stNodeInterpret *InputAnalogicValueX(stTreeInterpretContext *c);
stNodeInterpret *InputRealAnalogicValueX(stTreeInterpretContext *c);
stNodeInterpret *InputAnalogicValueY(stTreeInterpretContext *c);
stNodeInterpret *InputRealAnalogicValueY(stTreeInterpretContext *c);
stNodeInterpret *InputAnalogicValue(stTreeInterpretContext *c);
stNodeInterpret *InputRealAnalogicValue(stTreeInterpretContext *c);
stNodeInterpret *VitessePadAnalogique(stTreeInterpretContext *c);

stNodeInterpret *GenerateObject(stTreeInterpretContext *c);
stNodeInterpret *CountGeneratedObjects(stTreeInterpretContext *c);
stNodeInterpret *GetGlobalCounter(stTreeInterpretContext *c);
stNodeInterpret *GetSubMapId(stTreeInterpretContext *c);

/* Color */
stNodeInterpret *AddColor(stTreeInterpretContext *c);
stNodeInterpret *AddRed(stTreeInterpretContext *c);
stNodeInterpret *AddGreen(stTreeInterpretContext *c);
stNodeInterpret *AddBlue(stTreeInterpretContext *c);
stNodeInterpret *AddAlpha(stTreeInterpretContext *c);
stNodeInterpret *ColorRedGreenBlueAlpha(stTreeInterpretContext *c);
stNodeInterpret *ColorRedGreenBlue(stTreeInterpretContext *c);
stNodeInterpret *ColorRed(stTreeInterpretContext *c);
stNodeInterpret *ColorGreen(stTreeInterpretContext *c);
stNodeInterpret *ColorBlue(stTreeInterpretContext *c);
stNodeInterpret *ColorAlpha(stTreeInterpretContext *c);

/* Visual material */
stNodeInterpret *GetVMTColor(stTreeInterpretContext *c);
stNodeInterpret *GetVMTSpecularCoef(stTreeInterpretContext *c);
stNodeInterpret *GetVMTSpecularExponent(stTreeInterpretContext *c);
stNodeInterpret *GetVMTDiffuseCoef(stTreeInterpretContext *c);
stNodeInterpret *GetVMTAmbientCoef(stTreeInterpretContext *c);
stNodeInterpret *GetVMTTextureScrollingCoefU(stTreeInterpretContext *c);
stNodeInterpret *GetVMTTextureScrollingCoefV(stTreeInterpretContext *c);
stNodeInterpret *GetVMTFrame(stTreeInterpretContext *c);
stNodeInterpret *GetVMTNumberOfFrames(stTreeInterpretContext *c);

/* Multiplayer */
stNodeInterpret *MP_GetNumberOfPlayers(stTreeInterpretContext *c);
stNodeInterpret *MP_GetMaxNumberOfPlayers(stTreeInterpretContext *c);
stNodeInterpret *MP_GetViewPortOfPlayer(stTreeInterpretContext *c);
stNodeInterpret *MP_GetActiveMainActors(stTreeInterpretContext *c);

/* Joypad */
stNodeInterpret *PAD_GetHorizontalInvertion(stTreeInterpretContext *c);
stNodeInterpret *PAD_GetVerticalInvertion(stTreeInterpretContext *c);
stNodeInterpret *GetRLITableIndex(stTreeInterpretContext *c);
stNodeInterpret *PAD2_GetGlobalVector(stTreeInterpretContext *c);
stNodeInterpret *PAD2_GetHorizontalAxis(stTreeInterpretContext *c);
stNodeInterpret *PAD2_GetVerticalAxis(stTreeInterpretContext *c);
stNodeInterpret *PAD2_GetAnalogForce(stTreeInterpretContext *c);
stNodeInterpret *PAD2_GetTrueAnalogForce(stTreeInterpretContext *c);
stNodeInterpret *PAD2_GetRotationAngle(stTreeInterpretContext *c);
stNodeInterpret *PAD2_GetPadSector(stTreeInterpretContext *c);
stNodeInterpret *PAD2_GetLongDescValue(stTreeInterpretContext *c);

/* Text */
stNodeInterpret *TXT_GetNumberOfLines(stTreeInterpretContext *c);

/* Input */
stNodeInterpret *InputAnalogicValue2(stTreeInterpretContext *c);
stNodeInterpret *InputAnalogicValueV(stTreeInterpretContext *c);
stNodeInterpret *InputRealAnalogicValueV(stTreeInterpretContext *c);
stNodeInterpret *InputAnalogicValueZ(stTreeInterpretContext *c);
stNodeInterpret *InputRealAnalogicValueZ(stTreeInterpretContext *c);

/* Save file */
stNodeInterpret *SAV2_GetValue(stTreeInterpretContext *c);
stNodeInterpret *SAV2_GetValueIndex(stTreeInterpretContext *c);
stNodeInterpret *SAV2_GetNumberOfExistingSave(stTreeInterpretContext *c);
stNodeInterpret *SAV2_GetTimeSaveGame(stTreeInterpretContext *c);
stNodeInterpret *SAV2_GetDateSaveGame(stTreeInterpretContext *c);

stNodeInterpret *GetTransitionSectorID(stTreeInterpretContext *c);
stNodeInterpret *FFBPreCreateConstantEffect(stTreeInterpretContext *c);
stNodeInterpret *FFBCreateAndLaunchConstantEffect(stTreeInterpretContext *c);
stNodeInterpret *FFBStopEffectWithFade(stTreeInterpretContext *c);
stNodeInterpret *CreateCheapCharacterList(stTreeInterpretContext *c);
stNodeInterpret *TestCheapCharacterList(stTreeInterpretContext *c);
stNodeInterpret *GetClosestCheapCharacter(stTreeInterpretContext *c);
stNodeInterpret *GetFurthestCheapCharacter(stTreeInterpretContext *c);
stNodeInterpret *GetReactivateCheapCharacter(stTreeInterpretContext *c);
stNodeInterpret *LitPointsDeMagie(stTreeInterpretContext *c);
stNodeInterpret *LitPointsDeMagieMax(stTreeInterpretContext *c);
stNodeInterpret *AjouteEtLitPointsDeMagie(stTreeInterpretContext *c);
stNodeInterpret *AjouteEtLitPointsDeMagieMax(stTreeInterpretContext *c);
stNodeInterpret *EnleveEtLitPointsDeMagie(stTreeInterpretContext *c);
stNodeInterpret *EnleveEtLitPointsDeMagieMax(stTreeInterpretContext *c);
stNodeInterpret *LitPointsDair(stTreeInterpretContext *c);
stNodeInterpret *LitPointsDairMax(stTreeInterpretContext *c);
stNodeInterpret *AjouteEtLitPointsDair(stTreeInterpretContext *c);
stNodeInterpret *AjouteEtLitPointsDairMax(stTreeInterpretContext *c);
stNodeInterpret *EnleveEtLitPointsDair(stTreeInterpretContext *c);
stNodeInterpret *EnleveEtLitPointsDairMax(stTreeInterpretContext *c);
stNodeInterpret *PersoLePlusProche(stTreeInterpretContext *c);
stNodeInterpret *PersoLePlusProcheExtended(stTreeInterpretContext *c);
stNodeInterpret *PersoLePlusProcheDansSecteurCourant(stTreeInterpretContext *c);
stNodeInterpret *PersoLePlusProcheDansSecteurCourantExtended(stTreeInterpretContext *c);
stNodeInterpret *NearerActorInFieldOfVision(stTreeInterpretContext *c);
stNodeInterpret *NearerActorInFieldOfVisionExtended(stTreeInterpretContext *c);
stNodeInterpret *NearerActorOfFamilyInFieldOfVision(stTreeInterpretContext *c);
stNodeInterpret *NearerActorOfFamilyInFieldOfVisionExtended(stTreeInterpretContext *c);
stNodeInterpret *NearerActorInFieldOfVisionWithOffset(stTreeInterpretContext *c);
stNodeInterpret *NearerActorInFieldOfVisionWithOffsetExtended(stTreeInterpretContext *c);
stNodeInterpret *NearerActorOfFamilyInFOVWithOffset(stTreeInterpretContext *c);
stNodeInterpret *NearerActorOfFamilyInFOVWithOffsetExtended(stTreeInterpretContext *c);
stNodeInterpret *GetNbActivePerso(stTreeInterpretContext *c);
stNodeInterpret *CibleLaPlusProche(stTreeInterpretContext *c);
stNodeInterpret *CibleLaPlusProcheExtended(stTreeInterpretContext *c);
stNodeInterpret *CibleLaPlusProcheAvecAngles(stTreeInterpretContext *c);
stNodeInterpret *CibleLaPlusProcheAvecAnglesExtended(stTreeInterpretContext *c);

/* Graph */
stNodeInterpret *GetCloserNetwork(stTreeInterpretContext *c);
stNodeInterpret *NetWorkCloserCollisionWP(stTreeInterpretContext *c);
stNodeInterpret *ReseauWPLePlusProche(stTreeInterpretContext *c);
stNodeInterpret *ReseauWPLePlusProcheZPoids(stTreeInterpretContext *c);
stNodeInterpret *NetworkCloserWPOfType(stTreeInterpretContext *c);
stNodeInterpret *NetworkCloserCollisionWPOfType(stTreeInterpretContext *c);
stNodeInterpret *NetworkCloserWPOfExactType(stTreeInterpretContext *c);
stNodeInterpret *ReseauWPLePlusDansLAxe(stTreeInterpretContext *c);
stNodeInterpret *ReseauWPLePlusDansLAxe2(stTreeInterpretContext *c);
stNodeInterpret *ReseauWPLePlusProcheAvecAngle(stTreeInterpretContext *c);
stNodeInterpret *NetworkNextWPWithCapa(stTreeInterpretContext *c);
stNodeInterpret *NetworkNextWPWithExactCapa(stTreeInterpretContext *c);
stNodeInterpret *NetworkNextWPWithCapaOptimalWeight(stTreeInterpretContext *c);
stNodeInterpret *NetworkNextWPOfType(stTreeInterpretContext *c);
stNodeInterpret *NetworkNextWPOfExactType(stTreeInterpretContext *c);
stNodeInterpret *NetworkAffectTypeOfConnectedWP(stTreeInterpretContext *c);
stNodeInterpret *NetworkAffectTypeOfConnectedWPWithCapa(stTreeInterpretContext *c);
stNodeInterpret *ReseauCheminLePlusCourt(stTreeInterpretContext *c);
stNodeInterpret *NetworkBuildOrderedPath(stTreeInterpretContext *c);
stNodeInterpret *NetworkBuildOrderedPathCircular(stTreeInterpretContext *c);
stNodeInterpret *NetworkAllocateGraphToMSWay(stTreeInterpretContext *c);
stNodeInterpret *NetworkAllocateGraphToMSWayCircular(stTreeInterpretContext *c);
stNodeInterpret *NetworkBuildRandomPath(stTreeInterpretContext *c);
stNodeInterpret *NetworkBuildRandomCircularPath(stTreeInterpretContext *c);
stNodeInterpret *ReseauLitIndexCourant(stTreeInterpretContext *c);
stNodeInterpret *ReseauForceIndexCourant(stTreeInterpretContext *c);
stNodeInterpret *ReseauLitPremierIndex(stTreeInterpretContext *c);
stNodeInterpret *ReseauLitDernierIndex(stTreeInterpretContext *c);
stNodeInterpret *ReseauIncrementIndex(stTreeInterpretContext *c);
stNodeInterpret *ReseauDecrementIndex(stTreeInterpretContext *c);
stNodeInterpret *ReseauLitWPAIndex(stTreeInterpretContext *c);
stNodeInterpret *ReseauLitCapaciteLiaisonAIndex(stTreeInterpretContext *c);
stNodeInterpret *ReseauChangeCapaciteLiaisonAIndex(stTreeInterpretContext *c);
stNodeInterpret *ReseauLitPoidsLiaisonAIndex(stTreeInterpretContext *c);
stNodeInterpret *ReseauChangePoidsLiaisonAIndex(stTreeInterpretContext *c);
stNodeInterpret *NetworkGetIndexOfWPInMSWay(stTreeInterpretContext *c);
stNodeInterpret *NetworkForceWPToCurrent(stTreeInterpretContext *c);
stNodeInterpret *NetworkTestTheEnds(stTreeInterpretContext *c);
stNodeInterpret *ReseauLitCapaciteLiaisonDansGraph(stTreeInterpretContext *c);
stNodeInterpret *ReseauChangeCapaciteLiaisonDansGraph(stTreeInterpretContext *c);
stNodeInterpret *ReseauLitPoidsLiaisonDansGraph(stTreeInterpretContext *c);
stNodeInterpret *ReseauChangePoidsLiaisonDansGraph(stTreeInterpretContext *c);
stNodeInterpret *NetworkGetTypeOfWP(stTreeInterpretContext *c);

/* Capabilities */
stNodeInterpret *GetCapabilities(stTreeInterpretContext *c);
stNodeInterpret *CapabilityAtBitNumber(stTreeInterpretContext *c);

stNodeInterpret *GetScrollSpeed(stTreeInterpretContext *c);
stNodeInterpret *MTGetScrollSpeed(stTreeInterpretContext *c);
stNodeInterpret *GetNbFrame(stTreeInterpretContext *c);

/* Vector function */
stNodeInterpret *DotProduct(stTreeInterpretContext *c);
stNodeInterpret *CrossProduct(stTreeInterpretContext *c);
stNodeInterpret *Normalize(stTreeInterpretContext *c);

/* Superobject */
stNodeInterpret *GetSPOCoordinates(stTreeInterpretContext *c);
stNodeInterpret *GetSPOSighting(stTreeInterpretContext *c);
stNodeInterpret *GetSPOHorizon(stTreeInterpretContext *c);
stNodeInterpret *GetSPOBanking(stTreeInterpretContext *c);
stNodeInterpret *GetTractionFactor(stTreeInterpretContext *c);

/* Collide object type */
stNodeInterpret *GetCenterZDEType(stTreeInterpretContext *c);
stNodeInterpret *GetCenterZDMType(stTreeInterpretContext *c);
stNodeInterpret *GetCenterZDRType(stTreeInterpretContext *c);
stNodeInterpret *GetCenterZDDType(stTreeInterpretContext *c);

stNodeInterpret *TextAffiche(stTreeInterpretContext *c);
stNodeInterpret *MJTextAffiche(stTreeInterpretContext *c);
stNodeInterpret *GetCPUCounter(stTreeInterpretContext *c);
stNodeInterpret *TurnToPosition(stTreeInterpretContext *c);
stNodeInterpret *FormatMemCard(stTreeInterpretContext *c);
stNodeInterpret *GetDefaultLanguage(stTreeInterpretContext *c);
stNodeInterpret *PS2_MagicBox(stTreeInterpretContext *c);
stNodeInterpret *SAV2_DoVarsNeedToBeSet(stTreeInterpretContext *c);
stNodeInterpret *CINEGetGenerateEvent(stTreeInterpretContext *c);
stNodeInterpret *StringCompare(stTreeInterpretContext *c);
stNodeInterpret *EmptyString(stTreeInterpretContext *c);

/* Actor */
stNodeInterpret *VitesseHorizontaleDuPerso(stTreeInterpretContext *c);
stNodeInterpret *VitesseVerticaleDuPerso(stTreeInterpretContext *c);
stNodeInterpret *GetPersoZoomFactor(stTreeInterpretContext *c);
stNodeInterpret *GetPersoSighting(stTreeInterpretContext *c);
stNodeInterpret *GetPersoHorizon(stTreeInterpretContext *c);
stNodeInterpret *GetPersoBanking(stTreeInterpretContext *c);

/* ZDx */
stNodeInterpret *LitPositionZDM(stTreeInterpretContext *c);
stNodeInterpret *LitPositionZDE(stTreeInterpretContext *c);
stNodeInterpret *LitPositionZDD(stTreeInterpretContext *c);
stNodeInterpret *LitCentreZDM(stTreeInterpretContext *c);
stNodeInterpret *LitCentreZDE(stTreeInterpretContext *c);
stNodeInterpret *LitCentreZDD(stTreeInterpretContext *c);
stNodeInterpret *LitAxeZDM(stTreeInterpretContext *c);
stNodeInterpret *LitAxeZDE(stTreeInterpretContext *c);
stNodeInterpret *LitAxeZDD(stTreeInterpretContext *c);
stNodeInterpret *LitDimensionZDM(stTreeInterpretContext *c);
stNodeInterpret *LitDimensionZDE(stTreeInterpretContext *c);
stNodeInterpret *LitDimensionZDD(stTreeInterpretContext *c);

/* Vector */
stNodeInterpret *VecteurPointAxe(stTreeInterpretContext *c);
stNodeInterpret *VecteurPointSegment(stTreeInterpretContext *c);
stNodeInterpret *VectorContribution(stTreeInterpretContext *c);
stNodeInterpret *VectorCombination(stTreeInterpretContext *c);
stNodeInterpret *TemporalVectorCombination(stTreeInterpretContext *c);
stNodeInterpret *ScaledVector(stTreeInterpretContext *c);
stNodeInterpret *GetVectorNorm(stTreeInterpretContext *c);
stNodeInterpret *RotateVector(stTreeInterpretContext *c);
stNodeInterpret *VectorAngle(stTreeInterpretContext *c);
stNodeInterpret *VectorCos(stTreeInterpretContext *c);
stNodeInterpret *VectorSin(stTreeInterpretContext *c);
stNodeInterpret *GetNormalCollideVector(stTreeInterpretContext *c);
stNodeInterpret *GetNormalSlopeVector(stTreeInterpretContext *c);
stNodeInterpret *GetNormalSlopeVectorAndAngle(stTreeInterpretContext *c);
stNodeInterpret *GetNormalCollideVector2(stTreeInterpretContext *c);
stNodeInterpret *GetCollidePoint(stTreeInterpretContext *c);
stNodeInterpret *GetCollidePoint2(stTreeInterpretContext *c);
stNodeInterpret *GetHandsCollidePoint(stTreeInterpretContext *c);
stNodeInterpret *GetCollideRate(stTreeInterpretContext *c);
stNodeInterpret *GetCollideRate2(stTreeInterpretContext *c);
stNodeInterpret *GetCollideMaterialType(stTreeInterpretContext *c);
stNodeInterpret *GetCollideMaterialType2(stTreeInterpretContext *c);
stNodeInterpret *GetCollideMaterialDirection(stTreeInterpretContext *c);
stNodeInterpret *GetCollideMaterialCoeff(stTreeInterpretContext *c);
stNodeInterpret *GetCollideMaterialTypeForAi(stTreeInterpretContext *c);
stNodeInterpret *GetCollisionPoint(stTreeInterpretContext *c);
stNodeInterpret *GetCollisionVector(stTreeInterpretContext *c);
stNodeInterpret *GetCollisionPerso(stTreeInterpretContext *c);
stNodeInterpret *GetCollisionPointMaterial(stTreeInterpretContext *c);
stNodeInterpret *GetFirstCollisionInfo(stTreeInterpretContext *c);
stNodeInterpret *GetLastTraversedMaterialType(stTreeInterpretContext *c);
stNodeInterpret *GetLastTraversedMaterial(stTreeInterpretContext *c);
stNodeInterpret *GetCurrentCollidedGMT(stTreeInterpretContext *c);
stNodeInterpret *GetColliderType(stTreeInterpretContext *c);
stNodeInterpret *GetColliderActor(stTreeInterpretContext *c);
stNodeInterpret *GetColliderVector(stTreeInterpretContext *c);
stNodeInterpret *GetColliderReal(stTreeInterpretContext *c);
stNodeInterpret *GetLastCollisionActor(stTreeInterpretContext *c);
stNodeInterpret *ComputeRebondVector(stTreeInterpretContext *c);
stNodeInterpret *GetModuleAbsolutePosition(stTreeInterpretContext *c);
stNodeInterpret *GetModuleRelativePosition(stTreeInterpretContext *c);
stNodeInterpret *GetModuleZoomFactor(stTreeInterpretContext *c);
stNodeInterpret *GetModuleSighting(stTreeInterpretContext *c);
stNodeInterpret *GetModuleHorizon(stTreeInterpretContext *c);
stNodeInterpret *GetModuleBanking(stTreeInterpretContext *c);
stNodeInterpret *CastIntegerToChannel(stTreeInterpretContext *c);
stNodeInterpret *GetSlotDate(stTreeInterpretContext *c);
stNodeInterpret *GetSlotName(stTreeInterpretContext *c);
stNodeInterpret *GetSlotScore(stTreeInterpretContext *c);
stNodeInterpret *GetStringCharAt(stTreeInterpretContext *c);
stNodeInterpret *ScanAsciiKey(stTreeInterpretContext *c);
stNodeInterpret *GetFormattedTextInfo(stTreeInterpretContext *c);
stNodeInterpret *GetInputEntryName(stTreeInterpretContext *c);
stNodeInterpret *GetMechanicGravityFactor(stTreeInterpretContext *c);
stNodeInterpret *GetMechanicSlide(stTreeInterpretContext *c);
stNodeInterpret *GetMechanicRebound(stTreeInterpretContext *c);
stNodeInterpret *GetMechanicSlopeLimit(stTreeInterpretContext *c);
stNodeInterpret *GetMechanicInertiaX(stTreeInterpretContext *c);
stNodeInterpret *GetMechanicInertiaY(stTreeInterpretContext *c);
stNodeInterpret *GetMechanicInertiaZ(stTreeInterpretContext *c);
stNodeInterpret *GetMechanicTiltIntensity(stTreeInterpretContext *c);
stNodeInterpret *GetMechanicTiltInertia(stTreeInterpretContext *c);
stNodeInterpret *GetMechanicTiltOrigin(stTreeInterpretContext *c);
stNodeInterpret *GetMechanicMaxSpeed(stTreeInterpretContext *c);
stNodeInterpret *GetMechanicStreamPriority(stTreeInterpretContext *c);
stNodeInterpret *GetMechanicStreamSpeed(stTreeInterpretContext *c);
stNodeInterpret *GetMechanicStreamFactor(stTreeInterpretContext *c);
stNodeInterpret *GetSlideFactorX(stTreeInterpretContext *c);
stNodeInterpret *GetSlideFactorY(stTreeInterpretContext *c);
stNodeInterpret *GetSlideFactorZ(stTreeInterpretContext *c);
stNodeInterpret *JumpImpulsion(stTreeInterpretContext *c);
stNodeInterpret *GetSpeedAnim(stTreeInterpretContext *c);
stNodeInterpret *HierGetFather(stTreeInterpretContext *c);
stNodeInterpret *GetActivationZDD(stTreeInterpretContext *c);
stNodeInterpret *GetActivationZDM(stTreeInterpretContext *c);
stNodeInterpret *GetActivationZDE(stTreeInterpretContext *c);
stNodeInterpret *GetActivationZDR(stTreeInterpretContext *c);
stNodeInterpret *GetCollisionFrequency(stTreeInterpretContext *c);
stNodeInterpret *GetBrainFrequency(stTreeInterpretContext *c);
stNodeInterpret *GetLightFrequency(stTreeInterpretContext *c);
stNodeInterpret *GetBooleanInArray(stTreeInterpretContext *c);
stNodeInterpret *GetNumberOfBooleanInArray(stTreeInterpretContext *c);
stNodeInterpret *GetButtonName(stTreeInterpretContext *c);
stNodeInterpret *GetOneCustomFloat(stTreeInterpretContext *c);
stNodeInterpret *GetOneCustomLong(stTreeInterpretContext *c);
stNodeInterpret *GetOneCustomVector(stTreeInterpretContext *c);
stNodeInterpret *GetDriversAvailable(stTreeInterpretContext *c);
stNodeInterpret *GetCurrentLanguageId(stTreeInterpretContext *c);
stNodeInterpret *GetNbLanguages(stTreeInterpretContext *c);
stNodeInterpret *GetLanguageText(stTreeInterpretContext *c);
stNodeInterpret *TextToInt(stTreeInterpretContext *c);
stNodeInterpret *NbAvailableResolution(stTreeInterpretContext *c);
stNodeInterpret *CurrentResolution(stTreeInterpretContext *c);
stNodeInterpret *SaveResolution(stTreeInterpretContext *c);
stNodeInterpret *IsResolutionAvailable(stTreeInterpretContext *c);
stNodeInterpret *GetBrightness(stTreeInterpretContext *c);
stNodeInterpret *NameResolution(stTreeInterpretContext *c);
stNodeInterpret *GetTextureFiltering(stTreeInterpretContext *c);
stNodeInterpret *GetAntiAliasing(stTreeInterpretContext *c);
stNodeInterpret *GetSaturationDistance(stTreeInterpretContext *c);
stNodeInterpret *GetBackgroundDistance(stTreeInterpretContext *c);
stNodeInterpret *GetTooFarLimit(stTreeInterpretContext *c);
stNodeInterpret *GetTransparencyZoneMin(stTreeInterpretContext *c);
stNodeInterpret *GetTransparencyZoneMax(stTreeInterpretContext *c);
stNodeInterpret *GetCurrentFrame(stTreeInterpretContext *c);
stNodeInterpret *ExecuteVariable(stTreeInterpretContext *c);
stNodeInterpret *ComputeProtectKey(stTreeInterpretContext *c);
stNodeInterpret *Xor(stTreeInterpretContext *c);
stNodeInterpret *And(stTreeInterpretContext *c);
stNodeInterpret *Or(stTreeInterpretContext *c);
stNodeInterpret *Not(stTreeInterpretContext *c);
stNodeInterpret *DivUnsigned(stTreeInterpretContext *c);
stNodeInterpret *MulUnsigned(stTreeInterpretContext *c);
stNodeInterpret *AddUnsigned(stTreeInterpretContext *c);
stNodeInterpret *SubUnsigned(stTreeInterpretContext *c);
stNodeInterpret *GetMemoryValue(stTreeInterpretContext *c);
stNodeInterpret *GetXBoxRegion(stTreeInterpretContext *c);
stNodeInterpret *CreditsBegin(stTreeInterpretContext *c);
stNodeInterpret *CreditsGetChapter(stTreeInterpretContext *c);
stNodeInterpret *CreditsGetPageFromCurrentChapter(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetShiftTarget(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetShiftPos(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetDistMin(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetDistMax(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetBoundDistMin(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetBoundDistMax(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetAngleAlpha(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetAngleShiftAlpha(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetAngleTheta(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetAngleShiftTheta(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetLinearSpeed(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetLinearIncreaseSpeed(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetLinearDecreaseSpeed(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetAngularSpeed(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetAngularIncreaseSpeed(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetAngularDecreaseSpeed(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetTargetSpeed(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetTargetIncreaseSpeed(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetTargetDecreaseSpeed(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetFocal(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetZMin(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetZMax(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetTargetedSuperObject(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetTypeOfViewport(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetCameraOfViewport(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetMainCamera(stTreeInterpretContext *c);
stNodeInterpret *Cam_ComputeTargetWithTgtPerso(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetCurrentTargetPosition(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetSectorCameraType(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetBestPos(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetTransparencyFlag(stTreeInterpretContext *c);
stNodeInterpret *Cam_SetMirrorFlag(stTreeInterpretContext *c);
stNodeInterpret *Cam_GetMirrorFlag(stTreeInterpretContext *c);
stNodeInterpret *CAM_GetViewportCoordinates(stTreeInterpretContext *c);
stNodeInterpret *CSHBackMode(stTreeInterpretContext *c);
stNodeInterpret *CSHMarioMode(stTreeInterpretContext *c);
stNodeInterpret *CSHStrafeLockMode(stTreeInterpretContext *c);
stNodeInterpret *GetViewFinderDistance(stTreeInterpretContext *c);
stNodeInterpret *LinkInit(stTreeInterpretContext *c);
stNodeInterpret *LinkDoBehaviour(stTreeInterpretContext *c);
stNodeInterpret *LinkKill(stTreeInterpretContext *c);
stNodeInterpret *LinkGetNoCutpoints(stTreeInterpretContext *c);
stNodeInterpret *LinkGetCutPoint(stTreeInterpretContext *c);
stNodeInterpret *FollowLinkDirection(stTreeInterpretContext *c);
stNodeInterpret *SND_GetSoundHandle(stTreeInterpretContext *c);
stNodeInterpret *SND_GetSendSoundRequestHandleState(stTreeInterpretContext *c);
stNodeInterpret *SND_GetSendVoiceRequestHandleState(stTreeInterpretContext *c);
stNodeInterpret *SND_GetSendMusicRequestHandleState(stTreeInterpretContext *c);
stNodeInterpret *SND_GetSendAmbianceRequestHandleState(stTreeInterpretContext *c);
stNodeInterpret *SND_GetSendMenuSndRequestHandleState(stTreeInterpretContext *c);
stNodeInterpret *SND_GetSoundVolume(stTreeInterpretContext *c);
stNodeInterpret *SND_GetVoiceVolume(stTreeInterpretContext *c);
stNodeInterpret *SND_GetMusicVolume(stTreeInterpretContext *c);
stNodeInterpret *SND_GetAmbianceVolume(stTreeInterpretContext *c);
stNodeInterpret *SND_GetMenuSndVolume(stTreeInterpretContext *c);
stNodeInterpret *IsSoundPlaying(stTreeInterpretContext *c);
stNodeInterpret *SND_GetMusicMarker(stTreeInterpretContext *c);
stNodeInterpret *MonitoringSetVariable(stTreeInterpretContext *c);
stNodeInterpret *R3GetScore(stTreeInterpretContext *c);
stNodeInterpret *R3GetNbCage(stTreeInterpretContext *c);
stNodeInterpret *PS2DemoGetParameter(stTreeInterpretContext *c);
stNodeInterpret *GetScreenDisplayFrequency(stTreeInterpretContext *c);
stNodeInterpret *GetLastSaveGameOperationResult(stTreeInterpretContext *c);
stNodeInterpret *SinEffect_GetSinusPosition(stTreeInterpretContext *c);
stNodeInterpret *SFX_GetLightReflectionStatus(stTreeInterpretContext *c);
stNodeInterpret *SFX_GetWaterCirclesStatus(stTreeInterpretContext *c);
stNodeInterpret *SFX_IsLightReflectionPossible(stTreeInterpretContext *c);
stNodeInterpret *SFX_IsWaterCirclesPossible(stTreeInterpretContext *c);
stNodeInterpret *AGO_GetHandle(stTreeInterpretContext *c);
stNodeInterpret *IsMcValid(stTreeInterpretContext *c);
stNodeInterpret *FormatMc(stTreeInterpretContext *c);


#pragma mark - Conditions


stNodeInterpret *CollidePersoZDDNoWithPerso(stTreeInterpretContext *c); // 10
stNodeInterpret *CollideModuleZDDNoWithPerso(stTreeInterpretContext *c);
stNodeInterpret *CollidePersoAllZDDWithPerso(stTreeInterpretContext *c);
stNodeInterpret *CollidePersoZDDWithAnyPerso(stTreeInterpretContext *c);
stNodeInterpret *CollideModuleZDDWithAnyPerso(stTreeInterpretContext *c); // 14

stNodeInterpret *CollidePersoZDENoWithPersoZDENo(stTreeInterpretContext *c); // 15
stNodeInterpret *CollideModuleZDENoWithPersoZDENo(stTreeInterpretContext *c);
stNodeInterpret *CollidePersoZDENoWithModuleZDENo(stTreeInterpretContext *c);
stNodeInterpret *CollideModuleZDENoWithModuleZDENo(stTreeInterpretContext *c);
stNodeInterpret *CollidePersoZDENoWithPersoTypeZDE(stTreeInterpretContext *c);
stNodeInterpret *CollideModuleZDENoWithPersoTypeZDE(stTreeInterpretContext *c);
stNodeInterpret *CollidePersoTypeZDEWithPersoTypeZDE(stTreeInterpretContext *c);
stNodeInterpret *CollidePersoAllZDEWithPersoAllZDE(stTreeInterpretContext *c);
stNodeInterpret *CollidePersoTypeZDEWithPersoAllZDE(stTreeInterpretContext *c);
stNodeInterpret *CollidePersoAllZDEWithPersoTypeZDE(stTreeInterpretContext *c);
stNodeInterpret *CollidePersoZDENoWithTypeZDE(stTreeInterpretContext *c);
stNodeInterpret *CollideModuleZDENoWithTypeZDE(stTreeInterpretContext *c); // 26

stNodeInterpret *CollideWithGround(stTreeInterpretContext *c); // 27
stNodeInterpret *CollideWithWall(stTreeInterpretContext *c);
stNodeInterpret *CollideWithNothing(stTreeInterpretContext *c);
stNodeInterpret *CollideWithCeiling(stTreeInterpretContext *c);
stNodeInterpret *CollideWithPerso(stTreeInterpretContext *c);
stNodeInterpret *CollideWithWater(stTreeInterpretContext *c);
stNodeInterpret *CollideWithThisPerso(stTreeInterpretContext *c); // 33

stNodeInterpret *ZDMCollideWithGround(stTreeInterpretContext *c); // 34
stNodeInterpret *ZDMCollideWithWall(stTreeInterpretContext *c);
stNodeInterpret *ZDMCollideWithNothing(stTreeInterpretContext *c);
stNodeInterpret *ZDMCollideWithCeiling(stTreeInterpretContext *c); // 37

stNodeInterpret *IsPersoInList(stTreeInterpretContext *c); // 38
stNodeInterpret *IsModelInList(stTreeInterpretContext *c);
stNodeInterpret *IsFamilyInList(stTreeInterpretContext *c);
stNodeInterpret *ListEmptyTest(stTreeInterpretContext *c); // 41

stNodeInterpret *UserEvent_IsSet(stTreeInterpretContext *c); // 42
stNodeInterpret *UserEvent_IsSet2(stTreeInterpretContext *c);
stNodeInterpret *UserEvent_IsSet3(stTreeInterpretContext *c); // 44

stNodeInterpret *PressedBut(stTreeInterpretContext *c); // 45
stNodeInterpret *JustPressedBut(stTreeInterpretContext *c);
stNodeInterpret *ReleasedBut(stTreeInterpretContext *c);
stNodeInterpret *JustReleasedBut(stTreeInterpretContext *c); // 48

stNodeInterpret *IsTimeElapsed(stTreeInterpretContext *c); // 49

stNodeInterpret *IsValidObject(stTreeInterpretContext *c); // 50
stNodeInterpret *IsValidWayPoint(stTreeInterpretContext *c);
stNodeInterpret *IsValidGMT(stTreeInterpretContext *c);
stNodeInterpret *IsValidVMT(stTreeInterpretContext *c);
stNodeInterpret *IsValidAction(stTreeInterpretContext *c);
stNodeInterpret *IsValidText(stTreeInterpretContext *c);
stNodeInterpret *IsValidSPO(stTreeInterpretContext *c);
stNodeInterpret *IsValidGraph(stTreeInterpretContext *c); // 57

stNodeInterpret *SeePerso(stTreeInterpretContext *c); // 58
stNodeInterpret *IsActivable(stTreeInterpretContext *c); // 59
stNodeInterpret *IsAlreadyHandled(stTreeInterpretContext *c); // 60
stNodeInterpret *Alw_IsMine(stTreeInterpretContext *c); // 61

stNodeInterpret *IsPersoLightOn(stTreeInterpretContext *c); // 62
stNodeInterpret *IsPersoLightPulseOn(stTreeInterpretContext *c);
stNodeInterpret *IsPersoLightGyroPhareOn(stTreeInterpretContext *c); // 64

stNodeInterpret *IsRLITransitionInProgress(stTreeInterpretContext *c); // 65
stNodeInterpret *IsInAlwaysActiveList(stTreeInterpretContext *c); // 66
stNodeInterpret *IsAlwaysActive(stTreeInterpretContext *c); // 67
stNodeInterpret *IsAnActivePad(stTreeInterpretContext *c); // 68
stNodeInterpret *IsMultitap(stTreeInterpretContext *c); // 69
stNodeInterpret *SAV2_IsValid(stTreeInterpretContext *c); // 70
stNodeInterpret *IsWidescreen(stTreeInterpretContext *c); // 71
stNodeInterpret *EngineIsInPAL(stTreeInterpretContext *c); // 72

stNodeInterpret *IsZDMCollideWithObstacle(stTreeInterpretContext *c); // 73
stNodeInterpret *IsZDMCollideWithWall(stTreeInterpretContext *c);
stNodeInterpret *IsZDMCollideWithGround(stTreeInterpretContext *c);
stNodeInterpret *IsZDMCollideWithCeiling(stTreeInterpretContext *c); // 76

stNodeInterpret *CmtIdentifierContainsMask(stTreeInterpretContext *c); // 77
stNodeInterpret *HitByCollider(stTreeInterpretContext *c); // 78
stNodeInterpret *IsTypeOfGMTCollide(stTreeInterpretContext *c);
stNodeInterpret *IsInComport(stTreeInterpretContext *c);
stNodeInterpret *IsInReflexComport(stTreeInterpretContext *c);
stNodeInterpret *IsInAction(stTreeInterpretContext *c);
stNodeInterpret *ChangeActionEnable(stTreeInterpretContext *c); // 83

stNodeInterpret *EngineReinitRequested(stTreeInterpretContext *c); // 84
stNodeInterpret *IsThereMechEvent(stTreeInterpretContext *c); // 85
stNodeInterpret *CollisionWP(stTreeInterpretContext *c); // 86
stNodeInterpret *IsCustomBitSet(stTreeInterpretContext *c);
stNodeInterpret *IsPersoActive(stTreeInterpretContext *c);
stNodeInterpret *CheckActionEnd(stTreeInterpretContext *c);
stNodeInterpret *IsCurrentStateCustomBitSet(stTreeInterpretContext *c);
stNodeInterpret *IsGiBlock(stTreeInterpretContext *c);

stNodeInterpret *IsMechanicBlock(stTreeInterpretContext *c);
stNodeInterpret *IsMechanicAnimation(stTreeInterpretContext *c);
stNodeInterpret *IsMechanicCollide(stTreeInterpretContext *c);
stNodeInterpret *IsMechanicGravity(stTreeInterpretContext *c);
stNodeInterpret *IsMechanicTilt(stTreeInterpretContext *c);
stNodeInterpret *IsMechanicGi(stTreeInterpretContext *c);
stNodeInterpret *IsMechanicClimb(stTreeInterpretContext *c);
stNodeInterpret *IsMechanicOnGround(stTreeInterpretContext *c);
stNodeInterpret *IsMechanicSpider(stTreeInterpretContext *c);
stNodeInterpret *IsMechanicShoot(stTreeInterpretContext *c);
stNodeInterpret *IsMechanicSwim(stTreeInterpretContext *c);
stNodeInterpret *IsMechanicNeverFall(stTreeInterpretContext *c);
stNodeInterpret *IsMechanicCollisionControl(stTreeInterpretContext *c);
stNodeInterpret *IsMechanicKeepSpeedZ(stTreeInterpretContext *c);
stNodeInterpret *IsMechanicSpeedLimit(stTreeInterpretContext *c);
stNodeInterpret *IsMechanicInertia(stTreeInterpretContext *c);
stNodeInterpret *IsMechanicStream(stTreeInterpretContext *c);
stNodeInterpret *IsMechanicStickOnPlatform(stTreeInterpretContext *c);
stNodeInterpret *IsMechanicPatformCrash(stTreeInterpretContext *c);
stNodeInterpret *IsMechanicScale(stTreeInterpretContext *c);
stNodeInterpret *IsMechanicExec(stTreeInterpretContext *c);

stNodeInterpret *CanFall(stTreeInterpretContext *c);
stNodeInterpret *IsMechanicCrash(stTreeInterpretContext *c);
stNodeInterpret *IsNullVector(stTreeInterpretContext *c);
stNodeInterpret *HierIsSonOfActor(stTreeInterpretContext *c);
stNodeInterpret *IsMorphing(stTreeInterpretContext *c);
stNodeInterpret *CheckAnimEnd(stTreeInterpretContext *c);
stNodeInterpret *CheckAnimSmooth(stTreeInterpretContext *c);
stNodeInterpret *HasTheCapability(stTreeInterpretContext *c);
stNodeInterpret *HasOneOfTheCapabilities(stTreeInterpretContext *c);
stNodeInterpret *HasTheCapabilityNumber(stTreeInterpretContext *c);
stNodeInterpret *PersoHasTheCapability(stTreeInterpretContext *c);
stNodeInterpret *PersoHasOneOfTheCapabilities(stTreeInterpretContext *c);
stNodeInterpret *PersoHasTheCapabilityNumber(stTreeInterpretContext *c);
stNodeInterpret *MagnetIsActivated(stTreeInterpretContext *c);
stNodeInterpret *NEstPasEnCollisionAvecMap(stTreeInterpretContext *c);
stNodeInterpret *NEstPasEnCollisionAvecProjectile(stTreeInterpretContext *c);
stNodeInterpret *NEstPasEnCollisionAvecSecondCharact(stTreeInterpretContext *c);
stNodeInterpret *NEstPasEnCollisionAvecMainCharact(stTreeInterpretContext *c);
stNodeInterpret *NEstPasEnCollisionAvecAutresSecteurs(stTreeInterpretContext *c);

stNodeInterpret *IsInFamily(stTreeInterpretContext *c);
stNodeInterpret *IsInModel(stTreeInterpretContext *c);
stNodeInterpret *AJoypadIsConnected(stTreeInterpretContext *c);
stNodeInterpret *AKeyJustPressed(stTreeInterpretContext *c);
stNodeInterpret *AButtonPadJustPressed(stTreeInterpretContext *c);
stNodeInterpret *IsInDemoMode(stTreeInterpretContext *c);
stNodeInterpret *IsShapnessMax(stTreeInterpretContext *c);
stNodeInterpret *IsTooFar(stTreeInterpretContext *c);
stNodeInterpret *IsSubAnimPlaying(stTreeInterpretContext *c);
stNodeInterpret *TestCBSubAnim(stTreeInterpretContext *c);
stNodeInterpret *IsInSubAnim(stTreeInterpretContext *c);
stNodeInterpret *IsSubAnimNearEnd(stTreeInterpretContext *c);
stNodeInterpret *IsSameSPO(stTreeInterpretContext *c);
stNodeInterpret *PressedPadBut(stTreeInterpretContext *c);
stNodeInterpret *JustPressedPadBut(stTreeInterpretContext *c);
stNodeInterpret *ReleasedPadBut(stTreeInterpretContext *c);
stNodeInterpret *JustReleasedPadBut(stTreeInterpretContext *c);
stNodeInterpret *IsCinePlaying(stTreeInterpretContext *c);
stNodeInterpret *LoadInProgress(stTreeInterpretContext *c);
stNodeInterpret *SAV2LastError(stTreeInterpretContext *c);
stNodeInterpret *CheckMCStatus(stTreeInterpretContext *c);

stNodeInterpret *Cam_IsActive(stTreeInterpretContext *c);
stNodeInterpret *Cam_IsViewportOwner(stTreeInterpretContext *c);
stNodeInterpret *Cam_IsFlagNoDynamicTarget(stTreeInterpretContext *c);
stNodeInterpret *Cam_IsFlagNoAverageMoveTgtPerso(stTreeInterpretContext *c);
stNodeInterpret *Cam_IsFlagNoParseCutAngle(stTreeInterpretContext *c);
stNodeInterpret *Cam_IsFlagNoVisibility(stTreeInterpretContext *c);
stNodeInterpret *Cam_IsFlagNoVisibilityWithDynHie(stTreeInterpretContext *c);
stNodeInterpret *Cam_IsFlagNoDynChangeTheta(stTreeInterpretContext *c);
stNodeInterpret *Cam_IsFlagNoShiftUntilPosReached(stTreeInterpretContext *c);
stNodeInterpret *Cam_IsFlagNoDynSpeed(stTreeInterpretContext *c);
stNodeInterpret *Cam_IsFlagNoLinearParsing(stTreeInterpretContext *c);
stNodeInterpret *Cam_IsFlagNoLinearInertia(stTreeInterpretContext *c);
stNodeInterpret *Cam_IsFlagNoAngularParsing(stTreeInterpretContext *c);
stNodeInterpret *Cam_IsFlagNoAngularInertia(stTreeInterpretContext *c);
stNodeInterpret *Cam_IsFlagNoTargetParsing(stTreeInterpretContext *c);
stNodeInterpret *Cam_IsFlagNoTargetInertia(stTreeInterpretContext *c);
stNodeInterpret *Cam_IsFlagNoObstacle(stTreeInterpretContext *c);
stNodeInterpret *Cam_IsFlagFixedOrientation(stTreeInterpretContext *c);
stNodeInterpret *Cam_IsFlagForcedPosition(stTreeInterpretContext *c);
stNodeInterpret *Cam_IsFlagForcedTarget(stTreeInterpretContext *c);
stNodeInterpret *Cam_IsFlagForcedAxis(stTreeInterpretContext *c);
