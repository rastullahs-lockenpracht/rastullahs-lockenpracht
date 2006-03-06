package meshhandle;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.File;
import java.io.FilenameFilter;
import java.text.NumberFormat;
import java.util.Locale;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFormattedTextField;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;

import meshhandle.model.Mesh;
import meshhandle.skeleton.Skeleton;

public class MeshHandlerWindow extends JFrame implements ActionListener {

	/* Die eigentlichen Daten */
	private Mesh model;

	private Skeleton skeleton;

	private PropertyManager pm;

	/* Ein paar häufig gebrauchte Hilfsvariablen/-konstanten */

	private final String ogrepath;

	private String moduldir;

	private boolean noSkeleton;

	/* Layout-Hilfen */
	private JPanel centerPanel = new JPanel();

	private JPanel infoPanel = new JPanel();

	private JPanel materialsPanel = new JPanel();

	private JPanel factorPanel = new JPanel();

	private JPanel choosePanel = new JPanel();

	/* Fensterelemente */
	private JTextArea logField = new JTextArea();

	private JButton changeModule = new JButton("Modul ändern");

	private JList modulModels = new JList();

	private JScrollPane modelsPane = new JScrollPane(modulModels);

	private JButton choose = new JButton("Bearbeiten");

	private JTextField modelName = new JTextField();

	private JButton rename = new JButton("Umbenennen");

	private JLabel modelTriNumber = new JLabel();

	private JLabel modelRigged = new JLabel();

	private JTextArea modelMeasures = new JTextArea();

	private JFormattedTextField factorField = new JFormattedTextField();

	private JButton scale = new JButton("Skalieren");

	private JTextArea modelMaterials = new JTextArea();

	private JButton save = new JButton("Änderungen speichern");

public MeshHandlerWindow() {
		super("Der wunderbarer Meshhandler v0.7");
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		pm = new PropertyManager();
		ogrepath = pm.getOgreTools();
		moduldir = pm.getModule();

		this.setLayout(new BorderLayout());

		choosePanel.setLayout(new BoxLayout(choosePanel, BoxLayout.Y_AXIS));
		choosePanel.add(changeModule);
		changeModule.addActionListener(this);
		choosePanel.add(modelsPane);
		modelsLaden();
		choose.addActionListener(this);
		choosePanel.add(choose);

		centerPanel.setLayout(new GridLayout(2, 2));

		infoPanel.add(modelName);
		rename.addActionListener(this);
		infoPanel.add(rename);
		infoPanel.add(modelTriNumber);
		infoPanel.add(modelRigged);
		infoPanel.add(modelMeasures);
		centerPanel.add(infoPanel);

		materialsPanel.add(modelMaterials);
		centerPanel.add(materialsPanel);

		factorPanel.setLayout(new BoxLayout(factorPanel, BoxLayout.X_AXIS));
		NumberFormat format = NumberFormat.getNumberInstance(Locale.ENGLISH);
		format.setMinimumFractionDigits(2);
		factorField.setValue(new Float(1));
		factorField.setColumns(10);
		factorPanel.add(new JLabel("Skalieren um Faktor: "));
		factorPanel.add(factorField);
		scale.addActionListener(this);
		factorPanel.add(scale);

		centerPanel.add(factorPanel);
		this.getContentPane().add(logField, BorderLayout.NORTH);
		this.getContentPane().add(save, BorderLayout.EAST);
		this.getContentPane().add(choosePanel, BorderLayout.WEST);
		this.getContentPane().add(centerPanel, BorderLayout.CENTER);
		this.setSize(800, 600);
		this.setVisible(true);
	}	private void modelsLaden() {
		modulModels.removeAll();
		File modelsdir = new File(moduldir, "models");
		modulModels = new JList(modelsdir.list(new FilenameFilter() {
			public boolean accept(File f, String s) {
				return f.getName().endsWith("mesh");
			}
		}));

	}

	// private void createInfoPanel() {
	// infoPanel.setLayout(new BoxLayout(infoPanel, BoxLayout.Y_AXIS));
	// infoPanel.add(new JLabel("Name: " + model.getName()));
	//
	// infoPanel.add(new JLabel("Polies: " + model.getPolycount()));
	// if (noSkeleton) {
	// infoPanel.add(new JLabel("Rigged: No"));
	// } else {
	// infoPanel.add(new JLabel("Rigged: Yes"));
	// }
	// Vector3 measures = model.getMeasures();
	// infoPanel.add(new JLabel("Maße: " + measures.getX() + "m lang,"));
	// infoPanel.add(new JLabel(" " + measures.getY() + "m breit"));
	// infoPanel.add(new JLabel(" und " + measures.getZ() + "m hoch."));
	// }

