package meshhandle.model.module;

import java.io.File;
import java.io.FilenameFilter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import meshhandle.model.scene.Entity;
import meshhandle.model.scene.Scene;
import meshhandle.model.scene.SceneNode;

public class Module
{
	private final String mName;
	private List<Scene> mScenes;
	private Map<File, RubyFile> mRubyFiles;
	private List<String> mMeshFiles;
	private final String mPath;
	private List<String> mSoundFiles;
	private List<Module> mDependencies;

	public Module(String name, String path)
	{
		mName = name;
		mPath = path;		
		mRubyFiles = new HashMap<File, RubyFile>();
		mScenes = new ArrayList<Scene>();
		mSoundFiles = new ArrayList<String>();
	}
	
	public String getName()
	{
		return mName;
	}

	public void addReferencedRubyFile(RubyFile rbfile)
	{
		mRubyFiles.put(rbfile.getFile(), rbfile);
		updateMeshes();
	}
	
	public void addReferencedScene(Scene scene)
	{
		mScenes.add(scene);
		updateMeshes();
	}
	
	public void addDependency(Module mod)
	{
		mDependencies.add(mod);
	}

	private void updateMeshes()
	{
		mMeshFiles = new ArrayList<String>();
		for (Entry<File, RubyFile> curRb : mRubyFiles.entrySet())
		{
			mMeshFiles.addAll(curRb.getValue().getReferencedMeshFiles());
			mSoundFiles.addAll(curRb.getValue().getReferencedSoundFiles());
		}
		
		for (Scene curScene : mScenes)
		{
			for (SceneNode curSceneNode : curScene.getNodes())
			{
				if (curSceneNode instanceof Entity)
				{
					String meshfile = ((Entity)curSceneNode).getMeshFile();
					if (meshfile != null)
					mMeshFiles.add(meshfile);
				}
			}
		}
	}
	
	public List<String> getUsedMeshes()
	{
		return mMeshFiles;
	}
	
	public List<String> getAvailableMeshes()
	{
		List<String> rval = new ArrayList<String>();
		
		File meshdir = new File(mPath+"/models");
		if (!meshdir.exists())
		{
			return rval;
		}
		
		File[] meshfiles = meshdir.listFiles(new FilenameFilter()
		{
			public boolean accept(File dir, String name)
			{
				return name.endsWith("mesh");
			}
		});
		
		for (int i = 0; i < meshfiles.length; i++)
		{
			File mesh = meshfiles[i];
			rval .add(mesh.getName());
		}
		
		return rval;
	}
	
	public List<Module> getDependencies()
	{
		return mDependencies;
	}

	public List<RubyFile> getReferencedRubyFiles()
	{
		List<RubyFile> rval = new ArrayList<RubyFile>();
		rval.addAll(mRubyFiles.values());
		return rval;
	}

	public String getPath()
	{
		return mPath;
	}
	
	@Override
	public String toString()
	{
		return mName + " ("+mPath+")";
	}
}
