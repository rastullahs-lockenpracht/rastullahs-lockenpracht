float4 vLightPosition;
float4 cLightDiffuse;
float4 cAmbientLight;

float4 cMaterialDiffuse;
float4 cMaterialAmbient;
float4x4 matWorldViewProjection;

struct VS_INPUT 
{
   float4 Position : POSITION;
   float2 Texcoord : TEXCOORD0;
   float3 Normal :   NORMAL;
};

struct VS_OUTPUT 
{
   float4 Position :        POSITION;
   float2 Texcoord :        TEXCOORD0;
   float4 Color :           COLOR0;
};

VS_OUTPUT vs_main( VS_INPUT Input )
{
    VS_OUTPUT Output;

    float3 lightDir = normalize(vLightPosition.xyz -  (Input.Position * vLightPosition.w));

    // Move to World
    float NdotL = clamp( dot( normalize( Input.Normal ), lightDir ), 0.0, 1.0);
    Output.Position         = mul( matWorldViewProjection, Input.Position );
    Output.Texcoord         = Input.Texcoord;
    // Light the vertex
    Output.Color = saturate( ( cAmbientLight * cMaterialAmbient ) + ( cLightDiffuse * cMaterialDiffuse * NdotL ) );
    return( Output );
}

float scaleBase;
float scaleBaseR;
float scaleBaseG;

sampler2D splattingMap;
sampler2D base;
sampler2D baseR;
sampler2D baseG;

struct PS_INPUT 
{
   float2 Texcoord :        TEXCOORD0;
   float4 Color:   COLOR0;
};

float4 ps_main( PS_INPUT Input ) : COLOR0
{        
    float4 fvSplattingColor = tex2D( splattingMap, Input.Texcoord );
    float4 fvBaseColor      = tex2D( base, Input.Texcoord / scaleBase );

    fvBaseColor            *= 1.0-fvSplattingColor[0]; 
    fvBaseColor            += ( tex2D(baseR, Input.Texcoord / scaleBaseR ) )
                              * fvSplattingColor[0];
                              
    fvBaseColor            *= 1.0-fvSplattingColor[1]; 
    fvBaseColor            += ( tex2D(baseG, Input.Texcoord / scaleBaseG ) )
                              * fvSplattingColor[1];

    return( Input.Color * fvBaseColor );      
}


