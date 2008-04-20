float4 suncolor_inside;
float3 sundir;
float sunexp;
float sunglowpower;
float4 suncolor_outside;

float4 ps_main(float3 dir      :TEXCOORD0,
               float4 skycolor :TEXCOORD1
               ) : COLOR0
{  
   float4 suncolor = float4(0,0,0,1);
   float sunlight_out = pow(saturate(dot(normalize(sundir), normalize(dir))),sunexp);
   float sunlight_in = pow(saturate(dot(normalize(sundir), normalize(dir))),sunglowpower);
   
   suncolor +=suncolor_outside * sunlight_out;
   suncolor +=suncolor_inside * sunlight_in;
   //suncolor = lerp(suncolor, suncolor_inside, sunlight_in);
   //float4 suncolor = lerp(suncolor_outside,suncolor_inside,  sunlight_out)*sunlight_in;  
   //return lerp(skycolor,suncolor,sunlight );
   return (skycolor + suncolor);
   
}



