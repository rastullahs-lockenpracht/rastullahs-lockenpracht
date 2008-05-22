struct VS_OUTPUT 
{
   float4 Pos         : POSITION;
   float2 UV          : TEXCOORD0;
   float2 UV2         : TEXCOORD1;
   float3 Light1DirTS : TEXCOORD2;
   float3 Half1DirTS  : TEXCOORD3;
   float3 Light2DirTS : TEXCOORD4;
   float3 Half2DirTS  : TEXCOORD5;
};

VS_OUTPUT vs_main_1uv(float4 inPos     : POSITION,
                      float3 inNormal  : NORMAL,
                      float3 inTangent : TEXCOORD1,
                      float2 inUV      : TEXCOORD0,
                      uniform float4 EyePosOS,
                      uniform float4 Light1PosOS,
                      uniform float4 Light2PosOS,
                      uniform float4x4 WorldViewProjMatrix)
{
    VS_OUTPUT Out;

    Out.Pos = mul(WorldViewProjMatrix, inPos);
    Out.UV = inUV;
	Out.UV2 = inUV;
    
    float3 Light1Dir = normalize(Light1PosOS.xyz - inPos.xyz*Light1PosOS.w);
    float3 Light2Dir = normalize(Light2PosOS.xyz - inPos.xyz*Light2PosOS.w);
    float3 EyeDir = normalize(EyePosOS.xyz - inPos.xyz);

    float3 binormal = cross(inTangent, inNormal);
    //float3 binormal = cross(inNormal, inTangent);
    float3x3 TangentSpace;
    TangentSpace[0] = inTangent;
    TangentSpace[1] = binormal;
    TangentSpace[2] = inNormal;

    Out.Half1DirTS  = mul(TangentSpace, normalize(Light1Dir + EyeDir));
    Out.Half2DirTS  = mul(TangentSpace, normalize(Light2Dir + EyeDir));
    Out.Light1DirTS = mul(TangentSpace, Light1Dir);
    Out.Light2DirTS = mul(TangentSpace, Light2Dir);

    return Out;
}

VS_OUTPUT vs_main_2uv(float4 inPos     : POSITION,
                      float3 inNormal  : NORMAL,
                      float3 inTangent : TEXCOORD2,
                      float2 inUV      : TEXCOORD0,
                      float2 inUV2      : TEXCOORD1,
                      uniform float4 EyePosOS,
                      uniform float4 Light1PosOS,
                      uniform float4 Light2PosOS,
                      uniform float4x4 WorldViewProjMatrix)
{
    VS_OUTPUT Out;

    Out.Pos = mul(WorldViewProjMatrix, inPos);
    Out.UV = inUV;
    Out.UV2 = inUV2;

    float3 Light1Dir = normalize(Light1PosOS.xyz - inPos.xyz*Light1PosOS.w);
    float3 Light2Dir = normalize(Light2PosOS.xyz - inPos.xyz*Light2PosOS.w);
    float3 EyeDir = normalize(EyePosOS.xyz - inPos.xyz);

    float3 binormal = cross(inTangent, inNormal);
    //float3 binormal = cross(inNormal, inTangent);
    float3x3 TangentSpace;
    TangentSpace[0] = inTangent;
    TangentSpace[1] = binormal;
    TangentSpace[2] = inNormal;

    Out.Half1DirTS  = mul(TangentSpace, normalize(Light1Dir + EyeDir));
    Out.Half2DirTS  = mul(TangentSpace, normalize(Light2Dir + EyeDir));
    Out.Light1DirTS = mul(TangentSpace, Light1Dir);
    Out.Light2DirTS = mul(TangentSpace, Light2Dir);

    return Out;
}


