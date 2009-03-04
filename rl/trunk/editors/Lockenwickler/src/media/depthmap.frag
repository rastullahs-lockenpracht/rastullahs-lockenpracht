uniform float fNear;
uniform float fFar;

void main()
{
      float fDepth = gl_FragCoord.z / gl_FragCoord.w;
      float temp = fNear / fFar;
      temp = temp * fDepth;
      float fColor = 1.0 / temp;
      //float fColor = 1.0 - smoothstep(fNear, fFar, fDepth);
      gl_FragColor = vec4(fColor, fColor, fColor, 1.0);
}
