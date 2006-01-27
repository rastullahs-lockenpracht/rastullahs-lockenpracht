package meshhandle.scene;

public class Entity {
	private final String mName;
	private final String mMeshFile;

	public Entity(String name, String meshFile)
	{
		mName = name;
		mMeshFile = meshFile;
	}

	public String toXML() {
		return "<entity name=\""+mName+"\" meshFile=\""+mMeshFile+"\"/>";
	}
	
	
}
