package de.pantheon.tools.mapconverter;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
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
public class MapReader
{
    public static final int LINETYPE_EMPTY = 0;
    public static final int LINETYPE_OPENBRACE = 1;
    public static final int LINETYPE_CLOSEDBRACE = 2;
    public static final int LINETYPE_PLANEORTRIANGLE = 3;
    public static final int LINETYPE_ARGVAL = 4;
    public static final int LINETYPE_PATCH = 5;
    public static final int LINETYPE_PATCHSTART = 6;
    public static final int LINETYPE_PATCHEND = 7;
    
    private int mCurrLine = 0;
    private boolean mReadGeometry = true;
    private ArrayList mFileLines;
    private Map mMap;
    
    public MapReader()
    {
    	mFileLines = new ArrayList();
    }
    
    public Map readMapFile( String mapName )
    {
        mMap = new Map();
        
        try
        {
            BufferedReader buf = new BufferedReader( new FileReader( new File( mapName ) ) );
            
            try
            {
                String line;
                while( null != ( line = buf.readLine() ) )
                {
                    mFileLines.add( cleanString( line ) );
                }
                
                buf.close();
            }
            catch (IOException e1)
            {
            }

        }
        catch (FileNotFoundException e)
        {
            e.printStackTrace();
        }
        
        readMap();

        return mMap;
    }
    
    private void readMap(  )
    {
	    while( mCurrLine < mFileLines.size() )
	    {
	        String line = (String) mFileLines.get(mCurrLine);
	        int linetype = processLine( line );
	        mCurrLine++;
	        
	        switch( linetype )
	        {
	            case LINETYPE_EMPTY:
	            // Leere Zeile, wir machen nichts
	                break;
	            case LINETYPE_OPENBRACE:
	            // Es beginnt eine Entity
	                Entity ent = readEntity();
	                mMap.addEntity( ent ); 
	                break;
	            case LINETYPE_CLOSEDBRACE:                
	            // Sowas sollte hier nicht sein
	                throw new IllegalStateException( "Unexpected Map-Line - ClosedBrace for Base at at line "+mCurrLine );
	            case LINETYPE_PLANEORTRIANGLE:
	            // Plane hat hier nichts zu suchen                    
	                throw new IllegalStateException( "Unexpected Map-Line - Face for Base at at line "+mCurrLine );
	            case LINETYPE_ARGVAL:                  
	            // Argumente auch nicht... 
	                throw new IllegalStateException( "Unexpected Map-Line - ArgumentValue for Base at at line "+mCurrLine );
	            case LINETYPE_PATCH:                  
		            // falsch hier
		            throw new IllegalStateException( "Unexpected Map-Line - Patch for Base at at line "+mCurrLine );
	            case LINETYPE_PATCHEND:                  
		            // falsch hier
		            throw new IllegalStateException( "Unexpected Map-Line - PatchEnd for Base at at line "+mCurrLine );
	            case LINETYPE_PATCHSTART:                  
		            // falsch hier
		            throw new IllegalStateException( "Unexpected Map-Line - PatchStart for Base at at line "+mCurrLine );
	        }
	        
	        
	    }            
    }
    
    private Entity readEntity(  )
    {
        Entity ent = new Entity();
        
        while( mCurrLine < mFileLines.size() )
	    {
            String line = (String) mFileLines.get(mCurrLine);
	        int linetype = processLine( line );
	        mCurrLine++;
	        
	        switch( linetype )
	        {
	            case LINETYPE_EMPTY:
	            // Leere Zeile, wir machen nichts
	                break;
	            case LINETYPE_OPENBRACE:
	            // Es beginnt ein Brush
	                Brush brsh = readBrush();
	                if( mReadGeometry )
	                    ent.addBrush( brsh ); 
	                break;
	            case LINETYPE_CLOSEDBRACE:                
	            // Entity ist fertig
	                return ent;
	            case LINETYPE_PLANEORTRIANGLE:
	            // Plane hat hier nichts zu suchen                    
	                throw new IllegalStateException( "Unexpected Map-Line - Face for Entity at at line "+mCurrLine );
	            case LINETYPE_ARGVAL:                  
	                // Argument parsen
	                ent.addAttribute(line);
	                break;
	            case LINETYPE_PATCH:                  
		            // falsch hier
		            throw new IllegalStateException( "Unexpected Map-Line - Patch for Entity at at line "+mCurrLine );
	            case LINETYPE_PATCHEND:                  
		            // falsch hier
		            throw new IllegalStateException( "Unexpected Map-Line - PatchEnd for Entity at at line "+mCurrLine );
	            case LINETYPE_PATCHSTART:                  
		            // falsch hier
		            throw new IllegalStateException( "Unexpected Map-Line - PatchStart for Entity at at line "+mCurrLine );
	        }
	    } 
        
        return ent;
    }
    
