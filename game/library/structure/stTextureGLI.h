#ifndef stTextureGLI_h
#define stTextureGLI_h

#include "structure.h"

struct stTextureGLI {
    readonly int32 available;
    readonly int32 quality;
    readonly pointer data;
    readonly pointer colorTable;
    readonly pointer specularParam;
    readonly int32 caps;
    readonly uint16 height;
    readonly uint16 width;
    readonly uint16 realHeight;
    readonly uint16 realWidth;
    readonly float32 addU;
    readonly float32 addV;
    readonly int32 incrementEnable;
    readonly uint32 chromaKeyColor;
    readonly uint32 blendColor;
    readonly int32 numLod;
    readonly uint32 compressionCounter;
    readonly uint32 compressionType;
    readonly uint32 compressionMipMap;
    readonly pointer substitutingTexture;
    readonly uchar8 bilinearInterpolation;
    readonly uchar8 cyclingMode;
    readonly char8 filename[128];
    padding(2)
} typedef tdstTextureGLI;

struct stAnimatedTextureNodeGLI {
    readonly pointer texture;
    readonly float32 displayDuration;
    readonly pointer next;
} typedef tdstAnimatedTextureNodeGLI;

#endif /* stTextureGLI_h */
