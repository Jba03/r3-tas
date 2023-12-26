//
//  constants.h
//  r3-tas-memorymap
//
//  Created by Jba03 on 2023-11-15.
//

#ifndef constants_h
#define constants_h

#pragma mark - Pointers

/* GCN */
#define GCN_POINTER_ENGINE_STRUCTURE        0x803E7C0C
#define GCN_POINTER_INPUT_STRUCTURE         0x8042F5A8
#define GCN_POINTER_FIX                     0x804334CC
#define GCN_POINTER_LVL                     0x804334D0
#define GCN_POINTER_RND                     0x80436924
#define GCN_POINTER_GHOST_MODE              0x805D8580
#define GCN_POINTER_INACTIVE_DYNAMIC_WORLD  0x805D8594
#define GCN_POINTER_FATHER_SECTOR           0x805D8598
#define GCN_POINTER_DYNAMIC_WORLD           0x805D859C
#define GCN_POINTER_ACTUAL_WORLD            0x805D85A0
#define GCN_POINTER_MENU_SELECTION_V        0x805D884C
#define GCN_POINTER_MENU_RUMBLEPAD          0x805D89B0

#pragma mark - DNM

/* dynamics->base.flags */
#define dynamicsFlagAnimation                 (1 <<  0) // Use animation speed?
#define dynamicsFlagCollide                   (1 <<  1) // Geometry collision enabled?
#define dynamicsFlagGravity                   (1 <<  2) // Is gravity used?
#define dynamicsFlagTilt                      (1 <<  3) // ?
#define dynamicsFlagUnknown                   (1 <<  4) // ?
#define dynamicsFlagGrounded                  (1 <<  5) // Is on ground
#define dynamicsFlagClimb                     (1 <<  6) // Climbing
#define dynamicsFlagCollideDynamicsParam      (1 <<  7) // Use dynamics param when colliding?
#define dynamicsFlagCollidePreserveZMomentum  (1 <<  8) // Preseve Z-axis momentum when colliding with a wall
#define dynamicsFlagSpeedLimit                (1 <<  9) // Limit speed
#define dynamicsFlagInertia                   (1 << 10) // Has inertia?
#define dynamicsFlagStream                    (1 << 11) // Is affected by a stream?
#define dynamicsFlagStuckToPlatform           (1 << 12) // No slide on platform
#define dynamicsFlagHasScale                  (1 << 13) // Use scale parameters
#define dynamicsFlagSpeedImposeAbsolute       (1 << 14) // Impose absolute speed
#define dynamicsFlagSpeedProposeAbsolute      (1 << 15) // Propose absolute speed
#define dynamicsFlagSpeedAddAbsolute          (1 << 16) // Add absolute speed
#define dynamicsFlagSpeedImposeX              (1 << 17) // Impose absolute X-speed
#define dynamicsFlagSpeedImposeY              (1 << 18) // Impose absolute Y-speed
#define dynamicsFlagSpeedImposeZ              (1 << 19) // Impose absolute Z-speed
#define dynamicsFlagSpeedProposeX             (1 << 20) // Propose absolute X-speed
#define dynamicsFlagSpeedProposeY             (1 << 21) // Propose absolute Y-speed
#define dynamicsFlagSpeedProposeZ             (1 << 22) // Propose absolute Z-speed
#define dynamicsFlagSpeedAddX                 (1 << 23) // Add absolute X-speed
#define dynamicsFlagSpeedAddY                 (1 << 24) // Add absolute Y-speed
#define dynamicsFlagSpeedAddZ                 (1 << 25) // Add absolute Z-speed
#define dynamicsFlagLimitX                    (1 << 26) // Limit X (position?)
#define dynamicsFlagLimitY                    (1 << 27) // Limit Y (position?)
#define dynamicsFlagLimitZ                    (1 << 28) // Limit Z (position?)
#define dynamicsFlagRotationImpose            (1 << 29) // Impose axis rotation
#define dynamicsFlagPlatformLock              (1 << 30) // ?
#define dynamicsFlagTranslationImpose         (1 << 31) // Impose translation

