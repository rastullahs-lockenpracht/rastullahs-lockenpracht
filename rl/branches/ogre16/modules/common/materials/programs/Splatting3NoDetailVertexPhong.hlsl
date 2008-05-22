struct VS_OUTPUT 
{
   float4 Position :        POSITION;
   float2 Texcoord :        TEXCOORD0;
   float4 Color :           COLOR0;
};

VS_OUTPUT vs_main( 
    float4 Position : POSITION,
    float2 Texcoord : TEXCOORD0,
    float3 Normal :   NORMAL,

    uniform float4 vLightPosition,
    uniform float4 cLightDiffuse,
    uniform float4 cAmbientLight,

    uniform float4 cMaterialDiffuse,
    uniform float4 cMaterialAmbient,
    uniform float4x4 matWorldViewProjection )
{
    VS_OUTPUT Output;

    float3 lightDir = normalize(vLightPosition.xyz -  (Position * vLightPosition.w));

    // Move to World
    float NdotL = clamp( dot( normalize( Normal ), lightDir ), 0.0, 1.0);
    Output.Position         = mul( matWorldViewProjection, Position );
    Output.Texcoord         = Texcoord;
    // Light the vertex
    Output.Color = saturate( ( cAmbientLight * cMaterialAmbient ) + ( cLightDiffuse * cMaterialDiffuse * NdotL ) );
    return( Output );
}

float4 ps_main(     
    float2 Texcoord :   TEXCOORD0,
    float4 Color:       COLOR0,
    uniform float scaleBase,
    uniform float scaleBaseR,
    uniform float scaleBaseG,

    uniform sampler2D splattingMap,
    uniform sampler2D base,
    uniform sampler2D baseR,
    uniform sampler2D baseG ) : COLOR0
{        
    float4 fvSplattingColor = tex2D( splattingMap, Texcoord );
    float4 fvBaseColor      = tex2D( base, Texcoord / scaleBase );

    fvBaseColor            *= 1.0-fvSplattingColor[0]; 
    fvBaseColor            += ( tex2D(baseR, Texcoord / scaleBaseR ) )
                              * fvSplattingColor[0];
                              
    fvBaseColor            *= 1.0-fvSplattingColor[1]; 
    fvBaseColor            += ( tex2D(baseG, Texcoord / scaleBaseG ) )
                              * fvSplattingColor[1];

    return( Color * fvBaseColor );      
}


