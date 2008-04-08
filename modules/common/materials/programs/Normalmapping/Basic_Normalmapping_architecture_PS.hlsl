uniform float4 LightColor_0;
uniform float4 LightColor_1;
//uniform float4 LightColor_2;
uniform float4 groundcolor;
uniform float4 skycolor;
uniform float4 SpecColor;
uniform float SpecExp;
uniform sampler DiffuseMap;
uniform sampler NormalMap;
uniform sampler AOMap;


float4 ps_main(
               float4 UV        : TEXCOORD0,// uv 0 +1
               float4 LightDir_0  : TEXCOORD1,
               float3 HalfVect_0  : TEXCOORD2,
               float4 LightDir_1 : TEXCOORD3,
               float3 HalfVect_1 : TEXCOORD4,
               //float4 LightDir_2 : TEXCOORD5,
               //float4 HalfVect_2 : TEXCOORD6, //uv2.x
               float2 UV2		 : TEXCOORD5,
               float4 SkyDir     : TEXCOORD6, //uv2.y
               float3 EyeVect    : TEXCOORD7
               ) : COLOR0
{ 

   float Height = tex2D(NormalMap,UV.xy).a *2 -1;
   float3 NewEye= normalize(float3(EyeVect.x,-EyeVect.y,EyeVect.z));
   float2 offset= NewEye.xy * Height * 0.01;
   float2 NewUV = UV.xy+offset;
   
   float3 normal = tex2D(NormalMap,NewUV.xy).rgb *2 -1;
   float4 ambient= lerp(skycolor , groundcolor, ((dot(normal, -LightDir_0.xyz) + 1.0)/2.0));
   float4 ao = ((tex2D(AOMap,UV.zw))/1.5 +0.33);
   	
   float specAttn;
   float specAttn1;
   float4 AngleAttn;
   float4 AngleAttn1;
   
   
   AngleAttn= saturate(dot(normal,LightDir_0.xyz));
   AngleAttn1= saturate(dot(normal,LightDir_1.xyz));
   
   if (AngleAttn.r > 0.0)
   {
      specAttn = LightColor_0 * pow(clamp(dot(normal,normalize(HalfVect_0)),0,1),SpecExp)* LightDir_0.w;
      AngleAttn *= LightColor_0 * LightDir_0.w;
   }
   
   if (AngleAttn1.r > 0.0)
   {
      specAttn1  =LightColor_1 *  pow(clamp(dot(normal,normalize(HalfVect_1)),0,1),SpecExp)* LightDir_1.w;
      AngleAttn1 *= LightColor_1 * LightDir_1.w;
   }
  

  
   float4 light1 = pow(AngleAttn,2) + SpecColor * (specAttn) * AngleAttn;
   float4 light2 = AngleAttn1 + SpecColor * (specAttn1) * AngleAttn1;
   //float4 Diffuse =  + AngleAttn1 ;
   //float4 specular = SpecColor * (specAttn + specAttn1 ) * Diffuse;
   
   
   //return (tex2D(DiffuseMap,NewUV) * (ambient + Diffuse)*ao) +specular;
   return tex2D(DiffuseMap,NewUV)*( (ambient+ light1) *ao +light2) ;
   //return saturate(ambient);
}



