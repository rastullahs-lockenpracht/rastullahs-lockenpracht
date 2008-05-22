package meshhandle.model.module;

import java.util.ArrayList;
import java.util.List;

public class ModuleConfig
{
	private List<String> mModules;

	public ModuleConfig()
	{
		mModules = new ArrayList<String>();
	}

	public void addModule(String module)
	{
		System.out.println("Module: " + module);
		mModules.add(module);
	}
	
	public List<String> getModules()
	{
		return mModules;
	}
}
