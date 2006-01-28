package meshhandle.xml;

import java.io.IOException;

import javax.xml.parsers.ParserConfigurationException;

import meshhandle.data.Vector3;
import meshhandle.model.Face;
import meshhandle.model.Mesh;
import meshhandle.model.Submesh;
import meshhandle.model.Vertex;
import meshhandle.model.VertexBufferData;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

public class MeshLoader extends XMLLoader
{
	public static Mesh readMesh(String fileName) throws ParserConfigurationException, SAXException, IOException
	{
		Document doc = readDocument(fileName);
		
		Element sceneElem = (Element) doc.getElementsByTagName("mesh").item(0);
		Mesh scene = new Mesh();
		NodeList subMeshList = sceneElem.getElementsByTagName("submesh");
		for(int idx = 0; idx < subMeshList.getLength(); idx++)
		{
			Submesh scenenode = processSubmesh((Element)subMeshList.item(idx));
			scene.addSubmesh(scenenode);
		}
		return scene;
	}

	private static Submesh processSubmesh(Element submeshElem) 
	{
		Submesh submesh = new Submesh();
		submesh.setMaterial(submeshElem.getAttribute("material"));
		submesh.setUse32bitIndices(submeshElem.getAttribute("use32bitindexes"));
		submesh.setUseSharedVertices(submeshElem.getAttribute("usesharedvertices"));
		submesh.setOperationType(submeshElem.getAttribute("operationtype"));
		
		NodeList subMeshList = submeshElem.getElementsByTagName("face");
		for(int idx = 0; idx < subMeshList.getLength(); idx++)
		{
			Face face = processFace((Element)subMeshList.item(idx));
			submesh.addFace(face);
		}

		NodeList vertList = submeshElem.getElementsByTagName("vertex");
		for(int idx = 0; idx < vertList.getLength(); idx++)
		{
			Vertex vert = processVertex((Element)vertList.item(idx));
			submesh.addVertex(vert);
		}
		
		Element vertexBufferElem = (Element) submeshElem.getElementsByTagName("vertexbuffer").item(0);
		VertexBufferData vertexbuffer = new VertexBufferData();
		vertexbuffer.setNormals(vertexBufferElem.getAttribute("normals"));
		vertexbuffer.setPositions(vertexBufferElem.getAttribute("positions"));
		vertexbuffer.setTexCoordDims0(vertexBufferElem.getAttribute("texture_coord_dimensions_0"));
		vertexbuffer.setTexCoords(vertexBufferElem.getAttribute("texture_coords"));
		submesh.setVertexBufferData(vertexbuffer);
		
		return submesh;
	}

	private static Vertex processVertex(Element vertElem) {
		Vertex vert = new Vertex();
		Element normalElem = (Element) vertElem.getElementsByTagName("normal").item(0);
		if (normalElem != null)
		{
			vert.setNormal(Vector3.createFromXML(normalElem));
		}		
		Element positionElem = (Element) vertElem.getElementsByTagName("position").item(0);
		if (positionElem != null)
		{
			vert.setPosition(Vector3.createFromXML(positionElem));
		}		
		Element texCoordElem = (Element) vertElem.getElementsByTagName("texcoord").item(0);
		if (texCoordElem != null)
		{
			vert.setTexCoordU(Float.parseFloat(texCoordElem.getAttribute("u")));
			vert.setTexCoordV(Float.parseFloat(texCoordElem.getAttribute("v")));			
		}
		return vert;
	}

	private static Face processFace(Element faceElem) {
		Face face = new Face(
				faceElem.getAttribute("v1"),
				faceElem.getAttribute("v2"),
				faceElem.getAttribute("v3"));
		return face;
	}
}
