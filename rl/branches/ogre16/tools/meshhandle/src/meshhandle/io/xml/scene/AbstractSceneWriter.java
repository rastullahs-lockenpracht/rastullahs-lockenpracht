package meshhandle.io.xml.scene;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.Writer;

import meshhandle.model.scene.Entity;
import meshhandle.model.scene.Light;
import meshhandle.model.scene.NodeUserData;
import meshhandle.model.scene.Scene;
import meshhandle.model.scene.SceneNode;
import meshhandle.model.scene.SceneUserData;

public abstract class AbstractSceneWriter
{
	protected BufferedWriter mOutput;

	public AbstractSceneWriter(Writer outputStreamWriter)
	{
		mOutput = new BufferedWriter(outputStreamWriter);
	}
	
	public static AbstractSceneWriter createSceneWriter(String format, Writer outputWriter)
	{
		if (format.equals(SceneFormat2Writer.FORMAT_STRING))
		{
			return new SceneFormat2Writer(outputWriter);
		}
		
		if (format.equals(SceneFormat4Writer.FORMAT_STRING))
		{
			return new SceneFormat4Writer(outputWriter);
		}
		
		return null;
	}
	
	public abstract void write(SceneUserData scene) throws IOException;
	public abstract void write(Entity entity) throws IOException;
	public abstract void write(Light light) throws IOException;
	public abstract void write(NodeUserData scene) throws IOException;

	public void write(Scene scene) throws IOException
	{
		mOutput.append("<scene formatVersion=\"" + getFormatVersion() + "\">\n");
	    mOutput.append("    <nodes>\n");
	    for (SceneNode node : scene.getNodes()) 
	    {
	    	if (node instanceof Entity)
			{
				write((Entity)node);
			}
			else if (node instanceof Light)
			{
				write((Light)node);
			}        	
	        mOutput.append("\n");
	    }
	    mOutput.append("    </nodes>\n");
	    if (scene.getUserData() != null)
		{
			write(scene.getUserData());
		}
	    mOutput.append("</scene>");
	    mOutput.flush();
	}

	protected abstract String getFormatVersion();
}
