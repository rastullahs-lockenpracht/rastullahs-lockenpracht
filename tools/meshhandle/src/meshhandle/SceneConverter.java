package meshhandle;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import javax.xml.parsers.ParserConfigurationException;
import org.xml.sax.SAXException;
import meshhandle.io.xml.scene.AbstractSceneWriter;
import meshhandle.io.xml.scene.ISceneLoader;
import meshhandle.io.xml.scene.SceneFormat2Loader;
import meshhandle.io.xml.scene.SceneFormat3Loader;
import meshhandle.model.scene.Scene;

public class SceneConverter
{
	/**
	 * @param args
	 * @throws IOException 
	 * @throws SAXException 
	 * @throws ParserConfigurationException 
	 */
	public static void main(String[] args) throws IOException, ParserConfigurationException, SAXException
	{
		String infile = null;
		String outfile = null;
		String version = null;

		for (int idx = 0; idx < args.length; idx++)
		{
			String curParam = args[idx];
			if (curParam.equals("--input"))
			{
				infile = args[idx+1];
				idx++;
			}
			else if (curParam.equals("--output"))
			{
				outfile = args[idx+1];
				idx++;
			}
			else if (curParam.equals("--outputformat"))
			{
				version = args[idx+1];
				idx++;
			}
		}
		
		if (infile == null || version == null)
		{
			System.exit(0);
		}
		
		if (outfile == null)
		{
			outfile = infile.substring(0, infile.lastIndexOf(".")) + "_out.xml";
		}
		
		ISceneLoader sceneLoader = new SceneFormat3Loader();
		Scene scene = sceneLoader.readScene(infile);
		FileWriter outputWriter = new FileWriter(new File(outfile));
		AbstractSceneWriter sceneWriter = AbstractSceneWriter.createSceneWriter(version, outputWriter);
		sceneWriter.write(scene);
		outputWriter.close();
	}
}