    private Brush readBrush(  )
    {
        Brush brsh = new Brush();
        
        while( mCurrLine < mFileLines.size() )
	    {
            String line = (String) mFileLines.get(mCurrLine);
	        int linetype = processLine( line );
	        mCurrLine++;
	        
	        switch( linetype )
	        {
	            case LINETYPE_EMPTY:
	            // Leere Zeile, wir machen nichts
	                break;
	            case LINETYPE_OPENBRACE:
	            // Es beginnt ein Brush
	                throw new IllegalStateException( "Unexpected Map-Line - OpenBrace for Brush at at line "+mCurrLine );
	            case LINETYPE_CLOSEDBRACE:                
	            // Brush ist fertig
	                return brsh;
	            case LINETYPE_PLANEORTRIANGLE:
	            // Plane auslesen        	                
	                if( mReadGeometry )
	                {
	                    Plane plane = new Plane();
	                    plane.parsePlane(line);
	                    brsh.addPlane(plane);
	                }
	                break;
	            case LINETYPE_PATCH:
	                    mCurrLine++;
	                    Patch patch = readPatch();
	                    brsh.addPatch( patch );
	                break;
	            case LINETYPE_ARGVAL:                  
	            // Argument parsen
	                throw new IllegalStateException( "Unexpected Map-Line - ArgumentValue for Brush at at line "+mCurrLine );
	            case LINETYPE_PATCHEND:                  
		            // falsch hier
		            throw new IllegalStateException( "Unexpected Map-Line - PatchEnd for Brush at at line "+mCurrLine );
	            case LINETYPE_PATCHSTART:                  
		            // falsch hier
		            throw new IllegalStateException( "Unexpected Map-Line - PatchStart for Brush at at line "+mCurrLine );
	        }
	        
	       
	    } 
        
        return brsh;
    }
    
    private Patch readPatch(  )
    {
        Patch patch = new Patch();
        String texture = (String) mFileLines.get(mCurrLine++);
        patch.setTexture(texture);
        mCurrLine++;
        
        while( mCurrLine < mFileLines.size() )
	    {
            String line = (String) mFileLines.get(mCurrLine);
	        int linetype = processLine( line );
	        mCurrLine++;
	        
	        switch( linetype )
	        {
	            case LINETYPE_PATCHSTART: 
	            case LINETYPE_EMPTY:
	            // Leere Zeile, wir machen nichts
	                break;
	            case LINETYPE_PLANEORTRIANGLE:
	                if( mReadGeometry )
	                {
	                    Triangle tris = new Triangle();
	                    
	                    if( tris.parseTriangle(line, texture) )
	                        patch.addTriangle(tris);  
	                }
	                break;
	            case LINETYPE_PATCH:                  
		            // Argument parsen
		            throw new IllegalStateException( "Unexpected Map-Line - ArgumentValue for Patch at at line "+mCurrLine );
	            case LINETYPE_ARGVAL:                  
		            // Argument parsen
		            throw new IllegalStateException( "Unexpected Map-Line - ArgumentValue for Patch at at line "+mCurrLine );
	            case LINETYPE_PATCHEND: 	                
	                mCurrLine++;
		            return patch;
	        }
	    }
        
        return patch;
    }
    
    private String cleanString( String str )
    {
        //  Kommentar suchen
        int ipos = str.indexOf("//");      
    
        // Alles nach dem Kommentar abschneiden
        if( ipos != -1 )
            str = str.substring(0,ipos);
                    
        // 	Leerstellen entfernen
        return str.trim();
    }
    
    private int processLine( String line ) 
    {	
	    int rval;
	    
	    if( line.length() == 0 )
	        rval = LINETYPE_EMPTY;
	    else if( line.substring(0,1).equals("{") )
	    	rval = LINETYPE_OPENBRACE;
	    else if( line.substring(0,1).equals("}") )
	    	rval = LINETYPE_CLOSEDBRACE;
	    else if( line.substring(0,1).equals("(") && line.length() > 1 )
	    	rval = LINETYPE_PLANEORTRIANGLE;
	    else if( line.substring(0,1).equals("(") && line.length() == 1 )
	    	rval = LINETYPE_PATCHSTART;
	    else if( line.substring(0,1).equals(")") && line.length() == 1 )
	    	rval = LINETYPE_PATCHEND;
	    else if( line.substring(0,1).equals("\"") )
	    	rval = LINETYPE_ARGVAL;
	    else if( line.substring(0,8).equals( "patchDef" ) )
	        rval = LINETYPE_PATCH;   
	    else
	        throw new IllegalStateException( "Unknown Map-Line at line "+mCurrLine+" ("+line+") " );
	    
	
	    return rval;
	}
}
