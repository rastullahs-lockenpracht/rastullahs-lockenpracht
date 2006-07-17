float4x4 matWorldViewProjection;
float4x4 matWorld;

float4 vTimePacked;
float4 vLightPosition;
float4 cLightDiffuse;
float4 cAmbientLight;

float4 cMaterialDiffuse;
float4 cMaterialAmbient;
float fMaxMovementFactor;

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

    float4 worldPos = mul( matWorld, Input.Position)*31;
    // Apply 'random' value for position to disturb 
    vTimePacked[1] = vTimePacked[1] + sin(worldPos.z + vTimePacked[0] );
    vTimePacked[2] = vTimePacked[2] + cos(worldPos.x - vTimePacked[0] );

    // Calculate Offset  
    fMaxMovementFactor *= (1.0-Input.TexCoord.y);
    fMaxMovementFactor *= 1.0 + cos(-worldPos.x-worldPos.z-worldPos.y)*0.5;
    fMaxMovementFactor *= 0.5;
    // Apply Offset
    Input.Position.x += vTimePacked[1]*fMaxMovementFactor;
    Input.Position.z += vTimePacked[2]*fMaxMovementFactor;

    float3 lightDir = normalize(vLightPosition.xyz -  (Input.Position * vLightPosition.w));

    // Move to World
    float NdotL = clamp( dot( normalize( Input.Normal ), lightDir ), 0.0, 1.0);
    Output.Position = mul( matWorldViewProjection, Input.Position );
    Output.TexCoord = Input.TexCoord;

    // Light the vertex
    Output.Color = saturate( ( cAmbientLight * cMaterialAmbient ) + ( cLightDiffuse * cMaterialDiffuse * NdotL ) );

    // Return
    return( Output );
}