package meshhandle.model.mesh;

import java.util.ArrayList;

public class VertexBufferData {

	private boolean mColoursDiffuse;

	private boolean mColoursSpecular;

	private boolean mNormals;

	private boolean mPositions;

	private String mTexCoordDims0;

	private String mTexCoordDims1;

	private String mTexCoordDims2;

	private String mTexCoordDims3;

	private String mTexCoordDims4;

	private String mTexCoordDims5;

	private String mTexCoordDims6;

	private String mTexCoordDims7;

	private String mTexCoords;

	private ArrayList<Vertex> mVertices;

	public VertexBufferData() {
		mVertices = new ArrayList<Vertex>();
	}

	public void addVertex(Vertex vert) {
		mVertices.add(vert);
	}

	public ArrayList<Vertex> getMVertices() {
		return mVertices;
	}

	public boolean getNormals() {
		return mNormals;
	}

	public boolean getPositions() {
		return mPositions;
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

	public String getTexCoordDims3() {
		return mTexCoordDims3;
	}

	public String getTexCoordDims4() {
		return mTexCoordDims4;
	}

	public String getTexCoordDims5() {
		return mTexCoordDims5;
	}

	public String getTexCoordDims6() {
		return mTexCoordDims6;
	}

	public String getTexCoordDims7() {
		return mTexCoordDims7;
	}

	public String getTexCoords() {
		return mTexCoords;
	}

	public int getVertexCount() {
		return mVertices.size();
	}

	public ArrayList<Vertex> getVertices() {
		return mVertices;
	}

	public float getXMax() {
		float xmax = 0;
		for (Vertex vertex : mVertices) {
			if (vertex.getPosition() != null
					&& vertex.getPosition().getX() > xmax) {
				xmax = vertex.getPosition().getX();
			}
		}
		return xmax;
	}

	public float getXMin() {
		float xmin = 0;
		for (Vertex vertex : mVertices) {
			if (vertex.getPosition() != null
					&& vertex.getPosition().getX() < xmin) {
				xmin = vertex.getPosition().getX();
			}
		}
		return xmin;
	}

	public float getYMax() {
		float ymax = 0;
		for (Vertex vertex : mVertices) {
			if (vertex.getPosition() != null
					&& vertex.getPosition().getY() > ymax) {
				ymax = vertex.getPosition().getY();
			}
		}
		return ymax;
	}

	public float getYMin() {
		float ymin = 0;
		for (Vertex vertex : mVertices) {
			if (vertex.getPosition() != null
					&& vertex.getPosition().getY() < ymin) {
				ymin = vertex.getPosition().getY();
			}
		}
		return ymin;
	}

	public float getZMax() {
		float zmax = 0;
		for (Vertex vertex : mVertices) {
			if (vertex.getPosition() != null
					&& vertex.getPosition().getZ() > zmax) {
				zmax = vertex.getPosition().getZ();
			}
		}
		return zmax;
	}

	public float getZMin() {
		float zmin = 0;
		for (Vertex vertex : mVertices) {
			if (vertex.getPosition() != null
					&& vertex.getPosition().getZ() < zmin) {
				zmin = vertex.getPosition().getZ();
			}
		}
		return zmin;
	}

	public boolean isColoursDiffuse() {
		return mColoursDiffuse;
	}

	public boolean isColoursSpecular() {
		return mColoursSpecular;
	}

	public boolean isMNormals() {
		return mNormals;
	}

	public boolean isMPositions() {
		return mPositions;
	}

	public void scale(float factor) {
		for (Vertex vert : mVertices) {
			vert.scale(factor);
		}
	}

	public void setColoursDiffuse(boolean coloursDiffuse) {
		mColoursDiffuse = coloursDiffuse;
	}

	public void setColoursSpecular(boolean coloursSpecular) {
		mColoursSpecular = coloursSpecular;
	}

	public void setMNormals(boolean normals) {
		mNormals = normals;
	}

	public void setMPositions(boolean positions) {
		mPositions = positions;
	}

	public void setMVertices(ArrayList<Vertex> vertices) {
		mVertices = vertices;
	}

	public void setNormals(boolean b) {
		mNormals = b;
	}

	public void setPositions(boolean b) {
		mPositions = b;
	}

	public void setTexCoordDims0(String texCoordDims) {
		mTexCoordDims0 = texCoordDims;
	}

	public void setTexCoordDims1(String texCoordDims1) {
		mTexCoordDims1 = texCoordDims1;
	}

	public void setTexCoordDims2(String texCoordDims2) {
		mTexCoordDims2 = texCoordDims2;
	}

	public void setTexCoordDims3(String texCoordDims3) {
		mTexCoordDims3 = texCoordDims3;
	}

	public void setTexCoordDims4(String texCoordDims4) {
		mTexCoordDims4 = texCoordDims4;
	}

	public void setTexCoordDims5(String texCoordDims5) {
		mTexCoordDims5 = texCoordDims5;
	}

	public void setTexCoordDims6(String texCoordDims6) {
		mTexCoordDims6 = texCoordDims6;
	}

	public void setTexCoordDims7(String texCoordDims7) {
		mTexCoordDims7 = texCoordDims7;
	}

	public void setTexCoords(String texCoords) {
		mTexCoords = texCoords;
	}

	public void setVertices(ArrayList<Vertex> vertices) {
		mVertices = vertices;
	}

	public String toXML() {
		StringBuffer buf = new StringBuffer();
		buf.append("                <vertexbuffer");
		if (mPositions)
			buf.append(" positions=\"" + mPositions + "\"");
		if (mNormals)
			buf.append(" normals=\"" + mNormals + "\"");
		if (mColoursDiffuse)
			buf.append(" colours_diffuse=\"" + mColoursDiffuse + "\"");
		if (mColoursSpecular)
			buf.append(" colours_specular=\"" + mColoursSpecular + "\"");
		if (mTexCoordDims0 != null && mTexCoordDims0.length() > 0)
			buf.append(" texture_coord_dimensions_0=\"" + getTexCoordDims0()
					+ "\"");
		if (mTexCoordDims1 != null && mTexCoordDims1.length() > 0)
			buf.append(" texture_coord_dimensions_1=\"" + getTexCoordDims1()
					+ "\"");
		if (mTexCoordDims2 != null && mTexCoordDims2.length() > 0)
			buf.append(" texture_coord_dimensions_2=\"" + getTexCoordDims2()
					+ "\"");
		if (mTexCoordDims3 != null && mTexCoordDims3.length() > 0)
			buf.append(" texture_coord_dimensions_3=\"" + getTexCoordDims3()
					+ "\"");
		if (mTexCoordDims4 != null && mTexCoordDims4.length() > 0)
			buf.append(" texture_coord_dimensions_4=\"" + getTexCoordDims4()
					+ "\"");
		if (mTexCoordDims5 != null && mTexCoordDims5.length() > 0)
			buf.append(" texture_coord_dimensions_5=\"" + getTexCoordDims5()
					+ "\"");
		if (mTexCoordDims6 != null && mTexCoordDims6.length() > 0)
			buf.append(" texture_coord_dimensions_6=\"" + getTexCoordDims6()
					+ "\"");
		if (mTexCoordDims7 != null && mTexCoordDims7.length() > 0)
			buf.append(" texture_coord_dimensions_7=\"" + getTexCoordDims7()
					+ "\"");
		if (mTexCoords != null && mTexCoords.length() > 0)
			buf.append(" texture_coords=\"" + getTexCoords() + "\"");
		buf.append(">\n");
		for (Vertex vert : mVertices) {
			buf.append(vert.toXML() + "\n");
		}
		buf.append("                </vertexbuffer>\n");
		return buf.toString();
	}
}
