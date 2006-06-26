float4x4 matWorldViewProjection;
float4x4 matWorld;
float4x4 matTextureViewProjection;

struct VS_INPUT 
{
    float4 Position : POSITION0;
};

struct VS_OUTPUT 
{
    float4 Position :        POSITION0;
    float2 Texcoord :        TEXCOORD0;
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
    VS_OUTPUT Output;

    Output.Position = mul(matWorldViewProjection, Input.Position);
    float4 pos = mul(matWorld, Input.Position);
    Output.Texcoord = mul(matTextureViewProjection, pos);
      
    return Output;
}

