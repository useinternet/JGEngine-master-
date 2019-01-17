#include"Common.hlsl"





struct SceneData
{
    float4 PosW    : SV_Target0;
    float4 Albedo  : SV_Target1;
    float4 NormalW : SV_Target2;
    float2 DataIndex  : SV_Target3;
};
struct VS_IN
{
    float3 PosL     : POSITION;
    float3 NormalL  : NORMAL;
    float3 TangentL : TANGENT;
    float2 TexC     : TEXCOORD0;
};


#ifdef SHADER_INSTANCE_OBJECT

struct VS_OUT
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION0;
    float4 SSAOPosH : POSITION1;
    float3 NormalW : NORMAL;
    float3 TangentW : TANGENT;
    float2 TexC : TEXCOORD0;

    nointerpolation uint MatIndex : MATINDEX;
    nointerpolation uint CubeIndex : CUBEINDEX;
};
VS_OUT VS(VS_IN vin, uint instanceID : SV_InstanceID)
{
    VS_OUT vout;
    InstanceData insData = gInstanceData[instanceID];
    float4x4 world = insData.World;
    float4x4 texTransform = insData.TexTransform;
// ���͸���, ť��� �ε���
    uint matIndex = insData.MaterialIndex;
    vout.MatIndex = matIndex;
    vout.CubeIndex = insData.CubeMapIndex;
// 
    MaterialData matData = gMaterialData[matIndex];


// ���� ��ǥ 
    float4 posW = mul(float4(vin.PosL, 1.0f), world);
    vout.PosW = posW.xyz;
// ��� ź��Ʈ ������� ��ǥ
    vout.NormalW = mul(vin.NormalL, (float3x3) world);
    vout.TangentW = mul(vin.TangentL, (float3x3) world);
// �׷��� ��ġ ��ǥ ���
    // Transform to homogeneous clip space.
    vout.PosH = mul(posW, gViewProj);
	
// �ؽ��� ��ǥ ���
	// Output vertex attributes for interpolation across triangle.
    float4 texC = mul(float4(vin.TexC, 0.0f, 1.0f), texTransform);
    vout.TexC = mul(texC, matData.MatTransform).xy;
    // SSAO
    vout.SSAOPosH = mul(posW, gViewProjTex);
    return vout;
}

#else
struct VS_OUT
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION0;
    float4 SSAOPosH : POSITION1;
    float3 NormalW : NORMAL;
    float3 TangentW : TANGENT;
    float2 TexC : TEXCOORD0;
};
VS_OUT VS(VS_IN vin)
{
    VS_OUT vout;
    MaterialData matData = gMaterialData[gMaterialIndex];


    float4 PosW = mul(float4(vin.PosL, 1.0f), gWorld);
    vout.PosW = PosW.xyz;
    vout.NormalW = mul(vin.NormalL, (float3x3) gWorld);
    vout.TangentW = mul(vin.TangentL, (float3x3) gWorld);
    vout.PosH = mul(PosW, gViewProj);
    float4 texC = mul(float4(vin.TexC, 0.0f, 1.0f), gTexTransform);
    vout.TexC = mul(texC, matData.MatTransform).xy;
    vout.SSAOPosH = mul(PosW, gViewProjTex);
    return vout;
}

#endif


SceneData PS(VS_OUT pin)
{
	// �� ���� �ʱ�ȭ
    SceneData Output;
#ifdef SHADER_INSTANCE_OBJECT
    MaterialData matData = gMaterialData[pin.MatIndex];

#else
    MaterialData matData = gMaterialData[gMaterialIndex];
#endif

    float4 Scene_Albedo   = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float4 Scene_NormalW  = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float4 Scene_WorldPos = float4(0.0f, 0.0f, 0.0f, 1.0f);

	//
    float4 diffuseAlbedo = matData.DiffuseAlbedo;
    float3 fresnelR0     = matData.FresnelR0;
    float  roughness     = matData.Roughness;
    float shineness = 1 - roughness;
    float  refractive    = matData.Refractive;
    float3 normalW       = normalize(pin.NormalW);
    float3 posW          = pin.PosW;
    float2 texC          = pin.TexC;
    float3 toEyeW = normalize(gEyePosW - posW);
    float toEyeDistance = distance(gEyePosW, posW);


    if (toEyeDistance < 100.0f)
    {
        pin.SSAOPosH /= pin.SSAOPosH.w;
        diffuseAlbedo *= gTexture[gSSAOTextureIndex].Sample(gsamAnisotropicWrap, pin.SSAOPosH.xy, 0.0f).r;
    }
  
    
    diffuseAlbedo *= gTexture[matData.TextureIndex[0]].Sample(gsamAnisotropicWrap, texC);



	// SceneData�� ���� �ֱ�
    Scene_Albedo = diffuseAlbedo;
    Scene_NormalW = float4(normalize(normalW), 1.0f);
    Scene_WorldPos = float4(pin.PosW, 1.0f);


	// ���� Data ����
    Output.PosW     = Scene_WorldPos;
    Output.NormalW  = (Scene_NormalW + 1.0f) * 0.5f;
    Output.Albedo   = Scene_Albedo;

#ifdef SHADER_INSTANCE_OBJECT
        Output.DataIndex = float2(pin.MatIndex + 1, pin.CubeIndex + 1);
#else
    Output.DataIndex = float2(gMaterialIndex + 1, gCubeMapIndex + 1);
#endif


    return Output;
}