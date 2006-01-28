package meshhandle.skeleton;

public class BoneParentRelation {
    private final String mParent;

    private final String mBone;

    public BoneParentRelation(String bone, String parent) {
        mBone = bone;
        mParent = parent;
    }

    public String getParent() {
        return mParent;
    }

    public String getBone() {
        return mBone;
    }

    public String toXML() {
        return "        <boneparent bone=\"" + mBone + "\" parent=\"" + mParent
                + "\" />";
    }
}
