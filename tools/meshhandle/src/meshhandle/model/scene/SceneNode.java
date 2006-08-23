package meshhandle.model.scene;

import meshhandle.data.Quaternion;
import meshhandle.data.Vector3;

public class SceneNode
{
	private String mName;
	private Vector3 mPosition;
	private Quaternion mRotation;
	private Vector3 mScale;
	private Entity mEntity;
	private NodeUserData mUserData;

	public NodeUserData getUserData()
	{
		return mUserData;
	}

	public void setUserData(NodeUserData userData)
	{
		mUserData = userData;
	}

	public SceneNode(String name)
	{
		mName = name;
	}

	public Entity getEntity()
	{
		return mEntity;
	}

	public void setEntity(Entity entity)
	{
		mEntity = entity;
	}

	public String getName()
	{
		return mName;
	}

	public void setName(String name)
	{
		mName = name;
	}

	public Vector3 getPosition()
	{
		return mPosition;
	}

	public void setPosition(Vector3 position)
	{
		mPosition = position;
	}

	public Quaternion getRotation()
	{
		return mRotation;
	}

	public void setRotation(Quaternion rotation)
	{
		mRotation = rotation;
	}

	public Vector3 getScale()
	{
		return mScale;
	}

	public void setScale(Vector3 scale)
	{
		mScale = scale;
	}

	public String toXML()
	{
		StringBuffer buf = new StringBuffer();
		if (mName != null && !mName.equals(""))
			buf.append("        <node name=\"" + mName + "\">\n");
		else
			buf.append("        <node>\n");
		if (mPosition != null)
			buf.append("            " + mPosition.toXML("position") + "\n");
		if (mRotation != null)
			buf.append("            " + mRotation.toXML("rotation") + "\n");
		if (mScale != null)
			buf.append("            " + mScale.toXML("scale") + "\n");
		if (mEntity != null)
			buf.append("            " + mEntity.toXML() + "\n");
		if (mUserData != null)
			buf.append(mUserData.toXML());
		buf.append("        </node>");
		return buf.toString();
	}

	public void scale(float factor)
	{
		if (mPosition != null)
			mPosition.scale(factor);
		if (mUserData != null)
			mUserData.scale(factor);
	}

	@Override
	public String toString()
	{
		StringBuffer buf = new StringBuffer();
		if (mName != null && !mName.equals(""))
			buf.append(mName);
		else
			buf.append("n.n.");
		if (mEntity != null)
			buf.append(", " + mEntity.getMeshFile());
		return buf.toString();
	}

	public void setPhysicsProxy(String proxyType)
	{
		if (mUserData == null)
		{
			mUserData = new NodeUserData();
		}
		mUserData.setPhysicalBody(proxyType);
	}
}
