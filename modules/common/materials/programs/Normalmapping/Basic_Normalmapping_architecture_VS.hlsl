uniform float4x4 matViewProjection;
uniform float4 LightPosition_0;
uniform float4 LightPosition_1;
//uniform float4 LightPosition_2;
uniform float4 vViewPosition;
uniform float4 LightAttn_0;
uniform float4 LightAttn_1;
//uniform float4 LightAttn_2;

struct VS_OUTPUT 
{
   float4 Position   : POSITION0;
   float4 UV         : TEXCOORD0; // uv1 / 2
   float4 LightDir_0 : TEXCOORD1;
   float3 HalfVect_0 : TEXCOORD2;
   float4 LightDir_1 : TEXCOORD3;
   float3 HalfVect_1 : TEXCOORD4;
   //float4 LightDir_2 : TEXCOORD5;
   //float4 HalfVect_2 : TEXCOORD6; // uv2 x
   float2 UV2		 : TEXCOORD5;
   float4 SkyDir     : TEXCOORD6; // uv2 y
   float3 EyeVect    : TEXCOORD7;
   
};

VS_OUTPUT vs_main(      float4 inPosition : POSITION0,
                        float3 inNormal : NORMAL0,
                        float3 inTangent : TANGENT0,
                        float2 inUV:   TEXCOORD0,
                        float2 inUV1:   TEXCOORD1,
                        float2 inUV2:   TEXCOORD2  )
{
   VS_OUTPUT Output;

   Output.Position = mul( matViewProjection, inPosition );
   Output.UV.xy = inUV;
   Output.UV.zw = inUV1;
   Output.UV2 = inUV2;
   
   float3x3 TangentSpace;
   
   TangentSpace[0] = inTangent;   
   TangentSpace[1] = cross(inTangent,inNormal);   
   TangentSpace[2] = inNormal;
   
   float3 EyeVector = normalize(vViewPosition - inPosition);
   Output.EyeVect = mul(TangentSpace,EyeVector.xyz);
   
   //################# Light 0  ################
   float4 LightDir;
   LightDir.xyz = LightPosition_0 - inPosition * LightPosition_0.w;
   float Dist = length(LightDir.xyz);
   LightDir.xyz = LightDir.xyz /Dist;
   
   if (LightPosition_0.w != 0.0)
   LightDir.w = saturate(1/(LightAttn_0.y +
                            LightAttn_0.z * Dist +
                            LightAttn_0.w * Dist * Dist)*saturate(LightAttn_0.x-Dist));
   else
   LightDir.w = saturate(dot(LightDir.xyz,inNormal )+0.2);                         
   
   
   Output.HalfVect_0 = mul(TangentSpace, normalize(LightDir.xyz + EyeVector));
   Output.LightDir_0 = float4(mul(TangentSpace,LightDir.xyz),LightDir.w);
   
   //################# Light 1  ################
   LightDir;
   LightDir.xyz = LightPosition_1 - inPosition * LightPosition_1.w;
   Dist = length(LightDir.xyz);
   LightDir.xyz = LightDir.xyz /Dist;
   
   if (LightPosition_1.w != 0.0)
   LightDir.w = saturate(1/(LightAttn_1.y +
                            LightAttn_1.z * Dist +
                            LightAttn_1.w * Dist * Dist)*saturate(LightAttn_1.x-Dist));
   else
   LightDir.w = saturate(dot(LightDir.xyz,inNormal )+0.2);                         
   
   Output.HalfVect_1 = mul(TangentSpace, normalize(LightDir.xyz + EyeVector));
   Output.LightDir_1 = float4(mul(TangentSpace,LightDir.xyz),LightDir.w);
   
   
   
   // SkyLight
   Output.SkyDir.xyz    = mul(TangentSpace, float3(0,-1,0));
   Output.SkyDir.w = 1;
   
   return( Output );
   
}



