package meshhandle.model.scene;

public class Entity extends SceneNode
{
	private final String mName;
	private final String mMeshFile;
	private Boolean mCastShadow;
	private String mId;

	public Entity(String name, String meshFile)
	{
		super(name);
		mName = name;
		mMeshFile = meshFile;
	}

	public String getMeshFile()
	{
		return mMeshFile;
	}

	public String getName()
	{
		return mName;
	}

	public Boolean getCastShadow()
	{
		return mCastShadow;
	}

	public void setCastShadow(Boolean castShadow)
	{
		mCastShadow = castShadow;
	}

	public String getId()
	{
		return mId;
	}

	public void setId(String id)
	{
		mId = id;
	}
	
	@Override
	public String toString()
	{
		return super.toString() + " (" + mMeshFile + ")";
	}
}
