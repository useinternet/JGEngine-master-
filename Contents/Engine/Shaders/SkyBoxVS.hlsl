
#include"GraphicsDefine.hlsl"

#ifndef _SKYBOX_HLSL__
#define _SKYBOX_HLSL__
struct VertexIn
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float3 TangentL : TANGENT;
    float2 TexC : TEXCOORD;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosL : POSITION;
};

VertexOut VS(VertexIn vin, uint instanceID : SV_InstanceID)
{
    VertexOut vout;
    ObjectCB obj = gObjects[instanceID];
    float4x4 world = obj.World;
    
	// ���� ���� ��ġ�� �Թ�ü �� ��ȸ ���ͷ� ���
    vout.PosL = vin.PosL;

	// ���� ���� ���� ��ȯ
    float4 posW = mul(float4(vin.PosL, 1.0f), world);

	// �ϴñ��� �߽��� �׻� ī�޶� ��ġ�� �д�.
    posW.xyz += gToEye;

	// z/w = 1�� �ǵ��� z = w ���Ѵ�. 
    vout.PosH = mul(posW, gViewProj).xyww;

    return vout;
}

#endif