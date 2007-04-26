package meshhandle.io.xml;

import java.io.File;
import java.io.IOException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import meshhandle.data.ColourValue;
import meshhandle.data.Quaternion;
import meshhandle.data.Vector3;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.xml.sax.SAXException;

public class XMLLoader {
    protected static Document readDocument(String filename)
            throws ParserConfigurationException, SAXException, IOException {
        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
        DocumentBuilder builder = factory.newDocumentBuilder();
        Document document = builder.parse(new File(filename));
        return document;
    }
    
    protected ColourValue processColourValue(Node node) {
        NamedNodeMap attrs = node.getAttributes();
        return new ColourValue(Float.parseFloat(attrs.getNamedItem("r")
                .getNodeValue()), Float.parseFloat(attrs.getNamedItem("g")
                .getNodeValue()), Float.parseFloat(attrs.getNamedItem("b")
                .getNodeValue()));
    }
    
    protected Quaternion processQuaternion(Node node) {
        NamedNodeMap attrs = node.getAttributes();
        return new Quaternion(Float.parseFloat(attrs.getNamedItem("qw")
                .getNodeValue()), Float.parseFloat(attrs.getNamedItem("qx")
                .getNodeValue()), Float.parseFloat(attrs.getNamedItem("qy")
                .getNodeValue()), Float.parseFloat(attrs.getNamedItem("qz")
                .getNodeValue()));
    }

    protected Vector3 processVector3(Node node) {
        NamedNodeMap attrs = node.getAttributes();
        return new Vector3(Float.parseFloat(attrs.getNamedItem("x")
                .getNodeValue()), Float.parseFloat(attrs.getNamedItem("y")
                .getNodeValue()), Float.parseFloat(attrs.getNamedItem("z")
                .getNodeValue()));
    }




}