	// private void createMaterialsPanel() {
	// ArrayList<String> materials = model.getMaterials();
	// materialsPanel.setLayout(new GridLayout(materials.size(), 3));
	// JTextField[] newMaterialNames = new JTextField[materials.size()];
	// int i = 0;
	// for (String material : materials) {
	// materialsPanel.add(new JLabel("Material " + (i + 1)));
	// materialsPanel.add(new JLabel(material));
	// materialsPanel.add(new JLabel("Neuer Name: "));
	// newMaterialNames[i] = new JTextField(material);
	// materialsPanel.add(newMaterialNames[i]);
	// materialsPanel.add(new JPanel());
	// i++;
	// }
	// // materialsPanel.add(new JButton("Umbenennen"));
	// }
	//
	//		
	//
	// JButton scale = new JButton("Skalieren");
	// scale.addActionListener(new ActionListener() {
	// public void actionPerformed(ActionEvent ae) {
	// model.scale(Float.parseFloat(factorfield.getText()));
	// if (!noSkeleton) {
	// skeleton.scale(Float.parseFloat(factorfield.getText()));
	// }
	// createInfoPanel();
	// factorfield.setValue(new Float(1));
	// }
	// });
	// }
	//
	// private void complete() {
	// this.setVisible(false);
	// createInfoPanel();
	// createMaterialsPanel();
	// createFactorPanel();
	// centerPanel.repaint();
	// this.pack();
	// this.setVisible(true);
	// }

	public void loggen(String s) {
		logField.setEditable(true);
		logField.setText(logField.getText() + s + "\n");
		logField.setEditable(false);
	}

	public void actionPerformed(ActionEvent e) {
		// TODO Auto-generated method stub
		if (e.getSource() == choose) {
			//
			// chosePanel.setLayout(new BoxLayout(chosePanel,
			// BoxLayout.Y_AXIS));
			// chosePanel.add(chooser);
			// chosePanel.add(chose);
			//
			// /*
			// * Zusammenbau des Fensters.
			// */
			// confirm.addActionListener(new ActionListener() {
			// public void actionPerformed(ActionEvent e) {
			// File newXml = new File(modeldir + model.getName() + ".mesh.xml");
			// File newSkelXml = new File(modeldir + model.getSkeletonLink()
			// + ".xml");
			// try {
			// FileWriter xmlout = new FileWriter(newXml);
			// xmlout.write(model.toXML());
			// xmlout.close();
			// } catch (IOException e1) {
			// System.out.println("Fehler beim Schreiben der .mesh.xml");
			// e1.printStackTrace();
			// }
			// if (!noSkeleton) {
			// try {
			// FileWriter xmlout = new FileWriter(newSkelXml);
			// xmlout.write(skeleton.toXML());
			// xmlout.close();
			// } catch (IOException e1) {
			// System.out
			// .println("Fehler beim Schreiben der .skeleton.xml");
			// e1.printStackTrace();
			// }
			// if (ConverterUser.fromXMLToMesh(
			// // ogretools.getSelectedFile().getAbsolutePath()
			// PropertyManager.ogre.getAbsolutePath()
			// , newSkelXml.getAbsolutePath())) {
			// loggen(" skeleton geschrieben");
			// }
			// }
			// if (ConverterUser.fromXMLToMesh(
			// // ogretools.getSelectedFile().getAbsolutePath(),
			// PropertyManager.ogre.getAbsolutePath(),
			// newXml.getAbsolutePath()
			// )) {
			// loggen(" mesh geschrieben");
			// }
			// complete();
			// }
			// });

			// if (chooser.getSelectedFile() != null) {
			// // if (ogretools.getSelectedFile() == null) {
			// //
			// // } else {
			// modeldir = chooser.getSelectedFile().getParent() + "/";
			// // if
			// (ConverterUser.fromMeshToXML(ogretools.getSelectedFile().getAbsolutePath(),
			// if
			// (ConverterUser.fromMeshToXML(PropertyManager.ogre.getAbsolutePath(),
			// chooser
			// .getSelectedFile().getAbsolutePath())) {
			// loggen(chooser.getSelectedFile().getAbsolutePath()
			// + " wird bearbeitet.");
			// loggen(" .mesh.xml erzeugt.");
			// } else {
			// System.out
			// .println("Fehler beim Konvertieren des Mesh!");
			// }
			//
			// try {
			// model = MeshLoader.readMesh(chooser
			// .getSelectedFile().getAbsolutePath()
			// + ".xml");
			// noSkeleton = model.getSkeletonLink().equals("");
			// if (!noSkeleton
			// // && ConverterUser.fromMeshToXML(ogretools.getAbsolutePath(),
			// &&
			// ConverterUser.fromMeshToXML(PropertyManager.ogre.getAbsolutePath(),
			// modeldir + model.getSkeletonLink())) {
			// loggen(" .skeleton.xml erzeugt.");
			// skeleton = SkeletonLoader.readSkeleton(modeldir
			// + model.getSkeletonLink() + ".xml");
			// }
			// } catch (ParserConfigurationException e) {
			// e.printStackTrace();
			// } catch (SAXException e) {
			// e.printStackTrace();
			// } catch (IOException e) {
			// e.printStackTrace();
			// }
			// // ogretools.setVisible(false);
			// confirm.setVisible(true);
			// logField.setVisible(true);
			// complete();
			//
			// }
			// }
		} else if (e.getSource() == rename) {
		} else if (e.getSource() == scale) {

		}
	}
}