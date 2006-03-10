package meshhandle.sceneeditor;

import java.util.ArrayList;

import meshhandle.scene.SceneNode;

public class MeshFile 
{
	private final String mFilename;
	private ArrayList<SceneNode> mSceneNodes;

	public MeshFile(String filename) 
	{
		mFilename = filename;
		mSceneNodes = new ArrayList<SceneNode>();
	}
	
	public void addSceneNode(SceneNode node)
	{
		mSceneNodes.add(node);
	}
	
	public ArrayList<SceneNode> getSceneNodes() {
		return mSceneNodes;
	}
	
	@Override
	public String toString() 
	{
		return mFilename + " ("+mSceneNodes.size()+")";
	}
	
	
}
