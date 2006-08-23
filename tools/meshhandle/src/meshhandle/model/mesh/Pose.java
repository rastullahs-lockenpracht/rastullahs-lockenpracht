package meshhandle.model.mesh;

import java.util.ArrayList;

public class Pose {
	private String index;
	private ArrayList<PoseOffset> mPoseOffsets;
	private String name;
	private String target;
	public Pose() {
		mPoseOffsets = new ArrayList<PoseOffset>();
		target="";
		index="";
		name="";
	}
	public String getIndex() {
		return index;
	}
	public String getName() {
		return name;
	}
	public String getTarget() {
		return target;
	}
	public void scale(float factor) {
		for (PoseOffset poseOffset: mPoseOffsets){
			poseOffset.scale(factor);
		}
	}
	public void setIndex(String index) {
		this.index = index;
	}
	public void setName(String name) {
		this.name = name;
	}
	public void setTarget(String target) {
		this.target = target;
	}
	public String toXML() {
		StringBuffer buf = new StringBuffer();
		buf.append("        <pose");
		buf.append(" target=\"" + target + "\"");
		buf.append(" index=\"" + index + "\"");
		buf.append(" name=\"" + name + "\">\n");
		for (PoseOffset poseoffset : mPoseOffsets){
			buf.append(poseoffset.toXML());
		}
		buf.append("        </pose>\n");
		return buf.toString();
	}
}
