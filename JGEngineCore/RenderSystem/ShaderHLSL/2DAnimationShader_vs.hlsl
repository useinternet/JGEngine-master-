



cbuffer MatrixBuffer
{
	matrix WorldViewProjectionMatrix;
};
cbuffer AnimTransIncrementBuffer
{
	float WidthIncrement;
	float HeightIncrement;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

PixelInputType main(VertexInputType input)
{
	PixelInputType output;
	input.position.w = 1.0f;
	// �ý��� �̵���Ű��.. �ϴ� ���..
	output.position = mul(input.position, WorldViewProjectionMatrix);

	// �ؽ��� �̵�
	input.tex.x += WidthIncrement;
	input.tex.y += HeightIncrement;
	output.tex = input.tex;

	return output;
}