/* dynamics->base.endFlags */
#define dynamicsSizeBase              (1 <<  0)
#define dynamicsSizeAdvanced          (1 <<  1)
#define dynamicsSizeComplex           (1 <<  2)
#define dynamicsReserved              (1 <<  3)
#define dynamicsMechanicsChanged      (1 <<  4)
#define dynamicsPlatformCrash         (1 <<  5)
#define dynamicsCanFall               (1 <<  6)
#define dynamicsIsInit                (1 <<  7)
#define dynamicsSpiderMechanic        (1 <<  8)
#define dynamicsIsShoot               (1 <<  9)
#define dynamicsSafeValid             (1 << 10)
#define dynamicsComputeInvertMatrix   (1 << 11)
#define dynamicsChangeScale           (1 << 12)
#define dynamicsExec                  (1 << 13)
#define dynamicsCollisionReport       (1 << 14)
#define dynamicsNoGravity             (1 << 15)
#define dynamicsStop                  (1 << 16)
#define dynamicsSlidingGround         (1 << 17)
#define dynamicsAlways                (1 << 18)
#define dynamicsCrash                 (1 << 19)
#define dynamicsSwim                  (1 << 20)
#define dynamicsNeverFall             (1 << 21)
#define dynamicsHanging               (1 << 22)
#define dynamicsWallAdjust            (1 << 23)
#define dynamicsActorMove             (1 << 24)
#define dynamicsForceSafeWalk         (1 << 25)
#define dynamicsDontUseNewMechanic    (1 << 26)

/* Dynamics obstacle surface state */
#define dynamicsObstacleNone      (0 <<  0)
#define dynamicsObstacleGround    (1 <<  0)
#define dynamicsObstacleWall      (1 <<  2)
#define dynamicsObstacleCeiling   (1 <<  4)
#define dynamicsObstacleWater     (1 <<  6)
#define dynamicsObstacleError     (1 << 31)

#pragma mark - IPT

