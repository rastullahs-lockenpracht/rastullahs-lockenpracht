float3 fvLightPosition;
float4x4 matWorldViewProjection;

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
   float3 Normal :   NORMAL0;
};

struct VS_OUTPUT 
{
   float4 Position :        POSITION0;
   float2 Texcoord :        TEXCOORD0;
   float3 LightDirection :  TEXCOORD2;
   float3 Normal :          TEXCOORD3;
};

VS_OUTPUT vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;

   Output.Position         = mul( matWorldViewProjection, Input.Position );
   Output.Texcoord         = Input.Texcoord;
        
   Output.LightDirection   = fvLightPosition;
   Output.Normal           = Input.Normal;

   return( Output );
}


float4 materialAmbient;
float4 materialDiffuse;
float4 ambientLight;
float4 lightDiffuse;

float scaleBase;
float scaleBaseR;
float scaleBaseG;
float scaleBaseB;
float scaleBaseA;


sampler2D splattingMap;
sampler2D base;
sampler2D baseR;
sampler2D baseG;
sampler2D baseB;
sampler2D baseA;

struct PS_INPUT 
{
   float2 Texcoord :        TEXCOORD0;
   float3 Normal :          TEXCOORD3;
   float3 LightDirection:   TEXCOORD2;
};

float4 ps_main( PS_INPUT Input ) : COLOR0
{        
    float4 fvSplattingColor = tex2D( splattingMap, Input.Texcoord );

    float3 fvLightDirection = normalize( Input.LightDirection );
    float3 fvNormal         = normalize( Input.Normal );
    float  fNDotL           = dot( fvNormal, fvLightDirection ); 

    float4 fvBaseColor      = tex2D( base, Input.Texcoord / scaleBase );

    fvBaseColor            *= 1.0-fvSplattingColor[0]; 
    fvBaseColor            += ( tex2D(baseR, Input.Texcoord / scaleBaseR ) )
                              * fvSplattingColor[0];
                              
    fvBaseColor            *= 1.0-fvSplattingColor[1]; 
    fvBaseColor            += ( tex2D(baseG, Input.Texcoord / scaleBaseG ) )
                              * fvSplattingColor[1];
                              
    fvBaseColor            *= 1.0-fvSplattingColor[2]; 
    fvBaseColor            += ( tex2D(baseB, Input.Texcoord / scaleBaseB ) )
                              * fvSplattingColor[2];
                              
    fvBaseColor            *= 1.0-fvSplattingColor[3]; 
    fvBaseColor            += ( tex2D(baseA, Input.Texcoord / scaleBaseA ) )
                              * fvSplattingColor[3];   
                                                         
    float4 fvTotalAmbient   = ambientLight * materialAmbient * fvBaseColor; 
    float4 fvTotalDiffuse   = (lightDiffuse * materialDiffuse * fvBaseColor) * fNDotL ; 
   
    return( saturate( fvTotalAmbient + fvTotalDiffuse  ) );      
}


