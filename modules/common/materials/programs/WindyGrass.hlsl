float4x4 matWorldViewProjection;
float4x4 matWorld;

float fxTime;
float fzTime;
float fMaxMovementFactor;

float4 lightDirection;
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

VS_OUTPUT vs_main( VS_INPUT Input )
{
    VS_OUTPUT Output;

    // Calculate Offset
    float offset = (1.0-Input.TexCoord.y)*fMaxMovementFactor;
    float4 worldPos = mul( matWorld, Input.Position);
    // Apply 'random' value for position to disturb 
    fxTime = -0.5 + (fxTime + abs(sin(worldPos.x+worldPos.z)) )/2.0;
    fzTime = -0.5 + (fzTime + abs(cos(worldPos.x+worldPos.z)) )/2.0;
    // Apply Offset
    Input.Position.x += fxTime*offset;
    Input.Position.z += fzTime*offset;

    // Move to World
    Output.Position = mul( matWorldViewProjection, Input.Position );
    Output.TexCoord = Input.TexCoord;

    // Light the vertex
    float NdotL = saturate(  dot( normalize( Input.Normal ), normalize(lightDirection ) ) );
    float4 color = ( ambientLight * materialAmbient ) + ( lightDiffuse * materialDiffuse * NdotL );
    Output.Color = color;

    // return
    return( Output );
}