package meshhandle;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

import javax.swing.JOptionPane;

import meshhandle.io.xml.scene.AbstractSceneWriter;
import meshhandle.io.xml.scene.ISceneLoader;
import meshhandle.io.xml.scene.SceneFormat3Loader;
import meshhandle.io.xml.scene.SceneFormat4Writer;
import meshhandle.model.scene.Scene;

public class SceneConverter
{
	/**
	 * Opens and reads a xml-file 
	 *   --input
	 */
	public static void main(String[] args)
	{
		String infile = null;
		String outfile = null;
		String version = null;
		
		boolean infilefound = false;

		for (int idx = 0; idx < args.length; idx++)
		{
			String curParam = args[idx];
			if (curParam.equals("--input"))
			{
				infile = args[idx+1];
				infilefound = true;
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
			// All unused paramters are used as infiles
			else if( !infilefound )
			{
				infile = curParam;
			}
		}
		
		if( version == null )
		{
			version = SceneFormat4Writer.FORMAT_STRING;
		}
		
		if (infile == null )
		{
			showError( "No input file found" );
		}
		
		if (outfile == null)
		{
			File in = new File(infile);
			String inname = in.getName();
			String dirname = in.getParentFile().getAbsolutePath();
			outfile = inname.substring(0, inname.lastIndexOf(".")) + "_out.xml";
			outfile = dirname+"/"+outfile;
		}
		
		ISceneLoader sceneLoader = new SceneFormat3Loader();
		Scene scene = null;
		FileWriter outputWriter = null;
		
		try
		{
			scene = sceneLoader.readScene(infile);
		} 
		catch (Throwable t )
		{
			showError( "Could not read scene" );
		}
		
		try
		{
			outputWriter = new FileWriter(new File(outfile));
		} 
		catch (Throwable t )
		{
			showError( "Could not initialize outfile" );
		}
		
		AbstractSceneWriter sceneWriter = AbstractSceneWriter.createSceneWriter(version, outputWriter);
		
		if( sceneWriter == null )
		{
			showError( "No matching SceneWriter found" );
		}
		
		try
		{
			sceneWriter.write(scene);
		} 
		catch (IOException e)
		{
			showError( "Could not write to outfile" );
		}
		finally
		{
			try
			{
				outputWriter.close();
			} 
			catch (IOException e) {}
		}
	}

	private static void showError(String message)
	{
		JOptionPane.showMessageDialog(
				null, 
				message, 
				"Error",
				JOptionPane.ERROR_MESSAGE );
		System.exit(0);
	}
}
