package meshhandle.scene;

import java.util.ArrayList;

public class Scene {
    private String mFormatVersion;

    private ArrayList<SceneNode> mNodes;

    public Scene(String formatVersion) {
        mFormatVersion = formatVersion;
        mNodes = new ArrayList<SceneNode>();
    }

    public String toXML() {
        StringBuffer buf = new StringBuffer();
        buf.append("<scene formatVersion=\"" + mFormatVersion + "\">\n");
        buf.append("  <nodes>\n");
        for (SceneNode node : mNodes) {
            buf.append(node.toXML() + "\n");
        }
        buf.append("  </nodes>\n");
        buf.append("</scene>");
        return buf.toString();
    }

    public void scale(float factor) {
        for (SceneNode node : mNodes) {
            node.scale(factor);
        }
    }

    public void addNode(SceneNode scenenode) {
        mNodes.add(scenenode);
    }
}
