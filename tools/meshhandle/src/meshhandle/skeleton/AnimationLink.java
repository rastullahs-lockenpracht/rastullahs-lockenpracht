package meshhandle.skeleton;

public class AnimationLink {
	
	private final String mSkeletonName;
	private final float mScale;

	public AnimationLink(String skeletonName) {
		this(skeletonName, 1.0f);
	}

	public AnimationLink(String skeletonName, float scale) {
		mSkeletonName = skeletonName;
		mScale = scale;
	}

	public String toXML()
	{
		StringBuffer result = new StringBuffer();
		result.append("    <animationlink ");
		result.append("skeletonName=\"");
		result.append(mSkeletonName);
		result.append("\" scale=\"");
		result.append(mScale);
		result.append("\"/>");
		return result.toString();
	}
}
