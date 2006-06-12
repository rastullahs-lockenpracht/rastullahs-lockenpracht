uniform vec4 fvAmbient;
uniform vec4 fvDiffuse;

uniform float scaleBase;
uniform float scaleBaseR;
uniform float scaleBaseG;
uniform float scaleBaseB;
uniform float scaleBaseA;
uniform float scaleDetail;
uniform float scaleDetailR;
uniform float scaleDetailG;
uniform float scaleDetailB;
uniform float scaleDetailA;


uniform sampler2D splattingMap;
uniform sampler2D lightingMap;
uniform sampler2D base;
uniform sampler2D baseR;
uniform sampler2D baseG;
uniform sampler2D baseB;
uniform sampler2D baseA;

varying vec2 Texcoord;

void main( void )
{     
   vec4  fvSplattingColor = texture2D( splattingMap, Texcoord );
   vec4  fvLightColor     = ( texture2D( lightingMap, Texcoord ) - 0.5 ) * 2.0;
   
   vec4  fvBaseColor      = texture2D( base, Texcoord * scaleBase ) +
                             ( ( texture2D( base, Texcoord * scaleDetail )[3] - 0.5 ) * 2.0 );
   
   fvBaseColor            *= 1.0-fvSplattingColor[0]; 
   fvBaseColor            += ( texture2D(baseR, Texcoord * scaleBaseR ) + 
                               ((texture2D(baseR, Texcoord * scaleDetailR )[3] - 0.5) * 2.0) )
                             * fvSplattingColor[0];
   
   fvBaseColor            *= 1.0-fvSplattingColor[1]; 
   fvBaseColor            += ( texture2D(baseG, Texcoord * scaleBaseG ) + 
                               ((texture2D(baseG, Texcoord * scaleDetailG )[3] - 0.5) * 2.0) )
                             * fvSplattingColor[1];
   
   fvBaseColor            *= 1.0-fvSplattingColor[2]; 
   fvBaseColor            += ( texture2D(baseB, Texcoord * scaleBaseB ) + 
                               ((texture2D(baseB, Texcoord * scaleDetailB )[3] - 0.5) * 2.0) )
                             * fvSplattingColor[2];
                             
   fvBaseColor            *= 1.0-fvSplattingColor[3]; 
   fvBaseColor            += ( texture2D(baseB, Texcoord * scaleBaseA ) + 
                               ((texture2D(baseB, Texcoord * scaleDetailA )[3] - 0.5) * 2.0) )
                             * fvSplattingColor[3];
   
   vec4  fvTotalAmbient   = fvBaseColor * ( fvAmbient + fvLightColor ); 
   vec4  fvTotalDiffuse   = fvDiffuse * fvBaseColor;
  
   gl_FragColor = ( fvTotalAmbient + fvTotalDiffuse );  
}