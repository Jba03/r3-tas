#ifndef stMaterialGLI_h
#define stMaterialGLI_h

#include "structure.h"

struct stColorGEO {
    readonly float32 r;
    readonly float32 g;
    readonly float32 b;
    readonly float32 a;
} typedef tdstColorGEO;

struct stMaterialGLI {
    readonly uint32 type;
    readonly tdstColorGEO ambient;
    readonly tdstColorGEO diffuse;
    readonly tdstColorGEO specular;
    readonly tdstColorGEO color;
    readonly uint32 additionalType;
    readonly int32 specularExponent;
    readonly pointer texture;
    readonly float32 addU;
    readonly float32 addV;
    readonly float32 addConstantU;
    readonly float32 addConstantV;
    readonly int32 incrementEnable;
    readonly uint32 actualRefreshNumber;
    readonly pointer firstTextureNodeAnimated; // PS2::stAnimatedTextureNodeGLI, GCN::stTextureGLI
    readonly pointer actualTextureNodeAnimated; // PS2::stAnimatedTextureNodeGLI, GCN::stTextureGLI
    readonly int32 numDisplayModes;
    readonly float32 actualDisplayTime;
    readonly uchar8 locked;
    padding(3)
    readonly uint32 flags;
    readonly uint32 multiTextureType;
    readonly uint32 numTextureStages;
    /* ::multimaterial  */
} typedef tdstMaterialGLI;

#endif /* stMaterialGLI_h */
