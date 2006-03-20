package meshhandle.xml;

import java.io.IOException;

import javax.xml.parsers.ParserConfigurationException;

import meshhandle.data.Vector3;
import meshhandle.model.Face;
import meshhandle.model.Mesh;
import meshhandle.model.Submesh;
import meshhandle.model.Vertex;
import meshhandle.model.VertexBoneAssignment;
import meshhandle.model.VertexBufferData;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

public class MeshLoader extends XMLLoader {
    public static Mesh readMesh(String fileName)
            throws ParserConfigurationException, SAXException, IOException {
        Document doc = readDocument(fileName);

        Element meshElem = (Element) doc.getElementsByTagName("mesh").item(0);
        Mesh mesh = new Mesh();
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
        return mesh;
    }

    private static Submesh processSubmesh(Element submeshElem) {
        Submesh submesh = new Submesh();
        submesh.setMaterial(submeshElem.getAttribute("material"));
        submesh.setUse32bitIndices(submeshElem.getAttribute("use32bitindexes"));
        submesh.setUseSharedVertices(submeshElem
                .getAttribute("usesharedvertices"));
        submesh.setOperationType(submeshElem.getAttribute("operationtype"));

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

    private static VertexBufferData processVertexBuffer(Element vertexBufferElem) {
        VertexBufferData vertexbuffer = new VertexBufferData();
        vertexbuffer.setNormals(vertexBufferElem.getAttribute("normals"));
        vertexbuffer.setPositions(vertexBufferElem.getAttribute("positions"));
        vertexbuffer.setTexCoordDims0(vertexBufferElem
                .getAttribute("texture_coord_dimensions_0"));
        vertexbuffer.setTexCoordDims1(vertexBufferElem
                .getAttribute("texture_coord_dimensions_1"));
        vertexbuffer.setTexCoordDims2(vertexBufferElem
                .getAttribute("texture_coord_dimensions_2"));
        vertexbuffer.setTexCoords(vertexBufferElem
                .getAttribute("texture_coords"));
        vertexbuffer.setColorsDiffuse(vertexBufferElem
                .getAttribute("colours_diffuse"));

        NodeList vertList = vertexBufferElem.getElementsByTagName("vertex");
        for (int idx = 0; idx < vertList.getLength(); idx++) {
            Vertex vert = processVertex((Element) vertList.item(idx));
            vertexbuffer.addVertex(vert);
        }
        return vertexbuffer;
    }

    private static Vertex processVertex(Element vertElem) {
        Vertex vert = new Vertex();
        Element normalElem = (Element) vertElem.getElementsByTagName("normal")
                .item(0);
        if (normalElem != null) {
            vert.setNormal(Vector3.createFromXML(normalElem));
        }
        Element positionElem = (Element) vertElem.getElementsByTagName(
                "position").item(0);
        if (positionElem != null) {
            vert.setPosition(Vector3.createFromXML(positionElem));
        }
        Element texCoordElem = (Element) vertElem.getElementsByTagName(
                "texcoord").item(0);
        if (texCoordElem != null) {
            vert.setTexCoordU(Float.parseFloat(texCoordElem.getAttribute("u")));
            vert.setTexCoordV(Float.parseFloat(texCoordElem.getAttribute("v")));
        }
        
        Element colorDiffuseElem = (Element) vertElem.getElementsByTagName(
        	"colour_diffuse").item(0);
		if (colorDiffuseElem != null) {
		    vert.setColorDiffuse(colorDiffuseElem.getAttribute("value"));
		}
        return vert;
    }

    private static Face processFace(Element faceElem) {
        Face face = new Face(faceElem.getAttribute("v1"), faceElem
                .getAttribute("v2"), faceElem.getAttribute("v3"));
        return face;
    }
}
