#ifndef NUM_DIR_LIGHTS
#define NUM_DIR_LIGHTS 1
#endif

#ifndef NUM_POINT_LIGHTS
#define NUM_POINT_LIGHTS 0
#endif

#ifndef NUM_SPOT_LIGHTS
#define NUM_SPOT_LIGHTS 0
#endif

#include "Common.hlsl"

struct VertexIn
{ 
	float3 PosL     : POSITION;
	float3 NormalL  : NORMAL;
	float3 TangentL : TANGENT;
	float2 TexC     : TEXCOORD0;
};

struct VertexOut
{
	float4 PosH       : SV_POSITION;
	float4 SSAOPosH   : POSITION0;
	float3 PosW       : POSITION1;
	float3 NormalW    : NORMAL;
	float3 TangentW   : TANGENT;
	float2 TexC       : TEXCOORD0;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	MaterialData matData = gMaterialData[gMaterialIndex];
	// Transform to homogeneous clip space.
	float4 PosW = mul(float4(vin.PosL, 1.0f), gWorld);
	vout.PosW = PosW.xyz;

	vout.NormalW = mul(vin.NormalL, (float3x3)gWorld);
	vout.TangentW = mul(vin.TangentL, (float3x3)gWorld);

	vout.PosH = mul(PosW, gViewProj);
	vout.SSAOPosH = mul(PosW, gViewProjTex);

	float4 texC = mul(float4(vin.TexC, 0.0f, 1.0f), gTexTransform);
	vout.TexC = mul(texC, matData.MatTransform).xy;

	return vout;
}
float4 PS(VertexOut pin) : SV_Target
{
	// ���͸��� ������ �б�
	MaterialData matData = gMaterialData[gMaterialIndex];
    float4 diffuseAlbedo = matData.DiffuseAlbedo;
	float3 fresnelR0 = matData.FresnelR0;
	float roughness = matData.Roughness;
	float refractive = matData.Refractive;
	// �ؽ��� �б�
	diffuseAlbedo *= gTexture[matData.TextureIndex[0]].Sample(gsamAnisotropicWrap, pin.TexC);

	// ��� ���� ����ȭ
	pin.NormalW = normalize(pin.NormalW);

	// SSAO ���
	pin.SSAOPosH /= pin.SSAOPosH.w;
	float AmbientAccess = gSSAOMap.Sample(gsamLinearClamp, pin.SSAOPosH.xy, 0.0f).r;
	// ī�޶� ���� // �ֺ��� 
	float3 toEyeW = normalize(gEyePosW - pin.PosW);
	float4 ambient = AmbientAccess * gAmbientLight * diffuseAlbedo;





	// �׸��� ���
	float3 shadowFactor = float3(1.0f, 1.0f, 1.0f);
	float4 ShadowPosH = mul(float4(pin.PosW, 1.0f), gLights[0].ShadowTransform);
	shadowFactor[0] = CalcShadowFactor(ShadowPosH);

	// ����Ʈ ���
	const float shininess = 1.0f - matData.Roughness;
	Material mat = { diffuseAlbedo, matData.FresnelR0, shininess };
	float4 directLight = ComputeLighting(gLights, mat, pin.PosW, pin.NormalW, toEyeW, shadowFactor[0]);

	float4 litColor = ambient + directLight;

	
///////////////////////////////////////////////////
///////////////  ���� �� �ݻ� ���   ///////////////
	float4 reflectionColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float3 fresnelFactor   = float3(0.0f, 0.0f, 0.0f);
	float4 reflactionColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
#ifdef REFLECTION
	float3 r = reflect(-toEyeW, pin.NormalW);
	reflectionColor = gCubeMap.Sample(gsamLinearWrap, r);
	fresnelFactor = SchlickFresnel(fresnelR0, pin.NormalW, r);
#ifdef REFRACTION
	reflectionColor *= 0.5f;
#endif

#endif
#ifdef REFRACTION
	float3 g = refract(-toEyeW, pin.NormalW, refractive);
	reflactionColor = gCubeMap.Sample(gsamLinearWrap, g);
#ifdef REFLECTION
	reflactionColor *= 0.5f;
#endif
#endif
///////////////////////////////////////////////////
///////////////////////////////////////////////////
	litColor.rgb += shininess * fresnelFactor * (reflectionColor.rgb + reflactionColor.rgb);
	litColor.a = diffuseAlbedo.a;
	return litColor;
}