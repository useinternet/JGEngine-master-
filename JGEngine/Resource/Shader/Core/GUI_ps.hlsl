





struct PSInput
{
	float4 PosH  : SV_POSITION;
    float4 Color : COLOR;
};


float4 main(PSInput input) : SV_Target
{

    return input.Color;

}