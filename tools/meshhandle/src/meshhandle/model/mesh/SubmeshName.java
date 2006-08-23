package meshhandle.model.mesh;

public class SubmeshName {
	private String index;

	private String name;

	public String getIndex() {
		return index;
	}

	public String getName() {
		return name;
	}

	public void setIndex(String index) {
		this.index = index;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String toXML() {
		return ("        <submeshname name=\"" + name + "\" index=\"" + index + "\" />\n");
	}
}
