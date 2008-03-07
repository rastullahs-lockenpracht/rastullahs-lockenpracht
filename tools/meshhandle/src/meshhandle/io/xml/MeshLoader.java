package meshhandle.io.xml;

import java.io.IOException;
import javax.xml.parsers.ParserConfigurationException;
import meshhandle.model.mesh.Face;
import meshhandle.model.mesh.Mesh;
import meshhandle.model.mesh.Submesh;
import meshhandle.model.mesh.SubmeshName;
import meshhandle.model.mesh.Texture;
import meshhandle.model.mesh.Vertex;
import meshhandle.model.mesh.VertexBoneAssignment;
import meshhandle.model.mesh.VertexBufferData;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

public class MeshLoader extends XMLLoader {
	private static Face processFace(Element faceElem) {
		Face face = new Face(faceElem.getAttribute("v1"), faceElem
				.getAttribute("v2"), faceElem.getAttribute("v3"));
		return face;
	}

	private Submesh processSubmesh(Element submeshElem) {
		Submesh submesh = new Submesh();
		submesh.setMaterial(submeshElem.getAttribute("material"));
		submesh.setUse32bitIndices(submeshElem.getAttribute("use32bitindexes")
				.equals("true"));
		submesh.setUseSharedVertices(submeshElem.getAttribute(
				"usesharedvertices").equals("true"));
		submesh.setOperationType(submeshElem.getAttribute("operationtype"));

		NodeList texturesList = submeshElem.getElementsByTagName("texture");
		for (int idx = 0; idx < texturesList.getLength(); idx++) {
			Texture texture = processTexture((Element) texturesList.item(idx));
			submesh.addTexture(texture);
		}

		NodeList subMeshList = submeshElem.getElementsByTagName("face");
		for (int idx = 0; idx < subMeshList.getLength(); idx++) {
			Face face = processFace((Element) subMeshList.item(idx));
			submesh.addFace(face);
		}

		NodeList bufferList = submeshElem.getElementsByTagName("vertexbuffer");
		for (int i = 0; i < bufferList.getLength(); ++i) {
			VertexBufferData vertexBuffer = processVertexBuffer((Element) bufferList
					.item(i));
			submesh.addVertexBufferData(vertexBuffer);
		}

		NodeList boneAssignmentsList = submeshElem
				.getElementsByTagName("vertexboneassignment");
		for (int i = 0; i < boneAssignmentsList.getLength(); ++i) {
			VertexBoneAssignment boneAssignment = processVertexBoneAssignment((Element) boneAssignmentsList
					.item(i));
			submesh.addVertexBoneAssignment(boneAssignment);
		}

		return submesh;
	}

	private static SubmeshName processSubmeshName(Element submeshNameElem) {
		SubmeshName submeshName = new SubmeshName();
		submeshName.setIndex(submeshNameElem.getAttribute("index"));
		submeshName.setName(submeshNameElem.getAttribute("name"));
		return submeshName;
	}

	private static Texture processTexture(Element textureElem) {
		Texture texture = new Texture();
		texture.setAlias(textureElem.getAttribute("alias"));
		texture.setName(textureElem.getAttribute("name"));
		return texture;
	}

	private Vertex processVertex(Element vertElem) {
		Vertex vert = new Vertex();
		Element normalElem = (Element) vertElem.getElementsByTagName("normal")
				.item(0);
		if (normalElem != null) {
			vert.setNormal(processVector3(normalElem));
		}
		Element positionElem = (Element) vertElem.getElementsByTagName(
				"position").item(0);
		if (positionElem != null) {
			vert.setPosition(processVector3(positionElem));
		}
		NodeList texCoordElems = vertElem.getElementsByTagName("texcoord");
		for (int idx = 0; idx < texCoordElems.getLength(); idx++) {
			vert.addTexCoordU(Float.parseFloat(((Element) texCoordElems
					.item(idx)).getAttribute("u")));
			vert.addTexCoordV(Float.parseFloat(((Element) texCoordElems
					.item(idx)).getAttribute("v")));
		}
		Element colDiffElem = (Element) vertElem.getElementsByTagName(
				"colour_diffuse").item(0);
		if (colDiffElem != null) {
			vert.setColourDiffuse(colDiffElem.getAttribute("value"));
		}
		Element colSpecElem = (Element) vertElem.getElementsByTagName(
				"colour_specular").item(0);
		if (colSpecElem != null) {
			vert.setColourSpecular(colSpecElem.getAttribute("value"));
		}
		return vert;
	}

