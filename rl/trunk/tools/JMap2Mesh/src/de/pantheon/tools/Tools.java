package de.pantheon.tools;
/*
 * Created on 27.10.2004
 *
 * To change the template for this generated file go to
 * Window>Preferences>Java>Code Generation>Code and Comments
 */

/**
 * @author LopLop
 *
 * 2004
 */
public class Tools
{
    public static boolean contains(Object[] arr, Object str)
    {
        if( arr == null )
            return false;
        
        for (int i = 0; i < arr.length; i++)
        {
            if (arr[i].equals(str))
                return true;
        }

        return false;
    }
}
