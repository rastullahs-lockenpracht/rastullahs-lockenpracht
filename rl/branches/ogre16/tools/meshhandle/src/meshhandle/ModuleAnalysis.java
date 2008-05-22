package meshhandle;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.InvalidPropertiesFormatException;
import java.util.Properties;
import meshhandle.moduleanalysis.AnalysisWindow;

public class ModuleAnalysis
{
	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		new ModuleAnalysis().run();
	}

	private void run()
	{
		Properties options = new Properties();
		try
		{
			File conffile = new File("moduleanalysis.conf");
			options.load(new FileInputStream(conffile ));
			new AnalysisWindow(options).setVisible(true);
		}
		catch (InvalidPropertiesFormatException ex)
		{
			// TODO Auto-generated catch block
			ex.printStackTrace();
		}
		catch (FileNotFoundException ex)
		{
			// TODO Auto-generated catch block
			ex.printStackTrace();
		}
		catch (IOException ex)
		{
			// TODO Auto-generated catch block
			ex.printStackTrace();
		}
		
	}
}
