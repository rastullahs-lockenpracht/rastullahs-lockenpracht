float4x4 matViewProjection;
float4 vViewPosition;
float4 skycolor_top;
float4 skycolor_bottom;


struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float3 dir      : TEXCOORD0;
   float4 skycolor : TEXCOORD1;
   
};

VS_OUTPUT vs_main(float4 Position: POSITION,
                  float3 Normal: NORMAL)
{
   VS_OUTPUT Output;
   Output.Position = mul( matViewProjection, float4(Position.xyz + vViewPosition, 1));
   Output.dir= Position.xyz;
   Output.skycolor = lerp( skycolor_bottom, skycolor_top, saturate(Position.y/50));
   return( Output );
   
}



