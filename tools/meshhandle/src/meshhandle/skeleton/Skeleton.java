package meshhandle.skeleton;

import java.util.ArrayList;

public class Skeleton {
	private ArrayList<Bone> mBones;

	private ArrayList<BoneParentRelation> mBoneHierarchy;

	private ArrayList<Animation> mAnimations;

	private ArrayList<String> mAnimationLinks;

	public Skeleton() {
		mBones = new ArrayList<Bone>();
		mBoneHierarchy = new ArrayList<BoneParentRelation>();
		mAnimations = new ArrayList<Animation>();
		mAnimationLinks = new ArrayList<String>();
	}

	public void addBone(Bone bone) {
		mBones.add(bone);
	}

	public void addBoneHierarchyRelation(String bone, String parent) {
		mBoneHierarchy.add(new BoneParentRelation(bone, parent));
	}

	public void addAnimation(Animation ani) {
		mAnimations.add(ani);
	}

	public void scale(float factor) {
		for (Bone bone : mBones) {
			bone.scale(factor);
		}
		for (Animation ani : mAnimations) {
			ani.scale(factor);
		}
	}

	public String toXML() {
		StringBuffer buf = new StringBuffer();
		buf.append("<skeleton>\n");
		buf.append("    <bones>\n");
		for (Bone bone : mBones) {
			buf.append(bone.toXML() + "\n");
		}
		buf.append("    </bones>\n");
		buf.append("    <bonehierarchy>\n");
		for (BoneParentRelation rel : mBoneHierarchy) {
			buf.append(rel.toXML() + "\n");
		}
		buf.append("    </bonehierarchy>\n");

		if (!mAnimations.isEmpty()) {
			buf.append("    <animations>\n");
			for (Animation anim : mAnimations) {
				buf.append(anim.toXML() + "\n");
			}
			buf.append("    </animations>\n");
		}
		if (!mAnimationLinks.isEmpty()) {
			buf.append("    <animationlinks>\n");
			for (String link : mAnimationLinks) {
				buf.append("    	" + link + "\n");
			}
			buf.append("    </animationlinks>\n");
		}
		buf.append("</skeleton>");

		return buf.toString();
	}
}
