package de.pantheon.tools.mapconverter;
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
public class Vertex
{
    private Vector3 mPosition;
    private Vector3 mNormal;
    
    //private String mTexture;
    
    private double mU;
    private double mV;
    
    
    public Vertex( Vector3 vec )
    {
        mPosition = vec;
    }
    
    /**
     * @return Returns the mPosition.
     */
    public Vector3 getPosition()
    {
        return mPosition;
    }
    /**
     * @param position The mPosition to set.
     */
    public void setPosition(Vector3 position)
    {
        mPosition = position;
    }
    /**
     * @return Returns the mU.
     */
    public double getU()
    {
        return mU;
    }
    /**
     * @param mu The mU to set.
     */
    public void setU(double u)
    {
        mU = u;
    }
    /**
     * @return Returns the mV.
     */
    public double getV()
    {
        return mV;
    }
    /**
     * @param mv The mV to set.
     */
    public void setV(double v)
    {
        mV = v;
    }
    /**
     * @return Returns the mNormal.
     */
    public Vector3 getNormal()
    {
        return mNormal;
    }
    /**
     * @param normal The mNormal to set.
     */
    public void setNormal(Vector3 normal)
    {
        mNormal = normal;
    }
    
    
    public boolean equals(Object obj)
    {
        if( obj instanceof Vertex )
		{
            Vertex ver = (Vertex)obj;

			return mPosition.equals( ver.mPosition ) &&
				mNormal.equals( ver.mNormal ) &&
				mU == ver.mU &&
				mV == ver.mV;
		}
		else
		{
			return false;
		}
    }
    
    
    /**
     * @return Returns the mTexture.
     */
    /*public String getTexture()
    {
        return mTexture;
    }
    /**
     * @param tex The mTexture to set.
     */
    /*public void setTexture(String tex)
    {
        mTexture = tex;
    }*/
}
