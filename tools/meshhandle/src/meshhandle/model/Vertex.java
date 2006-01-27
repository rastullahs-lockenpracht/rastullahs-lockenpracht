package meshhandle.model;

import meshhandle.data.Vector3;

public class Vertex {

	private Vector3 mPosition;
	private Vector3 mNormal;
	private float mTexCoordU;
	private float mTexCoordV;
	
	public String toXML() {
		StringBuffer buf = new StringBuffer();
		buf.append("                    <vertex>\n");
		buf.append("                        "+mPosition.toXML("position")+"\n");
		buf.append("                        "+mNormal.toXML("normal")+"\n");
		buf.append("                        <texcoord u=\""+mTexCoordU+"\" v=\""+mTexCoordV+"\" />\n");
		buf.append("                    </vertex>");
		return buf.toString();
	}
	
	public Vector3 getNormal() {
		return mNormal;
	}
	public void setNormal(Vector3 normal) {
		mNormal = normal;
	}
	public Vector3 getPosition() {
		return mPosition;
	}
	public void setPosition(Vector3 position) {
		mPosition = position;
	}
	public float getTexCoordU() {
		return mTexCoordU;
	}
	public void setTexCoordU(float texCoordU) {
		mTexCoordU = texCoordU;
	}
	public float getTexCoordV() {
		return mTexCoordV;
	}
	public void setTexCoordV(float texCoordV) {
		mTexCoordV = texCoordV;
	}

	public void scale(float factor) {
		mPosition.scale(factor);
	}

}
