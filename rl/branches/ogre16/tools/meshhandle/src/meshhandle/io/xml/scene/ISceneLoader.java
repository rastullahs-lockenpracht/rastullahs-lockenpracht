package meshhandle.io.xml.scene;

import java.io.IOException;
import javax.xml.parsers.ParserConfigurationException;
import meshhandle.model.scene.Scene;
import org.xml.sax.SAXException;

public interface ISceneLoader
{

	public Scene readScene(String fileName) throws ParserConfigurationException,
			SAXException, IOException;
	
	public String getFormatVersion();
}
