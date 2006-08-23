package meshhandle.moduleanalysis;

import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;
import javax.swing.tree.DefaultMutableTreeNode;
import meshhandle.model.module.Module;

public class ModuleTreeBuilder
{
	private DefaultMutableTreeNode mRootNode;
	private List<Module> mModules;
	
	public ModuleTreeBuilder()
	{
	}

	public DefaultMutableTreeNode convert(List<Module> modules)
	{
		mModules = modules;
		mRootNode = new DefaultMutableTreeNode("Modules");
		
		for (Module mod : modules)
		{
			DefaultMutableTreeNode modNode = new DefaultMutableTreeNode(mod);
			mRootNode.add(modNode);
			
			for (String curMesh : mod.getAvailableMeshes())
			{
				modNode.add(new MeshTreeNode(curMesh));
			}
		}
		
		findDuplicateModels();
		findUnusedModels();
		
		return mRootNode;
	}

	private void findUnusedModels()
	{
	}

	private void findDuplicateModels()
	{
		List<MeshTreeNode> allMeshes = new ArrayList<MeshTreeNode>();
		for (Module mod : mModules)
		{
			DefaultMutableTreeNode modNode = findModuleNode(mod);
			for (Enumeration childEnum = modNode.children(); childEnum.hasMoreElements();)
			{
				MeshTreeNode curChild = (MeshTreeNode)childEnum.nextElement();
				MeshTreeNode duplicate = findMeshNode(allMeshes, curChild.getMeshFile());
				
				if (duplicate != null)
				{
					duplicate.setDuplicate(true);
					curChild.setDuplicate(true);
				}
				else
				{
					allMeshes.add(curChild);
				}
			}
		}

		for (Module mod : mModules)
		{
			for (String curMesh : mod.getUsedMeshes())
			{
				MeshTreeNode listElem = findMeshNode(allMeshes, curMesh);
				
				if (listElem != null)
				{
					listElem.setUnused(false);
				}
			}
		}
	}

	private MeshTreeNode findMeshNode(List<MeshTreeNode> allMeshes, String meshfile)
	{
		for (MeshTreeNode meshTreeNode : allMeshes)
		{
			if (meshfile.equals(meshTreeNode.getMeshFile()))
			{
				return meshTreeNode;
			}
		}
		
		System.out.println("'"+meshfile+"' not found.");
		return null;
	}

	private DefaultMutableTreeNode findModuleNode(Module mod)
	{		
		for(Enumeration modNodeEnum = mRootNode.children(); modNodeEnum.hasMoreElements();)
		{
			DefaultMutableTreeNode cur = (DefaultMutableTreeNode)modNodeEnum.nextElement();
			if (cur.getUserObject().equals(mod))
				return cur;
		}
		return null;
	}
}
