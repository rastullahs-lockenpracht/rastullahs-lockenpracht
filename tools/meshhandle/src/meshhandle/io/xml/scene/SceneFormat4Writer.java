package meshhandle.io.xml.scene;

import java.io.IOException;
import java.io.Writer;

import meshhandle.model.scene.Entity;
import meshhandle.model.scene.Light;
import meshhandle.model.scene.NodeUserData;
import meshhandle.model.scene.SceneNode;
import meshhandle.model.scene.SceneUserData;

public class SceneFormat4Writer extends AbstractSceneWriter
{
	public static final String FORMAT_STRING = "0.4.0";
	private int mEntities = 0;
	private int mLights = 0;

	public SceneFormat4Writer(Writer out)
	{
		super(out);
	}
	
	@Override
	public void write(SceneUserData scene) throws IOException
	{
		// TODO Auto-generated method stub
	}

	@Override
	public void write(Entity entity) throws IOException
	{
		mOutput.append("<entity");
		String name = entity.getName();
		if (name == null || "".equals(name))
		{
			name = entity.getId();
		}
		if (name == null || "".equals(name))
		{
			name = "entity_" + (mEntities++);
		}
		mOutput.append(" name=\"" + name + "\"");
		mOutput.append(" meshfile=\"" + entity.getMeshFile() + "\"");
		if (entity.getCastShadow() != null)
		{
			mOutput.append(" castShadows=\"" + entity.getCastShadow() + "\"");
		}
		mOutput.append(">\n");
		writeNode3DData(entity);
		write(entity.getUserData());
		mOutput.append("\n</entity>");
	}

	private void writeNode3DData(SceneNode node) throws IOException
	{
		if (node.getPosition() != null)
		{
			mOutput.append(node.getPosition().toXML("position"));
		}
		if (node.getRotation() != null)
		{
			mOutput.append(node.getRotation().toXML("rotation"));
		}
		if (node.getScale() != null)
		{
			mOutput.append(node.getScale().toXML("scale"));
		}
	}

	@Override
	public void write(Light light) throws IOException
	{
		mOutput.append("<entity");
		String name = light.getName();
		if (name == null || "".equals(name))
		{
			name = light.getId();
		}
		if (name == null || "".equals(name))
		{
			name = "light_" + (mLights++);
		}
		mOutput.append(" name=\"" + name + "\"");
		mOutput.append(" type=\"" + light.getType() + "\"");
		if (light.getVisible() != null)
		{
			mOutput.append(" visible=\"" + light.getVisible() + "\"");
		}
		
		if (light.getCastShadows() != null)
		{
			mOutput.append(" castShadows=\"" + light.getCastShadows() + "\"");
		}
		mOutput.append(">\n");
		writeNode3DData(light);
		write(light.getUserData());
		if (light.getDirection() != null)
		{
			mOutput.append(light.getDirection().toXML("direction"));
		}
		
		if (light.getSpotLightSettings() != null)
		{
			mOutput.append(light.getSpotLightSettings().toXML());
		}
		
		mOutput.append("\n</light>");
	}

	@Override
	public void write(NodeUserData userData) throws IOException
	{
		if (userData != null)
		{
			mOutput.append(userData.toXML());
		}
		// TODO Auto-generated method stub
	}

	@Override
	protected String getFormatVersion()
	{
		return FORMAT_STRING;
	}
}
