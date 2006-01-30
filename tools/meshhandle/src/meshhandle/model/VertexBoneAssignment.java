package meshhandle.model;

public class VertexBoneAssignment {
    private int vertexIndex;
    private int boneIndex;
    private float weight;
    
    public String toXML() {
        StringBuffer buf = new StringBuffer();
        buf.append("                <vertexboneassignment ");
        buf.append("vertexindex=\"").append(vertexIndex).append("\" ");
        buf.append("boneindex=\"").append(boneIndex).append("\" ");
        buf.append("weight=\"").append(weight).append("\"/>\n");
        return buf.toString();
    }
    
    public int getBoneIndex() {
        return boneIndex;
    }
    public void setBoneIndex(int boneIndex) {
        this.boneIndex = boneIndex;
    }
    public int getVertexIndex() {
        return vertexIndex;
    }
    public void setVertexIndex(int vertexIndex) {
        this.vertexIndex = vertexIndex;
    }
    public float getWeight() {
        return weight;
    }
    public void setWeight(float weight) {
        this.weight = weight;
    }
}
