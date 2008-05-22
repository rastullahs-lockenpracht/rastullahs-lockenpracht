package meshhandle.model.scene;

public class NodeUserData {
	
	public static final String[] PHYSICAL_BODIES = {null, "none", "mesh"};
	
	private Integer mStaticGeomGroup;

	private String mPhysicalBody;

	private Float mRenderingDistance;

	public String toXML() {
		StringBuffer buf = new StringBuffer();
		if (mStaticGeomGroup != null) {
			buf
					.append(
							"                <property type=\"INT\" name=\"staticgeom_group\" data=\"")
					.append(mStaticGeomGroup).append("\"/>\n");
		}
		if (mPhysicalBody != null && !mPhysicalBody.equals("")) {
			buf
					.append(
							"                <property type=\"STRING\" name=\"physical_body\" data=\"")
					.append(mPhysicalBody).append("\"/>\n");
		}
		if (mRenderingDistance != null) {
			buf
					.append(
							"                <property type=\"FLOAT\" name=\"renderingdistance\" data=\"")
					.append(mRenderingDistance).append("\"/>\n");
		}
		return buf.toString();
	}

	public String getPhysicalBody() {
		return mPhysicalBody;
	}

	public void setPhysicalBody(String physicalBody) {
		this.mPhysicalBody = physicalBody;
	}

	public Float getRenderingDistance() {
		return mRenderingDistance;
	}

	public void setRenderingDistance(Float renderingDistance) {
		this.mRenderingDistance = renderingDistance;
	}

	public Integer getStaticGeomGroup() {
		return mStaticGeomGroup;
	}

	public void setStaticGeomGroup(Integer staticGeomGroup) {
		this.mStaticGeomGroup = staticGeomGroup;
	}

	public void scale(float factor) {
		if (mRenderingDistance != null) {
			mRenderingDistance = mRenderingDistance * factor;
		}
	}
}
