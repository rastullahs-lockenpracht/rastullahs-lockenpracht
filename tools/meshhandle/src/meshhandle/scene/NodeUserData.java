package meshhandle.scene;

public class NodeUserData {
    private Integer staticGeomGroup;

    private String physicalBody;

    private Float renderingDistance;

    public String toXML() {
        StringBuffer buf = new StringBuffer();
        buf.append("            <userdata>\n");
        if (staticGeomGroup != null) {
            buf
                    .append(
                            "                <property type=\"INT\" name=\"staticgeom_group\" data=\"")
                    .append(staticGeomGroup).append("\"/>\n");
        }
        if (physicalBody != null && physicalBody != "") {
            buf
                    .append(
                            "                <property type=\"STRING\" name=\"physical_body\" data=\"")
                    .append(physicalBody).append("\"/>\n");
        }
        if (renderingDistance != null) {
            buf
                    .append(
                            "                <property type=\"FLOAT\" name=\"renderingdistance\" data=\"")
                    .append(renderingDistance).append("\"/>\n");
        }
        buf.append("            </userdata>\n");
        return buf.toString();
    }

    public String getPhysicalBody() {
        return physicalBody;
    }

    public void setPhysicalBody(String physicalBody) {
        this.physicalBody = physicalBody;
    }

    public Float getRenderingDistance() {
        return renderingDistance;
    }

    public void setRenderingDistance(Float renderingDistance) {
        this.renderingDistance = renderingDistance;
    }

    public Integer getStaticGeomGroup() {
        return staticGeomGroup;
    }

    public void setStaticGeomGroup(Integer staticGeomGroup) {
        this.staticGeomGroup = staticGeomGroup;
    }

    public void scale(float factor) {
        if (renderingDistance != null)
        {
            renderingDistance = renderingDistance * factor;
        }
    }
}
