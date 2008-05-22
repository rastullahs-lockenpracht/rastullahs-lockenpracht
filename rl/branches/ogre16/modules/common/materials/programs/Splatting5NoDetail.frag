varying vec4 diffuse,ambient;

uniform float scaleBase;
uniform float scaleBaseR;
uniform float scaleBaseG;
uniform float scaleBaseB;
uniform float scaleBaseA;

uniform sampler2D splattingMap;
uniform sampler2D lightingMap;
uniform sampler2D base;
uniform sampler2D baseR;
uniform sampler2D baseG;
uniform sampler2D baseB;
uniform sampler2D baseA;

void main( void )
{      
   vec4 fvSplattingColor  = texture2D( splattingMap, gl_TexCoord[0].st );
   vec4 fvLightColor      = ( texture2D( lightingMap, gl_TexCoord[0].st ) - 0.5 ) * 2.0;
   
   vec4 fvBaseColor       = texture2D( base, gl_TexCoord[0].st / scaleBase );
   
   fvBaseColor            *= 1.0-fvSplattingColor[0]; 
   fvBaseColor            += ( texture2D(baseR, gl_TexCoord[0].st / scaleBaseR) )
                             * fvSplattingColor[0];
   
   fvBaseColor            *= 1.0-fvSplattingColor[1]; 
   fvBaseColor            += ( texture2D(baseG, gl_TexCoord[0].st / scaleBaseG) )
                             * fvSplattingColor[1];
   
   fvBaseColor            *= 1.0-fvSplattingColor[2]; 
   fvBaseColor            += ( texture2D(baseB, gl_TexCoord[0].st / scaleBaseB)  )
                             * fvSplattingColor[2];
                             
   fvBaseColor            *= 1.0-fvSplattingColor[3]; 
   fvBaseColor            += ( texture2D(baseB, gl_TexCoord[0].st / scaleBaseA)  )
                             * fvSplattingColor[3];
   
   vec4  fvTotalAmbient   = fvBaseColor * ( ambient + fvLightColor ); 
   vec4  fvTotalDiffuse   = diffuse * fvBaseColor;
  
   gl_FragColor = ( fvTotalAmbient + fvTotalDiffuse );  
}