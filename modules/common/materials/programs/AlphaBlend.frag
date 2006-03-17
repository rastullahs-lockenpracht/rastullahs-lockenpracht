//varying vec3 lightDir, normal;
uniform sampler2D detailBase, detailG, detailB;
uniform sampler2D blendmap;

void main()
{
    vec4 blendFactors = texture2D(blendmap, gl_TexCoord[1].st);

    // base texture
    //vec4 currentColor = vec4(1, 1, 1, 1);
    vec4 currentColor = texture2D(detailBase, gl_TexCoord[0].st);

    // alpha blend second detail with current colour
    currentColor = (1.0 - blendFactors.g)*currentColor +
        blendFactors.g*texture2D(detailG, gl_TexCoord[0].st);

    // alpha blend third detail with current colour
    currentColor = (1.0 - blendFactors.b)*currentColor +
        blendFactors.b*texture2D(detailB, gl_TexCoord[0].st);

    // apply lightmap
    currentColor *= blendFactors.a;

    gl_FragColor = currentColor * gl_Color;
}

