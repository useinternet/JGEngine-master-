#include"Common.hlsl"

struct SceneData
{
    float3 PosW;
    float3 Albedo;
    float3 NormalW;
    float  Depth;
    uint   MatIndex;
    float  IsBackGround;
};
SceneData InstallData(float2 TexC)
{
    SceneData Data;
    float4 Albedo = gTexture[gAlbedoSceneIndex].Sample(gsamAnisotropicWrap, TexC);
    Data.Albedo = Albedo.rgb;
    float4 NormalW = gTexture[gNormalSceneIndex].Sample(gsamAnisotropicWrap, TexC);
    Data.NormalW = normalize((NormalW.xyz * 2.0f) - 1.0f);
    Data.PosW = gTexture[gWorldPosSceneIndex].Sample(gsamAnisotropicWrap, TexC).xyz;
    Data.Depth = gTexture[gDepthSceneIndex].Sample(gsamAnisotropicWrap, TexC).r;
    Data.MatIndex = gTexture[gMatSceneIndex].Sample(gsamAnisotropicWrap, TexC).r - 1;

    if (Albedo.a < 0.1f)
        clip(Albedo.a - 0.1f);

    Data.IsBackGround = NormalW.a;
    return Data;
}
static const float2 gTexCoords[6] =
{
    float2(0.0f, 1.0f),
    float2(0.0f, 0.0f),
    float2(1.0f, 0.0f),
    float2(0.0f, 1.0f),
    float2(1.0f, 0.0f),
    float2(1.0f, 1.0f)
};
 
struct VS_OUT
{
    float4 PosH : SV_POSITION;
    float2 TexC : TEXCOORD0;
};

VS_OUT VS(uint vid : SV_VertexID)
{
    VS_OUT vout;

    vout.TexC = gTexCoords[vid];


    vout.PosH = float4(2.0f * vout.TexC.x - 1.0f, 1.0f - 2.0f * vout.TexC.y, 0.0f, 1.0f);
 
    return vout;
}
float4 PS(VS_OUT pin) : SV_Target
{
    SceneData Data = InstallData(pin.TexC);

    if(Data.IsBackGround > 0.1f)
    {
        MaterialData matData = gMaterialData[Data.MatIndex];
    
    
        float3 toEyeW = normalize(gEyePosW - Data.PosW);
        float4 ambient = float4(Data.Albedo, 1.0f);

        const float shininess = 1.0f - matData.Roughness;
        Material mat = { float4(Data.Albedo, 1.0f), matData.FresnelR0, shininess };
        float3 result = 0.0f;
        float ShadowFactor = 1.0f;
        float4 ShadowPosH = float4(Data.PosW, 1.0f);
        if (gDirLightCount > 0)
        {
            ShadowPosH = mul(ShadowPosH, gLightData[0].ShadowTransform);
            ShadowFactor = CalcDirectionShadowFactor(ShadowPosH,
        gLightData[0].StaticShadowMapIndex,
        gLightData[0].DynamicShadowMapIndex);
      
            result += saturate(ShadowFactor * ComputeDirectionalLight(gLightData[0], mat, Data.NormalW, toEyeW));
        }
        int i = gDirLightCount;
        for (i = gDirLightCount; i < gDirLightCount + gPointLightCount; ++i)
        {
            ShadowFactor = CalcPointShadowFactor(Data.PosW, gLightData[i]);
    
            result += saturate(ShadowFactor * ComputePointLight(gLightData[i], mat, Data.PosW, Data.NormalW, toEyeW));

        }
        for (i = gDirLightCount + gPointLightCount; i < gDirLightCount + gPointLightCount + gSpotLightCount; ++i)
        {
            ShadowPosH = float4(Data.PosW, 1.0f);
            ShadowPosH = mul(ShadowPosH, gLightData[i].ShadowTransform);
            ShadowFactor = CalcSpotShadowFactor(ShadowPosH,
        gLightData[i].StaticShadowMapIndex,
        gLightData[i].DynamicShadowMapIndex);
     
            result += saturate(ShadowFactor * ComputeSpotLight(gLightData[i], mat, Data.PosW, Data.NormalW, toEyeW));
        }

        float4 directLight = float4(result, 0.0f);
        float4 litColor = ambient + saturate(directLight);        
        return litColor;
    }
    else
    {
        return float4(Data.Albedo, 1.0f);
    }
}