package meshhandle.sceneeditor;

import java.util.ArrayList;
import java.util.HashMap;

import javax.swing.tree.DefaultMutableTreeNode;

import meshhandle.model.scene.Entity;
import meshhandle.model.scene.Scene;
import meshhandle.model.scene.SceneNode;

public class SceneTreeCreator 
{
	public static DefaultMutableTreeNode createSceneTree(Scene scene)
	{
		DefaultMutableTreeNode root = new DefaultMutableTreeNode("Nodes");
		
		ArrayList<SceneNode> nodes = scene.getNodes();
		for (SceneNode node : nodes) 
		{
			root.add(new DefaultMutableTreeNode(node));
		}
		
		return root;
	}
	
	public static DefaultMutableTreeNode createEntityTree(Scene scene)
	{
		DefaultMutableTreeNode root = new DefaultMutableTreeNode("Entities");
		
		HashMap<String, DefaultMutableTreeNode> entityMap = new HashMap<String, DefaultMutableTreeNode>();
		ArrayList<SceneNode> nodes = scene.getNodes();
		for (SceneNode node : nodes) 
		{
			DefaultMutableTreeNode meshFileNode = null;

			Entity ent = null;
			if (node instanceof Entity)
			{
				ent = (Entity)node;
			}
			
			if (ent == null) 
			{
				meshFileNode = new DefaultMutableTreeNode("No meshfile");
				root.add(meshFileNode);
			} 
			else 
			{
				meshFileNode = entityMap.get(ent.getMeshFile());
				if (meshFileNode == null)
				{
					meshFileNode = new DefaultMutableTreeNode(ent.getMeshFile());
					root.add(meshFileNode);
					entityMap.put(ent.getMeshFile(), meshFileNode);
				}
			}
			
			meshFileNode.add(new DefaultMutableTreeNode(node));
		}
		
		for (int idx = 0; idx < root.getChildCount(); idx++) 
		{
			DefaultMutableTreeNode child = (DefaultMutableTreeNode) root.getChildAt(idx);
			String meshfile = (String) child.getUserObject();
			child.setUserObject(meshfile + " ("+child.getChildCount()+")");
		}
		return root;
	}
	
}
