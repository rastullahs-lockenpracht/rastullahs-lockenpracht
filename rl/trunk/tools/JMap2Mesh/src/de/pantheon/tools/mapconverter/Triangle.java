
package de.pantheon.tools.mapconverter;
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
    
    
    public Triangle(Vertex vec1, Vertex vec2, Vertex vec3, String texture )
    {
        mVec1 = vec1;
        mVec2 = vec2; 
        mVec3 = vec3; 
        mTexture = texture;
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
