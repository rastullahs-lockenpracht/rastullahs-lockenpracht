package meshhandle.scene;

public class Entity {
    private final String mName;

    private final String mMeshFile;

    public Entity(String name, String meshFile) {
        mName = name;
        mMeshFile = meshFile;
    }

    public String toXML() {
        if (mName != null && !mName.equals("")) {
            return "<entity name=\"" + mName + "\" meshFile=\"" + mMeshFile
                    + "\"/>";
        } else {
            return "<entity meshFile=\"" + mMeshFile + "\"/>";

        }
    }

}
