sampler Texture0;
samplerCUBE Texture1;
sampler Texture2;
float4 watercolor;
float reflectionexp;
float4 waterreflectcolor;

float4 Light1_color;
float4 Light2_color;
float4 Light3_color;

float4 ps_main(float4 UV        :TEXCOORD0,
               float3 eyeVect   :TEXCOORD1,
               float4 times     :TEXCOORD2,
               float3 Normal    :TEXCOORD3,
               float4 Color    : TEXCOORD4,
               float4 Half1    : TEXCOORD4,
   			   float4 Half2    : TEXCOORD5,
               float4 Half3    : TEXCOORD6
               ) : COLOR0
{   
   //return float4(tex2D(Texture0,UV).rgb, 0);
   
   
   float3 normal = /*normalize*/(Normal);
   float3 normalmap =tex2D(Texture0,UV.xy).rgb -0.5;
   float3 normalmap2 =tex2D(Texture0,UV.xy+0.3).rgb -0.5;
   float3 normalmap3 =tex2D(Texture0,UV.xy+0.6).rgb -0.5;
    
   //float blending = fSinTime0_X/2 +0.5; 
   normal.xy += normalmap.xy * times.x;
   normal.xy += normalmap2.xy * times.y;
   normal.xy += normalmap3.xy * times.z;
   normal = normalize(normal);
   normal.xy += (tex2D(Texture2, UV.zw).xy -0.5)* (1-times.w);
   //normal = normalize(normal);
   
   float4 spec= pow(saturate(dot(normal,normalize(Half1.xyz))),16)*Light1_color*Half1.w;
   spec		 += pow(saturate(dot(normal,normalize(Half2.xyz))),16)*Light2_color*Half2.w;
   spec		 += pow(saturate(dot(normal,normalize(Half3.xyz))),16)*Light3_color*Half3.w;
   
   float4 reflectcolor = float4(texCUBE(Texture1, reflect(eyeVect, normal)).rgb, 1)*waterreflectcolor;
   
   float4 endcolor = lerp(reflectcolor,watercolor, saturate(pow(dot(normalize(eyeVect),normal),reflectionexp)+0.2))+spec;
   //endcolor.a = 0.5;
   return endcolor;
   //return float4(spec.rgb,1);
   
   
}



