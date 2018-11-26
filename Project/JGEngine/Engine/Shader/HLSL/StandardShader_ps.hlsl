


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
/* ������ ������
1. �⺻ ���͸���� �ݻ� ������ �����ϴ��� ���ϴ��� �˻�( reflection = true, false )
 case 1 : �ݻ� �����̶��
    R1. view matrix�� �̷��� ���� �ؽ��ĸ� ���´�.( �ѹ��� ȭ�� ������ <- ��� �ø� ó��)
	    (reflectionColor�� * reflectionIntensity) �� �⺻ �ؽ���Ȥ�� Ambient�� ���ؼ� ���� ���� ��ȯ
	R2. �ش� ���͸����� Albedo�⺻���� ���� �ݻ� ���̴����� ���� �ؽ��İ� �ȴ�.
	R3. �⺻ ������ ����
case 2 : �ݻ� ������ �ƴ϶��
    1. �⺻ ������ ���� <- ���� �۾���
*/
// ���ݻ籤 ���� �⺻ = 1
//specularColor = 1,1,1,1
/* ���� Ÿ�� �װ�
   1      2      3     4
  |    worldPos     | Depth |   // ���� ��ǥ, ����
  |    normal       |SpecPw |   // ��� ��ǥ, ���ݻ籤 ����
  |    albedo       | specIns   |   // �⺻ �ݻ��, �ݻ� ����
  | specularColor   | pad   |   // ���ݻ籤 �� / 
*/
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 worldPos  : TEXCOORD1;
	float4 Pos : TEXCOORD2;
	float3 normal : NORMAL;
	float3 tangent  : TANGENT;
	float3 binormal : BINORMAL;
};
struct PixelOutputType
{
	float4 Pos_Depth     : SV_TARGET0;
	float4 Normal_SpecPw : SV_TARGET1;
	float4 Albedo_SpecIts : SV_TARGET2;
	float4 SpecColor_Pad : SV_TARGET3;
	float4 DepthTarget   : SV_TARGET4;
};
PixelOutputType main(PixelInputType input) : SV_TARGET
{
	PixelOutputType output;
//////////////////////////////////////////////////////////////////
///////////////////////  �־���� �� ����  ////////////////////////
//////////////////////////////////////////////////////////////////
float3 st_WorldPos;   float  st_Depth;
float3 st_Normal;     float  st_SpecPw;
float3 st_Albedo;     float  st_SpecIntensity;
float4 st_SpecColor;
//////////////////////////////////////////////////////////////////
//////////////////////////  ���  ////////////////////////////////
//////////////////////////////////////////////////////////////////



	float4 textureColor = Default_Texture.Sample(Default_WrapSampler, input.tex);
	float4 normalMap    = Normal_Texture.Sample(Default_WrapSampler, input.tex);
	float3 normalVec;
	//// ��� ���� ���� ������ �޾ƿ´�.
	normalMap = normalize((normalMap * 2) - 1.0f);

	// ��� ���� ���� ������ ���� �������� ����
	normalVec = input.normal + normalMap.x * input.tangent + normalMap.y * input.binormal;
	normalVec = normalize(normalVec);

//////////////////////////////////////////////////////////////////
////////////////////////   ���� �� ����   /////////////////////////
//////////////////////////////////////////////////////////////////
	// Pos_Depth
	st_WorldPos = input.worldPos.xyz;
	st_Depth    = input.worldPos.z / input.worldPos.w;
	// Normal_SpecPw
	st_Normal = normalVec;
	st_SpecPw = SpecularPower;
	// Albedo_Custom
	st_Albedo = textureColor.xyz;
	st_SpecIntensity = SpecularIntensity;
	// SpecColor_Pad
	st_SpecColor = float4(SpecularColor, 1.0f);
//////////////////////////////////////////////////////////////////
////////////////// ����Ÿ�ٿ� ���� ���� ////////////////////////////
//////////////////////////////////////////////////////////////////
	output.Pos_Depth     = float4(st_WorldPos, st_Depth);
	output.Normal_SpecPw = float4(st_Normal, st_SpecPw);
	output.Albedo_SpecIts = float4(st_Albedo, st_SpecIntensity);
	output.SpecColor_Pad = st_SpecColor;
	output.DepthTarget = float4(st_Depth, st_Depth, st_Depth, 1.0f);
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////



	return output;
}