package meshhandle.moduleanalysis;

import javax.swing.tree.DefaultMutableTreeNode;

public class MeshTreeNode extends DefaultMutableTreeNode
{
	private static final long serialVersionUID = 6096751072033048156L;
	private boolean mDuplicate;
	private boolean mUnused;
	private final String mMeshfile;

	public MeshTreeNode(String meshfile)
	{
		mMeshfile = meshfile;
		mUnused = true;
		mDuplicate = false;
	}
	
	public void setDuplicate(boolean duplicate)
	{
		mDuplicate = duplicate;
	}
	
	public boolean isDuplicate()
	{
		return mDuplicate;
	}
	
	public void setUnused(boolean unused)
	{
		mUnused = unused;
	}
	
	public boolean isUnused()
	{
		return mUnused;
	}
	
	@Override
	public String toString()
	{
		StringBuffer buf = new StringBuffer();
		if (mUnused || mDuplicate)
		{
			buf.append("<html><b>");
		}
		
		buf.append(mMeshfile);
		if (mUnused)
		{
			buf.append(" [unused]");
		}
		if (mDuplicate)
		{
			buf.append(" [duplicate]");
		}
		
		if (mUnused || mDuplicate)
		{
			buf.append("</b></html>");
		}
		return buf.toString();
	}

	public String getMeshFile()
	{
		return mMeshfile;
	}	
}
