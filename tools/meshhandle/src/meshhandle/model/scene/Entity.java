package meshhandle.model.scene;

public class Entity
{
	private final String mName;
	private final String mMeshFile;
	private Boolean mCastsShadows;
	private String mId;

	public Entity(String name, String meshFile)
	{
		mName = name;
		mMeshFile = meshFile;
	}

	public String toXML()
	{
		StringBuffer buf = new StringBuffer();
		buf.append("<entity");
		if (mName != null && !mName.equals(""))
		{
			buf.append(" name=\"" + mName + "\""); 
		}
		if (mId != null && !mId.equals(""))
		{
			buf.append(" id=\"" + mId + "\""); 
		}
		buf.append(" meshFile=\"" + mMeshFile + "\"");
		
		if (mCastsShadows != null)
		{
			if (mCastsShadows == true)
			{
				buf.append(" castsShadows=\"true\"");
			}
			else if (mCastsShadows == false)
			{
				buf.append(" castsShadows=\"false\"");
			}
		}
		buf.append("/>");
		
		return buf.toString();
	}

	public String getMeshFile()
	{
		return mMeshFile;
	}

	public String getName()
	{
		return mName;
	}

	public Boolean getCastsShadows()
	{
		return mCastsShadows;
	}

	public void setCastsShadows(Boolean castsShadows)
	{
		mCastsShadows = castsShadows;
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
