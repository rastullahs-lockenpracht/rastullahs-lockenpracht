uniform vec4 LightColor_0;
uniform vec4 LightColor_1;
uniform vec4 LightColor_2;
uniform vec4 groundcolor;
uniform vec4 skycolor;
uniform vec4 SpecColor;
uniform float SpecExp;

varying vec4 LightDir_0;
varying vec3 HalfVect_0;
varying vec4 LightDir_1;
varying vec3 HalfVect_1;
varying vec4 LightDir_2;
varying vec3 HalfVect_2;
varying vec3 SkyDir;

uniform sampler2D DiffuseMap;
uniform sampler2D NormalMap;

void main(void)
{
   vec3 normal = texture2D(NormalMap,gl_TexCoord[0].xy).rgb * 2.0 -1.0; 
   vec4 ambient= mix(skycolor , groundcolor, ((dot(normal, SkyDir) + 1.0)/2.0));
   
   
   float specAttn = 0.0;
   float specAttn1 = 0.0;
   float specAttn2 = 0.0;
   vec4 AngleAttn;
   vec4 AngleAttn1;
   vec4 AngleAttn2;
     
   AngleAttn= vec4(clamp(dot(normal,LightDir_0.xyz),0.0,1.0));
   AngleAttn1= vec4(clamp(dot(normal,LightDir_1.xyz),0.0,1.0));
   AngleAttn2= vec4(clamp(dot(normal,LightDir_2.xyz),0.0,1.0)); 
   
   
   
   if (AngleAttn.r > 0.0)
   {
      specAttn = float(LightColor_0 * pow(clamp(dot(normal,normalize(HalfVect_0)),0.0,1.0),SpecExp)* LightDir_0.w);
      AngleAttn *= LightColor_0 * LightDir_0.w;
   }
   
   if (AngleAttn1.r > 0.0)
   {
      specAttn1  = float(LightColor_1 *  pow(clamp(dot(normal,normalize(HalfVect_1)),0.0,1.0),SpecExp)* LightDir_1.w);
      AngleAttn1 *= LightColor_1 * LightDir_1.w;
   }
  
   if (AngleAttn2.r > 0.0)
   {
      specAttn2  = float(LightColor_2 *  pow(clamp(dot(normal,normalize(HalfVect_2)),0.0,1.0),SpecExp)* LightDir_2.w);
      AngleAttn2 *= LightColor_2 * LightDir_2.w;
   }
  
  
   vec4 Diffuse = AngleAttn + AngleAttn1 + AngleAttn2;
   vec4 specular = SpecColor * (specAttn + specAttn1 + specAttn2) * Diffuse;
   
   vec4 color = texture2D(DiffuseMap,gl_TexCoord[0].xy);

   
   gl_FragColor =  vec4((color.rgb * (ambient.rbg + Diffuse.rgb)) +specular.rgb ,color.a);
   
}