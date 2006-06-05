struct VS_OUTPUT
{
  float4 Pos        : POSITION;
  float2 UV         : TEXCOORD0;
  float3 Normal     : TEXCOORD1;
};

VS_OUTPUT main_vs(
    in float4 Pos : POSITION,
    in float2 Tex : TEXCOORD0,
    uniform float4x4 worldViewProj_matrix,
    uniform float3 camPos)
{ 
    VS_OUTPUT Out;
    float3 camDir = normalize(float3(camPos.x, 0.0, camPos.z));
    float3 upDir = float3(0.0, 1.0, 0.0);
    float3 rightDir = cross(camDir, upDir);

    float3 right = rightDir * length(float3(Pos.x, 0.0, Pos.z));
    right *= sign(Pos.x);

    float4 new_pos = float4(right.x, Pos.y, right.z, 1.0);

    Out.Pos = mul(worldViewProj_matrix, new_pos);
    Out.UV = Tex.xy;
    Out.Normal = mul(worldViewProj_matrix, float4(0.0, 1.0, 0.0, 1.0));

    return Out;
}

