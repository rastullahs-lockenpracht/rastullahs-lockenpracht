
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
public class Polygon
{
    private ArrayList mVertices;
    private Vector3 mCenter;
    private String mTexture;
    
    public Polygon()
    {
        mVertices = new ArrayList();
        mCenter = new Vector3( 0,0,0 );
    }
    
    public final ArrayList getVertices()
    {
        return mVertices;
    }
    
    public void addVector( Vector3 vec )
    {        
        for(int i = 0; i < mVertices.size(); i++ )
            if( ((Vertex)mVertices.get(i)).getPosition().nearly_equals( vec ) )
                 return;
        
        mVertices.add( new Vertex(vec) );
    }
    
    private void addVertex( Vertex vert )
    {
        mVertices.add(vert);
    }
    
    public final Vector3 getCenter()
    {
        return mCenter;
    }
    
    public void setTexture(String tex)
    {
        mTexture = tex;
    }
    public String getTexture()
    {
        return mTexture;
    }
    
    public void normalizeTexCoords()
    {
        if( mVertices.size() <= 0 )
            return;
        
	    boolean normU = true;
	    boolean normV = true;
	    
	    // Check which Coords can be normalized
	    for( int i = 0; i < mVertices.size(); i++ )
	    {
	        Vertex vert =  (Vertex)mVertices.get(i);
	        
	        if( vert.getU() < 1 && vert.getU() > -1 )
	            normU = false;

	        if( vert.getV() < 1 && vert.getV() > -1 )
	            normV = false;

	    }
	    
	    if( normU || normV )
	    {
	        int nearestU = 0;
	        double u = ((Vertex)mVertices.get(0)).getU();
	
	        int nearestV = 0;
	        double v = ((Vertex)mVertices.get(0)).getU();
	        
	        
	        if( normU )
	        {
	            if( u > 1 )
	                nearestU = (int)Math.floor(u);
	            else
	                nearestU = (int)Math.ceil(u);
	            
	        }
	        
	        if( normV )
	        {
	            if( v > 1 )
	                nearestV = (int)Math.floor(v);
	            else
	                nearestV = (int)Math.ceil(v);
	        }
	        
	        
	        for( int i = 0; i < mVertices.size(); i++ )
		    {
	            Vertex vert =  (Vertex)mVertices.get(i);
	            
	            if( normU )
	            {
	                u = vert.getU();
	                
	                if( Math.abs(u) < Math.abs(nearestU) )
	                {
	                    if( u > 1 )
	    	                nearestU = (int)Math.floor(u);
	    	            else
	    	                nearestU = (int)Math.ceil(u);
	                }
	            }
	            
	            if( normV )
	            {
	                v = vert.getV();
	                
	                if( Math.abs(v) < Math.abs(nearestV) )
	                {
	                    if( v > 1 )
	    	                nearestV = (int)Math.floor(v);
	    	            else
	    	                nearestV = (int)Math.ceil(v);
	                }
	            }
	        }
	        
	        for( int i = 0; i < mVertices.size(); i++ )
		    {
	            Vertex vert =  (Vertex)mVertices.get(i);
	            
	            vert.setU( vert.getU() - nearestU );
	            vert.setV( vert.getV() - nearestV );
	        }
	    }
    }
    
    
    public void sortVertices()
    {
        if( mVertices.size() <= 0 )
            return;
        
        for( int i = 0; i < mVertices.size()-2; i++ )
        {
            double smallestAngle = -1.0;
            int smallestAngleID = -1;
        
            Vector3 ma = Vector3.sub(((Vertex)mVertices.get(i)).getPosition(), mCenter );  
            ma = Vector3.normalize(ma);
            
            for( int j = i+1; j < mVertices.size(); j++ )
            {
                Vector3 mb = Vector3.sub(((Vertex)mVertices.get(j)).getPosition(), mCenter ); 
                mb = Vector3.normalize(mb);
                double angle = Vector3.dot(ma,mb);
                
                if( Vector3.dot(Vector3.cross(ma,mb),((Vertex)mVertices.get(j)).getNormal()) < 0.0 ) 
                    angle += 2.0;
                
                                    
                if( angle > smallestAngle )
                {
                    smallestAngle = angle;
                    smallestAngleID = j;
            	}
            }
            
            if( smallestAngleID != i+1 && smallestAngleID != -1 )
            {
                Vertex temp = (Vertex)mVertices.get(i+1);
                mVertices.set(i+1,mVertices.get(smallestAngleID));
                mVertices.set(smallestAngleID,temp);
            }
        }
        
        mVertices = reverse(mVertices);
    }
    
    public ArrayList reverse( ArrayList orig )
    {
       ArrayList reversed = new ArrayList() ;
       for ( int i = orig.size() - 1 ; i >= 0 ; i-- )
       {
           Object obj = orig.get( i ) ;
           reversed.add( obj ) ;
       }
       return reversed ;
    }
    
    public void calculateCenter()
    {
        if( mVertices.size() <= 0 )
            return;
    
        mCenter = new Vector3( 0,0,0 );
        
        for( int i = 0; i < mVertices.size(); i++)
            mCenter = Vector3.add(mCenter, ((Vertex)mVertices.get(i)).getPosition() );
        
        mCenter = Vector3.div(mCenter,mVertices.size());
    }
    
    public static ArrayList toTriangles( ArrayList polygons )
    {
        ArrayList triangles = new ArrayList();
	    
	    // In Dreiecke umwandeln
	    for( int i = 0; i < polygons.size(); i++ )
	    { 
	        Polygon p = (Polygon)polygons.get(i);
	        ArrayList verts = (ArrayList)p.getVertices().clone();

	        while( verts.size() >= 3 )
	        {
	            Triangle tri = new Triangle( 
	                    (Vertex)verts.get(0), 
	                    (Vertex)verts.get(1),
	                    (Vertex)verts.get(2),
	                    p.getTexture() );
	            
	            triangles.add( tri );    
	            
	            verts.remove(1);                    
	        }
	    }
	    
	    return triangles;
    }
}
