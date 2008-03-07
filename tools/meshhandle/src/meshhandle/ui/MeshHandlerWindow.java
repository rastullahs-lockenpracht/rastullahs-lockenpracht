package meshhandle.ui;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.io.File;
import java.io.FileWriter;
import java.io.FilenameFilter;
import java.io.IOException;
import java.text.NumberFormat;
import java.util.ArrayList;
import java.util.Locale;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.DefaultListModel;
import javax.swing.JButton;
import javax.swing.JFormattedTextField;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.ListSelectionModel;
import javax.swing.SpringLayout;
import javax.swing.SwingConstants;
import javax.xml.parsers.ParserConfigurationException;

import meshhandle.PropertyManager;
import meshhandle.converter.ConverterUser;
import meshhandle.data.Vector3;
import meshhandle.io.xml.MeshLoader;
import meshhandle.io.xml.SkeletonLoader;
import meshhandle.model.mesh.Mesh;
import meshhandle.model.skeleton.Skeleton;

import org.xml.sax.SAXException;

public class MeshHandlerWindow extends JFrame implements ActionListener {

	private static final long serialVersionUID = 1L;

	// Die eigentlichen Daten
	private Mesh model;

	private Skeleton skeleton;

	private PropertyManager propertyManager;

	private File oldModel;

	// Fensterelemente
	private JTextArea logField;

	private JScrollPane logPane;

	private JList modelList;

	private JTextField converterPathField;

	private JTextField moduleDirField;

	private JTextField modelNameEdit;

	private JTextField modelTriangleCountField;

	private JPanel skeletonPanel;

	private JTextField modelHasSkeletonField;

	private JTextArea modelMeasures;

	private JFormattedTextField factorField;

	private JTextArea modelMaterials;

