// Alpha blending shader with vertex lighting for one light
//uniform float detailScale;

void main()
{
    //ec3 normal, lightDir;
    //vec4 ambient, diffuse;
    //float NdotL;

    // apply detailScale to first texture coord
    //gl_TexCoord[0] = detailScale * gl_MultiTexCoord0;
    //gl_TexCoord[1] = gl_MultiTexCoord0;

    // compute lighting
    //ambient = gl_LightModel.ambient * gl_FrontMaterial.ambient;
    //normal = normalize(gl_NormalMatrix * gl_Normal);
    //lightDir = normalize(vec3(gl_LightSource[0].position));
    //NdotL = max(dot(normal, lightDir), 0.0);
    //diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;

    //gl_FrontColor = ambient + NdotL*diffuse;
    gl_FrontColor = vec4(1, 1, 1, 1);
    gl_Position = ftransform();
}


