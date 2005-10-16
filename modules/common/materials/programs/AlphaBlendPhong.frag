//varying vec3 lightDir, normal;
uniform sampler2D detail1, alpha2, detail2, alpha3, detail3;

varying vec3 normal, lightDir;
varying vec4 diffuse, ambient;

void main()
{
    // base texture
    vec4 currentColor = texture2D(detail1, gl_TexCoord[0].st);

    // alpha map of second detail texture
    float alpha = texture2D(alpha2, gl_TexCoord[1].st).a;
    // alpha blend second detail with current colour
    currentColor = (1.0 - alpha)*currentColor +
        alpha*texture2D(detail2, gl_TexCoord[0].st);

    // alpha map of third detail texture
    alpha = texture2D(alpha3, gl_TexCoord[1].st).a;
    // alpha blend third detail with current colour
    currentColor = (1.0 - alpha)*currentColor +
        alpha*texture2D(detail3, gl_TexCoord[0].st);

    // compute the lighting
    vec4 color = ambient;
    float nl = dot(normalize(normal),lightDir);
    if (nl > 0.0)
    {
        color += nl * diffuse;
    }
    
    gl_FragColor = currentColor * color;
}

