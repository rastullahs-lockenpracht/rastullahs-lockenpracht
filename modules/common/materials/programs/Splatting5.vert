varying vec4 diffuse,ambient;

void main( void )
{
    gl_Position = ftransform();
    gl_TexCoord[0] = gl_MultiTexCoord0;

    // Compute the diffuse, ambient and globalAmbient terms
    diffuse = gl_FrontMaterial.diffuse;
	ambient = gl_FrontMaterial.ambient;
}