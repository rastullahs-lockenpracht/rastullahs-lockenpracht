package meshhandle.model;

import java.util.ArrayList;

public class Mesh {
    private ArrayList<Submesh> mSubmeshes;

    private String mSkeletonLink;

    public Mesh() {
        mSubmeshes = new ArrayList<Submesh>();
        mSkeletonLink = "";
    }

    public void scale(float factor) {
        for (Submesh submesh : mSubmeshes) {
            submesh.scale(factor);
        }
    }

    public String toXML() {
        StringBuffer buf = new StringBuffer();
        buf.append("<mesh>\n");
        buf.append("    <submeshes>\n");
        for (Submesh submesh : mSubmeshes) {
            buf.append(submesh.toXML() + "\n");
        }
        buf.append("    </submeshes>\n");
        if (mSkeletonLink != null && mSkeletonLink != "") {
            buf.append("    <skeletonlink name=\"").append(mSkeletonLink)
                    .append("\" />\n");
        }
        buf.append("</mesh>");
        return buf.toString();
    }

    public void addSubmesh(Submesh submesh) {
        mSubmeshes.add(submesh);
    }

    public String getSkeletonLink() {
        return mSkeletonLink;
    }

    public void setSkeletonLink(String skeletonLink) {
        mSkeletonLink = skeletonLink;
    }
}
