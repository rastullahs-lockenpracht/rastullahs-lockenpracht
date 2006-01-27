package meshhandle.skeleton;

import meshhandle.data.Vector3;

public class Keyframe {
	private float mTime;
	private Vector3 mTranslation;
	private Vector3 mRotationAxis;
	private float mRotationAngle;
	private Vector3 mScale;
	
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
	public Vector3 getScale() {
		return mScale;
	}
	public void setScale(Vector3 scale) {
		mScale = scale;
	}
	public float getTime() {
		return mTime;
	}
	public void setTime(float time) {
		mTime = time;
	}
	public Vector3 getTranslation() {
		return mTranslation;
	}
	public void setTranslation(Vector3 translation) {
		mTranslation = translation;
	}
	public void scale(float factor) {
		mTranslation.scale(factor);		
	}
	public String toXML() {
        StringBuffer buf = new StringBuffer();
        buf.append("                        <keyframe time=\""+mTime+"\">\n");
        buf.append("                            "+mTranslation.toXML("translate")+"\n");
        buf.append("                            <rotate angle=\""+mRotationAngle+"\">\n");
        buf.append("                                "+mRotationAxis.toXML("axis")+"\n");
        buf.append("                            </rotate>\n");
        buf.append("                            "+mScale.toXML("scale")+"\n");
        buf.append("                        </keyframe>");

		return buf.toString();
	}
}
