package meshhandle;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileWriter;
import java.io.FilenameFilter;
import java.io.IOException;
import java.text.NumberFormat;
import java.util.ArrayList;
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
import javax.xml.parsers.ParserConfigurationException;

import meshhandle.converter.ConverterUser;
import meshhandle.data.Vector3;
import meshhandle.model.Mesh;
import meshhandle.skeleton.Skeleton;
import meshhandle.xml.MeshLoader;
import meshhandle.xml.SkeletonLoader;

import org.xml.sax.SAXException;

public class MeshHandlerWindow extends JFrame implements ActionListener {

	/* Die eigentlichen Daten */
	private Mesh model;

	private Skeleton skeleton;

	private PropertyManager pm;

	/* Ein paar häufig gebrauchte Hilfsvariablen/-konstanten */

	private final String ogrepath;

	private String moduldir;

	private boolean noSkeleton;

	private boolean nameChanged;

	private String oldname;

	/* Layout-Hilfen */
	private JPanel centerPanel = new JPanel();

	private JPanel infoPanel = new JPanel();

	private JPanel materialsPanel = new JPanel();

	private JPanel factorPanel = new JPanel();

	private JPanel choosePanel = new JPanel();

	/* Fensterelemente */
	private JTextArea logField = new JTextArea();

	private JScrollPane logPane = new JScrollPane(logField);

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
		modelsLaden();
		modelsPane = new JScrollPane(modulModels);
		choosePanel.add(modelsPane);
		choose.addActionListener(this);
		choosePanel.add(choose);

		centerPanel.setLayout(new FlowLayout());

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

