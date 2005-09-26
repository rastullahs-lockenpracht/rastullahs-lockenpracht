import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.text.NumberFormat;
import java.util.ArrayList;
import java.util.Locale;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFormattedTextField;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.JTextField;

public class MaterialsAendern extends JPanel {

	private PropertyManager pm = new PropertyManager();

	String[] matname;

	ArrayList materials;

	private void convert(String[] args) {

		/**
		 * Mit Hilfe des Converters wird die .mesh in eine .xml umgewandelt, die
		 * im OgreTools-Ordner abgelegt wird.
		 */

		// any error???
		int exitVal;
		try {
			Process proc = Runtime.getRuntime().exec(args);

			// any error message?
			StreamGobbler errorGobbler = new StreamGobbler(proc
					.getErrorStream(), "Add. inf.:");

			// any output?
			StreamGobbler outputGobbler = new StreamGobbler(proc
					.getInputStream(), "Output");

			// kick them off
			errorGobbler.start();
			outputGobbler.start();

			exitVal = proc.waitFor();
			System.out.println("ExitValue: " + exitVal);
			// proc.destroy();
		} catch (IOException e) {
			JOptionPane.showMessageDialog(null, "",
					"Fehler beim Ausführen des Converters",
					JOptionPane.ERROR_MESSAGE);
			e.printStackTrace();
		} catch (InterruptedException e) {
			JOptionPane.showMessageDialog(null, "",
					"Fehler beim Ausführen des Converters",
					JOptionPane.ERROR_MESSAGE);
			e.printStackTrace();
		}

	}

	/**
	 * Sucht eine übergebene Zeile, vorzugsweise die eines Quelltextes, nach den
	 * ersten beiden Gänsefüßchen ab und gibt den Inhalt dazwischen zurück.
	 * 
	 * @param zeile
	 * @return Die Position der ersten beiden Leerzeichen.
	 */
	public int[] gaenseFuesse(String zeile)

	{

		int zahl = 0;
		int[] ergebnis = new int[2];

		for (int i = 0; zahl < 2; i++) {
			char sucher = zeile.charAt(i);
			if (sucher == '"') {
				ergebnis[zahl] = i;
				zahl++;

			}
		}
		return ergebnis;

	}

