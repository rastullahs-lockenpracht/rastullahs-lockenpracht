package de.pantheon.geometry;

public class Vector3
{
    private double xyz[] = new double[3];
    
	public Vector3()
	{
		xyz[0] = 0;
		xyz[1] = 0;
		xyz[2] = 0;
	}

	public Vector3(double x, double y, double z)
	{
		xyz[0] = x;
		xyz[1] = y;
		xyz[2] = z;
	}

	public Vector3(double[] array)
	{
		xyz[0] = array[0];
		xyz[1] = array[1];
		xyz[2] = array[2];
	}
	
	public double[] array()
	{
		return (double[])xyz.clone();
	}
	
	public double at( int i )
	{
	    return xyz[i];
	}

	public static Vector3 add(Vector3 a,Vector3 b)
	{
		return new Vector3(
		        a.xyz[0] + b.xyz[0],
		        a.xyz[1] + b.xyz[1],
		        a.xyz[2] + b.xyz[2] );
	}
	
	public static Vector3 sub(Vector3 a,Vector3 b)
	{
		return new Vector3(
		        a.xyz[0] - b.xyz[0],
		        a.xyz[1] - b.xyz[1],
		        a.xyz[2] - b.xyz[2] );
	}

	public static Vector3 mul(Vector3 a,Vector3 b)
	{
		return new Vector3(
			a.xyz[0] - b.xyz[0],
			a.xyz[1] - b.xyz[1],
			a.xyz[2] - b.xyz[2] );
	}
	
	public static Vector3 mul(Vector3 a, double c)
	{
		return new Vector3(c*a.xyz[0], c*a.xyz[1], c*a.xyz[2]);
	}

	public static Vector3 div(Vector3 a,double c)
	{
	    if( c == 0 )
	        return new Vector3( Double.NaN,Double.NaN,Double.NaN );
	    else
	        return new Vector3(a.xyz[0]/c, a.xyz[1]/c, a.xyz[2]/c);
	}

	public static double dot(Vector3 a,Vector3 b)
	{
		return a.xyz[0]*b.xyz[0] +
			a.xyz[1]*b.xyz[1] +
			a.xyz[2]*b.xyz[2];
	}

	public static Vector3 cross(Vector3 a,Vector3 b)
	{   
	   	return new Vector3(
			a.xyz[1]*b.xyz[2] - a.xyz[2]*b.xyz[1],
			a.xyz[2]*b.xyz[0] - a.xyz[0]*b.xyz[2],
			a.xyz[0]*b.xyz[1] - a.xyz[1]*b.xyz[0]
		);
	}

	static public Vector3 mul( Vector3 a, Matrix4 b )
	   {
	      Vector3 result = new Vector3();
	      
	      // We assume a vector of 4 with w = 1
	      result.xyz[0] = a.xyz[0]*b.m[0][0] + a.xyz[1]*b.m[1][0] + a.xyz[2]*b.m[2][0] + 1*b.m[3][0];
	      result.xyz[1] = a.xyz[0]*b.m[0][1] + a.xyz[1]*b.m[1][1] + a.xyz[2]*b.m[2][1] + 1*b.m[3][1];
	      result.xyz[2] = a.xyz[0]*b.m[0][2] + a.xyz[1]*b.m[1][2] + a.xyz[2]*b.m[2][2] + 1*b.m[3][2];
	      // w = a.m_x*b.m[0][3] + a.m_y*b.m[1][3] + a.m_z*b.m[2][3] + 1*b.m[3][3];
	      return result;
	   }
	
	public static double mag(Vector3 a)
	{
		return Math.sqrt( a.xyz[0] * a.xyz[0] + a.xyz[1] * a.xyz[1] + a.xyz[2] * a.xyz[2] );
	}

	public static Vector3 normalize(Vector3 a)
	{
		return Vector3.div(a,mag(a));
	}

	public double x()
	{
		return xyz[0];
	}
	
	public double y()
	{
		return xyz[1];
	}

	public double z()
	{
		return xyz[2];
	}

	public String toString()
	{
		return "( " + xyz[0] + " " + xyz[1] + " " + xyz[2] + " )"; 
	}
	
	public boolean equals(Object obj)
	{
		if( obj instanceof Vector3 )
		{
			Vector3 b = (Vector3)obj;

			return xyz[0]==b.xyz[0] &&
			       xyz[1]==b.xyz[1] &&
			       xyz[2]==b.xyz[2];
		}
		else
		{
			return false;
		}
		
	}

}
