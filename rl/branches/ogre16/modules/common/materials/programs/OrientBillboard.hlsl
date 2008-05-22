struct VS_OUTPUT
{
  float4 Pos        : POSITION;
  float2 UV         : TEXCOORD0;
  float4 Color      : COLOR;
};

VS_OUTPUT main_vs(
    in float4 Pos : POSITION,
    in float2 Tex : TEXCOORD0,
    in float3 Normal : NORMAL,
    uniform float4x4 worldViewProj_matrix,
    uniform float3 camPos,
    uniform float4 lightPosition,
    uniform float4 lightDiffuse,
    uniform float4 ambientLight,
    uniform float4 materialDiffuse,
    uniform float4 materialAmbient)
{ 
    VS_OUTPUT Out;

    // Find new position aligned to camera...
    float3 camDir = normalize(float3(camPos.x, 0.0, camPos.z));
    float3 upDir = float3(0.0, 1.0, 0.0);
    float3 rightDir = cross(camDir, upDir);

    float3 right = rightDir * length(float3(Pos.x, 0.0, Pos.z));
    right *= sign(Pos.x);

    float4 new_pos = float4(right.x, Pos.y, right.z, 1.0);

    Out.Pos = mul(worldViewProj_matrix, new_pos);
    Out.UV = Tex.xy;

    // Light the vertex...
    float3 lightDir = normalize(lightPosition - new_pos);
    float3 eyeDir = camPos - new_pos;

    float4 color = ambientLight * materialAmbient;
    float NdotL = max(dot(Normal, lightDir), 0.0);

    color += lightDiffuse * materialDiffuse * NdotL;
    Out.Color = color;

    return Out;
}

