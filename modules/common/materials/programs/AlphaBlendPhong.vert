uniform float detailScale;

varying vec3 normal, lightDir;
varying vec4 diffuse, ambient;

void main()
{
    gl_TexCoord[0] = detailScale * gl_MultiTexCoord0;
    gl_TexCoord[1] = gl_MultiTexCoord0;

    // compute ambient
    ambient = gl_LightModel.ambient * gl_FrontMaterial.ambient;
    //ambient += gl_LightSource[0].ambient * gl_FrontMaterial.ambient;
    // vertex normal in eye space
    normal = normalize(gl_NormalMatrix * gl_Normal);
    lightDir = normalize(gl_LightSource[0].position.xyz);
    // compute diffuse
    diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;

    gl_Position = ftransform();
}