	public MeshHandlerWindow() {
		super("Der wunderbare Meshhandler v0.9");

		JPanel configPanel = new JPanel();
		configPanel.setBorder(BorderFactory.createCompoundBorder(BorderFactory
				.createTitledBorder("Konfiguration"), BorderFactory
				.createEmptyBorder(5, 5, 5, 5)));
		configPanel.setPreferredSize(new Dimension(800, 100));
		configPanel.setMinimumSize(new Dimension(600, 100));
		SpringLayout layout = new SpringLayout();
		configPanel.setLayout(layout);

		JLabel converterLbl = new JLabel("OgreXmlConverter: ", SwingConstants.TRAILING);
		converterPathField = new JTextField(Integer.MAX_VALUE);
		converterPathField.setEditable(false);
		converterLbl.setLabelFor(converterPathField);
		JButton changeConverterBtn = new JButton("Ändern..");
		configPanel.add(changeConverterBtn);
		configPanel.add(converterLbl);
		configPanel.add(converterPathField);

		JLabel moduleLbl = new JLabel("Modul-Unterverzeichnis: ",
				SwingConstants.TRAILING);
		moduleDirField = new JTextField(Integer.MAX_VALUE);
		moduleDirField.setEditable(false);
		moduleLbl.setLabelFor(moduleDirField);
		JButton changeModuleBtn = new JButton("Ändern..");
		configPanel.add(changeModuleBtn);
		configPanel.add(moduleLbl);
		configPanel.add(moduleDirField);
		layout.putConstraint(SpringLayout.WEST, moduleLbl, 5,
				SpringLayout.WEST, configPanel);
		layout.putConstraint(SpringLayout.NORTH, moduleLbl, 5,
				SpringLayout.NORTH, configPanel);
		layout.putConstraint(SpringLayout.EAST, changeModuleBtn, -5,
				SpringLayout.EAST, configPanel);
		layout.putConstraint(SpringLayout.NORTH, changeModuleBtn, 5,
				SpringLayout.NORTH, configPanel);
		layout.putConstraint(SpringLayout.NORTH, moduleDirField, 5,
				SpringLayout.NORTH, configPanel);
		layout.putConstraint(SpringLayout.EAST, moduleDirField, -5,
				SpringLayout.WEST, changeModuleBtn);
		layout.putConstraint(SpringLayout.WEST, moduleDirField, 5,
				SpringLayout.EAST, moduleLbl);

		layout.putConstraint(SpringLayout.WEST, converterLbl, 5,
				SpringLayout.WEST, configPanel);
		layout.putConstraint(SpringLayout.NORTH, converterLbl, 5,
				SpringLayout.SOUTH, changeModuleBtn);

		layout.putConstraint(SpringLayout.EAST, changeConverterBtn, -5,
				SpringLayout.EAST, configPanel);
		layout.putConstraint(SpringLayout.NORTH, changeConverterBtn, 5,
				SpringLayout.SOUTH, changeModuleBtn);

		layout.putConstraint(SpringLayout.NORTH, converterPathField, 5,
				SpringLayout.SOUTH, changeModuleBtn);
		layout.putConstraint(SpringLayout.EAST, converterPathField, -5,
				SpringLayout.WEST, changeConverterBtn);
		layout.putConstraint(SpringLayout.WEST, converterPathField, 5,
				SpringLayout.EAST, converterLbl);

		JPanel leftPanel = new JPanel();
		layout = new SpringLayout();
		leftPanel.setLayout(layout);

		modelList = new JList();
		modelList.setModel(new DefaultListModel());
		modelList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		JScrollPane modelListPanel = new JScrollPane(modelList);
		modelListPanel.setBorder(BorderFactory.createCompoundBorder(
				BorderFactory.createTitledBorder("Models"), BorderFactory
						.createEmptyBorder(0, 5, 5, 0)));
		modelListPanel.setPreferredSize(new Dimension(305, 360));
		modelListPanel.setMinimumSize(new Dimension(200, 100));
		leftPanel.setPreferredSize(new Dimension(325, 400));
		leftPanel.setMinimumSize(new Dimension(200, 140));
		layout.putConstraint(SpringLayout.NORTH, modelListPanel, 5,
				SpringLayout.NORTH, leftPanel);
		leftPanel.add(modelListPanel);

		JButton loadMeshBtn = new JButton("Laden");
		layout.putConstraint(SpringLayout.NORTH, loadMeshBtn, 5,
				SpringLayout.SOUTH, modelListPanel);
		leftPanel.add(loadMeshBtn);

		JPanel modelDetailsPanel = new JPanel();
		layout = new SpringLayout();
		modelDetailsPanel.setLayout(layout);
		modelDetailsPanel.setBorder(BorderFactory.createCompoundBorder(
				BorderFactory.createTitledBorder("Details"), BorderFactory
						.createEmptyBorder(5, 5, 5, 5)));

		JPanel namePanel = new JPanel();
		namePanel.setLayout(new BoxLayout(namePanel, BoxLayout.X_AXIS));
		JButton renameBtn = new JButton("Umbenennen");

		modelNameEdit = new JTextField(20);
		namePanel.add(modelNameEdit);
		namePanel.add(renameBtn);
		modelDetailsPanel.add(namePanel);
		layout.putConstraint(SpringLayout.NORTH, namePanel, 5,
				SpringLayout.NORTH, modelDetailsPanel);

		JPanel polyPanel = new JPanel();
		polyPanel.setLayout(new BoxLayout(polyPanel, BoxLayout.X_AXIS));

		JLabel modelTriNumberLbl = new JLabel("Triangle count: ",
				SwingConstants.TRAILING);
		modelTriangleCountField = new JTextField(7);
		modelTriangleCountField.setEditable(false);
		modelTriNumberLbl.setLabelFor(modelTriangleCountField);
		polyPanel.add(modelTriNumberLbl);
		polyPanel.add(modelTriangleCountField);
		modelDetailsPanel.add(polyPanel);
		layout.putConstraint(SpringLayout.NORTH, polyPanel, 10,
				SpringLayout.SOUTH, namePanel);

		skeletonPanel = new JPanel();
		skeletonPanel.setLayout(new BoxLayout(skeletonPanel, BoxLayout.X_AXIS));

		JLabel modelSkeletonLbl = new JLabel("Skeleton-Link: ", SwingConstants.TRAILING);
		modelHasSkeletonField = new JTextField(20);
		modelHasSkeletonField.setEditable(false);
		modelSkeletonLbl.setLabelFor(modelHasSkeletonField);
		skeletonPanel.add(modelSkeletonLbl);
		skeletonPanel.add(modelHasSkeletonField);
		modelDetailsPanel.add(skeletonPanel);
		JButton renameSkel = new JButton("Ändern");
		skeletonPanel.add(renameSkel);
		layout.putConstraint(SpringLayout.NORTH, skeletonPanel, 10,
				SpringLayout.SOUTH, polyPanel);

		modelMaterials = new JTextArea();
		modelMaterials.setEditable(false);
		modelMaterials.setPreferredSize(new Dimension(300, 70));
		modelMaterials.setBorder(BorderFactory.createCompoundBorder(
				BorderFactory.createTitledBorder("Verwendete Materials"),
				BorderFactory.createEmptyBorder(5, 5, 5, 5)));
		modelDetailsPanel.add(modelMaterials);
		layout.putConstraint(SpringLayout.NORTH, modelMaterials, 10,
				SpringLayout.SOUTH, skeletonPanel);

		modelMeasures = new JTextArea();
		modelMeasures.setEditable(false);
		modelMeasures.setPreferredSize(new Dimension(300, 100));
		modelMeasures.setBorder(BorderFactory.createCompoundBorder(
				BorderFactory.createTitledBorder("Ausmaße"), BorderFactory
						.createEmptyBorder(5, 5, 5, 5)));
		modelDetailsPanel.add(modelMeasures);
		layout.putConstraint(SpringLayout.NORTH, modelMeasures, 10,
				SpringLayout.SOUTH, modelMaterials);

		JPanel factorPanel = new JPanel();
		factorPanel.setLayout(new BoxLayout(factorPanel, BoxLayout.X_AXIS));

		JButton scaleBtn = new JButton("Skalieren");
		JLabel factorLabel = new JLabel("Skalieren um: ", SwingConstants.TRAILING);
		factorField = new JFormattedTextField(NumberFormat
				.getNumberInstance(Locale.ENGLISH));
		factorField.setColumns(10);
		factorLabel.setLabelFor(factorField);
		factorPanel.add(factorLabel);
		factorPanel.add(factorField);
		factorPanel.add(scaleBtn);
		modelDetailsPanel.add(factorPanel);
		layout.putConstraint(SpringLayout.NORTH, factorPanel, 10,
				SpringLayout.SOUTH, modelMeasures);

		factorField.setEditable(false);
		modelNameEdit.setEditable(false);

		modelDetailsPanel.setPreferredSize(new Dimension(390, 370));
		modelDetailsPanel.setMinimumSize(new Dimension(390, 370));

		JPanel rightPanel = new JPanel();
		layout = new SpringLayout();
		rightPanel.setLayout(layout);
		JButton exportBtn = new JButton("zu mesh");
		JButton saveBtn = new JButton("zu xml");
		layout.putConstraint(SpringLayout.SOUTH, exportBtn, -5,
				SpringLayout.SOUTH, rightPanel);
		layout.putConstraint(SpringLayout.SOUTH, saveBtn, -5,
				SpringLayout.NORTH, exportBtn);
		rightPanel.add(exportBtn);
		rightPanel.add(saveBtn);
		rightPanel.setPreferredSize(new Dimension(95, 370));
		rightPanel.setMinimumSize(new Dimension(95, 40));

		logField = new JTextArea();
		logField.setEditable(false);
		logPane = new JScrollPane(logField);
		logPane.setBorder(BorderFactory.createCompoundBorder(BorderFactory
				.createTitledBorder("Log"), BorderFactory.createEmptyBorder(5,
				5, 5, 5)));
		logPane.setPreferredSize(new Dimension(800, 100));
		logPane.setMinimumSize(new Dimension(400, 100));

		this.setLayout(new BorderLayout());
		this.getContentPane().add(configPanel, BorderLayout.NORTH);
		this.getContentPane().add(logPane, BorderLayout.SOUTH);
		this.getContentPane().add(rightPanel, BorderLayout.EAST);
		this.getContentPane().add(leftPanel, BorderLayout.WEST);
		this.getContentPane().add(modelDetailsPanel, BorderLayout.CENTER);
		this.pack();
		this.setVisible(true);

		// config panel with module dir and path to the xml converter
		// and controls to change both
		// Wovon ist hier die Rede.

		// Den Buttons wird ihre Funktionalit�t zugewiesen.
		changeModuleBtn.setActionCommand("ChangeModuleDirectory");
		changeModuleBtn.addActionListener(this);

		changeConverterBtn.setActionCommand("ChangeConverterPath");
		changeConverterBtn.addActionListener(this);

		loadMeshBtn.setActionCommand("LoadMesh");
		loadMeshBtn.addActionListener(this);

		renameBtn.setActionCommand("RenameModel");
		renameBtn.addActionListener(this);

		renameSkel.setActionCommand("RenameSkeleton");
		renameSkel.addActionListener(this);

		scaleBtn.setActionCommand("ScaleModel");
		scaleBtn.addActionListener(this);

		saveBtn.setActionCommand("SaveModel");
		saveBtn.addActionListener(this);

		saveBtn.setActionCommand("ExportModel");
		saveBtn.addActionListener(this);

		loggen("Meshhandler gestartet.");

		// Initialise propery manager and retrieve needed props
		propertyManager = new PropertyManager();
		String ogrepath = propertyManager.getOgreTools();
		converterPathField.setText(ogrepath != null ? ogrepath
				: "<nicht gesetzt>");
		String moduldir = propertyManager.getModule();
		moduleDirField.setText(moduldir != null ? moduldir : "<nicht gesetzt>");

		modelsLaden(moduldir, (DefaultListModel) modelList.getModel());
		loggen("OgreXMLConverter: " + ogrepath);
		loggen("Modul: " + moduldir);

	}

