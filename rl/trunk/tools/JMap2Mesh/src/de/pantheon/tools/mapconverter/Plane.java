
package de.pantheon.tools.mapconverter;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import de.pantheon.geometry.Matrix4;
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
public class Plane
{
    public final static int PLANE_POSITION_FRONT = 1;
    public final static int PLANE_POSITION_ONPLANE = 0;
    public final static int PLANE_POSITION_BACK = -1;

    // degree to radian
    private final static double dr = Math.PI/180; 
    private final static double EPSILON = 0.0001;
    
    private double mDistance;
    private Vector3 mNormal = null;
    private String mTexture; 
    
    private static final Pattern PLANE_HL_PATTERN = 
		Pattern.compile("\\( (.*?) \\) \\( (.*?) \\) \\( (.*?) \\) (.*?) \\[ (.*?) \\] \\[ (.*?) \\] (.*?) (.*?) (.*?)$");
    private static final Pattern PLANE_Q3_PATTERN = 
		Pattern.compile("\\( (.*?) \\) \\( (.*?) \\) \\( (.*?) \\) (.*?) (.*?) (.*?) (.*?) (.*?) (.*?)$");
    
    private double mTex_offsety;
    private double mTex_offsetx;
    
    private Vector3 mTex1;
    private Vector3 mTex2;
    
    private double mScale_x;
    private double mScale_y;
    
    private static Matrix4 sMatrix = Matrix4.identity();
    
    private static boolean sHalfLife = true;
        
    public Plane()
    {
    }

    public void parsePlane( String line )
    {
        if( sHalfLife )
            parseHLPlane( line );
    }
    
    private void parseQ3Plane( String line )
    {
        Matcher matcher = PLANE_HL_PATTERN.matcher(line);
              
        if( matcher.find() )
		{
            Vector3 v1 = new Vector3( parseDoubleArray( matcher.group(1) ) );
            Vector3 v2 = new Vector3( parseDoubleArray( matcher.group(2) ) );
            Vector3 v3 = new Vector3( parseDoubleArray( matcher.group(3) ) );
                        
            mTexture = matcher.group(4);
            
            mTex_offsetx = Double.parseDouble(matcher.group(5));
            mTex_offsety = Double.parseDouble(matcher.group(6));
            
            double rotation = Double.parseDouble(matcher.group(7));
            
            mScale_x = Double.parseDouble(matcher.group(8));
            mScale_y = Double.parseDouble(matcher.group(9));
            
            calculateHessian(v1, v2, v3);
		}
    }
    
    private void parseHLPlane( String line )
    {
        Matcher matcher = PLANE_HL_PATTERN.matcher(line);
        
        if( matcher.find() )
		{
            Vector3 v1 = new Vector3( parseDoubleArray( matcher.group(1) ) );
            Vector3 v2 = new Vector3( parseDoubleArray( matcher.group(2) ) );
            Vector3 v3 = new Vector3( parseDoubleArray( matcher.group(3) ) );
                        
            mTexture = matcher.group(4).toUpperCase();
            
            double[] tmpArr = parseDoubleArray( matcher.group(5) );
            mTex_offsetx = tmpArr[3];
            mTex1 = Vector3.mul(new Vector3( tmpArr ),1);
            
            tmpArr = parseDoubleArray( matcher.group(6) );
            mTex_offsety = tmpArr[3];
            mTex2 = Vector3.mul(new Vector3( tmpArr ),1);
             
            double rotation = Double.parseDouble(matcher.group(7));
            mScale_x = Double.parseDouble(matcher.group(8));
            mScale_y = Double.parseDouble(matcher.group(9));
            
            if( sMatrix != null )
            {
                v1 = Vector3.mul(v1,sMatrix);
                v2 = Vector3.mul(v2,sMatrix);
                v3 = Vector3.mul(v3,sMatrix);
                mTex1 = Vector3.mul(mTex1,sMatrix);
                mTex2 = Vector3.mul(mTex2,sMatrix);
            }
            
            // Hessische Normal Form berechnen       
            calculateHessian(v1, v2, v3); 
		}
    }
    
