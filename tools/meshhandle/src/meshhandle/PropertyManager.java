package meshhandle;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

import javax.swing.JDialog;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.filechooser.FileFilter;

public class PropertyManager {

	final File propFile = new File(System.getProperty("user.dir"), "properties");

	private String ogredir;

	private String moduldir;

	public PropertyManager() {
		if (propFile.exists()) {

			try {
				FileReader reader = new FileReader(propFile);
				BufferedReader bReader = new BufferedReader(reader);
				for (String s; (s = bReader.readLine()) != null;) {
					if (s.indexOf("CONVERTER") != -1) {
						ogredir = s.substring(("CONVERTER=").length()).trim();
					} else if (s.indexOf("MODULE") != -1) {
						moduldir = s.substring(("MODULE=").length()).trim();
					}
				}
				bReader.close();
				reader.close();
				if (ogredir == null) {
					changeOgredir();
				}
				if (moduldir == null) {
					changeModuldir();
				}

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
		} else {
			changeOgredir();
			changeModuldir();
		}

	}

	public void changeModuldir() {
		JFileChooser modulChooser = new JFileChooser(moduldir);
		modulChooser.setDialogType(JFileChooser.CUSTOM_DIALOG);

		modulChooser.setAcceptAllFileFilterUsed(false);
		modulChooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
		
//		if (modulChooser.is)
//		modulChooser.addActionListener(new ActionListener(){
//			public void actionPerformed(ActionEvent e){
//				if (new File(modulChooser.getSelectedFile(), "models").isDirectory()) {
//					modulChooser.setControlButtonsAreShown(true);
//				}	
//			}
//		});
		
		int returnVal = modulChooser.showOpenDialog(new JDialog());
		if (returnVal == JFileChooser.APPROVE_OPTION) {
			setModule(modulChooser.getSelectedFile().getAbsolutePath());
		}

	}

	public void changeOgredir() {

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
			propWriter.write("CONVERTER=" + ogredir+"\n");
			propWriter.write("MODULE=" + moduldir+"\n");
			propWriter.close();
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
		return moduldir;
	}

	public String getOgreTools() {
		return ogredir;
	}

}
