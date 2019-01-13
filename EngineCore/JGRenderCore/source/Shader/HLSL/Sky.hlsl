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
	float4 PosH     : SV_POSITION;
	float3 PosL     : POSITION;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	// ���� ���� ��ġ�� �Թ�ü �� ��ȸ ���ͷ� ���
	vout.PosL = vin.PosL;

	// ���� ���� ���� ��ȯ
	float4 posW = mul(float4(vin.PosL, 1.0f), gWorld);

	// �ϴñ��� �߽��� �׻� ī�޶� ��ġ�� �д�.
	posW.xyz += gEyePosW;

	// z/w = 1�� �ǵ��� z = w ���Ѵ�. 
	vout.PosH = mul(posW, gViewProj).xyww;

	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	return gCubeMap.Sample(gsamLinearWrap, pin.PosL);
}