#define IPT_E_Entry_FlyingModeHorizontalMouseAnalog 0
#define IPT_E_Entry_FlyingModeVerticalMouseAnalog 1
#define IPT_E_Entry_Action_Pad_AxeX 2
#define IPT_E_Entry_Action_Pad_AxeY 3
#define IPT_E_Entry_Action_Clavier_Haut 4
#define IPT_E_Entry_Action_Clavier_Bas 5
#define IPT_E_Entry_Action_Clavier_Droite 6
#define IPT_E_Entry_Action_Clavier_Gauche 7
#define IPT_E_Entry_Action_Clavier_Ralentir 8
#define IPT_E_Entry_Action_ZoomPlus 9
#define IPT_E_Entry_Action_ZoomMoins 10
#define IPT_E_Entry_Action_LookBendLeft 11
#define IPT_E_Entry_Action_LookBendRight 12
#define IPT_E_Entry_Action_Razorfly 13
#define IPT_E_Entry_Action_Sauter 14
#define IPT_E_Entry_Action_Helico 15
#define IPT_E_Entry_Action_Nage_Remonter 16
#define IPT_E_Entry_Action_Tirer 17
#define IPT_E_Entry_Action_WithdrawFromTakePhotoMode 18
#define IPT_E_Entry_Action_Bonus_B 19
#define IPT_E_Entry_Action_Strafe 20
#define IPT_E_Entry_Action_Baisser 21
#define IPT_E_Entry_Action_Nage_Plonger 22
#define IPT_E_Entry_Action_Lock 23
#define IPT_E_Entry_Action_Affiche_Jauge 24
#define IPT_E_Entry_Action_TirerGenerique 25
#define IPT_E_Entry_Action_TirerGauche 26
#define IPT_E_Entry_Action_TirerDroite 27
#define IPT_E_Entry_Action_Objet 28
#define IPT_E_Entry_Button_Start_Pad 29
#define IPT_E_Entry_Button_Start 30
#define IPT_E_Entry_Action_Menu_Entrer 31
#define IPT_E_Entry_Action_Menu_Annuler 32
#define IPT_E_Entry_Action_Menu_Valider 33
#define IPT_E_Entry_Action_Camera_Look 34
#define IPT_E_Entry_Action_Camera_Cut 35
#define IPT_E_Entry_Action_CouperCinematique 36
#define IPT_E_Entry_Action_TexteSuivant 37
#define IPT_E_Entry_Action_AppelerMurphy 38
#define IPT_E_Entry_Action_TransePolochus 39
#define IPT_E_Entry_Action_Affiche2_Jauge 40
#define IPT_E_Entry_Action_BebeGlobox_1 41
#define IPT_E_Entry_Action_BebeGlobox_2 42
#define IPT_E_Entry_Action_StartCourseBonux 43
#define IPT_E_Entry_Action_CamDebug 44
#define IPT_E_Entry_Action_CamDbgMoveLeft 45
#define IPT_E_Entry_Action_CamDbgMoveRight 46
#define IPT_E_Entry_Action_CamDbgMoveUp 47
#define IPT_E_Entry_Action_CamDbgMoveDown 48
#define IPT_E_Entry_Action_CamDbgIncrease 49
#define IPT_E_Entry_Action_CamDbgDecrease 50
#define IPT_E_Entry_Action_CamDbgFullScreen 51
#define IPT_E_Entry_Action_CamDbgMinimize 52
#define IPT_E_Entry_Action_CamDbgNewWindow 53
#define IPT_E_Entry_Action_CamDbgCycleWindow 54
#define IPT_E_Entry_Action_BrouillardActive 55
#define IPT_E_Entry_Action_BrouillardRouge 56
#define IPT_E_Entry_Action_BrouillardVert 57
#define IPT_E_Entry_Action_BrouillardBleu 58
#define IPT_E_Entry_Action_BrouillardIncrease 59
#define IPT_E_Entry_Action_BrouillardDecrease 60
#define IPT_E_Entry_Action_BrouillardBlendNear 61
#define IPT_E_Entry_Action_BrouillardBlendFar 62
#define IPT_E_Entry_Action_BrouillardDNear 63
#define IPT_E_Entry_Action_BrouillardDFar 64
#define IPT_E_Entry_Action_Quake 65
#define IPT_E_Entry_Action_Camera_TourneDroite 66
#define IPT_E_Entry_Action_Camera_TourneGauche 67
#define IPT_E_Entry_Action_Camera_TourneUp 68
#define IPT_E_Entry_Action_Camera_TourneDown 69
#define IPT_E_Entry_Action_Pad0_AxeX 70
#define IPT_E_Entry_Action_Pad0_AxeY 71
#define IPT_E_Entry_Action_Pad0_AxeV 72
#define IPT_E_Entry_Action_Pad0_AxeZ 73
#define IPT_E_Entry_Action_Pad1_AxeX 74
#define IPT_E_Entry_Action_Pad1_AxeY 75
#define IPT_E_Entry_Action_Pad2_AxeX 76
#define IPT_E_Entry_Action_Pad2_AxeY 77
#define IPT_E_Entry_Action_Pad3_AxeX 78
#define IPT_E_Entry_Action_Pad3_AxeY 79
#define IPT_E_Entry_Action_DbgReset 80
#define IPT_E_Entry_Action_DbgDown 81
#define IPT_E_Entry_Action_DbgYes 82
#define IPT_E_Entry_Action_DbgMoveLeft 83
#define IPT_E_Entry_Action_DbgMoveRight 84
#define IPT_E_Entry_Action_DbgMoveUp 85
#define IPT_E_Entry_Action_DbgMoveDown 86
#define IPT_E_Entry_Action_DbgIncrease 87
#define IPT_E_Entry_Action_DbgDecrease 88
#define IPT_E_Entry_Action_DbgFullScreen 89
#define IPT_E_Entry_Action_DbgUp 90
#define IPT_E_Entry_Action_DbgNextEffect 91
#define IPT_E_Entry_Action_Dbg 92
#define IPT_E_Entry_Action_DbgTrois 93
#define IPT_E_Entry_Action_DbgSept 94
#define IPT_E_Entry_Action_DbgNeuf 95
#define IPT_E_Entry_Action_DbgChangeEffect 96
#define IPT_E_Entry_PS2EnterSelectMode 97
#define IPT_E_Entry_PS2ChangeMap 98
#define IPT_E_Entry_PS2CaptureScreen 99
#define IPT_E_Entry_Action_F1 100
#define IPT_E_Entry_Action_F3 101
#define IPT_E_Entry_Action_F4 102
#define IPT_E_Entry_Action_F5 103
#define IPT_E_Entry_Action_F6 104
#define IPT_E_Entry_Action_F7 105
#define IPT_E_Entry_Action_F12 106
#define IPT_E_Entry_Action_TakePicture 107
#define IPT_E_Entry_Action_CrossRight 108
#define IPT_E_Entry_Action_CrossLeft 109
#define IPT_E_Entry_Action_CrossUp 110
#define IPT_E_Entry_Action_CrossDown 111
#define IPT_E_Entry_Action_Sauter2 112
#define IPT_E_Entry_Action_Tirer2 113
#define IPT_E_Entry_Action_Strafe2 114
#define IPT_E_Entry_Action_Baisser2 115
#define IPT_E_Entry_CheatMurfy 116
#define IPT_E_Entry_CheatElix 117
#define IPT_E_Entry_CheatVignet 118
#define IPT_E_Entry_CheatMorb20 119
#define IPT_E_Entry_CheatGrappin 120
#define IPT_E_Entry_CheatAccumulation 121
#define IPT_E_Entry_CheatVie 122
#define IPT_E_Entry_CheatLumz 123
#define IPT_E_Entry_CheatCine 124
#define IPT_E_Entry_CheatSpec 125
#define IPT_E_Entry_CheatShoot 126
#define IPT_E_Entry_Action_Debug 127
#define IPT_E_Entry_Action_Freeze 128
#define IPT_E_Entry_Action_Suivant 129
#define IPT_E_Entry_CheatMurfy2 130 /* */
#define IPT_E_Entry_Select_Pad 131
#define IPT_E_Entry_SpeedUp_Key 132
#define IPT_E_Entry_Jump_Key 133
#define IPT_E_Entry_LateralWalk_Key 134
#define IPT_E_Entry_Paf_Key 135
#define IPT_E_Entry_Jump_Pad 136
#define IPT_E_Entry_LateralWalk_Pad 137
#define IPT_E_Entry_Paf_Pad 138
#define IPT_E_Entry_CamFollow_Pad 139
#define IPT_E_Entry_Button_L 140
#define IPT_E_Entry_Button_Start2 141 /* */
#define IPT_E_Entry_Button_PreviousMenu 142
#define IPT_E_Entry_Button_Selection 143
#define IPT_E_Entry_Button_XAnalogic 144
#define IPT_E_Entry_Button_YAnalogic 145
#define IPT_E_Entry_Button_LeftAnalogic 146
#define IPT_E_Entry_Button_RightAnalogic 147
#define IPT_E_Entry_Button_UpAnalogic 148
#define IPT_E_Entry_Button_DownAnalogic 149
#define IPT_E_Entry_Button_Paf 150
#define IPT_E_Entry_Button_Jump 151
#define IPT_E_Entry_Button_LateralWalk 152
#define IPT_E_Entry_JoystickMoveOrder 153
#define IPT_E_Entry_PAD_BoutonDroit 154
#define IPT_E_Entry_Button_1 155
#define IPT_E_Entry_Button_2 156
#define IPT_E_Entry_Button_Left 157
#define IPT_E_Entry_Button_Right 158
#define IPT_E_Entry_Button_Up 159
#define IPT_E_Entry_Button_Down 160
#define IPT_E_Entry_Button_Help 161
#define IPT_E_Entry_Button_SpeedUp 162
#define IPT_E_Entry_KeyboardMoveOrder 163
#define IPT_E_Entry_MoveLeft 164
#define IPT_E_Entry_MoveRight 165
#define IPT_E_Entry_MoveUp 166
#define IPT_E_Entry_MoveDown 167
#define IPT_E_Entry_CamLeft_PAD 168
#define IPT_E_Entry_CamRight_PAD 169
#define IPT_E_Entry_BT_Camera_Look 170
#define IPT_E_Entry_BT_Camera_LookRight 171
#define IPT_E_Entry_BT_Camera_LookLeft 172
#define IPT_E_Entry_BT_Camera_LookUp 173
#define IPT_E_Entry_BT_Camera_LookDown 174
#define IPT_E_Entry_Cam_Camera_TurnLeft 175
#define IPT_E_Entry_Cam_Camera_TurnRight 176
#define IPT_E_Entry_Cam_Camera_CutBehind 177
#define IPT_E_Entry_Cam_Camera_SwitchMode 178
#define IPT_E_Entry_Cam_PAD_C 179
#define IPT_E_Entry_Cam_PAD_D 180
#define IPT_E_Entry_Cam_Camera_Quake 181
#define IPT_E_Entry_BT_Camera_Num1 182
#define IPT_E_Entry_BT_Camera_Num2 183
#define IPT_E_Entry_BT_Camera_Num6 184
#define IPT_E_Entry_BT_Camera_Num7 185
#define IPT_E_Entry_BT_Camera_Num8 186
#define IPT_E_Entry_BT_Camera_Num9 187
#define IPT_E_Entry_BT_Camera_IncDist 188
#define IPT_E_Entry_BT_Camera_DecDist 189
#define IPT_E_Entry_ReinitTheMap 190
#define IPT_E_Entry_ChangeLevelTest 191
#define IPT_E_Entry_FlyingMode 192
#define IPT_E_Entry_FlyingModeForward 193
#define IPT_E_Entry_FlyingModeBackward 194
#define IPT_E_Entry_FlyingModeRight 195
#define IPT_E_Entry_FlyingModeLeft 196
#define IPT_E_Entry_FlyingModeUp 197
#define IPT_E_Entry_FlyingModeDown 198
#define IPT_E_Entry_FlyingModeStrafeRight 199
#define IPT_E_Entry_FlyingModeStrafeLeft 200
#define IPT_E_Entry_Raster 201
#define IPT_E_Entry_FlyingModeDoubleSpeed 202
#define IPT_E_Entry_SoftReset 203
#define IPT_E_Entry_DemoRecord 204
#define IPT_E_Entry_DemoInitRecord 205
#define IPT_E_Entry_DemoSave 206
#define IPT_E_Entry_DemoLoad 207
#define IPT_E_Entry_DemoLaunch 208
#define IPT_E_Entry_EngineMenu_Display 209
#define IPT_E_Entry_EngineMenu_Enter 210
#define IPT_E_Entry_EngineMenu_Up 211
#define IPT_E_Entry_EngineMenu_Down 212
#define IPT_E_Entry_EngineMenu_Left 213
#define IPT_E_Entry_EngineMenu_Right 214
#define IPT_E_Entry_EngineMenu_PrevPage 215
#define IPT_E_Entry_EngineMenu_NextPage 216
#define IPT_E_Entry_EngineMenu_Shortcut1 217
#define IPT_E_Entry_EngineMenu_Shortcut2 218
#define IPT_E_Entry_EngineMenu_CloseSubMenu 219
#define IPT_E_Entry_EngineMenu_Plus 220
#define IPT_E_Entry_EngineMenu_Less 221
#define IPT_E_Entry_EngineMenu_Help 222
#define IPT_E_Entry_Action_TM_Lob_Pad0 223
#define IPT_E_Entry_Action_TM_Tir_Pad0 224
#define IPT_E_Entry_Action_TM_Lob_Pad1 225
#define IPT_E_Entry_Action_TM_Tir_Pad1 226

#endif /* constants_h */
