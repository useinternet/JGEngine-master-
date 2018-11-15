#include"Common_vs.hlsli"

cbuffer MatrixBuffer
{
	matrix wvpMatrix;
	matrix worldMatrix;
};
INPUTLAYOUT
struct VertexInputType
{
	INPUTSLOT(0)
	float3 position : POSITION;
	float2 tex      : TEXCOORD0;
	float3 normal   : NORMAL;
	float3 tangent  : TANGENT;
	float3 binormal : BINORMAL;
};
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 worldPos : TEXCOORD1;
	float3 normal   : NORMAL;
	float3 tangent  : TANGENT;
	float3 binormal : BINORMAL;
};
PixelInputType main(VertexInputType input)
{
	PixelInputType output;
	float4 position = float4(input.position, 1.0f);


	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(position, wvpMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// ���� ��ǥ
	output.worldPos = mul(position,worldMatrix);
	// �븻 ���� �� ���� ��Ʈ������ ���
	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	output.tangent = mul(input.tangent, (float3x3)worldMatrix);
	output.tangent = normalize(output.tangent);

	output.binormal = mul(input.binormal, (float3x3)worldMatrix);
	output.binormal = normalize(output.binormal);

	return output;
}