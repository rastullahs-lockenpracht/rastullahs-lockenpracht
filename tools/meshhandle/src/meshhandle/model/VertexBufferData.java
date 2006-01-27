package meshhandle.model;

public class VertexBufferData {

	private String mTexCoordDims0;
	private String mTexCoords;
	private String mNormals;
	private String mPositions;

	public void setNormals(String normals) {
		mNormals = normals;
	}

	public void setPositions(String positions) {
		mPositions = positions;
	}

	public void setTexCoordDims0(String texCoordDims) {
		mTexCoordDims0 = texCoordDims;
	}

	public void setTexCoords(String texCoords) {
		mTexCoords = texCoords;
	}

	public String getPositions() {
		return mPositions;
	}

	public String getNormals() {
		return mNormals;
	}

	public String getTexCoordDims() {
		return mTexCoordDims0;
	}

	public String getTexCoords() {
		return mTexCoords;
	}

}
