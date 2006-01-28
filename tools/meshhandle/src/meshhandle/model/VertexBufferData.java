package meshhandle.model;

import java.util.ArrayList;

public class VertexBufferData {

    private String mTexCoordDims0;

    private String mTexCoords;

    private String mNormals;

    private String mPositions;

    private ArrayList<Vertex> mVertices;

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
            buf.append(" texture_coord_dimensions_0=\"" + getTexCoordDims()
                    + "\"");
        if (mTexCoords != null && mTexCoords.length() > 0)
            buf.append(" texture_coords=\"" + getTexCoords()+ "\"");
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

    public void setTexCoords(String texCoords) {
        mTexCoords = texCoords;
    }

    public String getPositions() {
        return mPositions;
    }

    public String getNormals() {
        return mNormals;
    }

    public String getTexCoordDims() {
        return mTexCoordDims0;
    }

    public String getTexCoords() {
        return mTexCoords;
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
}
