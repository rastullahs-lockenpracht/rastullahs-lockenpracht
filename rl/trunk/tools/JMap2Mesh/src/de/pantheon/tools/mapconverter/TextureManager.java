package de.pantheon.tools.mapconverter;
import java.awt.Container;
import java.awt.Image;
import java.awt.MediaTracker;
import java.awt.Toolkit;
import java.io.File;
import java.util.HashMap;

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
public class TextureManager
{
    private static TextureManager sInstance = null;
    private static final String [] sKnownsFormats={"jpg","jpeg","png"};
    
    private HashMap mTextures;
    private String mTexturesDir;
    
    private TextureManager()
    {
        mTextures = new HashMap();
        mTexturesDir = "";
    }
    
    public final void setTexturesDir( String dir )
    {
        mTexturesDir = dir;
    }
    
    public final Texture loadTexture( String name )
    {

        
        if( mTextures.containsKey(name) )
            return (Texture)mTextures.get(name);
        else
        { 
            String filename = null;
            String ext = null;
            
            for( int i=0; i < sKnownsFormats.length; i++)
            {
                ext = sKnownsFormats[i];
                
                filename = mTexturesDir+"/"+name+"."+ext;
                
                File f = new File(filename);
                
                if( f.canRead() )
                    break;
                
                filename = null;
            }

            
            if( filename != null )
            {
	            try
	            {
	                Image image = Toolkit.getDefaultToolkit().getImage( filename );
	                MediaTracker mediaTracker = new MediaTracker(new Container());
	                mediaTracker.addImage(image, 0);
	                mediaTracker.waitForID(0);
	                
	                Texture tex = new Texture(
	                        image.getWidth(null),
	                        image.getHeight(null),
	                        ext );
	                
	                mTextures.put( name, tex );
	                return tex;
	            }
	            catch (InterruptedException e)
	            {
	                e.printStackTrace();
	            } 
            }
        }
        
        return null;
    }
    
    
    public static TextureManager getSingleton()
    {
        if( sInstance != null )
            return sInstance;
        else
        {
            sInstance = new TextureManager();
            return sInstance;
        }
    }
    
    class Texture {
        private int mWidth;
        private int mHeight;
        private String mExt;
        
        public Texture(int w, int h, String ext)
        {
            mWidth = w;
            mHeight = h;
            mExt = ext;
        }
                
        public int getHeight()
        {
            return mHeight;
        }
        public int getWidth()
        {
            return mWidth;
        }
        public String getExtension()
        {
            return mExt;
        }
    }
}
