package meshhandle.model.mesh;

import java.util.ArrayList;

public class Track {
	private String index;

	private ArrayList<Keyframe> keyframes;

	private String target;

	private String type;

	public Track() {
		keyframes = new ArrayList<Keyframe>();
		target = "";
		index = "";
		type = "";
	}

	public String getIndex() {
		return index;
	}

	public String getTarget() {
		return target;
	}

	public String getType() {
		return type;
	}

	public void setIndex(String index) {
		this.index = index;
	}

	public void setTarget(String target) {
		this.target = target;
	}

	public void setType(String type) {
		this.type = type;
	}

	public Object toXML() {
		StringBuffer buf = new StringBuffer();
		buf.append("        		<track");
		buf.append(" target=\"" + target + "\"");
		buf.append(" index=\"" + index + "\"");
		buf.append(" type=\"" + type + "\">\n");
		buf.append("        			<keyframes>\n");
		if (!keyframes.isEmpty()) {
			for (Keyframe keyframe : keyframes) {
				buf.append(keyframe.toXML(type.equals("pose")));
			}
		}
		buf.append("        			</keyframes>\n");
		buf.append("        		</track>\n");

		return buf;
	}
}
