
Texture2D Texture;
SamplerState WrapSamplerState;




cbuffer SpriteBuffer
{
	float4 SpriteColor_xyz_IsSprite_w;
	float4 ColorRize;
	float2 StartDrawTex;
	float2 EndDrawTex;
};



struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};



float4 main(PixelInputType input) : SV_TARGET
{
	float3 SpriteColor = SpriteColor_xyz_IsSprite_w.xyz;
	float  IsSprite = SpriteColor_xyz_IsSprite_w.w;
	
	// �ؽ��� �÷� ��������..
	float4 color = Texture.Sample(WrapSamplerState, input.tex);
	if (StartDrawTex.x > input.tex.x || input.tex.x > EndDrawTex.x)
	{
		color.a = 0.0f;
	}
	if (StartDrawTex.y > input.tex.y || input.tex.y > EndDrawTex.y)
	{
		color.a = 0.0f;
	}


	// ��������Ʈ �÷��� �ش�Ǵ� ���� ���İ� 0���� �Ѵ�.
	if ( IsSprite == 1.0f)
	{
		if (color.x == SpriteColor.x && color.y == SpriteColor.y &&
			color.z == SpriteColor.z)
		{
			color.a = 0.0f;
		}
	
	}
	color = color * ColorRize;
	return color;
}