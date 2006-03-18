// user settings
uniform sampler2D diffuseMap;
uniform sampler2D specMap;

varying vec4 diffuse,ambient;
varying vec3 normal,lightDir,halfVector;


void main()
{
    vec3 n,halfV;
    float NdotL,NdotHV;

    vec4 texColour = texture2D(diffuseMap, gl_TexCoord[0].st);
    float specularLevel = texture2D(specMap, gl_TexCoord[0].st).r;

    // The ambient term will always be present
    vec4 color = ambient*texColour;

    // a fragment shader can't write a varying variable, hence we need
    // a new variable to store the normalized interpolated normal
    n = normalize(normal);

    // compute the dot product between normal and ldir
    NdotL = max(dot(n,lightDir),0.0);

    if (NdotL > 0.0)
    {
        color += texColour * diffuse * NdotL;
        halfV = normalize(halfVector);
        NdotHV = max(dot(n,halfV),0.0);
        color += vec4(0.9, 0.9, 0.9, 1.0) * specularLevel * pow(NdotHV, 42.0);
    }

    gl_FragColor = color;
}

