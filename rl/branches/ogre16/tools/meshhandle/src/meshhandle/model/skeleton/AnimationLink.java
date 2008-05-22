package meshhandle.model.skeleton;

public class AnimationLink
{
	private final String mName;
	private Float mScale;

	public AnimationLink(String skeletonName)
	{
		mName = skeletonName;
		mScale = null;
	}

	public AnimationLink(String skeletonName, float scale)
	{
		mName = skeletonName;
		mScale = scale;
	}

	public String toXML()
	{
		// TODO Auto-generated method stub
		return null;
	}
}
