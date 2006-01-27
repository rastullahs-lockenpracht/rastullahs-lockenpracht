package meshhandle.scene;

import meshhandle.data.Quaternion;
import meshhandle.data.Vector3;

public class SceneNode {

	private String mName;
	private Vector3 mPosition;
	private Quaternion mRotation;
	private Vector3 mScale;
	private Entity mEntity;

	public SceneNode(String name)
	{
		mName = name;		
	}
	
	
	public Entity getEntity() {
		return mEntity;
	}


	public void setEntity(Entity entity) {
		mEntity = entity;
	}


	public String getName() {
		return mName;
	}


	public void setName(String name) {
		mName = name;
	}


	public Vector3 getPosition() {
		return mPosition;
	}


	public void setPosition(Vector3 position) {
		mPosition = position;
	}


	public Quaternion getRotation() {
		return mRotation;
	}


	public void setRotation(Quaternion rotation) {
		mRotation = rotation;
	}


	public Vector3 getScale() {
		return mScale;
	}


	public void setScale(Vector3 scale) {
		mScale = scale;
	}
	
	public String toXML() {
		StringBuffer buf = new StringBuffer();
		buf.append("    <node name=\""+mName+"\">\n");
		buf.append("      "+mPosition.toXML("position")+"\n");
		buf.append("      "+mRotation.toXML("rotation")+"\n");
		buf.append("      "+mScale.toXML("scale")+"\n");
		buf.append("      "+mEntity.toXML()+"\n");
		buf.append("    </node>");
		return buf.toString();
	}

	public void scale(float factor) {
		mPosition.scale(factor);
		mScale.scale(factor);
	}

}
