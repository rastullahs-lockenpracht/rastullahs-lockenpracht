package meshhandle.io.plaintext;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import meshhandle.model.module.RubyFile;

public class RubyFileLoader
{
	private static List<String> msSearchPaths;
	private static Pattern msRubyFilePattern;
	private static Pattern msSoundFilePattern;
	private static Pattern msMeshFilePattern;
	private static Pattern msSceneFilePattern;
	
	static 
	{
		msSearchPaths = new ArrayList<String>();
		
		msRubyFilePattern = Pattern.compile("[\\\"']([\\w]*\\.rb)[\\\"']");
		msSoundFilePattern = Pattern.compile("[\\\"']([\\w]*\\.ogg)[\\\"']");
		msMeshFilePattern = Pattern.compile("[\\\"']([\\w]*\\.mesh)[\\\"']");
		msSceneFilePattern = Pattern.compile("[\\\"']([\\w]*\\.scene)[\\\"']");
	}

	public static void addSearchPath(String path)
	{
		msSearchPaths.add(path);
	}
	
	public static RubyFile loadRubyFile(String filename)
	{
		File file = getFile(filename);
		if (file == null)
		{
			return null;
		}
		
		RubyFile rb = new RubyFile(file);
		processRubyFile(file, rb);
		return rb;
	}

	private static void processRubyFile(File file, RubyFile rb)
	{
		try
		{
			BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
			String line;
			
			while((line = br.readLine()) != null)
			{
				if (!isComment(line))
				{
					List<String> usedFiles = 
						extractUsedFiles(line);
					for (String fileRef : usedFiles)
					{
						rb.addReferencedFile(fileRef);
					}
				}
			}			
		}
		catch (FileNotFoundException ex)
		{
			ex.printStackTrace();
		}
		catch (IOException ex)
		{
			ex.printStackTrace();
		}
		
	}

	private static List<String> extractUsedFiles(String line)
	{
		List<String> filelist = new ArrayList<String>();
		
		Matcher m1 = msMeshFilePattern.matcher(line);
		while (m1.find())
		{
			filelist.add(m1.group(1));
		}
		Matcher m2 = msSoundFilePattern.matcher(line);
		while (m2.find())
		{
			filelist.add(m2.group(1));
		}
		Matcher m3 = msRubyFilePattern.matcher(line);
		while (m3.find())
		{
			filelist.add(m3.group(1));
		}
		Matcher m4 = msSceneFilePattern.matcher(line);
		while (m4.find())
		{
			filelist.add(m4.group(1));
		}
		return filelist;
	}

	private static boolean isComment(String line)
	{
		return line.startsWith("#");
	}

	private static File getFile(String filename)
	{
		File file = new File(filename);
		if (file.exists())
		{
			return file;
		}
		
		for (String curPath : msSearchPaths)
		{
			file = new File(curPath, filename);
			if (file.exists())
			{
				return file;
			}
		}
		return null;
	}
}
