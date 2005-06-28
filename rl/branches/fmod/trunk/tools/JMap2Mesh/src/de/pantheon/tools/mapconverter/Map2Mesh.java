
package de.pantheon.tools.mapconverter;

import java.io.File;
import  de.pantheon.tools.Tools;

/*
 * Created on 25.10.2004
 *
 * To change the template for this generated file go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */

/**
 * @author LopLop
 * 
 * 2004
 */
public class Map2Mesh
{

    public static void main(String[] args)
    {
        if (args.length < 1 || Tools.contains(args, "-help")
                || Tools.contains(args, "--help"))
        {
            System.out.println("\nUsage: Map2Mesh [switches] mapfile\n");
            System.out.println("Where [switches] are:");
            System.out.println("  -tD[directory]\twhere the textures can be found, default \"./textures\"");
            System.out.println("  -tI[ignoreList]\tthe Textures which are to be skipped, separated by ';'");
            System.out.println("  -oD[directory]\tthe Directory where the meshes/material will be stored");
            System.out.println("  -oN[name]\t\tthe basename of the outputfiles");
            System.out.println("  -mS[s]\t\tScale the Mapfile");
            System.out.println("  -hl\t\tExpects a HalfLife Map");
            System.out.println("  -q3\t\tExpects a Quake3 Map");
            System.out.println("\nNotes:\n\tTextures have to be in TGA, PNG, JPG or GIF Format, named as in the map");
            System.out.println("\nFor Example: ");
            System.out.println("\t Map2Mesh " +
            		"-tD\"C:\\Dokumente und Einstellungen/LopLop/Desktop/rl_dist/media/textures\" " +
            		"-oD\"C:\\Dokumente und Einstellungen/LopLop/Desktop/MV\" " +
            		"\"C:\\Dokumente und Einstellungen/LopLop/Desktop/map2mesh/a.map\"" );
            System.out.println("\t Map2Mesh " +
            		"-mS1 " +
            		"-tISKY;NULL;AAATRIGGER " +
            		"-oD../textures " +
            		"a.map" );
            System.exit(0);
        }

        String[] ignoreTextures = { "SKY", "NULL" };
        String outDir = ".";
        String texturesDir = "./textures";
        String outName = null;
        String mapFile = args[args.length-1];    
        double scale = 1;
        boolean isHalfLife = true;
        
        // Switches einlesen
        for (int i = 0; i < args.length-1; i++)
        {
            String first = args[i].substring(0,3);
            
            if( first.equals("-tD" ))
                texturesDir = args[i].substring(3);
            else if( first.equals("-tI" ))
                ignoreTextures = args[i].substring(3).split( ";" );
            else if( first.equals("-oD" ))
                outDir = args[i].substring(3);
            else if( first.equals("-oN" ))
                outName = args[i].substring(3); 
            else if( first.equals("-mS" ))
            {
                scale = Double.parseDouble(args[i].substring(3));     
            }
            else if( first.equals("-q3" ))
            {
                isHalfLife = false;     
            }
            else if( first.equals("-hl" ))
            {
                isHalfLife = true;     
            }
            else
                throw new IllegalArgumentException(  "Unknown Switch "+first );
  
        }
         
        // Kein Ausgabename gewählt
        if( outName == null )
        {
	        File f = new File( mapFile );
	        outName = f.getName();
	        
	        // Nur den Namen der Map wählen
	        int index = outName.lastIndexOf('.');
	        if( index > 0 )
	            outName = outName.substring(0,index);
	        
	        if( outName.equals(""))
	            outName = "unnamed";
        }
                
        Plane.setHalfLife( isHalfLife );
        
        if( scale > 0 && scale != 1 )
        {
            Plane.scale(scale);
        }
        //      Ogre Rotation
        Plane.rotateX(-90);
        
        
        TextureManager.getSingleton().setTexturesDir(texturesDir);     
        Map map = new MapReader().readMapFile( mapFile );
        
        OgreMeshWriter.writeMeshXML(
                map,
                outDir, 
                outName, 
                ignoreTextures);
        
        System.exit(0);
    }

}