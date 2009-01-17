//input
attribute vec3 tangent;
uniform vec4 vViewPosition;
uniform mat4 matViewProjection;

uniform vec4 LightPosition_0;
uniform vec4 LightPosition_1;
uniform vec4 LightPosition_2;
uniform vec4 LightAttn_0;
uniform vec4 LightAttn_1;
uniform vec4 LightAttn_2;


//output
varying vec4 LightDir_0;
varying vec3 HalfVect_0;
varying vec4 LightDir_1;
varying vec3 HalfVect_1;
varying vec4 LightDir_2;
varying vec3 HalfVect_2;
varying vec3 SkyDir;


void main(void)
{
   gl_TexCoord[0] = gl_MultiTexCoord0;
   gl_Position = matViewProjection * gl_Vertex;
   
   mat3 TangentSpace;
   
   TangentSpace[0] = tangent;
   TangentSpace[1] = cross(tangent,gl_Normal.xyz);
   TangentSpace[2] = gl_Normal.xyz;
   
   vec3 eyeVector = normalize(vViewPosition - gl_Vertex).xyz;
   
   
   //Light 1   
   vec4 LightDir;
   LightDir.w = 0.0;
   LightDir.xyz = (LightPosition_0 - gl_Vertex).xyz * LightPosition_0.w;
   float Dist = length(LightDir.xyz);
   LightDir.xyz = LightDir.xyz /Dist;
   
   if (LightPosition_0.w != 0.0)
   {
      LightDir.w = clamp(1.0/(LightAttn_0.y +
                            LightAttn_0.z * Dist +
                            LightAttn_0.w * Dist * Dist)*clamp(LightAttn_0.x-Dist,0.0,1.0),0.0,1.0);
   }
   
   
   HalfVect_0 = TangentSpace * normalize(LightDir.xyz + eyeVector);
   LightDir_0.xyz = TangentSpace*(LightDir.xyz);
   LightDir_0.w = LightDir.w;
   
   
   //Light 2
   LightDir.xyz = (LightPosition_1 - gl_Vertex).xyz * LightPosition_1.w;
   Dist = length(LightDir.xyz);
   LightDir.xyz = LightDir.xyz /Dist;
   
   if (LightPosition_1.w != 0.0)
   {
      LightDir.w = clamp(1.0/(LightAttn_1.y +
                            LightAttn_1.z * Dist +
                            LightAttn_1.w * Dist * Dist)*clamp(LightAttn_1.x-Dist,0.0,1.0),0.0,1.0);
   }
   
   
   HalfVect_1 = TangentSpace * normalize(LightDir.xyz + eyeVector);
   LightDir_1.xyz = TangentSpace*(LightDir.xyz);
   LightDir_1.w = LightDir.w;
   
   
  //Light 3
   LightDir.xyz = (LightPosition_2 - gl_Vertex).xyz * LightPosition_2.w;
   Dist = length(LightDir.xyz);
   LightDir.xyz = LightDir.xyz /Dist;
   
   if (LightPosition_2.w != 0.0)
   {
      LightDir.w = clamp(1.0/(LightAttn_2.y +
                            LightAttn_2.z * Dist +
                            LightAttn_2.w * Dist * Dist)*clamp(LightAttn_2.x-Dist,0.0,1.0),0.0,1.0);
   }
   
   
   HalfVect_2 = TangentSpace * normalize(LightDir.xyz + eyeVector);
   LightDir_2.xyz = TangentSpace*(LightDir.xyz);
   LightDir_2.w = LightDir.w; 
   
   
   
   // SkyLight
   SkyDir    = TangentSpace * vec3(0,-1,0);
}