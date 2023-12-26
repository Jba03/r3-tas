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

tdstNodeInterpret *fn_AIFunction_GetPersoAbsolutePosition(tdstTreeInterpretContext *c)
{
    tdstNodeInterpret *function = current();
    /* Only modify the node if the actor is valid */
    if ((c->isUltra && c->actorReference) || (!c->isUltra && c->owner))
    {
        tdstVector3D position = fnActorGetPosition(c->isUltra ? c->actorReference : c->owner);
        /* Allocate a new vector */
        
        /* Use the padding fields of the node to assign a system address */
        //pval(function) = (uint64*)position;
    }
    
//    if (c->isUltra)
//    {
//        tdstActor *actor = c->
//    }
//    else
//    {
//        tdstSuperObject *actorSO = fnSuperobe
//        fnSuperobjectGetGlobalMatrix(<#const tdstSuperObject *so#>)
//    }
//    
//    if (c->isUltra)
//    {
//        tdstNodeInterpret *function = current();
//        fnSuperobjectGetGlobalMatrix(<#const tdstSuperObject *so#>)
//        fnActorGetPosition(<#const tdstEngineObject *actor#>)
//    }
//    
    return function;
}

tdstNodeInterpret *GetMyAbsolutePosition(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetAngleAroundZToPerso(tdstTreeInterpretContext *c);

/* Used */
tdstNodeInterpret *DistanceToPerso(tdstTreeInterpretContext *c);

tdstNodeInterpret *DistanceXToPerso(tdstTreeInterpretContext *c);
tdstNodeInterpret *DistanceYToPerso(tdstTreeInterpretContext *c);
tdstNodeInterpret *DistanceZToPerso(tdstTreeInterpretContext *c);
tdstNodeInterpret *DistanceXYToPerso(tdstTreeInterpretContext *c);
tdstNodeInterpret *DistanceXZToPerso(tdstTreeInterpretContext *c);
tdstNodeInterpret *DistanceYZToPerso(tdstTreeInterpretContext *c);
tdstNodeInterpret *DistanceToPersoCenter(tdstTreeInterpretContext *c);
tdstNodeInterpret *DistanceXToPersoCenter(tdstTreeInterpretContext *c);
tdstNodeInterpret *DistanceYToPersoCenter(tdstTreeInterpretContext *c);
tdstNodeInterpret *DistanceZToPersoCenter(tdstTreeInterpretContext *c);
tdstNodeInterpret *DistanceXYToPersoCenter(tdstTreeInterpretContext *c);
tdstNodeInterpret *DistanceXZToPersoCenter(tdstTreeInterpretContext *c);
tdstNodeInterpret *DistanceYZToPersoCenter(tdstTreeInterpretContext *c);

tdstNodeInterpret *GetRadiusWP(tdstTreeInterpretContext *c);
tdstNodeInterpret *CircularInterpolationBetween3WP(tdstTreeInterpretContext *c);
tdstNodeInterpret *BezierBetween3WP(tdstTreeInterpretContext *c);
tdstNodeInterpret *DistanceToWP(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetWPAbsolutePosition(tdstTreeInterpretContext *c);

tdstNodeInterpret *Int(tdstTreeInterpretContext *c)
{
    tdstNodeInterpret *function = current();
    tdstNodeInterpret *expression = op();
    lval(function) = lval(expression);
    return function;
}

tdstNodeInterpret *RandomInt(tdstTreeInterpretContext *c)
{
    tdstNodeInterpret *function = current();
    tdstNodeInterpret *min = op();
    tdstNodeInterpret *max = op();
    
//    int32 v = fnRNDCall(c->globals->rnd, 1, 0, lval(min), lval(max));
//    lval(function) = v;
    
    return function;
}

tdstNodeInterpret *Real(tdstTreeInterpretContext *c);
tdstNodeInterpret *Sinus(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cosinus(tdstTreeInterpretContext *c);
tdstNodeInterpret *Square(tdstTreeInterpretContext *c);
tdstNodeInterpret *SquareRoot(tdstTreeInterpretContext *c);
tdstNodeInterpret *RandomReal(tdstTreeInterpretContext *c);
tdstNodeInterpret *MinimumReal(tdstTreeInterpretContext *c);
tdstNodeInterpret *MaximumReal(tdstTreeInterpretContext *c);
tdstNodeInterpret *DegreeToRadian(tdstTreeInterpretContext *c);
tdstNodeInterpret *RadianToDegree(tdstTreeInterpretContext *c);
tdstNodeInterpret *AbsoluteValue(tdstTreeInterpretContext *c);
tdstNodeInterpret *LimitRealInRange(tdstTreeInterpretContext *c);
tdstNodeInterpret *Sign(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cube(tdstTreeInterpretContext *c);
tdstNodeInterpret *Modulo(tdstTreeInterpretContext *c);
tdstNodeInterpret *TernInf(tdstTreeInterpretContext *c);
tdstNodeInterpret *TernSup(tdstTreeInterpretContext *c);
tdstNodeInterpret *TernEq(tdstTreeInterpretContext *c);
tdstNodeInterpret *TernInfEq(tdstTreeInterpretContext *c);
tdstNodeInterpret *TernSupEq(tdstTreeInterpretContext *c);
tdstNodeInterpret *TernOp(tdstTreeInterpretContext *c);
tdstNodeInterpret *TemporalRealCombination(tdstTreeInterpretContext *c);

tdstNodeInterpret *fn_AIFunction_HitPoints(tdstTreeInterpretContext *c)
{
    tdstNodeInterpret *function = current();
    tdstNodeInterpret *object = op();
    tdstActor *actor = pointer(host_byteorder_32(object->param));
    tdstStandardGameInfo *stdGame = fnActorGetStdGame(actor);
    
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
                    tdstNodeInterpret *add = op();
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
                    tdstNodeInterpret *add = op();
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
tdstNodeInterpret *ListSize(tdstTreeInterpretContext *c);
tdstNodeInterpret *GivePersoInList(tdstTreeInterpretContext *c);

/* Vector */
tdstNodeInterpret *AbsoluteVector(tdstTreeInterpretContext *c);
tdstNodeInterpret *RelativeVector(tdstTreeInterpretContext *c);
/* & space transform */
tdstNodeInterpret *VecteurLocalToGlobal(tdstTreeInterpretContext *c);
tdstNodeInterpret *VecteurGlobalToLocal(tdstTreeInterpretContext *c);

/* Magnet */
tdstNodeInterpret *GetMagnetStrength(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetMagnetFar(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetMagnetNear(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetMagnetDuration(tdstTreeInterpretContext *c);

/* Superobject */
tdstNodeInterpret *SPO_GetDrawFlag(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetSPO_GetCollidedSPO(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetSPO_GetCollidedWallSPO(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetSPO_GetCollidedCeilingSPO(tdstTreeInterpretContext *c);

/* Timer */
tdstNodeInterpret *GetTime(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetElapsedTime(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetDeltaT(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetFrameLength(tdstTreeInterpretContext *c);

/* Joypad */
tdstNodeInterpret *GetFirstActivePad(tdstTreeInterpretContext *c);
tdstNodeInterpret *InputAnalogicValueX(tdstTreeInterpretContext *c);
tdstNodeInterpret *InputRealAnalogicValueX(tdstTreeInterpretContext *c);
tdstNodeInterpret *InputAnalogicValueY(tdstTreeInterpretContext *c);
tdstNodeInterpret *InputRealAnalogicValueY(tdstTreeInterpretContext *c);
tdstNodeInterpret *InputAnalogicValue(tdstTreeInterpretContext *c);
tdstNodeInterpret *InputRealAnalogicValue(tdstTreeInterpretContext *c);
tdstNodeInterpret *VitessePadAnalogique(tdstTreeInterpretContext *c);

tdstNodeInterpret *GenerateObject(tdstTreeInterpretContext *c);
tdstNodeInterpret *CountGeneratedObjects(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetGlobalCounter(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetSubMapId(tdstTreeInterpretContext *c);

/* Color */
tdstNodeInterpret *AddColor(tdstTreeInterpretContext *c);
tdstNodeInterpret *AddRed(tdstTreeInterpretContext *c);
tdstNodeInterpret *AddGreen(tdstTreeInterpretContext *c);
tdstNodeInterpret *AddBlue(tdstTreeInterpretContext *c);
tdstNodeInterpret *AddAlpha(tdstTreeInterpretContext *c);
tdstNodeInterpret *ColorRedGreenBlueAlpha(tdstTreeInterpretContext *c);
tdstNodeInterpret *ColorRedGreenBlue(tdstTreeInterpretContext *c);
tdstNodeInterpret *ColorRed(tdstTreeInterpretContext *c);
tdstNodeInterpret *ColorGreen(tdstTreeInterpretContext *c);
tdstNodeInterpret *ColorBlue(tdstTreeInterpretContext *c);
tdstNodeInterpret *ColorAlpha(tdstTreeInterpretContext *c);

/* Visual material */
tdstNodeInterpret *GetVMTColor(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetVMTSpecularCoef(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetVMTSpecularExponent(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetVMTDiffuseCoef(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetVMTAmbientCoef(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetVMTTextureScrollingCoefU(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetVMTTextureScrollingCoefV(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetVMTFrame(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetVMTNumberOfFrames(tdstTreeInterpretContext *c);

/* Multiplayer */
tdstNodeInterpret *MP_GetNumberOfPlayers(tdstTreeInterpretContext *c);
tdstNodeInterpret *MP_GetMaxNumberOfPlayers(tdstTreeInterpretContext *c);
tdstNodeInterpret *MP_GetViewPortOfPlayer(tdstTreeInterpretContext *c);
tdstNodeInterpret *MP_GetActiveMainActors(tdstTreeInterpretContext *c);

/* Joypad */
tdstNodeInterpret *PAD_GetHorizontalInvertion(tdstTreeInterpretContext *c);
tdstNodeInterpret *PAD_GetVerticalInvertion(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetRLITableIndex(tdstTreeInterpretContext *c);
tdstNodeInterpret *PAD2_GetGlobalVector(tdstTreeInterpretContext *c);
tdstNodeInterpret *PAD2_GetHorizontalAxis(tdstTreeInterpretContext *c);
tdstNodeInterpret *PAD2_GetVerticalAxis(tdstTreeInterpretContext *c);
tdstNodeInterpret *PAD2_GetAnalogForce(tdstTreeInterpretContext *c);
tdstNodeInterpret *PAD2_GetTrueAnalogForce(tdstTreeInterpretContext *c);
tdstNodeInterpret *PAD2_GetRotationAngle(tdstTreeInterpretContext *c);
tdstNodeInterpret *PAD2_GetPadSector(tdstTreeInterpretContext *c);
tdstNodeInterpret *PAD2_GetLongDescValue(tdstTreeInterpretContext *c);

/* Text */
tdstNodeInterpret *TXT_GetNumberOfLines(tdstTreeInterpretContext *c);

/* Input */
tdstNodeInterpret *InputAnalogicValue2(tdstTreeInterpretContext *c);
tdstNodeInterpret *InputAnalogicValueV(tdstTreeInterpretContext *c);
tdstNodeInterpret *InputRealAnalogicValueV(tdstTreeInterpretContext *c);
tdstNodeInterpret *InputAnalogicValueZ(tdstTreeInterpretContext *c);
tdstNodeInterpret *InputRealAnalogicValueZ(tdstTreeInterpretContext *c);

/* Save file */
tdstNodeInterpret *SAV2_GetValue(tdstTreeInterpretContext *c);
tdstNodeInterpret *SAV2_GetValueIndex(tdstTreeInterpretContext *c);
tdstNodeInterpret *SAV2_GetNumberOfExistingSave(tdstTreeInterpretContext *c);
tdstNodeInterpret *SAV2_GetTimeSaveGame(tdstTreeInterpretContext *c);
tdstNodeInterpret *SAV2_GetDateSaveGame(tdstTreeInterpretContext *c);

tdstNodeInterpret *GetTransitionSectorID(tdstTreeInterpretContext *c);
tdstNodeInterpret *FFBPreCreateConstantEffect(tdstTreeInterpretContext *c);
tdstNodeInterpret *FFBCreateAndLaunchConstantEffect(tdstTreeInterpretContext *c);
tdstNodeInterpret *FFBStopEffectWithFade(tdstTreeInterpretContext *c);
tdstNodeInterpret *CreateCheapCharacterList(tdstTreeInterpretContext *c);
tdstNodeInterpret *TestCheapCharacterList(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetClosestCheapCharacter(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetFurthestCheapCharacter(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetReactivateCheapCharacter(tdstTreeInterpretContext *c);
tdstNodeInterpret *LitPointsDeMagie(tdstTreeInterpretContext *c);
tdstNodeInterpret *LitPointsDeMagieMax(tdstTreeInterpretContext *c);
tdstNodeInterpret *AjouteEtLitPointsDeMagie(tdstTreeInterpretContext *c);
tdstNodeInterpret *AjouteEtLitPointsDeMagieMax(tdstTreeInterpretContext *c);
tdstNodeInterpret *EnleveEtLitPointsDeMagie(tdstTreeInterpretContext *c);
tdstNodeInterpret *EnleveEtLitPointsDeMagieMax(tdstTreeInterpretContext *c);
tdstNodeInterpret *LitPointsDair(tdstTreeInterpretContext *c);
tdstNodeInterpret *LitPointsDairMax(tdstTreeInterpretContext *c);
tdstNodeInterpret *AjouteEtLitPointsDair(tdstTreeInterpretContext *c);
tdstNodeInterpret *AjouteEtLitPointsDairMax(tdstTreeInterpretContext *c);
tdstNodeInterpret *EnleveEtLitPointsDair(tdstTreeInterpretContext *c);
tdstNodeInterpret *EnleveEtLitPointsDairMax(tdstTreeInterpretContext *c);
tdstNodeInterpret *PersoLePlusProche(tdstTreeInterpretContext *c);
tdstNodeInterpret *PersoLePlusProcheExtended(tdstTreeInterpretContext *c);
tdstNodeInterpret *PersoLePlusProcheDansSecteurCourant(tdstTreeInterpretContext *c);
tdstNodeInterpret *PersoLePlusProcheDansSecteurCourantExtended(tdstTreeInterpretContext *c);
tdstNodeInterpret *NearerActorInFieldOfVision(tdstTreeInterpretContext *c);
tdstNodeInterpret *NearerActorInFieldOfVisionExtended(tdstTreeInterpretContext *c);
tdstNodeInterpret *NearerActorOfFamilyInFieldOfVision(tdstTreeInterpretContext *c);
tdstNodeInterpret *NearerActorOfFamilyInFieldOfVisionExtended(tdstTreeInterpretContext *c);
tdstNodeInterpret *NearerActorInFieldOfVisionWithOffset(tdstTreeInterpretContext *c);
tdstNodeInterpret *NearerActorInFieldOfVisionWithOffsetExtended(tdstTreeInterpretContext *c);
tdstNodeInterpret *NearerActorOfFamilyInFOVWithOffset(tdstTreeInterpretContext *c);
tdstNodeInterpret *NearerActorOfFamilyInFOVWithOffsetExtended(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetNbActivePerso(tdstTreeInterpretContext *c);
tdstNodeInterpret *CibleLaPlusProche(tdstTreeInterpretContext *c);
tdstNodeInterpret *CibleLaPlusProcheExtended(tdstTreeInterpretContext *c);
tdstNodeInterpret *CibleLaPlusProcheAvecAngles(tdstTreeInterpretContext *c);
tdstNodeInterpret *CibleLaPlusProcheAvecAnglesExtended(tdstTreeInterpretContext *c);

/* Graph */
tdstNodeInterpret *GetCloserNetwork(tdstTreeInterpretContext *c);
tdstNodeInterpret *NetWorkCloserCollisionWP(tdstTreeInterpretContext *c);
tdstNodeInterpret *ReseauWPLePlusProche(tdstTreeInterpretContext *c);
tdstNodeInterpret *ReseauWPLePlusProcheZPoids(tdstTreeInterpretContext *c);
tdstNodeInterpret *NetworkCloserWPOfType(tdstTreeInterpretContext *c);
tdstNodeInterpret *NetworkCloserCollisionWPOfType(tdstTreeInterpretContext *c);
tdstNodeInterpret *NetworkCloserWPOfExactType(tdstTreeInterpretContext *c);
tdstNodeInterpret *ReseauWPLePlusDansLAxe(tdstTreeInterpretContext *c);
tdstNodeInterpret *ReseauWPLePlusDansLAxe2(tdstTreeInterpretContext *c);
tdstNodeInterpret *ReseauWPLePlusProcheAvecAngle(tdstTreeInterpretContext *c);
tdstNodeInterpret *NetworkNextWPWithCapa(tdstTreeInterpretContext *c);
tdstNodeInterpret *NetworkNextWPWithExactCapa(tdstTreeInterpretContext *c);
tdstNodeInterpret *NetworkNextWPWithCapaOptimalWeight(tdstTreeInterpretContext *c);
tdstNodeInterpret *NetworkNextWPOfType(tdstTreeInterpretContext *c);
tdstNodeInterpret *NetworkNextWPOfExactType(tdstTreeInterpretContext *c);
tdstNodeInterpret *NetworkAffectTypeOfConnectedWP(tdstTreeInterpretContext *c);
tdstNodeInterpret *NetworkAffectTypeOfConnectedWPWithCapa(tdstTreeInterpretContext *c);
tdstNodeInterpret *ReseauCheminLePlusCourt(tdstTreeInterpretContext *c);
tdstNodeInterpret *NetworkBuildOrderedPath(tdstTreeInterpretContext *c);
tdstNodeInterpret *NetworkBuildOrderedPathCircular(tdstTreeInterpretContext *c);
tdstNodeInterpret *NetworkAllocateGraphToMSWay(tdstTreeInterpretContext *c);
tdstNodeInterpret *NetworkAllocateGraphToMSWayCircular(tdstTreeInterpretContext *c);
tdstNodeInterpret *NetworkBuildRandomPath(tdstTreeInterpretContext *c);
tdstNodeInterpret *NetworkBuildRandomCircularPath(tdstTreeInterpretContext *c);
tdstNodeInterpret *ReseauLitIndexCourant(tdstTreeInterpretContext *c);
tdstNodeInterpret *ReseauForceIndexCourant(tdstTreeInterpretContext *c);
tdstNodeInterpret *ReseauLitPremierIndex(tdstTreeInterpretContext *c);
tdstNodeInterpret *ReseauLitDernierIndex(tdstTreeInterpretContext *c);
tdstNodeInterpret *ReseauIncrementIndex(tdstTreeInterpretContext *c);
tdstNodeInterpret *ReseauDecrementIndex(tdstTreeInterpretContext *c);
tdstNodeInterpret *ReseauLitWPAIndex(tdstTreeInterpretContext *c);
tdstNodeInterpret *ReseauLitCapaciteLiaisonAIndex(tdstTreeInterpretContext *c);
tdstNodeInterpret *ReseauChangeCapaciteLiaisonAIndex(tdstTreeInterpretContext *c);
tdstNodeInterpret *ReseauLitPoidsLiaisonAIndex(tdstTreeInterpretContext *c);
tdstNodeInterpret *ReseauChangePoidsLiaisonAIndex(tdstTreeInterpretContext *c);
tdstNodeInterpret *NetworkGetIndexOfWPInMSWay(tdstTreeInterpretContext *c);
tdstNodeInterpret *NetworkForceWPToCurrent(tdstTreeInterpretContext *c);
tdstNodeInterpret *NetworkTestTheEnds(tdstTreeInterpretContext *c);
tdstNodeInterpret *ReseauLitCapaciteLiaisonDansGraph(tdstTreeInterpretContext *c);
tdstNodeInterpret *ReseauChangeCapaciteLiaisonDansGraph(tdstTreeInterpretContext *c);
tdstNodeInterpret *ReseauLitPoidsLiaisonDansGraph(tdstTreeInterpretContext *c);
tdstNodeInterpret *ReseauChangePoidsLiaisonDansGraph(tdstTreeInterpretContext *c);
tdstNodeInterpret *NetworkGetTypeOfWP(tdstTreeInterpretContext *c);

/* Capabilities */
tdstNodeInterpret *GetCapabilities(tdstTreeInterpretContext *c);
tdstNodeInterpret *CapabilityAtBitNumber(tdstTreeInterpretContext *c);

tdstNodeInterpret *GetScrollSpeed(tdstTreeInterpretContext *c);
tdstNodeInterpret *MTGetScrollSpeed(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetNbFrame(tdstTreeInterpretContext *c);

/* Vector function */
tdstNodeInterpret *DotProduct(tdstTreeInterpretContext *c);
tdstNodeInterpret *CrossProduct(tdstTreeInterpretContext *c);
tdstNodeInterpret *Normalize(tdstTreeInterpretContext *c);

/* Superobject */
tdstNodeInterpret *GetSPOCoordinates(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetSPOSighting(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetSPOHorizon(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetSPOBanking(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetTractionFactor(tdstTreeInterpretContext *c);

/* Collide object type */
tdstNodeInterpret *GetCenterZDEType(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetCenterZDMType(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetCenterZDRType(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetCenterZDDType(tdstTreeInterpretContext *c);

tdstNodeInterpret *TextAffiche(tdstTreeInterpretContext *c);
tdstNodeInterpret *MJTextAffiche(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetCPUCounter(tdstTreeInterpretContext *c);
tdstNodeInterpret *TurnToPosition(tdstTreeInterpretContext *c);
tdstNodeInterpret *FormatMemCard(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetDefaultLanguage(tdstTreeInterpretContext *c);
tdstNodeInterpret *PS2_MagicBox(tdstTreeInterpretContext *c);
tdstNodeInterpret *SAV2_DoVarsNeedToBeSet(tdstTreeInterpretContext *c);
tdstNodeInterpret *CINEGetGenerateEvent(tdstTreeInterpretContext *c);
tdstNodeInterpret *StringCompare(tdstTreeInterpretContext *c);
tdstNodeInterpret *EmptyString(tdstTreeInterpretContext *c);

/* Actor */
tdstNodeInterpret *VitesseHorizontaleDuPerso(tdstTreeInterpretContext *c);
tdstNodeInterpret *VitesseVerticaleDuPerso(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetPersoZoomFactor(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetPersoSighting(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetPersoHorizon(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetPersoBanking(tdstTreeInterpretContext *c);

/* ZDx */
tdstNodeInterpret *LitPositionZDM(tdstTreeInterpretContext *c);
tdstNodeInterpret *LitPositionZDE(tdstTreeInterpretContext *c);
tdstNodeInterpret *LitPositionZDD(tdstTreeInterpretContext *c);
tdstNodeInterpret *LitCentreZDM(tdstTreeInterpretContext *c);
tdstNodeInterpret *LitCentreZDE(tdstTreeInterpretContext *c);
tdstNodeInterpret *LitCentreZDD(tdstTreeInterpretContext *c);
tdstNodeInterpret *LitAxeZDM(tdstTreeInterpretContext *c);
tdstNodeInterpret *LitAxeZDE(tdstTreeInterpretContext *c);
tdstNodeInterpret *LitAxeZDD(tdstTreeInterpretContext *c);
tdstNodeInterpret *LitDimensionZDM(tdstTreeInterpretContext *c);
tdstNodeInterpret *LitDimensionZDE(tdstTreeInterpretContext *c);
tdstNodeInterpret *LitDimensionZDD(tdstTreeInterpretContext *c);

/* Vector */
tdstNodeInterpret *VecteurPointAxe(tdstTreeInterpretContext *c);
tdstNodeInterpret *VecteurPointSegment(tdstTreeInterpretContext *c);
tdstNodeInterpret *VectorContribution(tdstTreeInterpretContext *c);
tdstNodeInterpret *VectorCombination(tdstTreeInterpretContext *c);
tdstNodeInterpret *TemporalVectorCombination(tdstTreeInterpretContext *c);
tdstNodeInterpret *ScaledVector(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetVectorNorm(tdstTreeInterpretContext *c);
tdstNodeInterpret *RotateVector(tdstTreeInterpretContext *c);
tdstNodeInterpret *VectorAngle(tdstTreeInterpretContext *c);
tdstNodeInterpret *VectorCos(tdstTreeInterpretContext *c);
tdstNodeInterpret *VectorSin(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetNormalCollideVector(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetNormalSlopeVector(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetNormalSlopeVectorAndAngle(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetNormalCollideVector2(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetCollidePoint(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetCollidePoint2(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetHandsCollidePoint(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetCollideRate(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetCollideRate2(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetCollideMaterialType(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetCollideMaterialType2(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetCollideMaterialDirection(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetCollideMaterialCoeff(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetCollideMaterialTypeForAi(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetCollisionPoint(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetCollisionVector(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetCollisionPerso(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetCollisionPointMaterial(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetFirstCollisionInfo(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetLastTraversedMaterialType(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetLastTraversedMaterial(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetCurrentCollidedGMT(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetColliderType(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetColliderActor(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetColliderVector(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetColliderReal(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetLastCollisionActor(tdstTreeInterpretContext *c);
tdstNodeInterpret *ComputeRebondVector(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetModuleAbsolutePosition(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetModuleRelativePosition(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetModuleZoomFactor(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetModuleSighting(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetModuleHorizon(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetModuleBanking(tdstTreeInterpretContext *c);
tdstNodeInterpret *CastIntegerToChannel(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetSlotDate(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetSlotName(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetSlotScore(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetStringCharAt(tdstTreeInterpretContext *c);
tdstNodeInterpret *ScanAsciiKey(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetFormattedTextInfo(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetInputEntryName(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetMechanicGravityFactor(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetMechanicSlide(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetMechanicRebound(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetMechanicSlopeLimit(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetMechanicInertiaX(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetMechanicInertiaY(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetMechanicInertiaZ(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetMechanicTiltIntensity(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetMechanicTiltInertia(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetMechanicTiltOrigin(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetMechanicMaxSpeed(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetMechanicStreamPriority(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetMechanicStreamSpeed(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetMechanicStreamFactor(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetSlideFactorX(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetSlideFactorY(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetSlideFactorZ(tdstTreeInterpretContext *c);
tdstNodeInterpret *JumpImpulsion(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetSpeedAnim(tdstTreeInterpretContext *c);
tdstNodeInterpret *HierGetFather(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetActivationZDD(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetActivationZDM(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetActivationZDE(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetActivationZDR(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetCollisionFrequency(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetBrainFrequency(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetLightFrequency(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetBooleanInArray(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetNumberOfBooleanInArray(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetButtonName(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetOneCustomFloat(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetOneCustomLong(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetOneCustomVector(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetDriversAvailable(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetCurrentLanguageId(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetNbLanguages(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetLanguageText(tdstTreeInterpretContext *c);
tdstNodeInterpret *TextToInt(tdstTreeInterpretContext *c);
tdstNodeInterpret *NbAvailableResolution(tdstTreeInterpretContext *c);
tdstNodeInterpret *CurrentResolution(tdstTreeInterpretContext *c);
tdstNodeInterpret *SaveResolution(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsResolutionAvailable(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetBrightness(tdstTreeInterpretContext *c);
tdstNodeInterpret *NameResolution(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetTextureFiltering(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetAntiAliasing(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetSaturationDistance(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetBackgroundDistance(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetTooFarLimit(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetTransparencyZoneMin(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetTransparencyZoneMax(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetCurrentFrame(tdstTreeInterpretContext *c);
tdstNodeInterpret *ExecuteVariable(tdstTreeInterpretContext *c);
tdstNodeInterpret *ComputeProtectKey(tdstTreeInterpretContext *c);
tdstNodeInterpret *Xor(tdstTreeInterpretContext *c);
tdstNodeInterpret *And(tdstTreeInterpretContext *c);
tdstNodeInterpret *Or(tdstTreeInterpretContext *c);
tdstNodeInterpret *Not(tdstTreeInterpretContext *c);
tdstNodeInterpret *DivUnsigned(tdstTreeInterpretContext *c);
tdstNodeInterpret *MulUnsigned(tdstTreeInterpretContext *c);
tdstNodeInterpret *AddUnsigned(tdstTreeInterpretContext *c);
tdstNodeInterpret *SubUnsigned(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetMemoryValue(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetXBoxRegion(tdstTreeInterpretContext *c);
tdstNodeInterpret *CreditsBegin(tdstTreeInterpretContext *c);
tdstNodeInterpret *CreditsGetChapter(tdstTreeInterpretContext *c);
tdstNodeInterpret *CreditsGetPageFromCurrentChapter(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetShiftTarget(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetShiftPos(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetDistMin(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetDistMax(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetBoundDistMin(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetBoundDistMax(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetAngleAlpha(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetAngleShiftAlpha(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetAngleTheta(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetAngleShiftTheta(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetLinearSpeed(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetLinearIncreaseSpeed(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetLinearDecreaseSpeed(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetAngularSpeed(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetAngularIncreaseSpeed(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetAngularDecreaseSpeed(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetTargetSpeed(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetTargetIncreaseSpeed(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetTargetDecreaseSpeed(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetFocal(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetZMin(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetZMax(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetTargetedSuperObject(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetTypeOfViewport(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetCameraOfViewport(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetMainCamera(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_ComputeTargetWithTgtPerso(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetCurrentTargetPosition(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetSectorCameraType(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetBestPos(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetTransparencyFlag(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_SetMirrorFlag(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_GetMirrorFlag(tdstTreeInterpretContext *c);
tdstNodeInterpret *CAM_GetViewportCoordinates(tdstTreeInterpretContext *c);
tdstNodeInterpret *CSHBackMode(tdstTreeInterpretContext *c);
tdstNodeInterpret *CSHMarioMode(tdstTreeInterpretContext *c);
tdstNodeInterpret *CSHStrafeLockMode(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetViewFinderDistance(tdstTreeInterpretContext *c);
tdstNodeInterpret *LinkInit(tdstTreeInterpretContext *c);
tdstNodeInterpret *LinkDoBehaviour(tdstTreeInterpretContext *c);
tdstNodeInterpret *LinkKill(tdstTreeInterpretContext *c);
tdstNodeInterpret *LinkGetNoCutpoints(tdstTreeInterpretContext *c);
tdstNodeInterpret *LinkGetCutPoint(tdstTreeInterpretContext *c);
tdstNodeInterpret *FollowLinkDirection(tdstTreeInterpretContext *c);
tdstNodeInterpret *SND_GetSoundHandle(tdstTreeInterpretContext *c);
tdstNodeInterpret *SND_GetSendSoundRequestHandleState(tdstTreeInterpretContext *c);
tdstNodeInterpret *SND_GetSendVoiceRequestHandleState(tdstTreeInterpretContext *c);
tdstNodeInterpret *SND_GetSendMusicRequestHandleState(tdstTreeInterpretContext *c);
tdstNodeInterpret *SND_GetSendAmbianceRequestHandleState(tdstTreeInterpretContext *c);
tdstNodeInterpret *SND_GetSendMenuSndRequestHandleState(tdstTreeInterpretContext *c);
tdstNodeInterpret *SND_GetSoundVolume(tdstTreeInterpretContext *c);
tdstNodeInterpret *SND_GetVoiceVolume(tdstTreeInterpretContext *c);
tdstNodeInterpret *SND_GetMusicVolume(tdstTreeInterpretContext *c);
tdstNodeInterpret *SND_GetAmbianceVolume(tdstTreeInterpretContext *c);
tdstNodeInterpret *SND_GetMenuSndVolume(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsSoundPlaying(tdstTreeInterpretContext *c);
tdstNodeInterpret *SND_GetMusicMarker(tdstTreeInterpretContext *c);
tdstNodeInterpret *MonitoringSetVariable(tdstTreeInterpretContext *c);
tdstNodeInterpret *R3GetScore(tdstTreeInterpretContext *c);
tdstNodeInterpret *R3GetNbCage(tdstTreeInterpretContext *c);
tdstNodeInterpret *PS2DemoGetParameter(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetScreenDisplayFrequency(tdstTreeInterpretContext *c);
tdstNodeInterpret *GetLastSaveGameOperationResult(tdstTreeInterpretContext *c);
tdstNodeInterpret *SinEffect_GetSinusPosition(tdstTreeInterpretContext *c);
tdstNodeInterpret *SFX_GetLightReflectionStatus(tdstTreeInterpretContext *c);
tdstNodeInterpret *SFX_GetWaterCirclesStatus(tdstTreeInterpretContext *c);
tdstNodeInterpret *SFX_IsLightReflectionPossible(tdstTreeInterpretContext *c);
tdstNodeInterpret *SFX_IsWaterCirclesPossible(tdstTreeInterpretContext *c);
tdstNodeInterpret *AGO_GetHandle(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsMcValid(tdstTreeInterpretContext *c);
tdstNodeInterpret *FormatMc(tdstTreeInterpretContext *c);


#pragma mark - Conditions


tdstNodeInterpret *CollidePersoZDDNoWithPerso(tdstTreeInterpretContext *c); // 10
tdstNodeInterpret *CollideModuleZDDNoWithPerso(tdstTreeInterpretContext *c);
tdstNodeInterpret *CollidePersoAllZDDWithPerso(tdstTreeInterpretContext *c);
tdstNodeInterpret *CollidePersoZDDWithAnyPerso(tdstTreeInterpretContext *c);
tdstNodeInterpret *CollideModuleZDDWithAnyPerso(tdstTreeInterpretContext *c); // 14

tdstNodeInterpret *CollidePersoZDENoWithPersoZDENo(tdstTreeInterpretContext *c); // 15
tdstNodeInterpret *CollideModuleZDENoWithPersoZDENo(tdstTreeInterpretContext *c);
tdstNodeInterpret *CollidePersoZDENoWithModuleZDENo(tdstTreeInterpretContext *c);
tdstNodeInterpret *CollideModuleZDENoWithModuleZDENo(tdstTreeInterpretContext *c);
tdstNodeInterpret *CollidePersoZDENoWithPersoTypeZDE(tdstTreeInterpretContext *c);
tdstNodeInterpret *CollideModuleZDENoWithPersoTypeZDE(tdstTreeInterpretContext *c);
tdstNodeInterpret *CollidePersoTypeZDEWithPersoTypeZDE(tdstTreeInterpretContext *c);
tdstNodeInterpret *CollidePersoAllZDEWithPersoAllZDE(tdstTreeInterpretContext *c);
tdstNodeInterpret *CollidePersoTypeZDEWithPersoAllZDE(tdstTreeInterpretContext *c);
tdstNodeInterpret *CollidePersoAllZDEWithPersoTypeZDE(tdstTreeInterpretContext *c);
tdstNodeInterpret *CollidePersoZDENoWithTypeZDE(tdstTreeInterpretContext *c);
tdstNodeInterpret *CollideModuleZDENoWithTypeZDE(tdstTreeInterpretContext *c); // 26

tdstNodeInterpret *CollideWithGround(tdstTreeInterpretContext *c); // 27
tdstNodeInterpret *CollideWithWall(tdstTreeInterpretContext *c);
tdstNodeInterpret *CollideWithNothing(tdstTreeInterpretContext *c);
tdstNodeInterpret *CollideWithCeiling(tdstTreeInterpretContext *c);
tdstNodeInterpret *CollideWithPerso(tdstTreeInterpretContext *c);
tdstNodeInterpret *CollideWithWater(tdstTreeInterpretContext *c);
tdstNodeInterpret *CollideWithThisPerso(tdstTreeInterpretContext *c); // 33

tdstNodeInterpret *ZDMCollideWithGround(tdstTreeInterpretContext *c); // 34
tdstNodeInterpret *ZDMCollideWithWall(tdstTreeInterpretContext *c);
tdstNodeInterpret *ZDMCollideWithNothing(tdstTreeInterpretContext *c);
tdstNodeInterpret *ZDMCollideWithCeiling(tdstTreeInterpretContext *c); // 37

tdstNodeInterpret *IsPersoInList(tdstTreeInterpretContext *c); // 38
tdstNodeInterpret *IsModelInList(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsFamilyInList(tdstTreeInterpretContext *c);
tdstNodeInterpret *ListEmptyTest(tdstTreeInterpretContext *c); // 41

tdstNodeInterpret *UserEvent_IsSet(tdstTreeInterpretContext *c); // 42
tdstNodeInterpret *UserEvent_IsSet2(tdstTreeInterpretContext *c);
tdstNodeInterpret *UserEvent_IsSet3(tdstTreeInterpretContext *c); // 44

tdstNodeInterpret *PressedBut(tdstTreeInterpretContext *c); // 45
tdstNodeInterpret *JustPressedBut(tdstTreeInterpretContext *c);
tdstNodeInterpret *ReleasedBut(tdstTreeInterpretContext *c);
tdstNodeInterpret *JustReleasedBut(tdstTreeInterpretContext *c); // 48

tdstNodeInterpret *IsTimeElapsed(tdstTreeInterpretContext *c); // 49

tdstNodeInterpret *IsValidObject(tdstTreeInterpretContext *c); // 50
tdstNodeInterpret *IsValidWayPoint(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsValidGMT(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsValidVMT(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsValidAction(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsValidText(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsValidSPO(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsValidGraph(tdstTreeInterpretContext *c); // 57

tdstNodeInterpret *SeePerso(tdstTreeInterpretContext *c); // 58
tdstNodeInterpret *IsActivable(tdstTreeInterpretContext *c); // 59
tdstNodeInterpret *IsAlreadyHandled(tdstTreeInterpretContext *c); // 60
tdstNodeInterpret *Alw_IsMine(tdstTreeInterpretContext *c); // 61

tdstNodeInterpret *IsPersoLightOn(tdstTreeInterpretContext *c); // 62
tdstNodeInterpret *IsPersoLightPulseOn(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsPersoLightGyroPhareOn(tdstTreeInterpretContext *c); // 64

tdstNodeInterpret *IsRLITransitionInProgress(tdstTreeInterpretContext *c); // 65
tdstNodeInterpret *IsInAlwaysActiveList(tdstTreeInterpretContext *c); // 66
tdstNodeInterpret *IsAlwaysActive(tdstTreeInterpretContext *c); // 67
tdstNodeInterpret *IsAnActivePad(tdstTreeInterpretContext *c); // 68
tdstNodeInterpret *IsMultitap(tdstTreeInterpretContext *c); // 69
tdstNodeInterpret *SAV2_IsValid(tdstTreeInterpretContext *c); // 70
tdstNodeInterpret *IsWidescreen(tdstTreeInterpretContext *c); // 71
tdstNodeInterpret *EngineIsInPAL(tdstTreeInterpretContext *c); // 72

tdstNodeInterpret *IsZDMCollideWithObstacle(tdstTreeInterpretContext *c); // 73
tdstNodeInterpret *IsZDMCollideWithWall(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsZDMCollideWithGround(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsZDMCollideWithCeiling(tdstTreeInterpretContext *c); // 76

tdstNodeInterpret *CmtIdentifierContainsMask(tdstTreeInterpretContext *c); // 77
tdstNodeInterpret *HitByCollider(tdstTreeInterpretContext *c); // 78
tdstNodeInterpret *IsTypeOfGMTCollide(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsInComport(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsInReflexComport(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsInAction(tdstTreeInterpretContext *c);
tdstNodeInterpret *ChangeActionEnable(tdstTreeInterpretContext *c); // 83

tdstNodeInterpret *EngineReinitRequested(tdstTreeInterpretContext *c); // 84
tdstNodeInterpret *IsThereMechEvent(tdstTreeInterpretContext *c); // 85
tdstNodeInterpret *CollisionWP(tdstTreeInterpretContext *c); // 86
tdstNodeInterpret *IsCustomBitSet(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsPersoActive(tdstTreeInterpretContext *c);
tdstNodeInterpret *CheckActionEnd(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsCurrentStateCustomBitSet(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsGiBlock(tdstTreeInterpretContext *c);

tdstNodeInterpret *IsMechanicBlock(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsMechanicAnimation(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsMechanicCollide(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsMechanicGravity(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsMechanicTilt(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsMechanicGi(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsMechanicClimb(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsMechanicOnGround(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsMechanicSpider(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsMechanicShoot(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsMechanicSwim(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsMechanicNeverFall(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsMechanicCollisionControl(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsMechanicKeepSpeedZ(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsMechanicSpeedLimit(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsMechanicInertia(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsMechanicStream(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsMechanicStickOnPlatform(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsMechanicPatformCrash(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsMechanicScale(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsMechanicExec(tdstTreeInterpretContext *c);

tdstNodeInterpret *CanFall(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsMechanicCrash(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsNullVector(tdstTreeInterpretContext *c);
tdstNodeInterpret *HierIsSonOfActor(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsMorphing(tdstTreeInterpretContext *c);
tdstNodeInterpret *CheckAnimEnd(tdstTreeInterpretContext *c);
tdstNodeInterpret *CheckAnimSmooth(tdstTreeInterpretContext *c);
tdstNodeInterpret *HasTheCapability(tdstTreeInterpretContext *c);
tdstNodeInterpret *HasOneOfTheCapabilities(tdstTreeInterpretContext *c);
tdstNodeInterpret *HasTheCapabilityNumber(tdstTreeInterpretContext *c);
tdstNodeInterpret *PersoHasTheCapability(tdstTreeInterpretContext *c);
tdstNodeInterpret *PersoHasOneOfTheCapabilities(tdstTreeInterpretContext *c);
tdstNodeInterpret *PersoHasTheCapabilityNumber(tdstTreeInterpretContext *c);
tdstNodeInterpret *MagnetIsActivated(tdstTreeInterpretContext *c);
tdstNodeInterpret *NEstPasEnCollisionAvecMap(tdstTreeInterpretContext *c);
tdstNodeInterpret *NEstPasEnCollisionAvecProjectile(tdstTreeInterpretContext *c);
tdstNodeInterpret *NEstPasEnCollisionAvecSecondCharact(tdstTreeInterpretContext *c);
tdstNodeInterpret *NEstPasEnCollisionAvecMainCharact(tdstTreeInterpretContext *c);
tdstNodeInterpret *NEstPasEnCollisionAvecAutresSecteurs(tdstTreeInterpretContext *c);

tdstNodeInterpret *IsInFamily(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsInModel(tdstTreeInterpretContext *c);
tdstNodeInterpret *AJoypadIsConnected(tdstTreeInterpretContext *c);
tdstNodeInterpret *AKeyJustPressed(tdstTreeInterpretContext *c);
tdstNodeInterpret *AButtonPadJustPressed(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsInDemoMode(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsShapnessMax(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsTooFar(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsSubAnimPlaying(tdstTreeInterpretContext *c);
tdstNodeInterpret *TestCBSubAnim(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsInSubAnim(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsSubAnimNearEnd(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsSameSPO(tdstTreeInterpretContext *c);
tdstNodeInterpret *PressedPadBut(tdstTreeInterpretContext *c);
tdstNodeInterpret *JustPressedPadBut(tdstTreeInterpretContext *c);
tdstNodeInterpret *ReleasedPadBut(tdstTreeInterpretContext *c);
tdstNodeInterpret *JustReleasedPadBut(tdstTreeInterpretContext *c);
tdstNodeInterpret *IsCinePlaying(tdstTreeInterpretContext *c);
tdstNodeInterpret *LoadInProgress(tdstTreeInterpretContext *c);
tdstNodeInterpret *SAV2LastError(tdstTreeInterpretContext *c);
tdstNodeInterpret *CheckMCStatus(tdstTreeInterpretContext *c);

tdstNodeInterpret *Cam_IsActive(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_IsViewportOwner(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_IsFlagNoDynamicTarget(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_IsFlagNoAverageMoveTgtPerso(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_IsFlagNoParseCutAngle(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_IsFlagNoVisibility(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_IsFlagNoVisibilityWithDynHie(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_IsFlagNoDynChangeTheta(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_IsFlagNoShiftUntilPosReached(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_IsFlagNoDynSpeed(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_IsFlagNoLinearParsing(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_IsFlagNoLinearInertia(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_IsFlagNoAngularParsing(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_IsFlagNoAngularInertia(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_IsFlagNoTargetParsing(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_IsFlagNoTargetInertia(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_IsFlagNoObstacle(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_IsFlagFixedOrientation(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_IsFlagForcedPosition(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_IsFlagForcedTarget(tdstTreeInterpretContext *c);
tdstNodeInterpret *Cam_IsFlagForcedAxis(tdstTreeInterpretContext *c);
