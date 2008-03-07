package meshhandle.model.mesh;

public class PoseOffset {
	private String index;

	private float x;

	private float y;

	private float z;

	public PoseOffset() {
		index = "";
		x = Float.NaN;
		y = Float.NaN;
		z = Float.NaN;
	}

	public String getIndex() {
		return index;
	}

	public Float getX() {
		return x;
	}

	public Float getY() {
		return y;
	}

	public Float getZ() {
		return z;
	}

	private float round(float py) 
	{
		return Math.abs(py) < 1E-5 ? 0 : py;
	}

	public void scale(float factor) {
		this.setX(round(getX() * factor));
		this.setY(round(getY() * factor));
		this.setZ(round(getZ() * factor));
	}

	public void setIndex(String pindex) {
		this.index = pindex;
	}

	public void setX(Float px) {
		this.x = px;
	}

	public void setY(Float py) {
		this.y = py;
	}

	public void setZ(Float pz) {
		this.z = pz;
	}

	public Object toXML() {
		return ("            <poseoffset index=\"" + index + "\" x=\""
				+ round(x) + "\" y=\"" + round(y) + "\" z=\"" + round(z) + "\" />\n");
	}
}
