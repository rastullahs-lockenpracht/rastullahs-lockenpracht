struct VS_OUTPUT 
{
   float4 Position :        POSITION0;
   float2 Texcoord :        TEXCOORD0;
   float3 LightDirection :  TEXCOORD2;
   float3 Normal :          TEXCOORD3;
};

VS_OUTPUT vs_main(     
    float4 Position : POSITION0,
    float2 Texcoord : TEXCOORD0,
    float3 Normal :   NORMAL0,
    uniform float4 fvLightPosition,
    uniform float4x4 matWorldViewProjection )
{
   VS_OUTPUT Output;

   Output.Position         = mul( matWorldViewProjection, Position );
   Output.Texcoord         = Texcoord;

   Output.LightDirection   = normalize(fvLightPosition.xyz -  (Position * fvLightPosition.w));
   Output.Normal           = Normal;

   return( Output );
}

float4 ps_main(     
    float2 Texcoord :        TEXCOORD0,
    float3 Normal :          TEXCOORD3,
    float3 LightDirection:   TEXCOORD2,

    uniform float4 materialAmbient,
    uniform float4 materialDiffuse,
    uniform float4 ambientLight,
    uniform float4 lightDiffuse,

    uniform float scaleBase,
    uniform float scaleBaseR,
    uniform float scaleBaseG,
    uniform float scaleBaseB,
    uniform float scaleBaseA,

    uniform sampler2D splattingMap,
    uniform sampler2D base,
    uniform sampler2D baseR,
    uniform sampler2D baseG,
    uniform sampler2D baseB,
    uniform sampler2D baseA ) : COLOR0
{        
    float4 fvSplattingColor = tex2D( splattingMap, Texcoord );    
    float NdotL = clamp( dot( normalize( Normal ), LightDirection ), 0.0, 1.0);

    float4 fvBaseColor      = tex2D( base, Texcoord / scaleBase );

    fvBaseColor            *= 1.0-fvSplattingColor[0]; 
    fvBaseColor            += ( tex2D(baseR, Texcoord / scaleBaseR ) )
                              * fvSplattingColor[0];
                              
    fvBaseColor            *= 1.0-fvSplattingColor[1]; 
    fvBaseColor            += ( tex2D(baseG, Texcoord / scaleBaseG ) )
                              * fvSplattingColor[1];
                              
    fvBaseColor            *= 1.0-fvSplattingColor[2]; 
    fvBaseColor            += ( tex2D(baseB, Texcoord / scaleBaseB ) )
                              * fvSplattingColor[2];
                              
    fvBaseColor            *= 1.0-fvSplattingColor[3]; 
    fvBaseColor            += ( tex2D(baseA, Texcoord / scaleBaseA ) )
                              * fvSplattingColor[3];   
                                                         
    float4 fvTotalAmbient   = ambientLight * materialAmbient * fvBaseColor; 
    float4 fvTotalDiffuse   = (lightDiffuse * materialDiffuse * fvBaseColor) * NdotL ; 
   
    return( saturate( fvTotalAmbient + fvTotalDiffuse  ) );      
}


