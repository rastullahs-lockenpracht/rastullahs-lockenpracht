
package de.pantheon.tools.mapconverter;
import java.util.ArrayList;

import de.pantheon.geometry.Vector3;

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
public class Brush
{
    private ArrayList mPlanes;
    private ArrayList mPolygons;
    private ArrayList mPatches;
    
    public Brush()
    {
        mPlanes = new ArrayList();
        mPolygons = new ArrayList();
        mPatches = new ArrayList();
    }

    public final void addPlane( Plane plane )
    {
        mPlanes.add(plane);
        mPolygons.add(new Polygon());
    }
    
    public final void addPatch( Patch patch )
    {
        mPatches.add(patch);
    }
    
    public final ArrayList getPlanes()
    {
       return mPlanes;
    }
    public final ArrayList getPolygons()
    {
       return mPolygons;
    }
    public final ArrayList getPatches()
    {
       return mPatches;
    }
    public void transformToPolygons()
    {       
        // Cycle through all plane combinations.            
        for( int i = 0; i < mPlanes.size()-2; i++ )
        {
            for( int j = i+1; j < mPlanes.size()-1; j++  )
            {
            	for( int k = j+1; k < mPlanes.size(); k++ )
            	{
                	if( i != j && i != k && j != k )
                	{
                    	Vector3 inter = Plane.getIntersection( 
                    	        (Plane)mPlanes.get(i), (Plane)mPlanes.get(j), (Plane)mPlanes.get(k) );
                        
                        if( inter != null ) 
                        {
                            boolean bLegal = true;
                            
                            // System.out.print( inter );
                            
                            for( int p = 0; p < mPlanes.size(); p++ )
                            {
                                Plane plane = (Plane)mPlanes.get(p);
                                	
                                if( plane.classifyPoint(inter) == Plane.PLANE_POSITION_FRONT )
                                {
                                    bLegal = false;
                                    break;
                                }
                            }
                           
                            if( bLegal )
                            {
                                ((Polygon)mPolygons.get(i)).addVector( inter );
                                ((Polygon)mPolygons.get(j)).addVector( inter );
                                ((Polygon)mPolygons.get(k)).addVector( inter );
                            }              
                        }        
                	}
            	}
            }
        }
 
        
        for( int i = 0; i < mPolygons.size(); i++ )
        {
            Polygon poly = (Polygon)mPolygons.get(i);
            Plane p = ((Plane)mPlanes.get(i));
            
            poly.setTexture( p.getTexture() );            
            poly.calculateCenter();
            
            ArrayList verts = poly.getVertices();
            
            for( int j = 0; j < verts.size(); j++ )
            {
                Vertex v = (Vertex)verts.get(j);
                
            	double[] texCoords = p.getTextureCoordinates( 
            	        v.getPosition(), 
            	        poly.getCenter()  );
            	v.setU(texCoords[0]);
            	v.setV(texCoords[1]);
            	v.setNormal(p.getNormal());
            }

            poly.sortVertices();
            poly.normalizeTexCoords();
        }    
        
    }
    
    
}
