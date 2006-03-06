package meshhandle;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;

import javax.swing.JDialog;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.filechooser.FileFilter;

public class PropertyManager {

	final File propFile = new File(System.getProperty("user.dir"),
			"properties");

	private String ogredir;

	private String moduldir;

	public PropertyManager() {
		Properties pfade = new Properties();
		if (propFile.exists()) {

			try {
				InputStream stream = new FileInputStream(propFile);
				pfade.load(stream);
			} catch (FileNotFoundException e) {
				JOptionPane.showMessageDialog(null,
						"Sie haben kein Property-File.",
						"Fehler beim Laden der Properties",
						JOptionPane.ERROR_MESSAGE);
				e.printStackTrace();
			} catch (IOException e) {
				JOptionPane.showMessageDialog(null,
						"Keine Ahnung, was das schon wieder soll.",
						"Fehler beim Laden der Properties",
						JOptionPane.ERROR_MESSAGE);
				e.printStackTrace();
			}
		    ogredir = pfade.getProperty("CONVERTER");
			moduldir = pfade.getProperty("MODULE");
		} else {
			changeOgredir();
			changeModuldir();
		}

		// File rl = new File(pfade.getProperty("RL-PFAD"));
		// if (!rl.isDirectory()) {
		// JOptionPane
		// .showMessageDialog(
		// null,
		// "Geben Sie in den Properties ihr korrektes RL-Verzeichnis an.",
		// "Fehler beim Laden der Properties",
		// JOptionPane.ERROR_MESSAGE);
		// }

	}

	public void changeModuldir() {
		JFileChooser modulChooser = new JFileChooser();
		modulChooser.setDialogType(JFileChooser.CUSTOM_DIALOG);

		modulChooser.setAcceptAllFileFilterUsed(false);
		modulChooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
		modulChooser.setFileFilter(new FileFilter() {

			public String getDescription() {
				return "Modul-Verzeichnis";
			}

			public boolean accept(File f) {
				return ((new File(f, "models")).isDirectory());
			}
		});
		int returnVal = modulChooser.showOpenDialog(new JDialog());
		if (returnVal == JFileChooser.APPROVE_OPTION) {
			setModule(modulChooser.getSelectedFile().getAbsolutePath());
		}
				
	}

	private void changeOgredir() {

		JFileChooser ogretools = new JFileChooser();
		ogretools.setFileFilter(new FileFilter() {
			public boolean accept(File f) {
				return (f.getName().startsWith("OgreXMLConverter") || f
						.isDirectory());
			}

			public String getDescription() {
				return "OgreXMLConverter";
			}
		});
		ogretools.setControlButtonsAreShown(true);
		ogretools.setDialogTitle("OgreXMLConverter");
		int returnVal = ogretools.showOpenDialog(new JDialog());
		if (returnVal == JFileChooser.APPROVE_OPTION) {
			ogredir = ogretools.getSelectedFile().getAbsolutePath();
		}

	}

	private void write() {
		try {
			FileWriter propWriter = new FileWriter(propFile);
			propWriter.write("CONVERTER = " + ogredir);
			propWriter.write("MODULE = " + moduldir);
		} catch (IOException e) {
			JOptionPane.showMessageDialog(null,
					"Keine Ahnung, was das schon wieder soll.",
					"Fehler beim Neuschreiben der Properties",
					JOptionPane.ERROR_MESSAGE);
			e.printStackTrace();
		}

	}

	public void setModule(String changeddir) {
		moduldir = changeddir;
		write();
	}

	public String getModule() {
		Properties pfade = new Properties();
		
			try {
				FileInputStream propStream = new FileInputStream(propFile);
				pfade.load(propStream);
			} catch (FileNotFoundException e) {
				JOptionPane.showMessageDialog(null,
						"Sie haben kein Property-File.",
						"Fehler beim Laden der Properties",
						JOptionPane.ERROR_MESSAGE);
				e.printStackTrace();
			} catch (IOException e) {
				JOptionPane.showMessageDialog(null,
						"Keine Ahnung, was das schon wieder soll.",
						"Fehler beim Laden der Properties",
						JOptionPane.ERROR_MESSAGE);
				e.printStackTrace();
			}
			return pfade.getProperty("MODUL");
		}
		
	

	public String getOgreTools() {
		return ogredir;
	}

}
