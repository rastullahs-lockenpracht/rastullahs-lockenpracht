package meshhandle.model.scene;

public class SceneUserData {
    private Float defaultRenderingDistance;

    public String toXML() {
        StringBuffer buf = new StringBuffer();
        buf.append("        <userdata>\n");
        if (defaultRenderingDistance != null) {
            buf
                    .append(
                            "            <property type=\"FLOAT\" name=\"default_renderingdistance\" data=\"")
                    .append(defaultRenderingDistance).append("\"/>\n");
        }
        buf.append("        </userdata>\n");
        return buf.toString();
    }

    public Float getDefaultRenderingDistance() {
        return defaultRenderingDistance;
    }

    public void setDefaultRenderingDistance(Float renderingDistance) {
        this.defaultRenderingDistance = renderingDistance;
    }

    public void scale(float factor) {
        if (defaultRenderingDistance != null) {
            defaultRenderingDistance = defaultRenderingDistance * factor;
        }
    }
}