	private static VertexBoneAssignment processVertexBoneAssignment(
			Element assignmentElem) {
		VertexBoneAssignment assignment = new VertexBoneAssignment();
		assignment.setVertexIndex(Integer.parseInt(assignmentElem
				.getAttribute("vertexindex")));
		assignment.setBoneIndex(Integer.parseInt(assignmentElem
				.getAttribute("boneindex")));
		assignment.setWeight(Float.parseFloat(assignmentElem
				.getAttribute("weight")));
		return assignment;
	}

	private VertexBufferData processVertexBuffer(Element vertexBufferElem) {
		VertexBufferData vertexbuffer = new VertexBufferData();
		vertexbuffer.setNormals(vertexBufferElem.getAttribute("normals")
				.equals("true"));
		vertexbuffer.setPositions(vertexBufferElem.getAttribute("positions")
				.equals("true"));

		vertexbuffer.setColoursDiffuse(vertexBufferElem.getAttribute(
				"colours_diffuse").equals("true"));
		vertexbuffer.setColoursSpecular(vertexBufferElem.getAttribute(
				"colours_specular").equals("true"));
		vertexbuffer.setTexCoords(vertexBufferElem
				.getAttribute("texture_coords"));
		vertexbuffer.setTexCoordDims0(vertexBufferElem
				.getAttribute("texture_coord_dimensions_0"));
		vertexbuffer.setTexCoordDims1(vertexBufferElem
				.getAttribute("texture_coord_dimensions_1"));
		vertexbuffer.setTexCoordDims2(vertexBufferElem
				.getAttribute("texture_coord_dimensions_2"));
		vertexbuffer.setTexCoordDims3(vertexBufferElem
				.getAttribute("texture_coord_dimensions_3"));
		vertexbuffer.setTexCoordDims4(vertexBufferElem
				.getAttribute("texture_coord_dimensions_4"));
		vertexbuffer.setTexCoordDims5(vertexBufferElem
				.getAttribute("texture_coord_dimensions_5"));
		vertexbuffer.setTexCoordDims6(vertexBufferElem
				.getAttribute("texture_coord_dimensions_6"));
		vertexbuffer.setTexCoordDims7(vertexBufferElem
				.getAttribute("texture_coord_dimensions_7"));

		NodeList vertList = vertexBufferElem.getElementsByTagName("vertex");
		for (int idx = 0; idx < vertList.getLength(); idx++) {
			Vertex vert = processVertex((Element) vertList.item(idx));
			vertexbuffer.addVertex(vert);
		}
		return vertexbuffer;
	}

	public Mesh readMesh(String fileName)
			throws ParserConfigurationException, SAXException, IOException {
		Document doc = readDocument(fileName);

		Element meshElem = (Element) doc.getElementsByTagName("mesh").item(0);
		Mesh mesh = new Mesh();
		// TODO Sowohl die VertexBuffer innerhalb als auch die außerhalb von
		// Submeshes sind mit vertexbuffer getagged.
		// NodeList vertexBufferList =
		// meshElem.getElementsByTagName("vertexbuffer");
		// for (int idx=0; idx<vertexBufferList.getLength(); idx++){
		// VertexBufferData sharedVertex = processVertexBuffer((Element)
		// vertexBufferList.item(idx));
		// mesh.addVertexBuffer(sharedVertex);
		// }
		NodeList subMeshList = meshElem.getElementsByTagName("submesh");
		for (int idx = 0; idx < subMeshList.getLength(); idx++) {
			Submesh scenenode = processSubmesh((Element) subMeshList.item(idx));
			mesh.addSubmesh(scenenode);
		}
		NodeList skeletonLinkList = meshElem
				.getElementsByTagName("skeletonlink");
		Element skeletonLinkElem = (Element) skeletonLinkList.item(0);
		if (skeletonLinkElem != null)
			mesh.setSkeletonLink(skeletonLinkElem.getAttribute("name"));
		// TODO s. o.
		// NodeList boneAssignmentList = meshElem
		// .getElementsByTagName("boneassignments");
		// for (int idx = 0; idx < boneAssignmentList.getLength(); ++idx) {
		// VertexBoneAssignment vba = processVertexBoneAssignment((Element)
		// subMeshList
		// .item(idx));
		// mesh.addBoneAssignment(vba);
		//			
		// }
		NodeList submeshNameList = meshElem
				.getElementsByTagName("submeshname");
		for (int idx = 0; idx < submeshNameList.getLength(); idx++) {
			SubmeshName smn = processSubmeshName((Element) submeshNameList
					.item(idx));
			mesh.addSubmeshName(smn);
		}
		return mesh;
	}
}
