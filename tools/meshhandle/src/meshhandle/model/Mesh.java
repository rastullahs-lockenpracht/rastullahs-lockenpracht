package meshhandle.model;

import java.util.ArrayList;

import meshhandle.data.Vector3;

public class Mesh {
	private ArrayList<Submesh> mSubmeshes;

	private String mSkeletonLink;

	private String name;
	
	public Mesh() {
		mSubmeshes = new ArrayList<Submesh>();
		mSkeletonLink = "";
		name = "";
	}

	public void scale(float factor) {
		for (Submesh submesh : mSubmeshes) {
			submesh.scale(factor);
		}
	}

	public String toXML() {
		StringBuffer buf = new StringBuffer();
		buf.append("<mesh>\n");
		
		buf.append("    <submeshes>\n");
		for (Submesh submesh : mSubmeshes) {
			buf.append(submesh.toXML() + "\n");
		}
		buf.append("    </submeshes>\n");

		if (mSkeletonLink != null && !mSkeletonLink.equals("")) {
			buf.append("    <skeletonlink name=\"").append(mSkeletonLink)
					.append("\"/>\n");
		}
		
		buf.append("</mesh>");
		return buf.toString();
	}

	public void addSubmesh(Submesh submesh) {
		mSubmeshes.add(submesh);
	}

	public String getSkeletonLink() {
		return mSkeletonLink;
	}

	public void setSkeletonLink(String skeletonLink) {
		mSkeletonLink = skeletonLink;
	}

	public int getPolycount() {
		int polycount = 0;
		for (Submesh submesh : mSubmeshes) {
			polycount = polycount + submesh.getFaceCount();
		}
		return polycount;
	}

	public ArrayList<String> getMaterials() {
		ArrayList<String> materials = new ArrayList<String>(mSubmeshes.size());
		for (Submesh submesh : mSubmeshes) {
			materials.add(submesh.getMaterial());
		}
		return materials;
	}

	public Vector3 getMeasures() {
		float xmin = 0;
		float xmax = 0;
		float ymin = 0;
		float ymax = 0;
		float zmin = 0;
		float zmax = 0;
		for (Submesh submesh : mSubmeshes) {
			if (submesh.getXMin() < xmin) {
				xmin = submesh.getXMin();
			}
			if (submesh.getXMax() > xmax) {
				xmax = submesh.getXMax();
			}
			if (submesh.getYMin() < ymin) {
				ymin = submesh.getYMin();
			}
			if (submesh.getYMax() > ymax) {
				ymax = submesh.getYMax();
			}
			if (submesh.getZMin() < zmin) {
				zmin = submesh.getZMin();
			}
			if (submesh.getZMax() > zmax) {
				zmax = submesh.getZMax();
			}
		}
		return new Vector3((xmax - xmin), (ymax - ymin), (zmax - zmin));
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
		if (this.getSkeletonLink()!="")
		this.setSkeletonLink(name+".skeleton");
	}
}
