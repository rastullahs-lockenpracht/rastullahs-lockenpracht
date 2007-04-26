package meshhandle.io.xml.scene;

import meshhandle.model.scene.SceneNode;
import org.w3c.dom.Element;

public class SceneFormat3Loader extends SceneFormat2Loader
{
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
		scenenode.setUserData(processNodeUserData(sceneNodeElem));
		return scenenode;
	}


}
