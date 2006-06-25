package meshhandle.model;

import java.util.ArrayList;

import meshhandle.data.Vector3;

public class Keyframe {

	private ArrayList<String> mPoserefs;

	private ArrayList<Vector3> mPositions;

	private String time;

	public Keyframe() {
		mPositions = new ArrayList<Vector3>();
		mPoserefs = new ArrayList<String>();
	}

	public Object toXML(boolean isPose) {
		StringBuffer buf = new StringBuffer();
		buf.append("        			<keyframe time=\"" + time + "\" >\n");
		if (isPose && !mPoserefs.isEmpty()) {
			for (String poseref : mPoserefs) {
				buf.append(poseref);
			}
		} else if (!isPose && !mPositions.isEmpty()) {
			for (Vector3 position : mPositions) {
				buf.append(position.toXML("position"));
			}
		}
		buf.append("        			</keyframe>\n");
		return buf.toString();
	}

}
