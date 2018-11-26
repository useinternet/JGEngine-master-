
Texture2D T_Pos_Depth : register(t0);
Texture2D T_ShadowTexture : register(t1);
SamplerState ClampSampler : register(s0);
SamplerComparisonState SamplerShadow : register(s1);
cbuffer LightMatrixBuffer
{
	matrix LightViewProjMatrix;
	matrix InvCameraMatrix;
	matrix InvProjMatrix;

};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex      : TEXCOORD0;
};
float4 main(PixelInputType input) : SV_TARGET
{
	float  color = 0.0f;     // ���� �÷�
	float  Biaos = 0.001f; // ���̾(�ε��Ҽ��� ����)
	float3 WorldPos = T_Pos_Depth.Sample(ClampSampler, input.tex).xyz; // ���� ��ǥ �ؽ���

	// ���� ���� ��ġ �ľ�
	float4 position = float4(WorldPos, 1.0f);

	//position = mul(position, InvProjMatrix);

	position = mul(position, LightViewProjMatrix);
	position = mul(position, InvCameraMatrix);
	// ���� �ؽ���
	float2 projTex;
	projTex = position.xy / position.w;
	projTex.y = -projTex.y;
	projTex = 0.5f * projTex + 0.5f;

	//
	float LightDepth;   // ���� �������� ���� ���� ��
	float Depth;        // ī�޶� �������� ������ ���� ����
	if (saturate(projTex.x) == projTex.x && saturate(projTex.y) == projTex.y)
	{
		Depth = T_ShadowTexture.Sample(ClampSampler, projTex).w; // ���� �ؽ��� ���ø�( �������� ���� ���ø�)
		LightDepth = (position.z / position.w) - Biaos;      // 

		//color = T_ShadowTexture.SampleCmpLevelZero(SamplerShadow, projTex, LightDepth).r;
		if (LightDepth < Depth) //
		{
			color = 1.0f;
		}
	}
	//else
	//{
	//	color = 1.0f;
	//}
	return  float4(LightDepth, LightDepth, LightDepth, 1.0f);
}