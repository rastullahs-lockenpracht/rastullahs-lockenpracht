package meshhandle.model;

import java.util.ArrayList;

public class Submesh {
		
	private ArrayList<Face> mFaces;

	private String mMaterial;

	private String mOperationType;

	private ArrayList<Texture> mTextures;

	private boolean mUse32bitIndices;

	private boolean mUseSharedVertices;

	private ArrayList<VertexBoneAssignment> mVertexBoneAssignments;

	private ArrayList<VertexBufferData> mVertexBufferDatas;

	public Submesh() {
		mFaces = new ArrayList<Face>();
		mVertexBufferDatas = new ArrayList<VertexBufferData>();
		mVertexBoneAssignments = new ArrayList<VertexBoneAssignment>();
		mTextures = new ArrayList<Texture>();
	}

	public void addFace(Face face) {
		mFaces.add(face);
	}

	public void addTexture(Texture texture) {
		mTextures.add(texture);
	}

	public void addVertexBoneAssignment(VertexBoneAssignment boneAssignment) {
		mVertexBoneAssignments.add(boneAssignment);
	}

	public void addVertexBufferData(VertexBufferData vertexBufferData) {
		mVertexBufferDatas.add(vertexBufferData);
	}

	public int getFaceCount() {
		return mFaces.size();
	}

	public ArrayList<Face> getFaces() {
		return mFaces;
	}

	public String getMaterial() {
		return mMaterial;
	}

	public String getOperationType() {
		return mOperationType;
	}

	public boolean getUse32bitIndices() {
		return mUse32bitIndices;
	}

	public boolean getUseSharedVertices() {
		return mUseSharedVertices;
	}

	public float getXMax() {
		float xmax = 0;
		for (VertexBufferData vertexBuffer : mVertexBufferDatas) {
			if (vertexBuffer.getXMax() > xmax)
				xmax = vertexBuffer.getXMax();
		}
		return xmax;
	}

	public float getXMin() {
		float xmin = 0;
		for (VertexBufferData vertexBuffer : mVertexBufferDatas) {
			if (vertexBuffer.getXMin() < xmin)
				xmin = vertexBuffer.getXMin();
		}
		return xmin;
	}

	public float getYMax() {
		float ymax = 0;
		for (VertexBufferData vertexBuffer : mVertexBufferDatas) {
			if (vertexBuffer.getYMax() > ymax)
				ymax = vertexBuffer.getYMax();
		}
		return ymax;
	}

	public float getYMin() {
		float ymin = 0;
		for (VertexBufferData vertexBuffer : mVertexBufferDatas) {
			if (vertexBuffer.getYMin() < ymin)
				ymin = vertexBuffer.getYMin();
		}
		return ymin;
	}

	public float getZMax() {
		float zmax = 0;
		for (VertexBufferData vertexBuffer : mVertexBufferDatas) {
			if (vertexBuffer.getZMax() > zmax)
				zmax = vertexBuffer.getZMax();
		}
		return zmax;
	}

	public float getZMin() {
		float zmin = 0;
		for (VertexBufferData vertexBuffer : mVertexBufferDatas) {
			if (vertexBuffer.getZMin() < zmin)
				zmin = vertexBuffer.getZMin();
		}
		return zmin;
	}

	public void scale(float factor) {
		for (VertexBufferData buffer : mVertexBufferDatas) {
			buffer.scale(factor);
		}
	}

	public void setFaces(ArrayList<Face> faces) {
		mFaces = faces;
	}

	public void setMaterial(String material) {
		mMaterial = material;
	}

	public void setOperationType(String operationType) {
		mOperationType = operationType;
	}

	public void setUse32bitIndices(boolean use32bitIndices) {
		mUse32bitIndices = use32bitIndices;
	}

	public void setUseSharedVertices(boolean useSharedVertices) {
		mUseSharedVertices = useSharedVertices;
	}

	public String toXML() {
		StringBuffer buf = new StringBuffer();
		buf.append("        <submesh");
		buf.append(" material=\"" + mMaterial + "\"");
		buf.append(" usesharedvertices=\"" + mUseSharedVertices + "\"");
		buf.append(" use32bitindexes=\"" + mUse32bitIndices + "\"");
		buf.append(" operationtype=\"" + mOperationType + "\">\n");
		if (!mTextures.isEmpty()) {
			buf.append("            <textures>");
			for (Texture texture : mTextures) {
				buf.append("                <texture alias=\"" + texture.getAlias()
						+ "\"");
				buf.append(" name=\"" + texture.getName() + "\">\n");
			}
			buf.append("            </textures>\n");
		}
		if (!mFaces.isEmpty()) {
			buf.append("            <faces count=\"" + mFaces.size() + "\">\n");
			for (Face face : mFaces) {
				buf.append("                " + face.toXML() + "\n");
			}
			buf.append("            </faces>\n");
		}
		if (!mVertexBufferDatas.isEmpty()) {
			buf.append("            <geometry vertexcount=\""
					+ mVertexBufferDatas.get(0).getVertexCount() + "\">\n");
			for (VertexBufferData buffer : mVertexBufferDatas) {
				buf.append(buffer.toXML());
			}
			buf.append("            </geometry>\n");
		}
		if (!mVertexBoneAssignments.isEmpty()) {
			buf.append("            <boneassignments>\n");
			for (VertexBoneAssignment assignment : mVertexBoneAssignments) {
				buf.append(assignment.toXML());
			}
			buf.append("            </boneassignments>\n");
		}
		buf.append("        </submesh>");
		return buf.toString();
	}
}
