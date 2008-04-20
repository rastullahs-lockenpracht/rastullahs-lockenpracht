
float4 suncolor_outside;
sampler2D Clouds;
float4 cloudcolor;
float cloudexp;
float cloudambient;
float transexp;
float3 sundir;

float4 ps_main(float3 dir      :TEXCOORD0,
               float2 UV :TEXCOORD1
               ) : COLOR0
{  

   float4 cloudtex = tex2D(Clouds,UV);
   float3 normal =normalize( float3(cloudtex.rg ,1)/2+0.5);
   dir = normalize(dir);
   sundir = normalize(sundir);
   float cloudlight = pow(saturate(dot(dir,-sundir)+1)/2,cloudexp);
   
   float translusent = pow(saturate(dot(dir,sundir)),transexp) * cloudtex.b;
   float4 transcolor = (translusent) *suncolor_outside;
   
   
   return float4(cloudcolor.rgb*max(cloudlight ,cloudambient) + transcolor,cloudtex.a) ;
   
}



