/*
 * Created on 26.10.2004
 *
 * To change the template for this generated file go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */

package de.pantheon.tools.mapconverter;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import  de.pantheon.tools.Tools;
/**
 * @author LopLop
 *
 * 2004
 */
public class OgreMeshWriter
{
    public static void writeMeshXML( Map map, String outdir, String basename, String[] ignoreTextures )
    {
        ArrayList allTextures = new ArrayList();
        int mesh_count = 1;
     
        // Jede Entity ein einzelnes Mesh
        ArrayList entities = map.getEntities();     
        for( int i=0; i < entities.size(); i++ )
        {
            Entity ent = (Entity)entities.get(i);
            
            // Alles Polygone und Patches dieser Entity sammeln
            ArrayList polygons = new ArrayList();
            ArrayList patches = new ArrayList();
            
            ArrayList brushes = ent.getBrushes();
            for( int j=0; j < brushes.size(); j++ )
            {
                Brush brush = (Brush)brushes.get(j);
                ArrayList pos = brush.getPolygons();
                ArrayList pas = brush.getPatches();
                polygons.addAll( pos );
                patches.addAll( pas );
            }
            
            // Abbruch wenn keine Faces in dieser Entity sind
            if( polygons.size() <= 0 && patches.size() <= 0 )
                break;
            
            StringBuffer stream = new StringBuffer();
            // Faces in Dreiecke umwandeln
            ArrayList triangles = Polygon.toTriangles( polygons );    
            
            if( patches.size() > 0 )
                System.out.println( "Patches not yet supported. Skipping Patches. Sorry :(" );
            
            /*for( int j=0; j < patches.size(); j++ )
            {
                Patch p = (Patch)patches.get(j);
                triangles.addAll( p.getTriangles() );
            }*/
            
            // Alle Texturen sammeln
            ArrayList textures = new ArrayList();
            
            for( int j=0; j < triangles.size(); j++ )
            {
                Triangle tri = (Triangle)triangles.get(j);
                String texture = tri.getTexture();
                
                if( !textures.contains( texture ) && !Tools.contains(ignoreTextures, texture ) )
                    textures.add( texture );
                   
                if( !allTextures.contains( texture ) && !Tools.contains(ignoreTextures, texture ) )
                    allTextures.add( texture );
                
            }
            
            if( textures.size() <= 0 )
                break;

            stream.append("<mesh>\n");
            stream.append("\t<submeshes>\n");
            
            // Jede Textur muss ein einzelnes Submesh sein
            for( int j=0; j < textures.size(); j++ )
            {  
                String texture = (String)textures.get(j);

                ArrayList subTriangles = new ArrayList();
                
                for( int k=0; k < triangles.size(); k++ )
                { 
                    Triangle tri = (Triangle)triangles.get( k );
                
                    if( tri.getTexture().equals( texture ) )
                        subTriangles.add( tri );
                }    	
                
                stream.append("\t\t<submesh material=\"");
	            stream.append(basename);
	            stream.append("_");
	            stream.append(texture);
	            stream.append("\" usesharedvertices=\"false\" use32bitindexes=\"false\">\n");
                stream.append("\t\t\t<faces count=\"");
	            stream.append(subTriangles.size());
	            stream.append("\">\n");
                
                ArrayList vertices = new ArrayList();
                 // Alle Triangles
                for( int k=0; k < subTriangles.size(); k++ )
                {  
                    Triangle tri = (Triangle)subTriangles.get( k );

                    if( !vertices.contains( tri.getVec1() ) )
                        vertices.add( tri.getVec1() );
                    if( !vertices.contains( tri.getVec2() ) )
                        vertices.add( tri.getVec2() );
                    if( !vertices.contains( tri.getVec3() ) )
                        vertices.add( tri.getVec3() );
                    
                    stream.append("\t\t\t\t<face v1=\"");
    	            stream.append(vertices.lastIndexOf( tri.getVec1() ));
    	            stream.append("\" v2=\"");
    	            stream.append(vertices.lastIndexOf( tri.getVec2() ));
    	            stream.append("\" v3=\""+vertices.lastIndexOf( tri.getVec3() ));
    	            stream.append("\" />\n");  
                }
                
                stream.append("\t\t\t</faces>\n");
                stream.append("\t\t\t<geometry vertexcount=\"");
	            stream.append(vertices.size());
	            stream.append("\">\n");
                stream.append("\t\t\t\t<vertexbuffer positions=\"true\" normals=\"true\" colours_diffuse=\"false\" texture_coords=\"1\" texture_coord_dimensions_0=\"2\" >\n");
                
                // Alle Vertices der Triangles
                for( int k=0; k < vertices.size(); k++ )
                { 
                    Vertex vert = (Vertex)vertices.get(k);
                    
                    stream.append("\t\t\t\t\t<vertex>\n");
                    stream.append("\t\t\t\t\t\t<position x=\"");
    	            stream.append(vert.getPosition().x());
    	            stream.append("\" y=\"");
    	            stream.append(vert.getPosition().y());
    	            stream.append("\" z=\""+vert.getPosition().z());
    	            stream.append("\" />\n");
                    stream.append("\t\t\t\t\t\t<normal x=\"");
    	            stream.append((vert.getNormal().x()));
    	            stream.append("\" y=\"");
    	            stream.append((vert.getNormal().y()));
    	            stream.append("\" z=\"");
    	            stream.append((vert.getNormal().z()));
    	            stream.append("\" />\n");
                    stream.append("\t\t\t\t\t\t<texcoord u=\"");
    	            stream.append(vert.getU());
    	            stream.append("\" v=\"");
    	            stream.append(vert.getV());
    	            stream.append("\" />\n");
                    stream.append("\t\t\t\t\t</vertex>\n");
                }
                
                stream.append("\t\t\t\t</vertexbuffer>\n");
                stream.append("\t\t\t</geometry>\n");
                stream.append("\t\t</submesh>\n"); 
                
            }
            
            stream.append("\t</submeshes>\n");    
            stream.append("</mesh>\n");
            
            System.out.println( "File written for Entity "+mesh_count+": "+outdir+"/"+basename+"_"+mesh_count+".xml" );            
            writeToFile( outdir+"/"+basename+"_"+mesh_count+".xml",stream.toString() );
            stream = null;
            mesh_count++;
        }    
        
        StringBuffer stream = new StringBuffer();
        int numWritten = 0;
        
        for( int i=0; i < allTextures.size(); i++ )
        {
            String textureName = (String) allTextures.get(i);
            TextureManager.Texture tex = TextureManager.getSingleton().loadTexture(textureName);
            
            if( tex != null )
            {
	            stream.append("material ");
	            stream.append(basename);
	            stream.append("_");
	            stream.append(textureName);
	            stream.append("\n{\n");
	            stream.append("\ttechnique\n");
	            stream.append("\t{\n");
	            stream.append("\t\tpass\n");
	            stream.append("\t\t{\n");
	            stream.append("\t\t\ttexture_unit\n");
	            stream.append("\t\t\t{\n");
	            stream.append("\t\t\t\ttexture textures/");
	            stream.append(textureName);
	            stream.append(".");
	            stream.append(tex.getExtension());
	            stream.append("\n\t\t\t\tfiltering anisotropic anisotropic anisotropic\n");
	            stream.append("\t\t\t}\n");
	            stream.append("\t\t}\n");
	            stream.append("\t}\n");
	            stream.append("}\n");
	            numWritten++;
            }
        }
        
        if( numWritten > 0 )
        {
            System.out.println( "File written for "+numWritten+" of "+allTextures.size()+" Texture(s): "+outdir+"/"+basename+".material" );            
            writeToFile( outdir+"/"+basename+".material",stream.toString() );
        }
        else
            System.out.println( "No Material File written ("+allTextures.size()+" texture(s) missing). Does your texture-directory exist?" );
    }
    
    private static void writeToFile( String filename, String content)
    {
        File f = new File( filename );    
        
        try
        {
            if( f.exists() )
                f.delete();

            f.createNewFile();
            BufferedWriter buf = new BufferedWriter( new FileWriter(f) );
            buf.write( content );
            buf.close();
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
    }
}
