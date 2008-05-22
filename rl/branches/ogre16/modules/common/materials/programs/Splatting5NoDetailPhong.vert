varying vec3 normal;
varying vec3 lightDir;
varying vec4 ambient;
varying vec4 diffuse;
   
void main( void )
{
   gl_Position = ftransform();
   gl_TexCoord[0] = gl_MultiTexCoord0;
   lightDir = normalize(vec3(gl_LightSource[0].position));
   normal = normalize(gl_NormalMatrix * gl_Normal);

   // Compute the diffuse, ambient and globalAmbient terms
   diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
   ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
   ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient;
}