VS_OUTPUT vs_main_3uv(float4 inPos     : POSITION,
                      float3 inNormal  : NORMAL,
                      float3 inTangent : TEXCOORD3,
                      float2 inUV      : TEXCOORD0,
                      float2 inUV2      : TEXCOORD1,
                      uniform float4 EyePosOS,
                      uniform float4 Light1PosOS,
                      uniform float4 Light2PosOS,
                      uniform float4x4 WorldViewProjMatrix)
{
    VS_OUTPUT Out;

    Out.Pos = mul(WorldViewProjMatrix, inPos);
    Out.UV = inUV;
	Out.UV2 = inUV2;
    
    float3 Light1Dir = normalize(Light1PosOS.xyz - inPos.xyz*Light1PosOS.w);
    float3 Light2Dir = normalize(Light2PosOS.xyz - inPos.xyz*Light2PosOS.w);
    float3 EyeDir = normalize(EyePosOS.xyz - inPos.xyz);

    float3 binormal = cross(inTangent, inNormal);
    //float3 binormal = cross(inNormal, inTangent);
    float3x3 TangentSpace;
    TangentSpace[0] = inTangent;
    TangentSpace[1] = binormal;
    TangentSpace[2] = inNormal;

    Out.Half1DirTS  = mul(TangentSpace, normalize(Light1Dir + EyeDir));
    Out.Half2DirTS  = mul(TangentSpace, normalize(Light2Dir + EyeDir));
    Out.Light1DirTS = mul(TangentSpace, Light1Dir);
    Out.Light2DirTS = mul(TangentSpace, Light2Dir);

    return Out;
}




float4 ps_main(VS_OUTPUT In,
               uniform sampler DiffuseMap : register(s0),
               uniform sampler NormalMap  : register(s1),
               uniform sampler SpecMap	  : register(s2),
               uniform sampler aoMap  : register(s3),
               uniform float4  MaterialAmbient,
               uniform float4  MaterialDiffuse,
               uniform float   MaterialSpecularExponent,
               uniform float4  Light1Diffuse,
               uniform float4  Light1Specular,
               uniform float4  Light2Diffuse,
               uniform float4  Light2Specular,
               uniform float4  AmbientLight) : COLOR 
{
    // Read normal from texture. *2-1 to convert it from signed to unsigned.
    float4 Normal = tex2D(NormalMap, In.UV) * 2 - 1;
    float4 TexColour = tex2D(DiffuseMap, In.UV);
    float4 aoColour = tex2D(aoMap, In.UV2);
    
    // Spec
    float4 SpecColour= tex2D(SpecMap, In.UV);

    // Ambient component
    float4 Ambient = MaterialAmbient * AmbientLight;

    float NdotL1 = (dot(Normal.xyz, normalize(In.Light1DirTS)));
    float NdotL2 = (dot(Normal.xyz, normalize(In.Light2DirTS)));
    float4 Diffuse1, Diffuse2, Specular1, Specular2;
    
    // We only need to calculate lighting, if surface is facing the light.
    if (NdotL1 > 0.0)
    {
        // Diffuse component
        Diffuse1 = MaterialDiffuse * Light1Diffuse * NdotL1;

        // Specular component
        float SpecularAttn = pow(saturate(
            dot(Normal.xyz, normalize(In.Half1DirTS))),
                MaterialSpecularExponent);
        Specular1 = Light1Specular * SpecularAttn * SpecColour /2;
    }
    
    else
    {
        // Diffuse component
        Diffuse1 =((MaterialDiffuse*Light1Diffuse * NdotL1)/2.5);
    }

    if (NdotL2 > 0.0)
    {
        // Diffuse component
        Diffuse2 = MaterialDiffuse * Light2Diffuse * NdotL2;

        // Specular component
        float SpecularAttn = pow(saturate(
            dot(Normal.xyz, normalize(In.Half2DirTS))),
                MaterialSpecularExponent);
        Specular2 = Light2Specular * SpecularAttn * SpecColour /2;
    }
    
    else
    {
        // Diffuse component
        Diffuse2 =((MaterialDiffuse*Light2Diffuse * NdotL2)/2.5);
    }
    
    return (TexColour * (Ambient*1.5 + Diffuse1 + Diffuse2)) + Specular1 + Specular2;
}

