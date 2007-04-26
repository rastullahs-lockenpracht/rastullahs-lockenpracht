package meshhandle.model.scene;

import meshhandle.data.Quaternion;
import meshhandle.data.Vector3;

public class SceneNode
{
	private String mName;
	private Vector3 mPosition;
	private Quaternion mRotation;
	private Vector3 mScale;
	private NodeUserData mUserData;
	private String mId;

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
		if (mName != null && !mName.equals(""))
		{
			return mName;
		}
		else
		{
			return "n.n.";
		}
	}

	public void setPhysicsProxy(String proxyType)
	{
		if (mUserData == null)
		{
			mUserData = new NodeUserData();
		}
		mUserData.setPhysicalBody(proxyType);
	}

	public String getId()
	{
		return mId;
	}

	public void setId(String id)
	{
		mId = id;
	}
}
