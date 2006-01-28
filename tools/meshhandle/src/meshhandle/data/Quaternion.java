package meshhandle.data;

import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;

public class Quaternion {
    private float mX, mY, mZ, mW;

    public Quaternion(float w, float x, float y, float z) {
        mW = w;
        mX = x;
        mY = y;
        mZ = z;
    }

    public float getX() {
        return mX;
    }

    public void setX(float x) {
        mX = x;
    }

    public float getY() {
        return mY;
    }

    public void setY(float y) {
        mY = y;
    }

    public float getZ() {
        return mZ;
    }

    public void setZ(float z) {
        mZ = z;
    }

    public void scale(float factor) {
        mX *= factor;
        mY *= factor;
        mZ *= factor;
    }

    public static Quaternion createFromXML(Node node) {
        NamedNodeMap attrs = node.getAttributes();
        return new Quaternion(Float.parseFloat(attrs.getNamedItem("qw")
                .getNodeValue()), Float.parseFloat(attrs.getNamedItem("qx")
                .getNodeValue()), Float.parseFloat(attrs.getNamedItem("qy")
                .getNodeValue()), Float.parseFloat(attrs.getNamedItem("qz")
                .getNodeValue()));
    }

    public String toXML(String nodeName) {
        return "<" + nodeName + " qw=\"" + round(mW) + "\"" + " qx=\""
                + round(mX) + "\"" + " qy=\"" + round(mY) + "\"" + " qz=\""
                + round(mZ) + "\" />";
    }

    private float round(float y) {
        return Math.abs(y) < 1E-5 ? 0 : y;
    }

    public float getW() {
        return mW;
    }

    public void setW(float w) {
        mW = w;
    }

}
