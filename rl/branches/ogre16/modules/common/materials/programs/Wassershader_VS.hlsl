float4x4 matViewProjection;
float4 vViewPosition;
float fTime0_X;

float4 Light1_attenuation;
float4 Light2_attenuation;
float4 Light3_attenuation;

float4 Light1_position;
float4 Light2_position;
float4 Light3_position;


struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float4 UV       : TEXCOORD0;
   float3 eyeVect  : TEXCOORD1;
   float4 times    : TEXCOORD2;
   float3 Normal   : TEXCOORD3;
   float4 Half1    : TEXCOORD4;
   float4 Half2    : TEXCOORD5;
   float4 Half3    : TEXCOORD6;
   
};

VS_OUTPUT vs_main( float4 Position: POSITION,
                  float2 inUV:TEXCOORD0,
                  float2 inUV2:TEXCOORD1,
                  float3 inNormal : NORMAL,
                  float4 inColor : COLOR)
{
   VS_OUTPUT Output;
   Output.Normal = inNormal;
   Output.times.w = inColor.r; // putting blending factor into times to face space ;D
   Output.eyeVect = normalize(Position- vViewPosition);
   Output.Position = mul( matViewProjection, Position );
   Output.UV.xy = inUV*5;
   Output.UV.y += fTime0_X/4;
   Output.UV.zw = inUV2*5;
   Output.UV.w += fTime0_X*2;
   //Output.UV = inUV*5;
   float time = fTime0_X *3;
   Output.times.x = sin(time+10.5);
   Output.times.y = sin(time+2.0);
   Output.times.z = sin(time);
   
   
   //########################
   float4 LightDir;
   LightDir.xyz = Light1_position - Position * Light1_position.w;
   float Dist = length(LightDir.xyz);
   LightDir.xyz = LightDir.xyz /Dist;
   
   if (Light1_position.w != 0.0)
   Output.Half1.w = saturate(1/(Light1_attenuation.y +
                            		Light1_attenuation.z * Dist +
                            		Light1_attenuation.w * Dist * Dist));
   else Output.Half1.w = 0;
   
   Output.Half1.xyz = LightDir.xyz-Output.eyeVect;
   
   //########################
   LightDir.xyz = Light2_position - Position * Light2_position.w;
   Dist = length(LightDir.xyz);
   LightDir.xyz = LightDir.xyz /Dist;
   
   if (Light2_position.w != 0.0)
   Output.Half2.w = saturate(1/(Light2_attenuation.y +
                            		Light2_attenuation.z * Dist +
                            		Light2_attenuation.w * Dist * Dist));
   else Output.Half2.w = 0;
                            		
   Output.Half2.xyz = LightDir.xyz-Output.eyeVect;
   
   //########################
   LightDir.xyz = Light3_position - Position * Light3_position.w;
   Dist = length(LightDir.xyz);
   LightDir.xyz = LightDir.xyz /Dist;
   
   if (Light3_position.w != 0.0)
   Output.Half3.w = saturate(1/(Light3_attenuation.y +
                            		Light3_attenuation.z * Dist +
                            		Light3_attenuation.w * Dist * Dist));
   else Output.Half3.w = 0;
   
   Output.Half3.xyz = LightDir.xyz-Output.eyeVect;
   
   return( Output );
   
}



