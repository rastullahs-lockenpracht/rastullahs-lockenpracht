package meshhandle.model;

import java.util.ArrayList;

import meshhandle.data.Vector3;

public class Mesh {
	private LevelOfDetail lod;

	private ArrayList<Animation> mAnimations;

	private ArrayList<VertexBoneAssignment> mBoneAssignments;

	private ArrayList<Pose> mPoses;

	private String mSkeletonLink;

	private ArrayList<Submesh> mSubmeshes;

	private ArrayList<SubmeshName> mSubmeshNames;

	private String name;

	private ArrayList<VertexBufferData> sharedGeometry;

	public Mesh() {
		sharedGeometry = new ArrayList<VertexBufferData>();
		mSubmeshes = new ArrayList<Submesh>();
		mSkeletonLink = "";
		name = "";
		mBoneAssignments = new ArrayList<VertexBoneAssignment>();
		lod = new LevelOfDetail();
		mSubmeshNames = new ArrayList<SubmeshName>();
		mPoses = new ArrayList<Pose>();
		mAnimations = new ArrayList<Animation>();
	}

	public void addBoneAssignment(VertexBoneAssignment vba) {
		mBoneAssignments.add(vba);
	}

	public void addSubmesh(Submesh submesh) {
		mSubmeshes.add(submesh);
	}

	public void addSubmeshName(SubmeshName smn) {
		mSubmeshNames.add(smn);
	}

	public void addVertexBuffer(VertexBufferData sharedVertex) {
		sharedGeometry.add(sharedVertex);
	}

	public ArrayList<String> getMaterials() {
		ArrayList<String> materials = new ArrayList<String>(mSubmeshes.size());
		for (Submesh submesh : mSubmeshes) {
			if (!materials.contains(submesh.getMaterial())) {
				materials.add(submesh.getMaterial());
			}
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

	public int getPolycount() {
		int polycount = 0;
		for (Submesh submesh : mSubmeshes) {
			polycount = polycount + submesh.getFaceCount();
		}
		return polycount;
	}

	public String getSkeletonLink() {
		return mSkeletonLink;
	}

	public void scale(float factor) {
		for (Submesh submesh : mSubmeshes) {
			submesh.scale(factor);
		}
		for (Pose pose : mPoses) {
			pose.scale(factor);
		}
	}

	public void setName(String name) {
		this.name = name;
		if (this.getSkeletonLink() != "")
			this.setSkeletonLink(name + ".skeleton");
	}

	public void setSkeletonLink(String skeletonLink) {
		mSkeletonLink = skeletonLink;
	}

	public String toXML() {
		StringBuffer buf = new StringBuffer();
		buf.append("<mesh>\n");
		if (!sharedGeometry.isEmpty()) {
			buf.append("    <sharedgeometry vertexcount=\""
					+ sharedGeometry.get(0).getVertexCount() + "\">\n");
			for (VertexBufferData buffer : sharedGeometry) {
				buf.append(buffer.toXML());
			}

			buf.append("    </sharedgeometry>");
		}
		buf.append("    <submeshes>\n");
		for (Submesh submesh : mSubmeshes) {
			buf.append(submesh.toXML() + "\n");
		}
		buf.append("    </submeshes>\n");
		if (mSkeletonLink != null && !mSkeletonLink.equals("")) {
			buf.append("    <skeletonlink name=\"").append(mSkeletonLink)
					.append("\" />\n");
		}
		if (!mBoneAssignments.isEmpty()) {
			buf.append("    <boneassignments>\n");
			for (VertexBoneAssignment boneAssignment : mBoneAssignments) {
				buf.append(boneAssignment.toXML());
			}
			buf.append("    </boneassignments>\n");
		}
		if (!mSubmeshNames.isEmpty()) {
			buf.append("    <submeshnames>\n");
			for (SubmeshName submeshName : mSubmeshNames) {
				buf.append(submeshName.toXML());
			}
			buf.append("    </submeshnames>\n");
		}
		if (lod.exists()) {
			buf.append(lod.toXML());
			}
		if (!mPoses.isEmpty()) {
			buf.append("    <poses>\n");
			for (Pose pose : mPoses) {
				buf.append(pose.toXML());
			}
			buf.append("    </poses>\n");
		}
		if (!mAnimations.isEmpty()) {
			buf.append("    <animations>\n");
			for (Animation animation : mAnimations) {
				buf.append(animation.toXML() + "\n");
			}
			buf.append("    </poses>\n");
		}
		buf.append("</mesh>");
		return buf.toString();
	}
}
