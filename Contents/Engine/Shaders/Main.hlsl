

#include"GraphicsDefine.hlsl"



struct ToVs
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float3 TangentL : TANGENT;
    float2 TexC : TEXCOORD;
#ifdef USE_SKINNED
    float4 BoneWeights : WEIGHTS;
    uint4  BoneID      : BONEID;
#endif
};
struct VsToPs
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float3 TangentW : TANGENT;
    float2 TexC : TEXCOORD;
    uint InstanceID : INSTANCE;
};
VsToPs VS(ToVs vin, uint instanceID : SV_InstanceID)
{
    VsToPs output;
    ObjectCB obj = gObjects[instanceID];
    float4x4 world = obj.World;
    output.InstanceID = instanceID;

#ifdef USE_SKINNED
    float3 posL = float3(0.0f,0.0f,0.0f);
    float3 normalL = float3(0.0f,0.0f,0.0f);
    float3 tangentL = float3(0.0f,0.0f,0.0f);

    for(int i =0; i<4; ++i)
    {
       posL += vin.BoneWeights[i] * mul(float4(vin.PosL, 1.0f), gBoneTransform[vin.BoneID[i]]).xyz;
       normalL += vin.BoneWeights[i] * mul(vin.NormalL, (float3x3)gBoneTransform[vin.BoneID[i]]);
       tangentL += vin.BoneWeights[i] * mul(vin.TangentL, (float3x3)gBoneTransform[vin.BoneID[i]]);
    }
    vin.PosL     = posL;
    vin.NormalL  = normalL;
    vin.TangentL = tangentL;
#endif



    float4 PosW = mul(float4(vin.PosL, 1.0f), world);
    output.PosH = mul(PosW, gViewProj);
    output.PosW = PosW.xyz;
    output.NormalW = mul(vin.NormalL, (float3x3)world);
    output.TangentW = mul(vin.TangentL, (float3x3)world);
    output.TexC = vin.TexC;
    return output;
}




GBufferPack PS(VsToPs pin) : SV_Target
{
    GBufferPack gbuffer;


    float3 N = normalize(pin.NormalW);
    float3 T = normalize(pin.TangentW);





 
    ObjectCB object = gObjects[pin.InstanceID];
    MaterialCB material = gMaterials[object.MaterialIndex];

    

    float4 TextureColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
#ifdef USE_TEXTURE_SLOT0
    TextureColor = gTexture[0].Sample(gAnisotropicWrapSampler, pin.TexC);
#endif





    gbuffer.Albedo = float4(material.SurfaceColor, 1.0f) * TextureColor;
    gbuffer.Normal = float4((N + 1.0f) * 0.5f, 1.0f);
    gbuffer.Specular = float4(material.Roughness, material.Roughness, 1.0f, 1.0f);
    gbuffer.Depth = 0.0f;

    return gbuffer;
}
