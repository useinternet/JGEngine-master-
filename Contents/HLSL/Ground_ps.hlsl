


Texture2D Default_Texture;
Texture2D Normal_Texture;



SamplerState Default_WrapSampler;


cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
	float padding;
};
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent  : TANGENT;
	float3 binormal : BINORMAL;
};
float4 main(PixelInputType input) : SV_TARGET
{
    float3 lightDir;
    float lightIntensity;
    float4 color;
	float4 textureColor = Default_Texture.Sample(Default_WrapSampler, input.tex);
	float4 normalMap = Normal_Texture.Sample(Default_WrapSampler, input.tex);
	float3 normalVec;
	//// ��� ���� ���� ������ �޾ƿ´�.
	normalMap = normalize((normalMap * 2) - 1.0f);

	// ��� ���� ���� ������ ���� �������� ����
	normalVec = input.normal + normalMap.x * input.tangent + normalMap.y * input.binormal;
	normalVec = normalize(normalVec);
	//
    color = ambientColor;

    lightDir = -lightDirection;

	lightIntensity = saturate(dot(normalVec, lightDir));
	if (lightIntensity > 0.0f)
	{
		color += (diffuseColor * lightIntensity);
		color = saturate(color) * textureColor;
	}
	return color * normalMap.a;
}