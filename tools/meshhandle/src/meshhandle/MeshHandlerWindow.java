package meshhandle;

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
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.xml.parsers.ParserConfigurationException;

import meshhandle.converter.ConverterUser;
import meshhandle.data.Vector3;
import meshhandle.model.Mesh;
import meshhandle.skeleton.Skeleton;
import meshhandle.xml.MeshLoader;
import meshhandle.xml.SkeletonLoader;

import org.xml.sax.SAXException;

public class MeshHandlerWindow extends JFrame implements ActionListener,
		ListSelectionListener {

	private static final long serialVersionUID = 1L;

	// Die eigentlichen Daten
	private Mesh model;

	private Skeleton skeleton;

	private PropertyManager propertyManager;

	private boolean nameChanged;

	private boolean scaled;

	private String oldname;

	// Fensterelemente
	private JTextArea logField;

	private JScrollPane logPane;

	private JList modelList;

	private JScrollPane modelsPane;

	private JTextField converterPathField;

	private JTextField moduleDirField;

	private JTextField modelNameEdit;

	private JTextField modelTriangleCountField;

	private JTextField modelHasSkeletonField;

	private JTextArea modelMeasures;

	private JFormattedTextField factorField;

	private JTextArea modelMaterials;

	private JButton saveBtn;

	public MeshHandlerWindow() {
		super("Der wunderbarer Meshhandler v0.8");

		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		// Setup form elements..

		// config panel with module dir and path to the xml converter
		// and controls to change both

		JLabel moduleLbl = new JLabel("Modul-Verzeichnis: ", JLabel.TRAILING);
		moduleDirField = new JTextField(Integer.MAX_VALUE);
		moduleDirField.setEditable(false);
		moduleLbl.setLabelFor(moduleDirField);

		JButton changeModuleBtn = new JButton("Ändern..");
		changeModuleBtn.setActionCommand("ChangeModuleDirectory");
		changeModuleBtn.addActionListener(this);

		JLabel converterLbl = new JLabel("OgreXmlConverter: ", JLabel.TRAILING);
		converterPathField = new JTextField(Integer.MAX_VALUE);
		converterPathField.setEditable(false);
		converterLbl.setLabelFor(converterPathField);

		JButton changeConverterBtn = new JButton("Ändern..");
		changeConverterBtn.setActionCommand("ChangeConverterPath");
		changeConverterBtn.addActionListener(this);

		JPanel configPanel = new JPanel();
		configPanel.setBorder(BorderFactory.createCompoundBorder(BorderFactory
				.createTitledBorder("Konfiguration"), BorderFactory
				.createEmptyBorder(5, 5, 5, 5)));
		configPanel.setPreferredSize(new Dimension(800, 100));
		configPanel.setMinimumSize(new Dimension(600, 100));
		SpringLayout layout = new SpringLayout();
		configPanel.setLayout(layout);

		configPanel.add(changeConverterBtn);
		configPanel.add(changeModuleBtn);
		configPanel.add(moduleLbl);
		configPanel.add(converterLbl);
		configPanel.add(converterPathField);
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

		JButton loadMeshBtn = new JButton("Laden");
		loadMeshBtn.setActionCommand("LoadMesh");
		loadMeshBtn.addActionListener(this);

		modelList = new JList();
		modelList.setModel(new DefaultListModel());
		modelList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		modelList.addListSelectionListener(this);
		modelList.setPreferredSize(new Dimension(300, 400));

		JScrollPane modelListPanel = new JScrollPane(modelList);
		modelListPanel.setBorder(BorderFactory.createCompoundBorder(
				BorderFactory.createTitledBorder("Models"), BorderFactory
						.createEmptyBorder(5, 5, 5, 5)));

		modelListPanel.setPreferredSize(new Dimension(350, 250));
		modelListPanel.setMinimumSize(new Dimension(200, 100));

		JPanel namePanel = new JPanel();
		namePanel.setLayout(new BoxLayout(namePanel, BoxLayout.X_AXIS));
		JButton renameBtn = new JButton("Umbenennen");
		renameBtn.setActionCommand("RenameModel");
		renameBtn.addActionListener(this);

		modelNameEdit = new JTextField("            ");
		namePanel.add(loadMeshBtn);
		namePanel.add(modelNameEdit);
		namePanel.add(renameBtn);

		JPanel polyPanel = new JPanel();
		polyPanel.setLayout(new BoxLayout(polyPanel, BoxLayout.X_AXIS));
		JLabel modelTriNumberLbl = new JLabel("Triangle count: ");
		modelTriangleCountField = new JTextField(6);
		//modelTriNumberLbl.setLabelFor(modelTriangleCountField);
		polyPanel.add(modelTriNumberLbl);
		polyPanel.add(modelTriangleCountField);

		JPanel skeletonPanel = new JPanel();
		skeletonPanel.setLayout(new BoxLayout(skeletonPanel, BoxLayout.X_AXIS));
		JLabel modelSkeletonLbl = new JLabel("Skeleton: ");
		modelHasSkeletonField = new JTextField("     ");
		skeletonPanel.add(modelSkeletonLbl);
		skeletonPanel.add(modelHasSkeletonField);

		modelMeasures = new JTextArea("\n\n\n");
		modelMaterials = new JTextArea("                  ");

		JPanel materialsPanel = new JPanel();
		materialsPanel.add(modelMaterials);

		factorField = new JFormattedTextField(NumberFormat
				.getNumberInstance(Locale.ENGLISH));
		factorField.setValue(new Float(1));
		factorField.setColumns(10);

		JButton scaleBtn = new JButton("Skalieren");
		scaleBtn.setActionCommand("ScaleModel");
		scaleBtn.addActionListener(this);

		JPanel factorPanel = new JPanel();
		factorPanel.setLayout(new BoxLayout(factorPanel, BoxLayout.X_AXIS));
		factorPanel.add(new JLabel("Skalieren um Faktor: "));
		factorPanel.add(factorField);
		factorPanel.add(scaleBtn);

		JPanel modelDetailsPanel = new JPanel();
		layout = new SpringLayout();
		modelDetailsPanel.setLayout(layout);
		modelDetailsPanel.add(namePanel);
		modelDetailsPanel.add(polyPanel);
		modelDetailsPanel.add(skeletonPanel);
		modelDetailsPanel.add(modelMeasures);
		modelDetailsPanel.add(materialsPanel);
		modelDetailsPanel.add(factorPanel);

		// layout.putConstraint(SpringLayout.NORTH, modelNameEdit, 5,
		// SpringLayout.NORTH, modelDetailsPanel);
		// layout.putConstraint(SpringLayout.WEST, modelNameEdit, 5,
		// SpringLayout.WEST, modelDetailsPanel);
		layout.putConstraint(SpringLayout.NORTH, namePanel, 5,
				SpringLayout.NORTH, modelDetailsPanel);
		layout.putConstraint(SpringLayout.NORTH, polyPanel, 10,
				SpringLayout.SOUTH, namePanel);
		layout.putConstraint(SpringLayout.NORTH, skeletonPanel, 10,
				SpringLayout.SOUTH, polyPanel);
		layout.putConstraint(SpringLayout.NORTH, modelMeasures, 10,
				SpringLayout.SOUTH, skeletonPanel);
		layout.putConstraint(SpringLayout.NORTH, materialsPanel, 10,
				SpringLayout.SOUTH, modelMeasures);
		layout.putConstraint(SpringLayout.NORTH, factorPanel, 10,
				SpringLayout.SOUTH, materialsPanel);

		saveBtn = new JButton("Speichern");
		saveBtn.setActionCommand("SaveModel");
		saveBtn.addActionListener(this);

		logField = new JTextArea("\n");
		logField.setEditable(false);

		logPane = new JScrollPane(logField);

		this.setLayout(new BorderLayout());
		this.getContentPane().add(configPanel, BorderLayout.NORTH);
		this.getContentPane().add(saveBtn, BorderLayout.EAST);
		this.getContentPane().add(modelListPanel, BorderLayout.WEST);
		this.getContentPane().add(modelDetailsPanel, BorderLayout.CENTER);
		this.getContentPane().add(logPane, BorderLayout.SOUTH);

		// Initialise propery manager and retrieve needed props
		propertyManager = new PropertyManager();
		String ogrepath = propertyManager.getOgreTools();
		converterPathField.setText(ogrepath != null ? ogrepath
				: "<nicht gesetzt>");
		String moduldir = propertyManager.getModule();
		moduleDirField.setText(moduldir != null ? moduldir : "<nicht gesetzt>");

		modelsLaden(moduldir, (DefaultListModel) modelList.getModel());
		loggen("Meshhandler gestartet.");
		loggen("OgreXMLConverter: " + ogrepath);
		loggen("Modul: " + moduldir);

		this.pack();
	}

	private void modelsLaden(String moduleDir, DefaultListModel listModel) {
		listModel.clear();
		File modelsdir = new File(moduleDir, "models");
		String[] meshNames = modelsdir.list(new FilenameFilter() {
			public boolean accept(File f, String s) {
				return s.endsWith(".mesh");
			}
		});
		for (int i = 0; i < meshNames.length; ++i) {
			listModel.addElement(meshNames[i]);
		}
		loggen("Modul: " + moduleDir);
	}

	private void paintMeasures() {
		Vector3 measures = model.getMeasures();
		modelMeasures.setEditable(true);
		modelMeasures.setText("Maße: " + measures.getX() + " m lang,\n "
				+ measures.getZ() + " m breit\n und " + measures.getY()
				+ " m hoch.");
		modelMeasures.setEditable(false);
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
			modelsPane.invalidate();
			modelsPane.repaint();

		} else if (e.getActionCommand().equals("LoadMesh")
				&& modelList.getSelectedValue() != null) {
			String moduleDir = propertyManager.getModule();
			String converterPath = propertyManager.getOgreTools();

			if (ConverterUser.fromMeshToXML(converterPath, moduleDir
					+ "/models/" + modelList.getSelectedValue())) {
				loggen(modelList.getSelectedValue() + " wird bearbeitet.");
				loggen(" .mesh.xml erzeugt.");
			} else {
				loggen("Fehler beim Konvertieren des Mesh!");
			}
			nameChanged = false;
			try {
				model = MeshLoader.readMesh(moduleDir + "/models/"
						+ modelList.getSelectedValue() + ".xml");

				loggen(".mesh.xml eingelesen");
			} catch (Exception e1) {
				loggen("Fehler beim Einlesen der .mesh.xml. (Siehe Konsole)");
				e1.printStackTrace();
			}

			boolean hasSkeleton = !model.getSkeletonLink().equals("");
			model.setName(((String) modelList.getSelectedValue()).substring(0,
					((String) modelList.getSelectedValue()).length() - 5));
			if (hasSkeleton
					&& ConverterUser.fromMeshToXML(converterPath, moduleDir
							+ "/models/" + model.getSkeletonLink())) {
				loggen(" .skeleton.xml erzeugt.");
				try {
					skeleton = SkeletonLoader.readSkeleton(moduleDir
							+ "/models/" + model.getSkeletonLink() + ".xml");
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
			modelNameEdit.setSize(modelNameEdit.getPreferredSize());
			modelTriangleCountField.setText(String
					.valueOf(model.getPolycount()));
			modelHasSkeletonField.setText(hasSkeleton ? "Animierbar"
					: "Statisch");

			scaled = false;
			paintMeasures();

			modelMaterials.setEditable(true);
			modelMaterials.setText("");
			ArrayList<String> materials = model.getMaterials();
			for (String material : materials) {
				modelMaterials.append("Material: " + material + "\n");
			}
			modelMaterials.setEditable(false);
			loggen(model.getName() + " geladen.");

			this.setSize(800, 600);
			this.invalidate();
			this.repaint();

		} else if (e.getActionCommand().equals("RenameModel") && model != null) {
			boolean hasSkeleton = !model.getSkeletonLink().equals("");
			oldname = model.getName();
			nameChanged = true;
			model.setName(modelNameEdit.getText());
			if (!hasSkeleton)
				model.setSkeletonLink(modelNameEdit + ".skeleton");
		} else if (e.getActionCommand().equals("ScaleModel") && model != null) {
			boolean hasSkeleton = !model.getSkeletonLink().equals("");
			float factor = (float) (Double.parseDouble(factorField.getText()));
			model.scale(factor);
			if (hasSkeleton) {
				skeleton.scale(factor);
			}
			scaled = true;
			paintMeasures();
			factorField.setValue(new Float(1));

		} else if (e.getActionCommand().equals("SaveModel") && model != null) {
			save();
		}
	}

	public void loggen(String s) {
		logPane.invalidate();
		logField.setEditable(true);
		logField.setText(logField.getText() + s + "\n");
		logField.setEditable(false);
		logPane.repaint();
	}

	protected void processWindowEvent(WindowEvent e) {
		super.processWindowEvent(e);
		if (e.getID() == WindowEvent.WINDOW_CLOSING) {
			propertyManager.setModule(propertyManager.getModule());
			System.exit(0);
		}
	}

	public void valueChanged(ListSelectionEvent e) {
		if (modelList.getSelectedIndex() >= 0 && (scaled || nameChanged)) {
			int returnVal = JOptionPane.showOptionDialog(this,
					"Vor dem Fortfahren speichern?", "Änderungen speichern?",
					JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE,
					null, null, null);
			if (returnVal == JOptionPane.YES_OPTION) {
				save();
			}
		}
		if (modelList.getSelectedIndex() >= 0) {
			modelNameEdit.setText((String) modelList.getSelectedValue());
			modelTriangleCountField.setText("              ");
			modelHasSkeletonField.setText("                  ");
			modelMaterials.setText("\n\n\n");
		}
	}

	private void save() {
		boolean hasSkeleton = !model.getSkeletonLink().equals("");
		String moduleDir = propertyManager.getModule();
		String converterPath = propertyManager.getOgreTools();

		File newXml = new File(moduleDir + "/models", model.getName()
				+ ".mesh.xml");
		File newSkelXml = new File(moduleDir + "/models", model
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
		if (hasSkeleton) {
			try {
				FileWriter xmlout = new FileWriter(newSkelXml);
				xmlout.write(skeleton.toXML());
				xmlout.close();
			} catch (IOException e1) {
				System.out.println("Fehler beim Schreiben der .skeleton.xml");
				e1.printStackTrace();
			}
			if (ConverterUser.fromXMLToMesh(converterPath, newSkelXml
					.getAbsolutePath())) {
				loggen(" skeleton geschrieben");
			}
		}
		if (ConverterUser
				.fromXMLToMesh(converterPath, newXml.getAbsolutePath())) {
			loggen(" mesh geschrieben");
		}
		// TODO Anstelle des Abgleichs mit nameChanged kann man die
		// Files vielleicht auch einzeln untereinander abgleichen und
		// die abweichenden Löschen.

		if (nameChanged) {
			(new File(moduleDir + "/models", oldname + ".mesh")).delete();
			(new File(moduleDir + "/models", oldname + ".mesh.xml")).delete();
			if (hasSkeleton) {
				(new File(moduleDir + "/models", oldname + ".skeleton"))
						.delete();
				(new File(moduleDir + "/models", oldname + ".skeleton.xml"))
						.delete();
			}
		}
	}
}