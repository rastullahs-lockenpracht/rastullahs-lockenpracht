package meshhandle.model;

import java.util.ArrayList;

public class LevelOfDetail {
	private ArrayList<String> lodmanual;

	private boolean manual;

	private ArrayList<Face> mLodFaceList;

	private String numLevels;

	private String submeshindex;

	public LevelOfDetail() {
		lodmanual = new ArrayList<String>();
		numLevels = "";
		manual = false;
		mLodFaceList = new ArrayList<Face>();
		submeshindex = "";
	}

	public boolean exists(){
		return (!lodmanual.isEmpty()||numLevels.length()>0);
	}

	public String getNumLevels() {
		return numLevels;
	}

	public boolean isManual() {
		return manual;
	}

	public void setManual(boolean manual) {
		this.manual = manual;
	}

	public void setNumLevels(String numLevels) {
		this.numLevels = numLevels;
	}
	public String toXML() {
		StringBuffer buf = new StringBuffer();
		buf.append("            <levelofdetail");
		buf.append(" numlevels=\"" + numLevels + "\"");
		buf.append(" manual=\"" + manual + "\">");
		if (manual) {
			buf.append("            	<lodmanual " + lodmanual + "/>\n");
		} else if (!mLodFaceList.isEmpty()) {
			buf.append("            	<lodgenerated " + lodmanual + ">\n");
			buf.append("	            	<lodfacelist submeshindex=\""
					+ submeshindex + "\" numfaces=\"" + mLodFaceList.size()
					+ "\">\n");
			for (Face lodface : mLodFaceList) {
				buf.append("	            		" + lodface.toXML() + "\n");
			}
			buf.append("	            	</lodfacelist>");
			buf.append("            	</lodgenerated>\n");
		}
		buf.append("            </levelofdetail>\n");
		return buf.toString();
	}
}