	protected MaterialsAendern(final File mesh) {
		final File[] files = pm.propertiesLaden();

		this.setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));

		JPanel namenPane = new JPanel();
		namenPane.setLayout(new BoxLayout(namenPane, BoxLayout.X_AXIS));
		namenPane.add(new JLabel("Alter Name:"));
		final JTextField oldname = new JTextField(mesh.getName().substring(0,
				mesh.getName().length() - 5));
		oldname.setEditable(false);
		namenPane.add(oldname);
		namenPane.add(new JLabel("Neuer Name;"));
		final JTextField newname = new JTextField();
		namenPane.add(newname);
		this.add(namenPane);

		JPanel materialsPane = new JPanel();
		materialsPane.setLayout(new BoxLayout(materialsPane, BoxLayout.Y_AXIS));
		materialsPane.add(new JLabel("Das Model nutzt folgende Materials:"));

		String[] args = { files[1].getAbsolutePath() + "/OgreXMLConverter",
				mesh.getAbsolutePath(),
				files[1].getAbsolutePath() + "/" + mesh.getName() + ".xml" };
		convert(args);
		final File model = new File(files[1].getAbsolutePath(), mesh.getName()
				+ ".xml");
		materials = new ArrayList();
		double xmax = 0;
		double xmin = 0;
		double ymax = 0;
		double ymin = 0;
		double zmax = 0;
		double zmin = 0;

		try {
			FileReader xmlreader = new FileReader(model);
			BufferedReader reader = new BufferedReader(xmlreader);
			for (String s; (s = reader.readLine()) != null;) {
				if (s.indexOf("<submesh material=") != -1) {
					int a = "        <submesh material=\"".length();
					int b = s.substring(a).indexOf("\"");
					JPanel aPane = new JPanel();
					aPane.setLayout(new BoxLayout(aPane, BoxLayout.X_AXIS));
					aPane.add(new JLabel(s.substring(a, a + b)));
					JTextField newmat = new JTextField(s.substring(a, a + b));
					materials.add(newmat);
					aPane.add(newmat);
					materialsPane.add(aPane);
				} else if (s.indexOf("<position x=\"") != -1) {
					int[] positionx = gaenseFuesse(s);
					float x = Float.parseFloat(s.substring(positionx[0] + 1,
							positionx[1]));
					if (x > xmax) {
						xmax = x;
					}
					if (x < xmin) {
						xmin = x;
					}
					int[] positiony = gaenseFuesse(s
							.substring(positionx[1] + 1));
					float y = Float.parseFloat(s
							.substring(positionx[1] + positiony[0] + 2,
									positionx[1] + positiony[1] + 1));
					if (y > ymax) {
						ymax = y;
					}
					if (y < ymin) {
						ymin = y;
					}
					int[] positionz = gaenseFuesse(s.substring(positionx[1]
							+ positiony[1] + 2));
					float z = Float.parseFloat(s.substring(positionx[1]
							+ positiony[1] + positionz[0] + 3, positionx[1]
							+ positiony[1] + positionz[1] + 2));
					if (z > zmax) {
						zmax = z;
					}
					if (z < zmin) {
						zmin = z;
					}
				}
			}
		} catch (FileNotFoundException e) {
			JOptionPane.showMessageDialog(null,
					"Skandal! Frisch konvertiert und schon verschwunden!",
					"Datei nicht gefunden", JOptionPane.ERROR_MESSAGE);
			e.printStackTrace();
		} catch (IOException e) {
			JOptionPane.showMessageDialog(null,
					"Fehler beim Lesen der xml-Datei", "Lesefehler",
					JOptionPane.ERROR_MESSAGE);
			e.printStackTrace();
		}
		JTextArea size = new JTextArea("Maße: \n" + (xmax - xmin) + " auf\n"
				+ (ymax - ymin) + " auf\n" + (zmax - zmin) + " Meter.");
		size.setEditable(false);
		this.add(size);
		NumberFormat format = NumberFormat.getNumberInstance(Locale.ENGLISH);
		format.setMinimumFractionDigits(3);
		final JFormattedTextField factorfield = new JFormattedTextField(format);
		factorfield.setValue(new Float(1));
		factorfield.setColumns(10);
		this.add(factorfield);

		materials.trimToSize();

		this.add(materialsPane);

		JButton ok = new JButton("OK");
		ok.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (xmlneuschreiben(factorfield, model, newname)) {
					mesh.delete();
					String[] args = {
							files[1].getAbsolutePath() + "/OgreXMLConverter",
							files[1].getAbsolutePath() + "/" + mesh.getName()
									+ ".xml",
							mesh.getParent() + "/" + newname.getText()
									+ ".mesh" };
					convert(args);
					model.deleteOnExit();
					File skeleton = new File(mesh.getParent(), oldname
							.getText()
							+ ".skeleton");
					skeleton.renameTo(new File(skeleton.getParent(), newname
							.getText()
							+ ".skeleton"));
					
					
					
				} else {
					System.out.println("Fehler!");
				}
			}
		});

		this.add(ok);

	}

	public boolean xmlneuschreiben(JTextField factorfield, File model,
			JTextField newname) {
		matname = new String[materials.size()];
		for (int i = 0; i < materials.size(); i++) {
			matname[i] = ((JTextField) materials.get(i)).getText();
		}

		float factor = Float.parseFloat(factorfield.getText());
		int i = 0;
		StringBuffer dateiinhalt = new StringBuffer();
		try {
			FileReader xmlreader = new FileReader(model);
			BufferedReader reader = new BufferedReader(xmlreader);

			for (String s; (s = reader.readLine()) != null;) {
				if (s.indexOf("<submesh material=") != -1 && i < matname.length) {

					dateiinhalt.append(s.substring(0,
							"        <submesh material=\"".length())
							+ matname[i]
							+ s.substring("        <submesh material=\""
									.length()
									+ s.substring(
											"        <submesh material=\""
													.length()).indexOf("\""))
							// Hier funktioniert schonmal irgendetwas nicht!!!
							+ "\n");
					i++;
				} else if (s.indexOf("<position x=\"") != -1) {
					int[] positionx = gaenseFuesse(s);
					float x = Float.parseFloat(s.substring(positionx[0] + 1,
							positionx[1]));

					int[] positiony = gaenseFuesse(s
							.substring(positionx[1] + 1));
					float y = Float.parseFloat(s
							.substring(positionx[1] + positiony[0] + 2,
									positionx[1] + positiony[1] + 1));

					int[] positionz = gaenseFuesse(s.substring(positionx[1]
							+ positiony[1] + 2));
					float z = Float.parseFloat(s.substring(positionx[1]
							+ positiony[1] + positionz[0] + 3, positionx[1]
							+ positiony[1] + positionz[1] + 2));
					dateiinhalt.append(s.substring(0, positionx[0])
							+ "\""
							+ (x * factor)
							+ "\""
							+ s.substring(positionx[1] + 1, positionx[1]
									+ positiony[0] + 1)
							+ "\""
							+ (y * factor)
							+ "\""
							+ s.substring(positionx[1] + positiony[1] + 2,
									positionx[1] + positiony[1] + positionz[0]
											+ 2)
							+ "\""
							+ (z * factor)
							+ "\""
							+ s.substring(positionx[1] + positiony[1]
									+ positionz[1] + 3) + "\n");

				} else if (s.indexOf("<skeletonlink name=\"") != -1) {
					int[] pos = gaenseFuesse(s);
					dateiinhalt.append(s.substring(0, pos[0] + 1)
							+ newname.getText() + ".skeleton"
							+ s.substring(pos[1]) + "\n");
				} else {
					dateiinhalt.append(s + "\n");
				}
			}

			byte[] neuedatei = String.valueOf(dateiinhalt).getBytes();
			FileOutputStream out = new FileOutputStream(model);
			out.write(neuedatei);
			out.close();
			return true;
		} catch (FileNotFoundException e1) {
			e1.printStackTrace();
			return false;

		} catch (IOException e1) {
			e1.printStackTrace();
			return false;
		}
	}
}
