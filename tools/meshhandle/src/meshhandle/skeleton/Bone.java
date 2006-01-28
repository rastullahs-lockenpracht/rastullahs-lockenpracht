package meshhandle.skeleton;

import meshhandle.data.Vector3;

public class Bone {
    private Vector3 mPosition;

    private Vector3 mRotationAxis;

    private float mRotationAngle;

    private final int mId;

    private final String mName;

    public Bone(int id, String name) {
        mId = id;
        mName = name;
    }

    public Vector3 getPosition() {
        return mPosition;
    }

    public void setPosition(Vector3 position) {
        mPosition = position;
    }

    public float getRotationAngle() {
        return mRotationAngle;
    }

    public void setRotationAngle(float rotationAngle) {
        mRotationAngle = rotationAngle;
    }

    public Vector3 getRotationAxis() {
        return mRotationAxis;
    }

    public void setRotationAxis(Vector3 rotationAxis) {
        mRotationAxis = rotationAxis;
    }

    public void scale(float factor) {
        mPosition.scale(factor);
    }

    public String getName() {
        return mName;
    }

    public int getId() {
        return mId;
    }

    public String toXML() {
        StringBuffer buf = new StringBuffer();
        buf
                .append("        <bone id=\"" + mId + "\" name=\"" + mName
                        + "\">\n");
        buf.append("            " + mPosition.toXML("position") + "\n");
        buf.append("            <rotation angle=\"" + mRotationAngle + "\">\n");
        buf.append("                " + mRotationAxis.toXML("axis") + "\n");
        buf.append("            </rotation>\n");
        buf.append("        </bone>");
        return buf.toString();
    }
}
