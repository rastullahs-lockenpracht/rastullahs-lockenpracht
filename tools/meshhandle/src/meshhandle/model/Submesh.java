package meshhandle.model;

import java.util.ArrayList;

public class Submesh {
	private String mMaterial;

	private String mUseSharedVertices;

	private String mUse32bitIndices;

	private String mOperationType;

	private ArrayList<Face> mFaces;

	private ArrayList<VertexBufferData> mVertexBufferDatas;

	private ArrayList<VertexBoneAssignment> mVertexBoneAssignments;

	public Submesh() {
		mFaces = new ArrayList<Face>();
		mVertexBufferDatas = new ArrayList<VertexBufferData>();
		mVertexBoneAssignments = new ArrayList<VertexBoneAssignment>();
	}

	public void scale(float factor) {
		for (VertexBufferData buffer : mVertexBufferDatas) {
			buffer.scale(factor);
		}
	}

	public String toXML() {
		StringBuffer buf = new StringBuffer();
		buf.append("        <submesh");
		buf.append(" material=\"" + mMaterial + "\"");
		buf.append(" usesharedvertices=\"" + mUseSharedVertices + "\"");
		buf.append(" use32bitindexes=\"" + mUse32bitIndices + "\"");
		buf.append(" operationtype=\"" + mOperationType + "\">\n");

		buf.append("            <faces count=\"" + mFaces.size() + "\">\n");
		for (Face face : mFaces) {
			buf.append("                " + face.toXML() + "\n");
		}
		buf.append("            </faces>\n");

		buf.append("            <geometry vertexcount=\""
				+ mVertexBufferDatas.get(0).getVertexCount() + "\">\n");
		for (VertexBufferData buffer : mVertexBufferDatas) {
			buf.append(buffer.toXML());
		}
		buf.append("            </geometry>\n");
		buf.append("            <boneassignments>\n");
		for (VertexBoneAssignment assignment : mVertexBoneAssignments) {
			buf.append(assignment.toXML());
		}
		buf.append("            </boneassignments>\n");

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

	public void addVertexBufferData(VertexBufferData vertexBufferData) {
		mVertexBufferDatas.add(vertexBufferData);
	}

	public void addFace(Face face) {
		mFaces.add(face);
	}

	public void addVertexBoneAssignment(VertexBoneAssignment boneAssignment) {
		mVertexBoneAssignments.add(boneAssignment);
	}

	public int getFaceCount() {
		return mFaces.size();
	}

	public float getXMin() {
		float xmin = 0;
		for (VertexBufferData vertexBuffer : mVertexBufferDatas) {
			if (vertexBuffer.getXMin() < xmin)
				xmin = vertexBuffer.getXMin();
		}
		return xmin;
	}

	public float getXMax() {
		float xmax = 0;
		for (VertexBufferData vertexBuffer : mVertexBufferDatas) {
			if (vertexBuffer.getXMax() > xmax)
				xmax = vertexBuffer.getXMax();
		}
		return xmax;
	}

	public float getYMin() {
		float ymin = 0;
		for (VertexBufferData vertexBuffer : mVertexBufferDatas) {
			if (vertexBuffer.getYMin() < ymin)
				ymin = vertexBuffer.getYMin();
		}
		return ymin;
	}

	public float getYMax() {
		float ymax = 0;
		for (VertexBufferData vertexBuffer : mVertexBufferDatas) {
			if (vertexBuffer.getYMax() > ymax)
				ymax = vertexBuffer.getYMax();
		}
		return ymax;
	}

	public float getZMin() {
		float zmin = 0;
		for (VertexBufferData vertexBuffer : mVertexBufferDatas) {
			if (vertexBuffer.getZMin() < zmin)
				zmin = vertexBuffer.getZMin();
		}
		return zmin;
	}

	public float getZMax() {
		float zmax = 0;
		for (VertexBufferData vertexBuffer : mVertexBufferDatas) {
			if (vertexBuffer.getZMax() > zmax)
				zmax = vertexBuffer.getZMax();
		}
		return zmax;
	}
}
