float4x4 matViewProjection;
float4 vViewPosition;
float fTime0_1;
float2 wind;
float cloudscale;

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float3 dir      : TEXCOORD0;
   float2 UV : TEXCOORD1;
   
};

VS_OUTPUT vs_main(float4 Position: POSITION,
                  float3 Normal: NORMAL,
                  float2 UV: TEXCOORD0,
                  float3 Tangent: TANGENT)
{
   VS_OUTPUT Output;
   
   
   Output.Position = mul( matViewProjection, float4(Position.xyz + vViewPosition, 1));
   Output.dir= Position.xyz;
   //Output.skycolor = lerp( skycolor_bottom, skycolor_top, saturate(Position.y/50));
   Output.UV =UV+ wind* (fTime0_1/300);
   Output.UV *= cloudscale;
   return( Output );
   
}



