
package de.pantheon.tools.mapconverter;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import de.pantheon.geometry.Vector3;

/*
 * Created on 26.10.2004
 *
 * To change the template for this generated file go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */

/**
 * @author LopLop
 *
 * 2004
 */
public class Triangle
{
    private Vertex mVec1;
    private Vertex mVec2; 
    private Vertex mVec3; 
    private String mTexture;
    
    public Triangle()
    {
    
    }
    
    public Triangle(Vertex vec1, Vertex vec2, Vertex vec3, String texture )
    {
        mVec1 = vec1;
        mVec2 = vec2; 
        mVec3 = vec3; 
        mTexture = texture;
    }

    private static final Pattern TRIANGLE_PATTERN =  
        Pattern.compile("\\( \\( (.*?) \\) \\( (.*?) \\) \\( (.*?) \\) \\)$");
    
    public boolean parseTriangle( String line, String texture )
    {
        mTexture = texture;
        
        Matcher matcher = TRIANGLE_PATTERN.matcher(line);
        
        if( matcher.find() )
    	{
            mVec1 = new Vertex();
            mVec1.parseVertex( matcher.group(1) );
            mVec2 = new Vertex();
            mVec2.parseVertex( matcher.group(2) );
            mVec3 = new Vertex();
            mVec3.parseVertex( matcher.group(3) );
            
            
            Vector3 normal = Vector3.normalize(Vector3.cross( Vector3.sub(mVec1.getPosition(),mVec2.getPosition()), Vector3.sub(mVec3.getPosition(),mVec2.getPosition()) ) );
            mVec1.setNormal( normal );
            mVec2.setNormal( normal );
            mVec3.setNormal( normal );
            
            double d = normal.x();
            
            if( d == d )
                // NaN
                return true;
    	}
        
        return false;
    }
    
    /* (non-Javadoc)
     * @see java.lang.Object#equals(java.lang.Object)
     */
    public boolean equals(Object obj)
    {
        if( obj instanceof Triangle )
		{
            Triangle tri = (Triangle)obj;

			return mVec1.equals( tri.mVec1 ) &&
				   mVec2.equals( tri.mVec2 ) &&
				   mVec3.equals( tri.mVec3 ) &&
				   mTexture.equals(tri.mTexture);
		}
		else
		{
			return false;
		}
    }
    /**
     * @return Returns the mTexture.
     */
    public String getTexture()
    {
        return mTexture;
    }
    /**
     * @return Returns the mVec1.
     */
    public Vertex getVec1()
    {
        return mVec1;
    }
    /**
     * @return Returns the mVec2.
     */
    public Vertex getVec2()
    {
        return mVec2;
    }
    /**
     * @return Returns the mVec3.
     */
    public Vertex getVec3()
    {
        return mVec3;
    }
}