	public void loggen(String s) {
		logPane.invalidate();
		logField.setText(logField.getText() + s + "\n");
		logPane.repaint();
	}

	public void actionPerformed(ActionEvent e) {
		if (e.getActionCommand().equals("ChangeConverterPath")) {
			propertyManager.changeOgredir();
			converterPathField.setText(propertyManager.getOgreTools());

		} else if (e.getActionCommand().equals("ChangeModuleDirectory")) {
			propertyManager.changeModuldir();
			modelsLaden(propertyManager.getModule(),
					(DefaultListModel) modelList.getModel());
			moduleDirField.setText(propertyManager.getModule());

		} else if (e.getActionCommand().equals("LoadMesh")
				&& modelList.getSelectedValue() != null) {
			oldModel = new File(propertyManager.getModule(), (String) modelList
					.getSelectedValue());
			String converterPath = propertyManager.getOgreTools();

			if (!oldModel.getName().endsWith(".xml")) {
				if (ConverterUser.fromMeshToXML(converterPath, oldModel
						.getAbsolutePath())) {
					loggen(modelList.getSelectedValue() + " wird bearbeitet.");
					loggen(" .mesh.xml erzeugt.");
					
				} else {
					loggen("Fehler beim Konvertieren des Mesh!");
				}
			}
			try {
				String modelPath = oldModel.getAbsolutePath() + ".xml";
				model = new MeshLoader().readMesh(modelPath);
			} catch (Exception e1) {
				loggen("Fehler beim Einlesen der .mesh.xml. (Siehe Konsole)");
				e1.printStackTrace();
			}
			loggen(".mesh.xml eingelesen");

			boolean hasSkeleton = model.getSkeletonLink().length() > 0;

			model.setName(((String) modelList.getSelectedValue()).substring(0,
					((String) modelList.getSelectedValue()).indexOf(".")));
			if (hasSkeleton
					&& ConverterUser.fromMeshToXML(converterPath,
							oldModel.getAbsolutePath()
									.substring(
											0,
											oldModel.getAbsolutePath()
													.lastIndexOf(System.getProperty("file.separator")) + 1)
									+ model.getSkeletonLink())) {
				loggen(" .skeleton.xml erzeugt.");
				try {
					skeleton = new SkeletonLoader()
							.readSkeleton(oldModel.getAbsolutePath()
									.substring(
											0,
											oldModel.getAbsolutePath()
													.lastIndexOf(System.getProperty("file.separator")) + 1)
									+ model.getSkeletonLink() + ".xml");
					loggen(".skeleton.xml eingelesen.");
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
			}

			modelNameEdit.setText(model.getName());
			modelTriangleCountField.setText(String
					.valueOf(model.getPolycount()));
			modelHasSkeletonField.setText(model.getSkeletonLink());
			modelHasSkeletonField.setEditable(true);
			skeletonPanel.setVisible(hasSkeleton);

			paintMeasures();

			ArrayList<String> materials = model.getMaterials();
			for (String material : materials) {
				modelMaterials.append(material + "\n");
			}

			factorField.setValue(new Float(1));
			factorField.setEditable(true);
			modelNameEdit.setEditable(true);

			loggen(model.getName() + " geladen.");

			this.setSize(800, 600);
			this.invalidate();
			this.repaint();

		} else if (e.getActionCommand().equals("RenameModel") && model != null) {
			model.setName(modelNameEdit.getText());
		} else if (e.getActionCommand().equals("RenameSkeleton")
				&& model.getSkeletonLink().length() > 0) {
			model.setSkeletonLink(modelHasSkeletonField.getText());
		} else if (e.getActionCommand().equals("ScaleModel") && model != null) {
			boolean hasSkeleton = model.getSkeletonLink().length() > 0;
			float factor = (float) (Double.parseDouble(factorField.getText()));
			model.scale(factor);
			if (hasSkeleton) {
				skeleton.scale(factor);
			}
			paintMeasures();
			factorField.setValue(new Float(1));

		} else if (e.getActionCommand().equals("SaveModel") && model != null) {
			save();
		} else if (e.getActionCommand().equals("ExportModel") && model != null) {
			save();
			convert();
		}
	}

	private void convert() {
		if (model.getSkeletonLink().length() > 0
				&& ConverterUser.fromXMLToMesh(propertyManager.getOgreTools(),
						propertyManager.getModule() + "/"
								+ model.getSkeletonLink() + ".xml")) {
			loggen(" skeleton geschrieben");
			(new File(propertyManager.getModule(), model.getSkeletonLink()))
					.delete();

		}
		if (ConverterUser.fromXMLToMesh(propertyManager.getOgreTools(),
				propertyManager.getModule() + "/" + model.getName()
						+ ".mesh.xml")) {
			loggen(" mesh geschrieben");
			oldModel.delete();
		}

	}

	private void paintMeasures() {
		Vector3 measures = model.getMeasures();
		modelMeasures.setText(measures.getX() + " m lang,\n " + measures.getZ()
				+ " m breit\n und " + measures.getY() + " m hoch.");

	}

	private void modelsLaden(String moduleDir, DefaultListModel listModel) {
		listModel.clear();
		File modelsdir;
		if (new File(moduleDir, "models").exists()) {
			modelsdir = new File(moduleDir, "models");
		} else {
			modelsdir = new File(moduleDir);
		}
		String[] meshNames = modelsdir.list(new FilenameFilter() {
			public boolean accept(File f, String s) {
				return s.endsWith(".mesh") || s.endsWith(".mesh.xml");
			}
		});
		if (meshNames.length != 0) {
			for (int i = 0; i < meshNames.length; ++i) {
				listModel.addElement(meshNames[i]);
			}
			loggen("Modul: " + moduleDir);
		} else {
			JOptionPane.showMessageDialog(this,

			"Das angegebene Verzeichnis scheint keine Models zu enthalten.",
					"Keine Models gefunden", JOptionPane.ERROR_MESSAGE);
		}
	}

	protected void processWindowEvent(WindowEvent e) {
		super.processWindowEvent(e);
		if (e.getID() == WindowEvent.WINDOW_CLOSING) {
			propertyManager.setModule(propertyManager.getModule());
			System.exit(0);
		}
	}

	private void save() {
		boolean hasSkeleton = model.getSkeletonLink().length() > 0;
		String moduleDir = propertyManager.getModule();

		File newXml = new File(moduleDir + "/", model.getName() + ".mesh.xml");
		if (newXml.exists()
				&& newXml.renameTo(new File(moduleDir + "/", model.getName()
						+ "_old.mesh.xml"))) {
			loggen("Vorheriges xml nach " + model.getName()
					+ "_old.mesh.xml verschoben.");
		}
		File newSkelXml = new File(moduleDir + "/", model.getSkeletonLink()
				+ ".xml");
		if (newSkelXml.exists()
				&& newSkelXml.renameTo(new File(moduleDir + "/_old", model
						.getSkeletonLink()
						+ ".xml"))) {
			loggen("Vorheriges xml nach _old" + model.getSkeletonLink()
					+ ".xml verschoben.");
		}
		try {
			FileWriter xmlout = new FileWriter(newXml);
			xmlout.write(model.toXML());
			xmlout.close();
		} catch (IOException e1) {
			loggen("Fehler beim Schreiben der .mesh.xml");
			e1.printStackTrace();
		}
		if (hasSkeleton) {
			try {
				FileWriter xmlout = new FileWriter(newSkelXml);
				xmlout.write(skeleton.toXML());
				xmlout.close();
			} catch (IOException e1) {
				System.out.println("Fehler beim Schreiben der .skeleton.xml");
				e1.printStackTrace();
			}
		}

	}
}