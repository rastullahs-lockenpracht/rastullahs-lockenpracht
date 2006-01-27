package meshhandle.model;

import java.util.ArrayList;

public class AnimationTrack {
	private String mBone;
	private ArrayList<Keyframe> mFrames;
	public AnimationTrack(String bone) {
		mBone = bone;
		mFrames = new ArrayList<Keyframe>();
	}
	public String getBone() {
		return mBone;
	}
	public void setBone(String bone) {
		mBone = bone;
	}
	public ArrayList<Keyframe> getFrames() {
		return mFrames;
	}
	public void setFrames(ArrayList<Keyframe> frames) {
		mFrames = frames;
	}
	public void scale(float factor) {
		for (Keyframe frame : mFrames) {
			frame.scale(factor);
		}		
	}
	public void addKeyframe(Keyframe keyframe) {
		mFrames.add(keyframe);		
	}
	public String toXML() 
	{
		StringBuffer buf = new StringBuffer();
		buf.append("                <track bone=\""+mBone+"\">\n");
		buf.append("                    <keyframes>\n");
		for (Keyframe frame : mFrames) 
		{
			buf.append(frame.toXML()+"\n");
		}
		buf.append("                    </keyframes>\n");		
		buf.append("                </track>");
		return buf.toString();
	}
}
