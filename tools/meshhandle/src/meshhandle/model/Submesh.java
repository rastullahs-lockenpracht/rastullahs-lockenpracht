package meshhandle.model;

import java.util.ArrayList;

public class Submesh {
	private String mMaterial;
	private String mUseSharedVertices;
	private String mUse32bitIndices;
	private String mOperationType;
	
	private ArrayList<Face> mFaces;
	private ArrayList<Vertex> mVertices;
	private VertexBufferData mVertexBufferData;
	
	public Submesh() {
		mFaces = new ArrayList<Face>();
		mVertices = new ArrayList<Vertex>();
	}
	
	public void scale(float factor)
	{
		for (Vertex vert : mVertices) {
			vert.scale(factor);
		}
	}
	
	public String toXML() {
		StringBuffer buf = new StringBuffer();
		buf.append("        <submesh");
		buf.append(" material=\""+mMaterial+"\"");
		buf.append(" usesharedvertices=\""+mUseSharedVertices+"\"");
		buf.append(" use32bitindexes=\""+mUse32bitIndices+"\"");
		buf.append(" operationtype=\""+mOperationType+"\">\n");
		
		buf.append("            <faces count=\""+mFaces.size()+"\">\n");
		for (Face face : mFaces) 
		{
			buf.append("                "+face.toXML()+"\n");
		}
		buf.append("            </faces>\n");
		
		buf.append("            <geometry vertexcount=\""+mVertices.size()+"\">\n");
		buf.append("                <vertexbuffer");
		buf.append(" positions=\""+mVertexBufferData.getPositions()+"\"");
		buf.append(" normals=\""+mVertexBufferData.getNormals()+"\"");
		buf.append(" texture_coord_dimensions_0=\""+mVertexBufferData.getTexCoordDims()+"\"");
		buf.append(" texture_coords=\""+mVertexBufferData.getTexCoords()+"\">\n");
		for (Vertex vert : mVertices) {
			buf.append(vert.toXML()+"\n");
		}
		buf.append("                </vertexbuffer>\n");
		buf.append("            </geometry>\n");
		
		buf.append("        </submesh>");
		return buf.toString();
	}

	public ArrayList<Face> getFaces() {
		return mFaces;
	}

	public void setFaces(ArrayList<Face> faces) {
		mFaces = faces;
	}

	public String getMaterial() {
		return mMaterial;
	}

	public void setMaterial(String material) {
		mMaterial = material;
	}

	public String getOperationType() {
		return mOperationType;
	}

	public void setOperationType(String operationType) {
		mOperationType = operationType;
	}

	public String getUse32bitIndices() {
		return mUse32bitIndices;
	}

	public void setUse32bitIndices(String use32bitIndices) {
		mUse32bitIndices = use32bitIndices;
	}

	public String getUseSharedVertices() {
		return mUseSharedVertices;
	}

	public void setUseSharedVertices(String useSharedVertices) {
		mUseSharedVertices = useSharedVertices;
	}

	public VertexBufferData getVertexBufferData() {
		return mVertexBufferData;
	}

	public void setVertexBufferData(VertexBufferData vertexBufferData) {
		mVertexBufferData = vertexBufferData;
	}

	public ArrayList<Vertex> getVertices() {
		return mVertices;
	}

	public void setVertices(ArrayList<Vertex> vertices) {
		mVertices = vertices;
	}

	public void addFace(Face face) {
		mFaces.add(face);
	}
	public void addVertex(Vertex vert) {
		mVertices.add(vert);
	}
}
