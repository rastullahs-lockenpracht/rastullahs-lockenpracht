package meshhandle.io.xml.scene;

import java.io.IOException;
import java.io.Writer;

import meshhandle.model.scene.Entity;
import meshhandle.model.scene.Light;
import meshhandle.model.scene.NodeUserData;
import meshhandle.model.scene.SceneNode;
import meshhandle.model.scene.SceneUserData;

public class SceneFormat2Writer extends AbstractSceneWriter
{
	public static final String FORMAT_STRING = "0.2.0";

	public SceneFormat2Writer(Writer out)
	{
		super(out);
	}
	
	@Override
	protected String getFormatVersion()
	{
		return FORMAT_STRING;
	}
	
	@Override
	public void write(SceneUserData scene) throws IOException
	{
		// TODO Auto-generated method stub
	}

	private void writeHeader(SceneNode node) throws IOException
	{
		if (node.getName() != null && !node.getName().equals(""))
		{
			mOutput.append("        <node name=\"" + node.getName() + "\">\n");
		}
		else
		{
			mOutput.append("        <node>\n");
		}
		
		if (node.getPosition() != null)
		{
			mOutput.append("            " + node.getPosition().toXML("position") + "\n");
		}
		if (node.getRotation() != null)
		{
			mOutput.append("            " + node.getRotation().toXML("rotation") + "\n");
		}
		if (node.getScale() != null)
		{
			mOutput.append("            " + node.getScale().toXML("scale") + "\n");
		}
	}
	
	private void writeFooter(SceneNode node) throws IOException
	{
		if (node.getUserData() != null)
		{
			write(node.getUserData());
		}
		mOutput.append("        </node>");
	}

	@Override
	public void write(Entity entity) throws IOException 
	{
		writeHeader(entity);
		mOutput.append("            ");
		mOutput.append("<entity");
		if (entity.getName() != null && !entity.getName().equals(""))
		{
			mOutput.append(" name=\"" + entity.getName() + "\""); 
		}
		if (entity.getId() != null && !entity.getId().equals(""))
		{
			mOutput.append(" id=\"" + entity.getId() + "\""); 
		}
		mOutput.append(" meshFile=\"" + entity.getMeshFile() + "\"");
		
		if (entity.getCastShadow() != null)
		{
			if (entity.getCastShadow() == true)
			{
				mOutput.append(" castShadow=\"true\"");
			}
			else if (entity.getCastShadow() == false)
			{
				mOutput.append(" castShadow=\"false\"");
			}
		}
		mOutput.append("/>\n");
		writeFooter(entity);
	}

	@Override
	public void write(NodeUserData userData) throws IOException
	{
		if (userData != null)
		{
			mOutput.append("<userData>");
			mOutput.append(userData.toXML());
			mOutput.append("</userData>");
		}
	}

	@Override
	public void write(Light light) throws IOException
	{
		// TODO Auto-generated method stub
		writeHeader(light);
		mOutput.append("            ");
		mOutput.append("<light");
		if (light.getName() != null && !light.getName().equals(""))
		{
			mOutput.append(" name=\"" + light.getName() + "\""); 
		}
		if (light.getId() != null && !light.getId().equals(""))
		{
			mOutput.append(" id=\"" + light.getId() + "\""); 
		}
		
		mOutput.append(" type=\"" + light.getType() + "\"");
		
		if (light.getCastShadows() != null)
		{
			if (light.getCastShadows() == true)
			{
				mOutput.append(" castShadows=\"true\"");
			}
			else if (light.getCastShadows() == false)
			{
				mOutput.append(" castShadows=\"false\"");
			}
		}
		mOutput.append("/>\n");
		
		if (light.getDiffuseColour() != null)
		{
			mOutput.append("              ");
			mOutput.append(light.getDiffuseColour().toXML("colourDiffuse"));
		}
		if (light.getSpecularColour() != null)
		{
			mOutput.append("              ");
			mOutput.append(light.getSpecularColour().toXML("colourSpecular"));
		}
		if (light.getAttenuation() != null)
		{
			mOutput.append("              ");
			mOutput.append(light.getAttenuation().toXML("lightAttenuation"));
		}
		
		if (light.getSpotLightSettings() != null)
		{
			mOutput.append("              ");
			mOutput.append(light.getSpotLightSettings().toXML());
		}
		else if (light.getDirection() != null)
		{
			mOutput.append("              ");
			mOutput.append(light.getDirection().toXML("direction"));
		}

		writeFooter(light);
	}
}
