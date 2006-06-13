float3 fvLightPosition;
float4x4 matWorldViewProjection;

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
   
};

struct VS_OUTPUT 
{
   float4 Position :        POSITION0;
   float2 Texcoord :        TEXCOORD0;
   
};

VS_OUTPUT vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;

   Output.Position         = mul( matWorldViewProjection, Input.Position );
   Output.Texcoord         = Input.Texcoord;
      
   return( Output );
}



float4 materialAmbient;
float4 materialDiffuse;

float scaleBase;
float scaleBaseR;
float scaleBaseG;
float scaleBaseB;
float scaleBaseA;
float scaleDetail;
float scaleDetailR;
float scaleDetailG;
float scaleDetailB;
float scaleDetailA;

sampler2D splattingMap;
sampler2D lightingMap;
sampler2D base;
sampler2D baseR;
sampler2D baseG;
sampler2D baseB;
sampler2D baseA;

struct PS_INPUT 
{
   float2 Texcoord :        TEXCOORD0;
   float3 Normal :          TEXCOORD3;
   
};

float4 ps_main( PS_INPUT Input ) : COLOR0
{        
    float4 fvSplattingColor = tex2D( splattingMap, Input.Texcoord );
    float4 fvLightColor     = ( tex2D( lightingMap, Input.Texcoord ) - 0.5 ) * 2.0;
   
    float4 fvBaseColor      = tex2D( base, Input.Texcoord / scaleBase ) + 
                               (tex2D(base, Input.Texcoord / scaleDetail )[3] - 0.5);

    fvBaseColor            *= 1.0-fvSplattingColor[0]; 
    fvBaseColor            += ( tex2D(baseR, Input.Texcoord / scaleBaseR ) + 
                               (tex2D(baseR, Input.Texcoord / scaleDetailR )[3] - 0.5) )
                              * fvSplattingColor[0];
                              
    fvBaseColor            *= 1.0-fvSplattingColor[1]; 
    fvBaseColor            += ( tex2D(baseG, Input.Texcoord / scaleBaseG ) + 
                               (tex2D(baseG, Input.Texcoord / scaleDetailG )[3] - 0.5) )
                              * fvSplattingColor[1];
                              
    fvBaseColor            *= 1.0-fvSplattingColor[2]; 
    fvBaseColor            += ( tex2D(baseB, Input.Texcoord / scaleBaseB ) + 
                               (tex2D(baseB, Input.Texcoord / scaleDetailB )[3] - 0.5) )
                              * fvSplattingColor[2];
                              
    fvBaseColor            *= 1.0-fvSplattingColor[3]; 
    fvBaseColor            += ( tex2D(baseA, Input.Texcoord / scaleBaseA ) + 
                               (tex2D(baseA, Input.Texcoord / scaleDetailA )[3] - 0.5) )
                              * fvSplattingColor[3];   
                                                         
    float4 fvTotalAmbient   = (materialAmbient + fvLightColor ) *  fvBaseColor; 
    float4 fvTotalDiffuse   = materialDiffuse * fvBaseColor; 
   
    return( saturate( fvTotalAmbient + fvTotalDiffuse  ) );      
}


