package de.pantheon.tools.mapconverter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

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
public class Entity
{
    private ArrayList mBrushes;
    private HashMap mAttributes;
    
    private static final Pattern ATTRIBUTE_PATTERN = 
		Pattern.compile( "\"(.*?)\" \"(.*?)\"" );
    
    public Entity()
    {
        mBrushes = new ArrayList();
        mAttributes = new HashMap();
    }
    
    public final void addAttribute( String key, String value )
    {
        mAttributes.put(key,value);
    }
    
    public final void addAttribute( String line )
    {
        Matcher matcher = ATTRIBUTE_PATTERN.matcher(line);
        
        if( matcher.find() )
		{
            addAttribute( matcher.group(1), matcher.group(2) );
		}
    }
    
    public final String getAttributeValue( String key )
    {
        Object obj = mAttributes.get(key);
        
        if( obj != null )
            return (String)obj;
        else
            return null;
    }
    
    public final void addBrush( Brush brsh )
    {
        mBrushes.add(brsh);
        brsh.transformToPolygons();
    }
    
    public final ArrayList getBrushes()
    {
       return mBrushes;
    }
    
    public final HashMap getAttributes()
    {
       return mAttributes;
    }
}
