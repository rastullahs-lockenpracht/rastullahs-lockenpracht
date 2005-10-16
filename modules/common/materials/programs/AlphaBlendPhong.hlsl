// custom shader parameters
float g_detailScale;

// used matrices
float4x4 g_World;
float4x4 g_WorldViewProjection;

// material attributes
float4 g_MaterialAmbientColor;
float4 g_MaterialDiffuseColor;

// light attributes
float4 g_LightPos;
float4 g_LightAmbient;
float4 g_LightDiffuse;

texture g_Detail1;
texture g_Alpha2;
texture g_Detail2;
texture g_Alpha3;
texture g_Detail3;

// texture samplers
sampler detail1Sampler = 
sampler_state
{
    Texture = <g_Detail1>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = Repeat;
    AddressV = Repeat;
};

sampler alpha2Sampler = 
sampler_state
{
    Texture = <g_Alpha2>;    
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};
sampler detail2Sampler = 
sampler_state
{
    Texture = <g_Detail2>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = Repeat;
    AddressV = Repeat;
};

sampler alpha3Sampler = 
sampler_state
{
    Texture = <g_Alpha3>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

sampler detail3Sampler = 
sampler_state
{
    Texture = <g_Detail3>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = Repeat;
    AddressV = Repeat;
};

struct VS_INPUT
{
    float4 position  : POSITION;
    float3 normal    : NORMAL;
    float2 texCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 position : POSITION;
    float2 detailUV : TEXCOORD0;
    float2 alphaUV  : TEXCOORD1;
    float3 normal   : TEXCOORD2;
    float4 lightPos : TEXCOORD3;
};

VS_OUTPUT main_vp(VS_INPUT IN)
{
    VS_OUTPUT OUT;

    OUT.detailUV = g_detailScale * IN.texCoord;
    OUT.alphaUV = IN.texCoord;

    // vertex normal in eye space
    OUT.normal = normalize(mul(IN.normal, g_WorldViewProjection));
    OUT.lightPos = normalize(g_LightPos);
    OUT.position = mul(IN.position, g_WorldViewProjection);

    return OUT;
}

struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;
};

PS_OUTPUT main_fp(VS_OUTPUT IN)
{
    PS_OUTPUT OUT;

    // base texture
    float4 currentColor = tex2D(detail1Sampler, IN.detailUV);

    // alpha map of second detail texture
    float alpha = tex2D(alpha2Sampler, IN.alphaUV).a;
    // alpha blend second detail with current colour
    currentColor = (1.0 - alpha)*currentColor +
        alpha*tex2D(detail2Sampler, IN.detailUV);

    // alpha map of third detail texture
    alpha = tex2D(alpha3Sampler, IN.alphaUV).a;
    // alpha blend third detail with current colour
    currentColor = (1.0 - alpha)*currentColor +
        alpha*tex2D(detail3Sampler, IN.detailUV);

    // compute the lighting
    float4 color = g_LightAmbient * g_MaterialAmbientColor;
    float nl = dot(normalize(IN.normal), IN.lightPos);
    if (nl > 0.0)
    {
        color += nl * g_LightDiffuse * g_MaterialDiffuseColor;
    }
    
    OUT.RGBColor = currentColor * color;
    
    return OUT;
}

