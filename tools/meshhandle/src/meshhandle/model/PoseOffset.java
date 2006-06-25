package meshhandle.model;

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

	private float round(float y) {
		return Math.abs(y) < 1E-5 ? 0 : y;
	}

	public void scale(float factor) {
		this.setX(round(getX() * factor));
		this.setY(round(getY() * factor));
		this.setZ(round(getZ() * factor));
	}

	public void setIndex(String index) {
		this.index = index;
	}

	public void setX(Float x) {
		this.x = x;
	}

	public void setY(Float y) {
		this.y = y;
	}

	public void setZ(Float z) {
		this.z = z;
	}

	public Object toXML() {
		return ("            <poseoffset index=\"" + index + "\" x=\""
				+ round(x) + "\" y=\"" + round(y) + "\" z=\"" + round(z) + "\" />\n");
	}
}