    /**
     * @param v1
     * @param v2
     * @param v3
     */
    private void calculateHessian(Vector3 v1, Vector3 v2, Vector3 v3)
    {
        mNormal = Vector3.cross( Vector3.sub(v1,v2), Vector3.sub(v3,v2) );
        mNormal = Vector3.normalize(mNormal);
        mDistance = -Vector3.dot(mNormal,v2);
    }

    public static void rotateX( double angle  )
    {
         if( sMatrix == null )
             sMatrix = Matrix4.identity();
         
         sMatrix = Matrix4.mul(sMatrix, Matrix4.rotatex(angle*dr));
    }
    
    public static void scale( double x, double y, double z )
    {
         if( sMatrix == null )
             sMatrix = Matrix4.identity();
         
         sMatrix = Matrix4.mul(sMatrix, Matrix4.scale(x,y,z));
    }
    
    public static void resetMatrix(  )
    {
         sMatrix = null;
    }
    
    public double[] getTextureCoordinates( Vector3 vertPos, Vector3 center )
    {
        if( sHalfLife )
            return getHLTextureCoordinates( vertPos, center );
        else
            return null;
    }
    
    private double[] getHLTextureCoordinates( Vector3 vertPos, Vector3 center )
    {
        TextureManager.Texture tex = TextureManager.getSingleton().loadTexture( mTexture );
        
        int width = 128;
        int height = 128;
        
        if( tex != null )
        {
            width = tex.getWidth();
            height = tex.getHeight();
        }
        
        double[] ret = new double[2];
        ret[0] = ( Vector3.dot(mTex1,vertPos)/width/mScale_x  ) + ( mTex_offsetx / width );  
        ret[1] = ( Vector3.dot(mTex2,vertPos)/height/mScale_y  ) + ( mTex_offsety / height ); 
        
        return ret;
    }
    
    private double distanceToPlane ( Vector3 vec )
	{
		return ( Vector3.dot( mNormal, vec ) + mDistance );
	}

	public int classifyPoint ( Vector3 vec )
	{
		double distance = distanceToPlane ( vec );

		if ( distance > EPSILON )
		{
			return PLANE_POSITION_FRONT;
		}
		else if ( distance < -EPSILON )
		{
			return PLANE_POSITION_BACK;
		}

		return PLANE_POSITION_ONPLANE;
	}
    
    private double[] parseDoubleArray( String line )
    {
        String[] arr = line.split(" ");
        double[] ret = new double[arr.length];

        for( int i = 0; i < arr.length; i++ )
        {
            ret[i] = Double.parseDouble(arr[i]);
        }
        
        return ret;
    }
    
    public final Vector3 getNormal()
    {
        return mNormal;
    }
    public final double getDistance()
    {
        return mDistance;
    }
    public final String getTexture()
    {
        return mTexture;
    }
    // Get the intersection point of three planes.
    //                              -1
    //    |x|     | n1.x n1.y n1.z |     |d1|
    //    |y|  =  | n2.x n2.y n2.z |  X  |d2|
    //    |z|     | n3.x n3.y n3.z |     |d3|
    public static Vector3 getIntersection( Plane p1, Plane p2, Plane p3 )
    {
        double denom = Vector3.dot(p1.getNormal(), Vector3.cross(p2.getNormal(),p3.getNormal()) );

        // 2 Ebenen sind wohl parallel
        if( denom == 0.0 )
            return null;
        
        // Muha, nicht zulange anstarren, das beisst
        Vector3 a = Vector3.mul(Vector3.cross(p2.getNormal(),p3.getNormal()), -p1.getDistance() ); 
        Vector3 b = Vector3.mul(Vector3.cross(p3.getNormal(),p1.getNormal()), p2.getDistance() ); 
        Vector3 c = Vector3.mul(Vector3.cross(p1.getNormal(),p2.getNormal()), -p3.getDistance() );
        
        return Vector3.div(((Vector3.sub(a, Vector3.sub(b,c)))),denom);
    }
}
