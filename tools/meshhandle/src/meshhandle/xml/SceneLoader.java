package meshhandle.xml;

import java.io.IOException;

import javax.xml.parsers.ParserConfigurationException;

import meshhandle.data.Quaternion;
import meshhandle.data.Vector3;
import meshhandle.scene.Entity;
import meshhandle.scene.Scene;
import meshhandle.scene.SceneNode;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

public class SceneLoader extends XMLLoader
{
	public static Scene readScene(String fileName) throws ParserConfigurationException, SAXException, IOException
	{
		Document doc = readDocument(fileName);
		
		Element sceneElem = (Element) doc.getElementsByTagName("scene").item(0);
		Scene scene = new Scene(sceneElem.getAttribute("formatVersion"));
		NodeList nodeNodeList = sceneElem.getElementsByTagName("node");
		for(int idx = 0; idx < nodeNodeList.getLength(); idx++)
		{
			SceneNode scenenode = processNode((Element)nodeNodeList.item(idx));
			scene.addNode(scenenode);
		}
		return scene;
	}

	private static SceneNode processNode(Element sceneNodeElem) 
	{
		SceneNode scenenode = new SceneNode(sceneNodeElem.getAttribute("name"));
		scenenode.setPosition(Vector3.createFromXML(sceneNodeElem.getElementsByTagName("position").item(0)));
		scenenode.setScale(Vector3.createFromXML(sceneNodeElem.getElementsByTagName("scale").item(0)));
		scenenode.setRotation(Quaternion.createFromXML(sceneNodeElem.getElementsByTagName("rotation").item(0)));
		Element entityElem = (Element) sceneNodeElem.getElementsByTagName("entity").item(0);
		scenenode.setEntity(new Entity(entityElem.getAttribute("name"), entityElem.getAttribute("meshFile")));
		return scenenode;
	}
}
