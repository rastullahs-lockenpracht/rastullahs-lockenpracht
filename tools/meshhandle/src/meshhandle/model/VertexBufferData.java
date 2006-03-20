package meshhandle.model;

import java.util.ArrayList;

public class VertexBufferData {

	private String mTexCoordDims0;
	private String mTexCoordDims1;
	private String mTexCoordDims2;
	private String mTexCoords;
	private String mNormals;
	private String mPositions;
	private ArrayList<Vertex> mVertices;
	private String mColorsDiffuse;

	public VertexBufferData() {
		mVertices = new ArrayList<Vertex>();
	}

	public void scale(float factor) {
		for (Vertex vert : mVertices) {
			vert.scale(factor);
		}
	}

	public int getVertexCount() {
		return mVertices.size();
	}

	public String toXML() {
		StringBuffer buf = new StringBuffer();
		buf.append("                <vertexbuffer");
		if (mPositions != null && mPositions.length() > 0)
		
			buf.append(" positions=\"" + mPositions + "\"");
		
		if (mNormals != null && mNormals.length() > 0)
			buf.append(" normals=\"" + mNormals + "\"");
		
		if (mTexCoordDims0 != null && mTexCoordDims0.length() > 0)
			buf.append(" texture_coord_dimensions_0=\"" + mTexCoordDims0
					+ "\"");
		if (mTexCoordDims1 != null && mTexCoordDims1.length() > 0)
			buf.append(" texture_coord_dimensions_1=\"" + mTexCoordDims1
					+ "\"");
		if (mTexCoordDims2 != null && mTexCoordDims2.length() > 0)
			buf.append(" texture_coord_dimensions_2=\"" + mTexCoordDims2
					+ "\"");
		
		if (mTexCoords != null && mTexCoords.length() > 0)
			buf.append(" texture_coords=\"" + getTexCoords() + "\"");
		
		if (mColorsDiffuse != null && mColorsDiffuse.length() > 0)
			buf.append(" colours_diffuse=\"" + getColorsDiffuse() + "\"");
		
		buf.append(">\n");
		for (Vertex vert : mVertices) {
			buf.append(vert.toXML() + "\n");
		}
		buf.append("                </vertexbuffer>\n");
		return buf.toString();
	}

	public void setNormals(String normals) {
		mNormals = normals;
	}

	public void setPositions(String positions) {
		mPositions = positions;
	}

	public void setTexCoordDims0(String texCoordDims) {
		mTexCoordDims0 = texCoordDims;
	}
	public void setTexCoordDims1(String attribute) {
		mTexCoordDims1 = attribute;
	}
	public void setTexCoordDims2(String attribute) {
		mTexCoordDims2 = attribute;
	}

	public void setTexCoords(String texCoords) {
		mTexCoords = texCoords;
	}

	public String getPositions() {
		return mPositions;
	}

	public String getNormals() {
		return mNormals;
	}

	public String getTexCoordDims0() {
		return mTexCoordDims0;
	}
	public String getTexCoordDims1() {
		return mTexCoordDims1;
	}
	public String getTexCoordDims2() {
		return mTexCoordDims2;
	}

	public String getTexCoords() {
		return mTexCoords;
	}
	
	public void setColorsDiffuse(String attribute) {
		mColorsDiffuse = attribute;
	}
	
	public String getColorsDiffuse() {
		return mColorsDiffuse;
	}

	public ArrayList<Vertex> getVertices() {
		return mVertices;
	}

	public void setVertices(ArrayList<Vertex> vertices) {
		mVertices = vertices;
	}

	public void addVertex(Vertex vert) {
		mVertices.add(vert);
	}

	public float getXMin() {
		float xmin = 0;
		for (Vertex vertex : mVertices) {
			if (vertex.getPosition() != null && vertex.getPosition().getX() < xmin) {
				xmin = vertex.getPosition().getX();
			}
		}
		return xmin;
	}

	public float getXMax() {
		float xmax = 0;
		for (Vertex vertex : mVertices) {
			if (vertex.getPosition() != null && vertex.getPosition().getX() > xmax) {
				xmax = vertex.getPosition().getX();
			}
		}
		return xmax;
	}

	public float getYMin() {
		float ymin = 0;
		for (Vertex vertex : mVertices) {
			if (vertex.getPosition() != null && vertex.getPosition().getY() < ymin) {
				ymin = vertex.getPosition().getY();
			}
		}
		return ymin;
	}

	public float getYMax() {
		float ymax = 0;
		for (Vertex vertex : mVertices) {
			if (vertex.getPosition() != null && vertex.getPosition().getY() > ymax) {
				ymax = vertex.getPosition().getY();
			}
		}
		return ymax;
	}

	public float getZMin() {
		float zmin = 0;
		for (Vertex vertex : mVertices) {
			if (vertex.getPosition() != null && vertex.getPosition().getZ() < zmin) {
				zmin = vertex.getPosition().getZ();
			}
		}
		return zmin;
	}

	public float getZMax() {
		float zmax = 0;
		for (Vertex vertex : mVertices) {
			if (vertex.getPosition() != null && vertex.getPosition().getZ() > zmax) {
				zmax = vertex.getPosition().getZ();
			}
		}
		return zmax;
	}

}
