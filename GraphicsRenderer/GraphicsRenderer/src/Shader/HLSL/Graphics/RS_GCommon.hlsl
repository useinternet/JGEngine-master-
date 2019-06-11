#include"GraphicsStructDefine.hlsl"

#ifndef __COMMON_ROOTSIGNATURE_DEFINE_HLSL__
#define __COMMON_ROOTSIGNATURE_DEFINE_HLSL__




#define NUM_TEXTURESLOT   8


#define GBUFFER_ALBEDO    0 
#define GBUFFER_NORMAL    1
#define GBUFFER_SPECULAR  2
#define GBUFFER_METALLIC  3
#define GBUFFER_ROUGHNESS 4
#define GBUFFER_A0        5
#define GBUFFER_CUSTOM0   6
#define GBUFFER_CUSTOM1   7


#define GBUFFER_SPECMAP   0
#define GBUFFER_IRRMAP    1
#define GBUFFER_SPECBRDF  6


// ������Ʈ ����
StructuredBuffer<ObjectCB> gObjectCBs : register(t0, space0);

// ��� ���� 
cbuffer PassCBuffer : register(b0, space0)
{
    PassCB gPassCB;
}


// �ؽ��� ����
Texture2D   gTexture[NUM_TEXTURESLOT] : register(t0, space1);
TextureCube gCubeTexture[NUM_TEXTURESLOT] : register(t0, space2);


// ���÷� ����
SamplerState gAnisotropicWrap   :  register(s0, space0);
SamplerState gAnisotropicClamp  : register(s1, space0);
SamplerState gLinearWrap        : register(s2, space0);
SamplerState gLinearClamp       : register(s3, space0);
SamplerState gPointWrap         : register(s4, space0);
SamplerState gPointClamp        : register(s5, space0);
#endif