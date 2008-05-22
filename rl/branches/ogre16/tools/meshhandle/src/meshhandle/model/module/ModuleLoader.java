package meshhandle.model.module;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import javax.xml.parsers.ParserConfigurationException;
import meshhandle.io.plaintext.RubyFileLoader;
import meshhandle.io.xml.scene.SceneFormat2Loader;
import meshhandle.model.scene.Scene;
import org.xml.sax.SAXException;

public class ModuleLoader
{
	public static void resolve(Module module)
	{
		boolean rerun = false;
		Collection<RubyFile> rubyfiles = module.getReferencedRubyFiles();
		
		for (RubyFile file : rubyfiles)
		{
			System.out.println("Processing "+file.getFile().getAbsolutePath());
			List<Scene> scenes = getScenes(module, file);
			for (Scene curScene : scenes)
			{
				module.addReferencedScene(curScene);
			}
			
			for (String refFile : file.getReferencedRubyFiles())
			{
				RubyFile refRbFile = RubyFileLoader.loadRubyFile(refFile);
				if (refRbFile != null
					&& !rubyfiles.contains(refRbFile))
				{
					module.addReferencedRubyFile(refRbFile);
					rerun = true;
				}
			}
		}
		
		if (rerun)
		{
			resolve(module);
		}
	}

	private static List<Scene> getScenes(Module mod, RubyFile refRbFile)
	{
		List<Scene> list = new ArrayList<Scene>();
		
		List<String> referencedSceneFiles = refRbFile.getReferencedSceneFiles();
		for (String sceneFile : referencedSceneFiles)
		{
			try
			{
				System.out.print("Processing "+mod.getPath() + "/maps/" + sceneFile);
				Scene scene = new SceneFormat2Loader().readScene(
						mod.getPath() + "/maps/" + sceneFile);
				list.add(scene);
				System.out.println(" done");
			}
			catch (ParserConfigurationException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			catch (SAXException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			catch (IOException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
		}
		return list;
	}
}
