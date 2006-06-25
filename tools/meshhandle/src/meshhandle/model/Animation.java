package meshhandle.model;

import java.util.ArrayList;

public class Animation {
	private String length;

	private String name;

	private ArrayList<Track> tracks;

	public Animation() {
		tracks = new ArrayList<Track>();
		name = "";
		length = "";
	}

	public String getLength() {
		return length;
	}

	public String getName() {
		return name;
	}

	public void setLength(String length) {
		this.length = length;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String toXML() {
		StringBuffer buf = new StringBuffer();
		buf.append("        <animation");
		buf.append(" name=\"" + name + "\"");
		buf.append(" length=\"" + length + "\">\n");
		for (Track track : tracks) {
			buf.append(track.toXML());
		}
		buf.append("        </animation>\n");
		return buf.toString();
	}
}
