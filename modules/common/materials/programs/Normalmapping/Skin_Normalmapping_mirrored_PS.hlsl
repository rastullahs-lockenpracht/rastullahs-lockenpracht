uniform float4 LightColor_0;
uniform float4 LightColor_1;
uniform float4 LightColor_2;
uniform float4 groundcolor;
uniform float4 skycolor;
uniform float4 SkinColor;
uniform float SpecExp;
uniform float skinscale;
uniform sampler2D DiffuseMap;
uniform sampler2D NormalMap;
uniform sampler2D SpecMap;
uniform sampler1D LookupSkin;
uniform sampler2D skintexture;


float4 ps_main(
               float2 UV         : TEXCOORD0,
               float4 LightDir_0 : TEXCOORD1,
               float3 HalfVect_0 : TEXCOORD2,
               float4 LightDir_1 : TEXCOORD3,
               float3 HalfVect_1 : TEXCOORD4,
               float4 LightDir_2 : TEXCOORD5,
               float3 HalfVect_2 : TEXCOORD6,
               float3 SkyDir     : TEXCOORD7
               ) : COLOR0
{ 

   float3 normal = (tex2D(NormalMap,UV.xy).rgb *2 -1) + tex2D(skintexture,UV.xy*skinscale)-0.5;
   if (UV.x > 1)
   {
	  normal = float3(normal.x, -normal.y, normal.z);
   }
   float4 ambient= lerp(skycolor , groundcolor, ((dot(normal, SkyDir) + 1.0)/2.0));

   
   
   float specAttn;
   float4 AngleAttn;
   float4 AngleAttn1;
   float4 AngleAttn2;
   
   AngleAttn= saturate(dot(normal,LightDir_0.xyz));
   AngleAttn1= saturate(dot(normal,LightDir_1.xyz));
   AngleAttn2= saturate(dot(normal,LightDir_2.xyz));
   
   float4 Diffuse;
   

      specAttn =LightColor_0 *  pow(clamp(dot(normal,normalize(HalfVect_0)),0,1),SpecExp)* LightDir_0.w;
      AngleAttn = tex1D(LookupSkin, AngleAttn);
      Diffuse = AngleAttn* LightColor_0 * LightDir_0.w;  
   

      specAttn  +=LightColor_1 *  pow(clamp(dot(normal,normalize(HalfVect_1)),0,1),SpecExp)* LightDir_1.w;
      AngleAttn1 = tex1D(LookupSkin, AngleAttn1);
      Diffuse += AngleAttn1* LightColor_1 * LightDir_1.w;

  

      specAttn  +=LightColor_2 *  pow(clamp(dot(normal,normalize(HalfVect_2)),0,1),SpecExp)* LightDir_2.w;
      AngleAttn2 = tex1D(LookupSkin, AngleAttn2);
      Diffuse += AngleAttn2* LightColor_2 * LightDir_2.w;

  
  
   //float4 Diffuse = AngleAttn + AngleAttn1 + AngleAttn2;
   float4 specular = tex2D(SpecMap, UV.xy) * specAttn * (float(Diffuse.r + Diffuse.g + Diffuse.b)/3);
   float4 diffuseColor = (tex2D(DiffuseMap,UV.xy)) ;
   
   return float4(((SkinColor.rgb-0.5 + diffuseColor.rgb) * (ambient + Diffuse)) +specular.rgb , diffuseColor.a);
}



