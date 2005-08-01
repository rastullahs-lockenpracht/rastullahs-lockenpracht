/*
 * Created on 27.10.2004
 *
 * To change the template for this generated file go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */
package de.pantheon.geometry;

/**
 * @author LopLop
 *
 * 2004
 */
public class Matrix4
{
    public double[][] m;
    
    public Matrix4()
    {
       m = new double[4][4];
    }
		
    public Matrix4(double m00, double m01, double m02, double m03,
                   double m10, double m11, double m12, double m13,
                   double m20, double m21, double m22, double m23,
                   double m30, double m31, double m32, double m33)
    {
       m = new double[4][4];
       
       m[0][0]=m00;  m[0][1]=m01;  m[0][2]=m02;  m[0][3]=m03;
       m[1][0]=m10;  m[1][1]=m11;  m[1][2]=m12;  m[1][3]=m13;
       m[2][0]=m20;  m[2][1]=m21;  m[2][2]=m22;  m[2][3]=m23;
       m[3][0]=m30;  m[3][1]=m31;  m[3][2]=m32;  m[3][3]=m33;
    }
    
    static public Matrix4 mul(Matrix4 a, Matrix4 b)
    {
       Matrix4 result = new Matrix4();
       
       for(int col=0; col<4; col++)
       {
          for(int row=0; row<4; row++)
          {
             result.m[row][col] = 0;
             for(int i=0; i<4; i++)
             {
                result.m[row][col] += a.m[row][i] * b.m[i][col];
             }
          }
       }
       
       return result;
       
    }    
    
    static public Matrix4 identity()
    {
       Matrix4 result = new Matrix4( 1, 0, 0, 0,
                                     0, 1, 0, 0,
                                     0, 0, 1, 0,
                                     0, 0, 0, 1 );
       return result;
    }
    
    static public Matrix4 rotatez(double angle)
    {
       double cosA = (double)Math.cos(angle);
       double sinA = (double)Math.sin(angle);
       
       Matrix4 result = new Matrix4();
       result.m[0][0]= cosA;   result.m[0][1]=sinA;  result.m[0][2]=0;  result.m[0][3]=0;
       result.m[1][0]= -sinA;  result.m[1][1]=cosA;  result.m[1][2]=0;  result.m[1][3]=0;
       result.m[2][0]= 0;      result.m[2][1]=0;     result.m[2][2]=1;  result.m[2][3]=0;
       result.m[3][0]= 0;      result.m[3][1]=0;     result.m[3][2]=0;  result.m[3][3]=1;
       return result;
    }

    static public Matrix4 rotatey(double angle)
    {
       double cosA = (double)Math.cos(angle);
       double sinA = (double)Math.sin(angle);
       
       Matrix4 result = new Matrix4();
       result.m[0][0]= cosA;   result.m[0][1]=0;  result.m[0][2]= -sinA;  result.m[0][3]=0;
       result.m[1][0]= 0;      result.m[1][1]=1;  result.m[1][2]= 0;      result.m[1][3]=0;
       result.m[2][0]= +sinA;  result.m[2][1]=0;  result.m[2][2]= cosA;   result.m[2][3]=0;
       result.m[3][0]= 0;      result.m[3][1]=0;  result.m[3][2]= 0;      result.m[3][3]=1;
       return result;
    }
    
    static public Matrix4 rotatex(double angle)
    {
       double cosA = (double)Math.cos(angle);
       double sinA = (double)Math.sin(angle);
       
       Matrix4 result = new Matrix4();
       result.m[0][0]= 1;  result.m[0][1]= 0;      result.m[0][2]= 0;     result.m[0][3]=0;
       result.m[1][0]= 0;  result.m[1][1]= cosA;   result.m[1][2]= sinA;  result.m[1][3]=0;
       result.m[2][0]= 0;  result.m[2][1]= -sinA;  result.m[2][2]= cosA;  result.m[2][3]=0;
       result.m[3][0]= 0;  result.m[3][1]= 0;      result.m[3][2]= 0;     result.m[3][3]=1;
       return result;
    }
    
    static public Matrix4 rotate( double angle, Vector3 axis )
    {
       double x = axis.x();
       double y = axis.y();
       double z = axis.z();
       double c = Math.cos(angle);
       double s = Math.sin(angle);
       double t = 1 - c;
       
       Matrix4 result = new Matrix4();
       
       result.m[0][0]= t*x*x + c;  		result.m[0][1]= t*x*y - z*s;    result.m[0][2]= t*x*z + y*s;    result.m[0][3]=0;
       result.m[1][0]= t*x*y + z*s;  	result.m[1][1]= t*y*y + c;   	result.m[1][2]= t*y*z - x*s;  	result.m[1][3]=0;
       result.m[2][0]= t*x*z - y*s;  	result.m[2][1]= t*y*z + x*s;  	result.m[2][2]= t*z*z + c;  	result.m[2][3]=0;
       result.m[3][0]= 0;  				result.m[3][1]= 0;      		result.m[3][2]= 0;     			result.m[3][3]=1;
 
       
       return result;
    }
    
    static public Matrix4 translate(double x, double y, double z)
    {
       Matrix4 result = Matrix4.identity();
       
       result.m[3][0] = x;
       result.m[3][1] = y;
       result.m[3][2] = z;
       return result;
    }
    
    static public Matrix4 scale(double x, double y, double z)
    {
       Matrix4 result = Matrix4.identity();
       
       result.m[0][0] = x;
       result.m[1][1] = y;
       result.m[2][2] = z;
       return result;
    }
    
    public String toString()
    {
       String ret = "";
       for(int i=0; i<4; i++)
           ret += " " + m[i][0] + " " + m[i][1] + " " + m[i][2] + " " + m[i][3] ;
       
       return ret;
    }
    

}