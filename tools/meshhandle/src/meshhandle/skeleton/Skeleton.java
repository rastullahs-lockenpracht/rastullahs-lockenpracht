package meshhandle.skeleton;

import java.util.ArrayList;
import java.util.List;

public class Skeleton {
    private List<Bone> mBones;
    private List<BoneParentRelation> mBoneHierarchy;
    private List<Animation> mAnimations;
	private List<AnimationLink> mAnimationLinks;

    public Skeleton() {
        mBones = new ArrayList<Bone>();
        mBoneHierarchy = new ArrayList<BoneParentRelation>();
        mAnimations = new ArrayList<Animation>();
        mAnimationLinks = new ArrayList<AnimationLink>();
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

    public void addAnimationLink(AnimationLink link) {
		mAnimationLinks.add(link);
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

        if (!mBones.isEmpty())
        {
        	buf.append("    <bones>\n");
        	for (Bone bone : mBones) {
        		buf.append(bone.toXML() + "\n");
        	}
        	buf.append("    </bones>\n");
        }
        
        if (!mBoneHierarchy.isEmpty())
        {
	        buf.append("    <bonehierarchy>\n");
	        for (BoneParentRelation rel : mBoneHierarchy) {
	            buf.append(rel.toXML() + "\n");
	        }
	        buf.append("    </bonehierarchy>\n");
        }
        
        if (!mAnimations.isEmpty()) 
        {
        	buf.append("    <animations>\n");
        	for (Animation anim : mAnimations) {
        		buf.append(anim.toXML() + "\n");
        	}
        	buf.append("    </animations>\n"); 
        }
        
        if (!mAnimationLinks.isEmpty()) 
        {
        	buf.append("    <animationlinks>\n");
        	for (AnimationLink animlink : mAnimationLinks) {
        		buf.append(animlink.toXML() + "\n");
        	}
        	buf.append("    </animationlinks>\n"); 
        }
 
        buf.append("</skeleton>");

        return buf.toString();
    }
}
