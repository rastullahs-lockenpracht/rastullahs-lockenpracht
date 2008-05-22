package meshhandle.model.scene;

import java.util.ArrayList;

public class Scene
{
	private ArrayList<SceneNode> mNodes;
	private SceneUserData mUserData;

	public Scene()
	{
		mNodes = new ArrayList<SceneNode>();
	}

	public void scale(float factor)
	{
		for (SceneNode node : mNodes)
		{
			node.scale(factor);
		}
		if (mUserData != null)
			mUserData.scale(factor);
	}

	public void addNode(SceneNode scenenode)
	{
		mNodes.add(scenenode);
	}

	public void addUserData(SceneUserData sceneUserData)
	{
		mUserData = sceneUserData;
	}

	public ArrayList<SceneNode> getNodes()
	{
		return mNodes;
	}

	public void removeNode(SceneNode node)
	{
		if (mNodes.contains(node))
		{
			mNodes.remove(node);
		}
	}

	public SceneUserData getUserData()
	{
		return mUserData;
	}
}
