float4x4 matWorldViewProjection;

float fxTime;
float fzTime;
float fMaxMovementFactor;

float4 lightPosition;
float4 lightDiffuse;
float4 ambientLight;
float4 materialDiffuse;
float4 materialAmbient;

struct VS_INPUT 
{
    float4 Position : POSITION;
    float2 TexCoord : TEXCOORD0;
    float3 Normal   : NORMAL;
};

struct VS_OUTPUT 
{
    float4 Position : POSITION;
    float2 TexCoord : TEXCOORD0;
    float4 Color    : COLOR;
};

#define TWOPI 6.28318531f

VS_OUTPUT vs_main( VS_INPUT Input )
{
    VS_OUTPUT Output;

    // Calculate Offset
    float offset = (1.0-Input.TexCoord.y)*fMaxMovementFactor;
    // Apply 'random' value for position to disturb 
    fxTime = -0.5 + (fxTime + abs(sin(Input.Position.x/8+Input.Position.z/4)) )/2.0;
    fzTime = -0.5 + (fzTime + abs(cos(Input.Position.x/4+Input.Position.z/8)) )/2.0;
    // Apply Offset
    Input.Position.x += fxTime*offset;
    Input.Position.z += fzTime*offset;

    // Move to World
    Output.Position = mul( matWorldViewProjection, Input.Position );
    Output.TexCoord = Input.TexCoord;

    // Light the vertex
    float3 lightDir = normalize(lightPosition - Input.Position );

    float4 color = ambientLight * materialAmbient;
    float NdotL = max(dot(Input.Normal, lightDir), 0.0);

    color += lightDiffuse * materialDiffuse * NdotL;
    Output.Color = color;

    // return
     
    return( Output );
}