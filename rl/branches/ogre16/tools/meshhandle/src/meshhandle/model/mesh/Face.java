package meshhandle.model.mesh;

public class Face {
    private final String mVertex1;

    private final String mVertex2;

    private final String mVertex3;

    public Face(String vertex1, String vertex2, String vertex3) {
        mVertex1 = vertex1;
        mVertex2 = vertex2;
        mVertex3 = vertex3;
    }

    public String toXML() {
        return "<face v1=\"" + mVertex1 + "\" v2=\"" + mVertex2 + "\" v3=\""
                + mVertex3 + "\" />";
    }
}
