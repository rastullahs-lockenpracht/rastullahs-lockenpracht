package de.pantheon.tools.mapconverter;
import java.util.ArrayList;

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
public class Map
{
    private ArrayList mEntities;
    
    public Map()
    {
        mEntities = new ArrayList();
    }
    
    public final void addEntity( Entity ent )
    {
        mEntities.add(ent);
    }
    
    public final ArrayList getEntities()
    {
       return mEntities;
    }
}
