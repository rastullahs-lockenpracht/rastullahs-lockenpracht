package meshhandle;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

import javax.xml.parsers.ParserConfigurationException;

import meshhandle.io.xml.MeshLoader;
import meshhandle.io.xml.SkeletonLoader;
import meshhandle.model.mesh.Mesh;
import meshhandle.model.skeleton.Skeleton;

import org.xml.sax.SAXException;

public class ImExportTest {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		try {
			Mesh testmesh = MeshLoader
					.readMesh("D:/Eigene Dateien/Java/meshhandle2/Mr_Cylinder.mesh.xml");
			FileWriter writer = new FileWriter(
					"D:/Eigene Dateien/Java/meshhandle2/Mr_Cylinder2.mesh.xml");
			writer.append(testmesh.toXML());
			writer.close();
		} catch (ParserConfigurationException e) {
			// 
			e.printStackTrace();
		} catch (SAXException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		int i = 0;
		try {

			// TODO An das eigene System anpassen.
			FileReader fr1 = new FileReader(new File(
					"D:/Eigene Dateien/Java/meshhandle2/Mr_Cylinder.mesh.xml"));
			FileReader fr2 = new FileReader(new File(
					"D:/Eigene Dateien/Java/meshhandle2/Mr_Cylinder2.mesh.xml"));
			BufferedReader reader1 = new BufferedReader(fr1);
			BufferedReader reader2 = new BufferedReader(fr2);
			String line1;
			String line2;
			StringBuffer result = new StringBuffer();
			while ((line1 = reader1.readLine()) != null
					&& (line2 = reader2.readLine()) != null) {
				i++;
				if (!line2.trim().equals(line1.trim())) {
					result.append(i + ":|" + line1 + "|\n");
					result.append(i + ":|" + line2 + "|\n\n");

				}
			}
			// TODO An das eigene System anpassen.
			FileWriter writer = new FileWriter(
					"D:/Eigene Dateien/Java/meshhandle2/resultmesh.rtf");
			writer.append(result);
			writer.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		try {
			Skeleton testskel = SkeletonLoader
					.readSkeleton("D:/Eigene Dateien/Java/meshhandle2/Mr_Cylinder_mesh.skeleton.xml");
			FileWriter writer = new FileWriter(
					"D:/Eigene Dateien/Java/meshhandle2/Mr_Cylinder2_mesh.skeleton.xml");
			writer.append(testskel.toXML());
			writer.close();
		} catch (ParserConfigurationException e) {
			// 
			e.printStackTrace();
		} catch (SAXException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		i = 0;
		try {

			// TODO An das eigene System anpassen.
			FileReader fr1 = new FileReader(new File(
					"D:/Eigene Dateien/Java/meshhandle2/Mr_Cylinder_mesh.skeleton.xml"));
			FileReader fr2 = new FileReader(new File(
					"D:/Eigene Dateien/Java/meshhandle2/Mr_Cylinder2_mesh.skeleton.xml"));
			BufferedReader reader1 = new BufferedReader(fr1);
			BufferedReader reader2 = new BufferedReader(fr2);
			String line1;
			String line2;
			StringBuffer result = new StringBuffer();
			while ((line1 = reader1.readLine()) != null
					&& (line2 = reader2.readLine()) != null) {
				i++;
				if (!line2.trim().equals(line1.trim())) {
					result.append(i + ":|" + line1 + "|\n");
					result.append(i + ":|" + line2 + "|\n\n");

				}
			}
			// TODO An das eigene System anpassen.
			FileWriter writer = new FileWriter(
					"D:/Eigene Dateien/Java/meshhandle2/resultskel.rtf");
			writer.append(result);
			writer.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}
