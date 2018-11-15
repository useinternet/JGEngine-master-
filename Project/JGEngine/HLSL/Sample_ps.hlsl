


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
	float  CustomVar;         // �⺻ 0
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
  |    albedo       | pad   |   // �⺻ �ݻ��, �ݻ� ����
  | specularColor   | pad   |   // ���ݻ籤 �� / 
*/
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
	float4 textureColor = Default_Texture.Sample(Default_WrapSampler, input.tex);
	float4 normalMap    = Normal_Texture.Sample(Default_WrapSampler, input.tex);
	float3 normalVec;
	//// ��� ���� ���� ������ �޾ƿ´�.
	normalMap = normalize((normalMap * 2) - 1.0f);

	// ��� ���� ���� ������ ���� �������� ����
	normalVec = input.normal + normalMap.x * input.tangent + normalMap.y * input.binormal;
	normalVec = normalize(normalVec);

	textureColor *= float4(normalVec,1.0f);


	return textureColor;
}