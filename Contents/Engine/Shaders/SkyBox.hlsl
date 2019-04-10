
#include"GraphicsDefine.hlsl"


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


GBufferPack PS(VertexOut pin) : SV_Target
{
    GBufferPack gbuffer;
    float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);
#ifdef USE_CUBETEXTURE_SLOT0
    color = gCubeMap[0].Sample(gLinearWrapSampler, pin.PosL);
#endif


    gbuffer.Albedo = float4(color.xyz, 0.0f);
    gbuffer.Normal = float4(0.0f, 0.0f, 0.0f, 0.0f);
    gbuffer.Specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    gbuffer.Depth = 1.0f;
    return gbuffer;
}