#include"Common.hlsli"


Texture2D Default_Texture;
Texture2D Normal_Texture;
SamplerState Default_WrapSampler;

cbuffer Material
{
	float3 Ambient;           // �⺻ 1
	float  ReflectIntensity;  // �⺻ 0
	float3 SpecularColor;     // �⺻ 1
	float  SpecularPower;     // �⺻ 1
	float3 Emissive;          // �⺻ 1
	float  SpecularIntensity;         // �⺻ 0
};
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 worldPos  : TEXCOORD1;
	float3 normal : NORMAL;
	float3 tangent  : TANGENT;
	float3 binormal : BINORMAL;
};
PS_TARGET_OUT main(PixelInputType input)
{
	float4 textureColor = Default_Texture.Sample(Default_WrapSampler, input.tex);
	float4 normalMap    = Normal_Texture.Sample(Default_WrapSampler, input.tex);
	float3 normalVec;
	//// ��� ���� ���� ������ �޾ƿ´�.
	normalMap = normalize((normalMap * 2) - 1.0f);

	// ��� ���� ���� ������ ���� �������� ����
	normalVec = input.normal + normalMap.x * input.tangent + normalMap.y * input.binormal;
	normalVec = normalize(normalVec);

	return PackData(textureColor.rgb, normalVec, SpecularIntensity, SpecularPower);
}
