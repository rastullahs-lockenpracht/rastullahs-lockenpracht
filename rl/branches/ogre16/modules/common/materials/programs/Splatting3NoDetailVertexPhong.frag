uniform float scaleBase;
uniform float scaleBaseR;
uniform float scaleBaseG;
uniform float scaleBaseB;
uniform float scaleBaseA;

uniform sampler2D splattingMap;
uniform sampler2D base;
uniform sampler2D baseR;
uniform sampler2D baseG;
uniform sampler2D baseB;
uniform sampler2D baseA;

varying vec3 normal;
varying vec3 lightDir;
varying vec4 ambient;
varying vec4 diffuse;

void main( void )
{      
   vec4 fvSplattingColor  = texture2D( splattingMap, gl_TexCoord[0].st );
   vec3 n = normalize(normal);
   float fNDotL           = max(dot(n,lightDir),0.0);

   vec4 fvBaseColor       = texture2D( base, gl_TexCoord[0].st / scaleBase );
   
   fvBaseColor            *= 1.0-fvSplattingColor[0]; 
   fvBaseColor            += ( texture2D(baseR, gl_TexCoord[0].st / scaleBaseR) )
                             * fvSplattingColor[0];
   
   fvBaseColor            *= 1.0-fvSplattingColor[1]; 
   fvBaseColor            += ( texture2D(baseG, gl_TexCoord[0].st / scaleBaseG) )
                             * fvSplattingColor[1];
   
   vec4  fvTotalAmbient   = fvBaseColor * ambient ; 
   vec4  fvTotalDiffuse   = diffuse * fNDotL * fvBaseColor;
  
   gl_FragColor = ( fvTotalAmbient + fvTotalDiffuse );  
}