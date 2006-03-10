package meshhandle.scene;

import java.util.ArrayList;

public class Scene {
    private String mFormatVersion;

    private ArrayList<SceneNode> mNodes;

    private SceneUserData mUserData;

    public Scene(String formatVersion) {
        mFormatVersion = formatVersion;
        mNodes = new ArrayList<SceneNode>();
    }

    public String toXML() {
        StringBuffer buf = new StringBuffer();
        buf.append("<scene formatVersion=\"" + mFormatVersion + "\">\n");
        buf.append("    <nodes>\n");
        for (SceneNode node : mNodes) {
            buf.append(node.toXML() + "\n");
        }
        buf.append("    </nodes>\n");
        if (mUserData != null)
            buf.append(mUserData.toXML());
        buf.append("</scene>");
        return buf.toString();
    }

    public void scale(float factor) {
        for (SceneNode node : mNodes) {
            node.scale(factor);
        }
        if (mUserData != null)
            mUserData.scale(factor);
    }

    public void addNode(SceneNode scenenode) {
        mNodes.add(scenenode);
    }

    public void addUserData(SceneUserData sceneUserData) {
        mUserData = sceneUserData;
    }

	public ArrayList<SceneNode> getNodes() {
		return mNodes;
	}

	public void removeNode(SceneNode node) {
		if (mNodes.contains(node)) {
			mNodes.remove(node);
		}
	}
}
