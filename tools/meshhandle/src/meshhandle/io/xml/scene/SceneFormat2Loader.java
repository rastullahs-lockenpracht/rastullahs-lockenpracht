package meshhandle.io.xml.scene;

import java.io.IOException;
import javax.xml.parsers.ParserConfigurationException;
import meshhandle.io.xml.XMLLoader;
import meshhandle.model.scene.Entity;
import meshhandle.model.scene.Light;
import meshhandle.model.scene.NodeUserData;
import meshhandle.model.scene.Scene;
import meshhandle.model.scene.SceneNode;
import meshhandle.model.scene.SceneUserData;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

public class SceneFormat2Loader extends XMLLoader implements ISceneLoader
{
	public Scene readScene(String fileName)
			throws ParserConfigurationException, SAXException, IOException
	{
		Document doc = readDocument(fileName);
		Element sceneElem = (Element)doc.getFirstChild();
		Scene scene = new Scene();
		NodeList nodeNodeList = sceneElem.getElementsByTagName("node");
		for (int idx = 0; idx < nodeNodeList.getLength(); idx++)
		{
			SceneNode scenenode = processNode((Element)nodeNodeList.item(idx));
			scene.addNode(scenenode);
		}
		for (int idx = 0; idx < nodeNodeList.getLength(); idx++)
		{
			Element elem = (Element)nodeNodeList.item(idx);
			if (elem.getParentNode() == sceneElem)
			{
				SceneUserData sceneUserData = processSceneUserData(elem);
				scene.addUserData(sceneUserData);
			}
		}
		return scene;
	}

	public String getFormatVersion()
	{
		return "0.2.0";
	}
	
	protected SceneUserData processSceneUserData(Element userDataElem)
	{
		SceneUserData sceneUserData = new SceneUserData();
		NodeList nl = userDataElem.getElementsByTagName("property");
		for (int i = 0; i < nl.getLength(); ++i)
		{
			Element elem = (Element)nl.item(i);
			if (elem.getAttribute("name").equals("default_renderingdistance"))
			{
				sceneUserData.setDefaultRenderingDistance(Float.parseFloat(elem
						.getAttribute("data")));
			}
		}
		return sceneUserData;
	}

	protected SceneNode processNode(Element sceneNodeElem)
	{
		SceneNode scenenode = null;
		
		Element elem = (Element)sceneNodeElem.getElementsByTagName("entity").item(0);
		if (elem != null)
		{
			scenenode = processEntity(elem);
		}

		elem = (Element)sceneNodeElem.getElementsByTagName("light").item(0);
		if (elem != null)
		{
			scenenode = processLight(elem);
		}

		elem = (Element)sceneNodeElem.getElementsByTagName("position")
				.item(0);
		if (elem != null)
		{
			scenenode.setPosition(processVector3(elem));
		}
		elem = (Element)sceneNodeElem.getElementsByTagName("scale").item(0);
		if (elem != null)
		{
			scenenode.setScale(processVector3(elem));
		}
		elem = (Element)sceneNodeElem.getElementsByTagName("rotation").item(0);
		if (elem != null)
		{
			scenenode.setRotation(processQuaternion(elem));
		}
		elem = (Element)sceneNodeElem.getElementsByTagName("userdata").item(0);
		if (elem != null)
		{
			scenenode.setUserData(processNodeUserData(elem));
		}
		return scenenode;
	}

	protected Entity processEntity(Element elem)
	{
		String name = elem.getAttribute("name");
		String file = elem.getAttribute("meshFile");
		Entity entity = new Entity(name, file);
		if (elem.hasAttribute("id"))
		{
			entity.setId(elem.getAttribute("id"));
		}
		if (elem.hasAttribute("castShadow"))
		{
			entity.setCastShadow(Boolean.valueOf(elem.getAttribute("castShadow")));
		}
		return entity;
	}
	
	protected Light processLight(Element elem)
	{
		String name = elem.getAttribute("name");
		Light light = new Light(name);
		
		if (elem.hasAttribute("id"))
		{
			light.setId(elem.getAttribute("id"));
		}

		String type = elem.getAttribute("type");
		light.setType(type);
		
		if (elem.hasAttribute("visible"))
		{
			boolean visible = Boolean.parseBoolean(elem.getAttribute("visible"));
			light.setVisible(visible);
		}
		
		Node diffColElem = elem.getElementsByTagName("colourDiffuse").item(0);
		if (diffColElem != null)
		{
			light.setDiffuseColour(processColourValue(diffColElem));
		}
		Node specColElem = elem.getElementsByTagName("colourSpecular").item(0);
		if (specColElem != null)
		{
			light.setDiffuseColour(processColourValue(specColElem));
		}
		
		return light;
	}

	protected NodeUserData processNodeUserData(Element userDataElem)
	{
		NodeUserData nodeUserData = new NodeUserData();
		NodeList nl = userDataElem.getElementsByTagName("property");
		for (int i = 0; i < nl.getLength(); ++i)
		{
			Element elem = (Element)nl.item(i);
			if (elem.getAttribute("name").equals("renderingdistance"))
			{
				nodeUserData.setRenderingDistance(Float.parseFloat(elem
						.getAttribute("data")));
			}
			else if (elem.getAttribute("name").equals("staticgeom_group"))
			{
				nodeUserData.setStaticGeomGroup(Integer.parseInt(elem
						.getAttribute("data")));
			}
			else if (elem.getAttribute("name").equals("physical_body"))
			{
				nodeUserData.setPhysicalBody(elem.getAttribute("data"));
			}
		}
		return nodeUserData;
	}
}
