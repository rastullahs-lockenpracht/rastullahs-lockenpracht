/*
 * Created on 10.11.2004
 *
 * To change the template for this generated file go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */
package de.pantheon.tools.mapconverter;

import java.util.ArrayList;

/**
 * @author LopLop
 *
 * 2004
 */
public class Patch
{
    private ArrayList mTriangles;
    private String mTexture;
    
    public Patch()
    {
        mTriangles = new ArrayList();
    }
    
    public void addTriangle( Triangle tri )
    {
        mTriangles.add( tri );
    }
    
    public ArrayList getTriangles(  )
    {
        return mTriangles;
    }
    
    public void setTexture( String name )
    {
        mTexture = name;
    }
    
    
}
