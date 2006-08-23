package meshhandle.io.plaintext;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import meshhandle.model.module.ModuleConfig;

public class ConfigFileLoader
{

	public static ModuleConfig loadModuleConfig(String moduleRoot)
	{
		ModuleConfig conf = new ModuleConfig();
		try
		{
			BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(new File(moduleRoot, "modules.cfg"))));
			String line;
			while((line = br.readLine()) != null)
			{
				if (!line.startsWith("#") && line.contains("="))
				{
					conf.addModule(line.substring(line.indexOf('=') + 1).trim());
				}
			}
		}
		catch (FileNotFoundException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		catch (IOException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return conf;
	}
}
