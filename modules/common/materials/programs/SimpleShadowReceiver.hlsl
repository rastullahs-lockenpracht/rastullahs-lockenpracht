struct VS_OUTPUT 
{
    float4 Position :        POSITION0;
    float2 Texcoord :        TEXCOORD0;
    float4 Color    :        COLOR;
};

VS_OUTPUT vs_main(
    float4 Position : POSITION0,
    uniform float4x4 matWorldViewProjection,
    uniform float4x4 matWorld,
    uniform float4x4 matTextureViewProjection )
{
    VS_OUTPUT Output;

    Output.Position = mul(matWorldViewProjection, Position);
    float4 pos = mul(matWorld, Position);
    Output.Texcoord = mul(matTextureViewProjection, pos);
	Output.Color = float4(1,1,1,1);
    return Output;
}

