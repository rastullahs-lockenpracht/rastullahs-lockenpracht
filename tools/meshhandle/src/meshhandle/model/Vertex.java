package meshhandle.model;

import meshhandle.data.Vector3;

public class Vertex {

    private Vector3 mPosition;
    private Vector3 mNormal;
    private Float mTexCoordU;
    private Float mTexCoordV;
	private String mColorDiffuse;

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
        if (mTexCoordU != null) {
            buf.append("                        <texcoord u=\"" + mTexCoordU
                    + "\" v=\"" + mTexCoordV + "\" />\n");
        }
        if (mColorDiffuse != null) {
        	buf.append("                        <color_diffuse value=\"" + mColorDiffuse+"\" />");
        }
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

    public Float getTexCoordU() {
        return mTexCoordU;
    }

    public void setTexCoordU(Float texCoordU) {
        mTexCoordU = texCoordU;
    }

    public Float getTexCoordV() {
        return mTexCoordV;
    }

    public void setTexCoordV(Float texCoordV) {
        mTexCoordV = texCoordV;
    }
    
    public String getColorDiffuse() {
		return mColorDiffuse;
	}
    
    public void setColorDiffuse(String colorDiffuse) {
		mColorDiffuse = colorDiffuse;
	}

    public void scale(float factor) {
        if (mPosition != null)
            mPosition.scale(factor);
    }

}
