uniform float fNear;
uniform float fFar;
varying float depth;

void main()
{
      //float fDepth = gl_FragCoord.z / gl_FragCoord.w;
      //float fColor = 1.0 - smoothstep(fNear, fFar, fDepth);
      float fDepth = depth / gl_FragCoord.w;
      float fColor = smoothstep(gl_DepthRange.near, gl_DepthRange.far, fDepth);
      gl_FragColor = vec4(fColor, fColor, fColor, 1.0);

}
