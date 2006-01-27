package meshhandle.skeleton;

import java.util.ArrayList;

public class Animation {
	private String mName;
	private float mLength;
	private ArrayList<AnimationTrack> mTracks;
	
	public Animation(String name, float length)
	{
		mName = name;
		mLength = length;
		mTracks = new ArrayList<AnimationTrack>();
	}
	
	public float getLength() {
		return mLength;
	}
	public void setLength(float length) {
		mLength = length;
	}
	public String getName() {
		return mName;
	}
	public void setName(String name) {
		mName = name;
	}
	public ArrayList<AnimationTrack> getTracks() {
		return mTracks;
	}
	public void setTracks(ArrayList<AnimationTrack> tracks) {
		mTracks = tracks;
	}
	public void scale(float factor) {
		for (AnimationTrack track : mTracks) {
			track.scale(factor);
		}		
	}

	public void addTrack(AnimationTrack track) {
		mTracks.add(track);		
	}

	public String toXML() {
		StringBuffer buf = new StringBuffer();
		buf.append("        <animation name=\""+mName+"\" length=\""+mLength+"\">\n");
		buf.append("            <tracks>\n");
		for (AnimationTrack track : mTracks) 
		{
			buf.append(track.toXML()+"\n");
		}		
		buf.append("            </tracks>\n");
		buf.append("        </animation>");
		return buf.toString();
	}
}
