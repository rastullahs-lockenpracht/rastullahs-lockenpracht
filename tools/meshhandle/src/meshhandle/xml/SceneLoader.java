package meshhandle.xml;

import java.io.IOException;

import javax.xml.parsers.ParserConfigurationException;

import meshhandle.data.Quaternion;
import meshhandle.data.Vector3;
import meshhandle.scene.Entity;
import meshhandle.scene.NodeUserData;
import meshhandle.scene.Scene;
import meshhandle.scene.SceneNode;
import meshhandle.scene.SceneUserData;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

public class SceneLoader extends XMLLoader {
    public static Scene readScene(String fileName)
            throws ParserConfigurationException, SAXException, IOException {
        Document doc = readDocument(fileName);

        Element sceneElem = (Element) doc.getElementsByTagName("scene").item(0);
        Scene scene = new Scene(sceneElem.getAttribute("formatVersion"));
        NodeList nodeNodeList = sceneElem.getElementsByTagName("node");
        for (int idx = 0; idx < nodeNodeList.getLength(); idx++) {
            SceneNode scenenode = processNode((Element) nodeNodeList.item(idx));
            scene.addNode(scenenode);
        }
        nodeNodeList = sceneElem.getElementsByTagName("userData");
        for (int idx = 0; idx < nodeNodeList.getLength(); idx++) {
            Element elem = (Element) nodeNodeList.item(idx);
            if (elem.getParentNode() == sceneElem) {
                SceneUserData sceneUserData = processSceneUserData(elem);
                scene.addUserData(sceneUserData);
            }
        }
        return scene;
    }

    private static SceneUserData processSceneUserData(Element userDataElem) {
        SceneUserData sceneUserData = new SceneUserData();
        NodeList nl = userDataElem.getElementsByTagName("property");
        for (int i = 0; i < nl.getLength(); ++i) {
            Element elem = (Element) nl.item(i);
            if (elem.getAttribute("name").equals("default_renderingdistance")) {
                sceneUserData.setDefaultRenderingDistance(Float
                        .parseFloat(userDataElem.getAttribute("data")));

            }
        }
        return sceneUserData;
    }

    private static SceneNode processNode(Element sceneNodeElem) {
        SceneNode scenenode = new SceneNode(sceneNodeElem.getAttribute("name"));
        Element elem = (Element) sceneNodeElem.getElementsByTagName("position")
                .item(0);
        if (elem != null)
            scenenode.setPosition(Vector3.createFromXML(elem));

        elem = (Element) sceneNodeElem.getElementsByTagName("scale").item(0);
        if (elem != null)
            scenenode.setScale(Vector3.createFromXML(elem));

        elem = (Element) sceneNodeElem.getElementsByTagName("rotation").item(0);
        if (elem != null)
            scenenode.setRotation(Quaternion.createFromXML(elem));

        elem = (Element) sceneNodeElem.getElementsByTagName("entity").item(0);
        if (elem != null)
            scenenode.setEntity(new Entity(elem.getAttribute("name"), elem
                    .getAttribute("meshFile")));

        elem = (Element) sceneNodeElem.getElementsByTagName("userData").item(0);
        if (elem != null)
            scenenode.setUserData(processNodeUserData(elem));
        return scenenode;
    }

    private static NodeUserData processNodeUserData(Element userDataElem) {
        NodeUserData nodeUserData = new NodeUserData();
        NodeList nl = userDataElem.getElementsByTagName("property");
        for (int i = 0; i < nl.getLength(); ++i) {
            Element elem = (Element) nl.item(i);
            if (elem.getAttribute("name").equals("renderingdistance")) {
                nodeUserData.setRenderingDistance(Float.parseFloat(userDataElem
                        .getAttribute("data")));

            } else if (elem.getAttribute("name").equals("staticgeom_group")) {
                nodeUserData.setStaticGeomGroup(Integer.parseInt(userDataElem
                        .getAttribute("data")));

            } else if (elem.getAttribute("name").equals("physical_body")) {
                nodeUserData.setPhysicalBody(userDataElem.getAttribute("data"));

            }
        }
        return nodeUserData;
    }

}
