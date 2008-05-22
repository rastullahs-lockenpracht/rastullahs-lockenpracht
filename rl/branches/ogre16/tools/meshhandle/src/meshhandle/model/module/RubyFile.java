package meshhandle.model.module;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class RubyFile
{
	private final File mFile;
	private List<String> mReferencedRubyFiles;
	private List<String> mReferencedSceneFiles;
	private List<String> mReferencedSoundFiles;
	private List<String> mReferencedMeshFiles;
	
	public RubyFile(File file)
	{
		mFile = file;
		mReferencedMeshFiles = new ArrayList<String>();
		mReferencedSceneFiles = new ArrayList<String>();
		mReferencedSoundFiles = new ArrayList<String>();
		mReferencedRubyFiles = new ArrayList<String>();
	}
	
	public void addReferencedFile(String filename)
	{
		if (filename.endsWith(".rb"))
			mReferencedRubyFiles.add(filename);
		else if (filename.endsWith(".ogg") || filename.endsWith(".wav"))
			mReferencedSoundFiles.add(filename);
		else if (filename.endsWith(".scene"))
			mReferencedSceneFiles.add(filename);
		else if (filename.endsWith(".mesh"))
			mReferencedMeshFiles.add(filename);
	}

	public File getFile()
	{
		return mFile;
	}

	public List<String> getReferencedMeshFiles()
	{
		return mReferencedMeshFiles;
	}

	public List<String> getReferencedRubyFiles()
	{
		return mReferencedRubyFiles;
	}

	public List<String> getReferencedSceneFiles()
	{
		return mReferencedSceneFiles;
	}

	public List<String> getReferencedSoundFiles()
	{
		return mReferencedSoundFiles;
	}	
	
	@Override
	public boolean equals(Object obj)
	{
		if (!(obj instanceof RubyFile))
			return false;
		
		RubyFile rbf = (RubyFile)obj;
		return rbf.getFile().getAbsolutePath().equals(mFile.getAbsolutePath());
	}
}