		save.addActionListener(this);
		logField.setEditable(false);
		this.getContentPane().add(logPane, BorderLayout.NORTH);
		this.getContentPane().add(save, BorderLayout.EAST);
		this.getContentPane().add(choosePanel, BorderLayout.WEST);
		this.getContentPane().add(centerPanel, BorderLayout.CENTER);
		this.setSize(800, 600);
		this.setVisible(true);
		loggen("Meshhandler gestartet.");
		loggen("OgreXMLConverter: " + ogrepath);
		loggen("Modul: " + moduldir);
	}

	private void modelsLaden() {
		modulModels.removeAll();
		File modelsdir = new File(moduldir, "models");
		modulModels = new JList(modelsdir.list(new FilenameFilter() {
			public boolean accept(File f, String s) {
				return s.endsWith(".mesh");
			}
		}));
		loggen("Modul: " + moduldir);
	}

	private void paintMeasures() {
		Vector3 measures = model.getMeasures();
		modelMeasures.setEditable(true);
		modelMeasures.setText("Maße: " + measures.getX() + "m lang,\n "
				+ measures.getY() + "m breit\n und " + measures.getZ()
				+ "m hoch.");
		modelMeasures.setEditable(false);
		infoPanel.invalidate();
		infoPanel.repaint();

	}

	public void actionPerformed(ActionEvent e) {
		if (e.getSource() == changeModule) {
			pm.changeModuldir();
			moduldir = pm.getModule();
			modelsLaden();
			modelsPane.invalidate();
			modelsPane.repaint();

		} else if (e.getSource() == choose) {

			if (modulModels.getSelectedValue() != null) {
				if (ConverterUser.fromMeshToXML(ogrepath, moduldir + "/models/"
						+ modulModels.getSelectedValue())) {
					loggen(modulModels.getSelectedValue() + " wird bearbeitet.");
					loggen(" .mesh.xml erzeugt.");
				} else {
					loggen("Fehler beim Konvertieren des Mesh!");
				}
				nameChanged = false;
				try {
					model = MeshLoader.readMesh(moduldir + "/models/"
							+ modulModels.getSelectedValue() + ".xml");
				} catch (ParserConfigurationException e1) {
					loggen("Fehler beim Einlesen der .mesh.xml. (Siehe Konsole)");
					e1.printStackTrace();
				} catch (SAXException e1) {
					loggen("Fehler beim Einlesen der .mesh.xml. (Siehe Konsole)");
					e1.printStackTrace();
				} catch (IOException e1) {
					loggen("Fehler beim Einlesen der .mesh.xml. (Siehe Konsole)");
					e1.printStackTrace();
				}
				loggen(".mesh.xml eingelesen");

				noSkeleton = model.getSkeletonLink().equals("");
				model.setName(((String) modulModels.getSelectedValue())
						.substring(0, ((String) modulModels.getSelectedValue())
								.length() - 4));
				if (!noSkeleton
						&& ConverterUser.fromMeshToXML(ogrepath, moduldir
								+ model.getSkeletonLink())) {
					loggen(" .skeleton.xml erzeugt.");
					try {
						skeleton = SkeletonLoader.readSkeleton(moduldir
								+ model.getSkeletonLink() + ".xml");
					} catch (SAXException e1) {
						loggen("Fehler beim Einlesen der .skeleton.xml. (Siehe Konsole)");
						e1.printStackTrace();
					} catch (ParserConfigurationException e1) {
						loggen("Fehler beim Einlesen der .skeleton.xml. (Siehe Konsole)");
						e1.printStackTrace();
					} catch (IOException e1) {
						loggen("Fehler beim Einlesen der .skeleton.xml. (Siehe Konsole)");
						e1.printStackTrace();
					}
					loggen(".skeleton.xml eingelesen.");
					modelName.setText(model.getName());
					modelTriNumber.setText(model.getPolycount() + " Dreiecke");
					if (noSkeleton)
						modelRigged.setText("Statisch");
					else
						modelRigged.setText("Animierbar");

					paintMeasures();

					modelMaterials.setEditable(true);
					ArrayList<String> materials = model.getMaterials();
					for (String material : materials) {
						modelMaterials.append(material);
					}
					modelMaterials.setEditable(false);
					materialsPanel.invalidate();
					materialsPanel.repaint();
					loggen(model.getName() + " geladen.");

					this.setSize(800, 600);
					this.invalidate();
					this.repaint();

				} else if (e.getSource() == rename) {
					oldname = model.getName();
					nameChanged = true;
					model.setName(modelName.getText());
					if (!noSkeleton)
						model.setSkeletonLink(modelName + ".skeleton");
				} else if (e.getSource() == scale) {
					model.scale(Float.parseFloat(factorField.getText()));
					if (!noSkeleton) {
						skeleton.scale(Float.parseFloat(factorField.getText()));
					}
					paintMeasures();
					factorField.setValue(new Float(1));
				}
			} else if (e.getSource() == save) {
				if (nameChanged) {
					(new File(moduldir + "/models", oldname + ".mesh"))
							.delete();
					(new File(moduldir + "/models", oldname + ".mesh.xml"))
							.delete();
					if (!noSkeleton) {
						(new File(moduldir + "/models", oldname + ".skeleton"))
								.delete();
						(new File(moduldir + "/models", oldname
								+ ".skeleton.xml")).delete();
					}
				}

				File newXml = new File(moduldir + "/models", model.getName()
						+ ".mesh.xml");
				File newSkelXml = new File(moduldir + "/models", model
						.getSkeletonLink()
						+ ".xml");
				try {
					FileWriter xmlout = new FileWriter(newXml);
					xmlout.write(model.toXML());
					xmlout.close();
				} catch (IOException e1) {
					loggen("Fehler beim Schreiben der .mesh.xml");
					e1.printStackTrace();
				}
				if (!noSkeleton) {
					try {
						FileWriter xmlout = new FileWriter(newSkelXml);
						xmlout.write(skeleton.toXML());
						xmlout.close();
					} catch (IOException e1) {
						System.out
								.println("Fehler beim Schreiben der .skeleton.xml");
						e1.printStackTrace();
					}
					if (ConverterUser.fromXMLToMesh(ogrepath, newSkelXml
							.getAbsolutePath())) {
						loggen(" skeleton geschrieben");
					}
				}
				if (ConverterUser.fromXMLToMesh(ogrepath, newXml
						.getAbsolutePath())) {
					loggen(" mesh geschrieben");
				}
			}
		}
	}

	public void loggen(String s) {
		logField.setEditable(true);
		logField.setText(logField.getText() + s + "\n");
		logField.setEditable(false);
	}

}