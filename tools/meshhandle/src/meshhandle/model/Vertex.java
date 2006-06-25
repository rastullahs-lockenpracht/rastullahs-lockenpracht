package meshhandle.model;

import java.util.ArrayList;

import meshhandle.data.Vector3;

public class Vertex {

	private String mColourDiffuseValue;

	private String mColourSpecularValue;

	private Vector3 mNormal;

	private Vector3 mPosition;

	private ArrayList<Float> mTexCoordU;

	private ArrayList<Float> mTexCoordV;

	public Vertex() {
		mTexCoordU = new ArrayList<Float>();

		mTexCoordV = new ArrayList<Float>();
	}

	public void addTexCoordU(Float texCoordU) {
		mTexCoordU.add(texCoordU);
	}

	public void addTexCoordV(Float texCoordV) {
		mTexCoordV.add(texCoordV);
	}

	public String getColourDiffuse() {
		return mColourDiffuseValue;
	}

	public String getColourSpecular() {
		return mColourSpecularValue;
	}

	public Vector3 getNormal() {
		return mNormal;
	}

	public Vector3 getPosition() {
		return mPosition;
	}

	public void scale(float factor) {
		if (mPosition != null)
			mPosition.scale(factor);
	}

	public void setColourDiffuse(String colourDiffuse) {
		mColourDiffuseValue = colourDiffuse;
	}

	public void setColourSpecular(String colourSpecularValue) {
		mColourSpecularValue = colourSpecularValue;
	}

	public void setNormal(Vector3 normal) {
		mNormal = normal;
	}

	public void setPosition(Vector3 position) {
		mPosition = position;
	}

	public String toXML() {
		StringBuffer buf = new StringBuffer();
		buf.append("                    <vertex>\n");
		if (mPosition != null) {
			buf.append("                        " + mPosition.toXML("position")
					+ "\n");
		}
		if (mNormal != null) {
			buf.append("                        " + mNormal.toXML("normal")
					+ "\n");
		}
		if (mColourDiffuseValue != null && mColourDiffuseValue.length() > 0) {
			buf.append(("                        <colour_diffuse value=\""
					+ mColourDiffuseValue + "\" /> \n"));
		}
		if (mColourSpecularValue != null && mColourSpecularValue.length() > 0) {
			buf.append(("                        <colour_specular value=\""
					+ mColourSpecularValue + "\" /> \n"));
		}
		if (mTexCoordU != null && !mTexCoordU.isEmpty()) {
			for (int i = 0; i < mTexCoordU.size(); i++) {
				buf.append("                        <texcoord u=\""
						+ mTexCoordU.get(i) + "\" v=\"" + mTexCoordV.get(i)
						+ "\" />\n");
			}
		}
		buf.append("                    </vertex>");
		return buf.toString();
	}

}
