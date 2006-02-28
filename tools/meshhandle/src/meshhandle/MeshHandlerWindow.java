package meshhandle;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.text.NumberFormat;
import java.util.ArrayList;
import java.util.Locale;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFormattedTextField;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.filechooser.FileFilter;
import javax.xml.parsers.ParserConfigurationException;

import meshhandle.converter.ConverterUser;
import meshhandle.data.Vector3;
import meshhandle.model.Mesh;
import meshhandle.skeleton.Skeleton;
import meshhandle.xml.MeshLoader;
import meshhandle.xml.SkeletonLoader;

import org.xml.sax.SAXException;

public class MeshHandlerWindow extends JFrame {

	private Mesh model;

	private Skeleton skeleton;

	private String modeldir;

	private JFileChooser ogretools = new JFileChooser();

	private JPanel centerPanel;

	private JPanel infoPanel = new JPanel();

	private JPanel materialsPanel = new JPanel();

	private JPanel factorPanel = new JPanel();

	private JTextArea logField = new JTextArea();

	private boolean noSkeleton;

	public MeshHandlerWindow() {
		super("Der wunderbarer Meshhandler v0.7");
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setLayout(new BorderLayout());

		/*
		 * Die Auswahl des zu bearbeitenden Models.
		 */
		final MeshChooser chooser = new MeshChooser();
		JButton chose = new JButton("Bearbeiten");

		/*
		 * Angabe des Ogre-Tool-Pfads.
		 */

		ogretools.setFileFilter(new FileFilter() {
			public boolean accept(File f) {
				return (f.getName().startsWith("OgreXMLConverter") || f
						.isDirectory());
			}

			public String getDescription() {
				return "OgreXMLConverter";
			}
		});
		ogretools.setControlButtonsAreShown(false);
		ogretools.setDialogTitle("OgreXMLConverter");

		final JButton confirm = new JButton("Best‰tigen");
		
		JPanel chosePanel = new JPanel();

		/* Die interaktiven Dinge */
		chose.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				if (chooser.getSelectedFile() != null) {
					if (ogretools.getSelectedFile() == null) {

					} else {
						modeldir = chooser.getSelectedFile().getParent() + "/";
						if (ConverterUser.fromMeshToXML(ogretools
								.getSelectedFile().getAbsolutePath(), chooser
								.getSelectedFile().getAbsolutePath())) {
							loggen(chooser.getSelectedFile().getAbsolutePath()
									+ " wird bearbeitet.");
							loggen("         .mesh.xml erzeugt.");
						} else {
							System.out
									.println("Fehler beim Konvertieren des Mesh!");
						}

						try {
							model = MeshLoader.readMesh(chooser
									.getSelectedFile().getAbsolutePath()
									+ ".xml");
							noSkeleton = model.getSkeletonLink().equals("");
							if (!noSkeleton
									&& ConverterUser.fromMeshToXML(ogretools
											.getSelectedFile()
											.getAbsolutePath(), modeldir + model.getSkeletonLink())) {
								loggen("         .skeleton.xml erzeugt.");
								skeleton = SkeletonLoader.readSkeleton(modeldir
										+ model.getSkeletonLink() + ".xml");
							}
						} catch (ParserConfigurationException e) {
							e.printStackTrace();
						} catch (SAXException e) {
							e.printStackTrace();
						} catch (IOException e) {
							e.printStackTrace();
						}
						ogretools.setVisible(false);
						confirm.setVisible(true);
						logField.setVisible(true);
						complete();

					}
				}
			}

		});
		chosePanel.setLayout(new BoxLayout(chosePanel, BoxLayout.Y_AXIS));
		chosePanel.add(chooser);
		chosePanel.add(chose);

		/*
		 * Zusammenbau des Fensters.
		 */
		confirm.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				File newXml = new File(modeldir + model.getName() + ".mesh.xml");
				File newSkelXml = new File(modeldir + model.getSkeletonLink()
						+ ".xml");
				try {
					FileWriter xmlout = new FileWriter(newXml);
					xmlout.write(model.toXML());
					xmlout.close();
				} catch (IOException e1) {
					System.out.println("Fehler beim Schreiben der .mesh.xml");
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
					if (ConverterUser.fromXMLToMesh(ogretools.getSelectedFile()
							.getAbsolutePath(), newSkelXml.getAbsolutePath())) {
						loggen("     skeleton geschrieben");
					}
				}
				if (ConverterUser.fromXMLToMesh(ogretools.getSelectedFile()
						.getAbsolutePath(), newXml.getAbsolutePath())) {
					loggen("     mesh geschrieben");
				}
				complete();
			}
		});
		centerPanel = new JPanel();
		centerPanel.setLayout(new GridLayout(2, 2));
		centerPanel.add(infoPanel);
		centerPanel.add(materialsPanel);
		centerPanel.add(factorPanel);
		this.getContentPane().add(logField, BorderLayout.NORTH);
		this.getContentPane().add(confirm, BorderLayout.SOUTH);
		this.getContentPane().add(ogretools, BorderLayout.NORTH);
		this.getContentPane().add(chosePanel, BorderLayout.WEST);
		this.getContentPane().add(centerPanel, BorderLayout.CENTER);
		confirm.setVisible(false);
		this.pack();
		this.setVisible(true);
	}

	private void createInfoPanel() {
		infoPanel.setLayout(new BoxLayout(infoPanel, BoxLayout.Y_AXIS));
		infoPanel.add(new JLabel("Name: " + model.getName()));

		infoPanel.add(new JLabel("Polies: " + model.getPolycount()));
		if (noSkeleton) {
			infoPanel.add(new JLabel("Rigged: No"));
		} else {
			infoPanel.add(new JLabel("Rigged: Yes"));
		}
		Vector3 measures = model.getMeasures();
		infoPanel.add(new JLabel("Maﬂe: " + measures.getX() + "m lang,"));
		infoPanel.add(new JLabel("            " + measures.getY() + "m breit"));
		infoPanel.add(new JLabel("     und " + measures.getZ() + "m hoch."));
	}

	private void createMaterialsPanel() {
		ArrayList<String> materials = model.getMaterials();
		materialsPanel.setLayout(new GridLayout(materials.size(), 3));
		JTextField[] newMaterialNames = new JTextField[materials.size()];
		int i = 0;
		for (String material : materials) {
			materialsPanel.add(new JLabel("Material " + (i + 1)));
			materialsPanel.add(new JLabel(material));
			materialsPanel.add(new JLabel("Neuer Name: "));
			newMaterialNames[i] = new JTextField(material);
			materialsPanel.add(newMaterialNames[i]);
			materialsPanel.add(new JPanel());
			i++;
		}
		// materialsPanel.add(new JButton("Umbenennen"));
	}

	private void createFactorPanel() {
		factorPanel.setLayout(new BoxLayout(factorPanel, BoxLayout.X_AXIS));
		NumberFormat format = NumberFormat.getNumberInstance(Locale.ENGLISH);
		format.setMinimumFractionDigits(3);
		final JFormattedTextField factorfield = new JFormattedTextField(format);
		factorfield.setValue(new Float(1));
		factorfield.setColumns(10);
		factorPanel.add(new JLabel("Skalieren um Faktor: "));
		factorPanel.add(factorfield);

		JButton scale = new JButton("Skalieren");
		scale.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent ae) {
				model.scale(Float.parseFloat(factorfield.getText()));
				if (!noSkeleton) {
					skeleton.scale(Float.parseFloat(factorfield.getText()));
				}
				createInfoPanel();
				factorfield.setValue(new Float(1));
			}
		});
		factorPanel.add(scale);

	}

	private void removeOgreChooser() {
		this.remove(ogretools);

	}

	private void complete() {
		this.setVisible(false);
		createInfoPanel();
		createMaterialsPanel();
		createFactorPanel();
		centerPanel.repaint();
		this.pack();
		this.setVisible(true);
	}

	public void loggen(String s) {
		logField.setEditable(true);
		logField.setText(logField.getText() + s + "\n");
		logField.setEditable(false);
	}
}