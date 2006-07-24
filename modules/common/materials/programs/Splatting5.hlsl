struct VS_OUTPUT 
{
   float4 Position :        POSITION0;
   float2 Texcoord :        TEXCOORD0;
};

VS_OUTPUT vs_main( 
    float4 Position : POSITION0,
    float2 Texcoord : TEXCOORD0,
    uniform float3 fvLightPosition,
    uniform float4x4 matWorldViewProjection )
{
   VS_OUTPUT Output;

   Output.Position         = mul( matWorldViewProjection, Position );
   Output.Texcoord         = Texcoord;
      
   return( Output );
   
}

float4 ps_main(     
    float2 Texcoord :        TEXCOORD0,
    float3 Normal :          TEXCOORD3,

    uniform float4 materialAmbient,
    uniform float4 materialDiffuse,

    uniform float scaleBase,
    uniform float scaleBaseR,
    uniform float scaleBaseG,
    uniform float scaleBaseB,
    uniform float scaleBaseA,
    uniform float scaleDetail,
    uniform float scaleDetailR,
    uniform float scaleDetailG,
    uniform float scaleDetailB,
    uniform float scaleDetailA,

    uniform sampler2D splattingMap,
    uniform sampler2D lightingMap,
    uniform sampler2D base,
    uniform sampler2D baseR,
    uniform sampler2D baseG,
    uniform sampler2D baseB,
    uniform sampler2D baseA ) : COLOR0
{        
    float4 fvSplattingColor = tex2D( splattingMap, Texcoord );
    float4 fvLightColor     = ( tex2D( lightingMap, Texcoord ) - 0.5 ) * 2.0;
   
    float4 fvBaseColor      = tex2D( base, Texcoord / scaleBase ) + 
                               (tex2D(base, Texcoord / scaleDetail )[3] - 0.5);

    fvBaseColor            *= 1.0-fvSplattingColor[0]; 
    fvBaseColor            += ( tex2D(baseR, Texcoord / scaleBaseR ) + 
                               (tex2D(baseR, Texcoord / scaleDetailR )[3] - 0.5) )
                              * fvSplattingColor[0];
                              
    fvBaseColor            *= 1.0-fvSplattingColor[1]; 
    fvBaseColor            += ( tex2D(baseG, Texcoord / scaleBaseG ) + 
                               (tex2D(baseG, Texcoord / scaleDetailG )[3] - 0.5) )
                              * fvSplattingColor[1];
                              
    fvBaseColor            *= 1.0-fvSplattingColor[2]; 
    fvBaseColor            += ( tex2D(baseB, Texcoord / scaleBaseB ) + 
                               (tex2D(baseB, Texcoord / scaleDetailB )[3] - 0.5) )
                              * fvSplattingColor[2];
                              
    fvBaseColor            *= 1.0-fvSplattingColor[3]; 
    fvBaseColor            += ( tex2D(baseA, Texcoord / scaleBaseA ) + 
                               (tex2D(baseA, Texcoord / scaleDetailA )[3] - 0.5) )
                              * fvSplattingColor[3];   
                                                         
    float4 fvTotalAmbient   = (materialAmbient + fvLightColor ) *  fvBaseColor; 
    float4 fvTotalDiffuse   = materialDiffuse * fvBaseColor; 
   
    return( saturate( fvTotalAmbient + fvTotalDiffuse  ) );      
}


