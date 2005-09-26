import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FilenameFilter;

import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JPanel;

public class DialogKlasse {
	/**
	 * @param args
	 */
	JFrame frame = new JFrame();
	JPanel modelsPane=new JPanel();
	JPanel modulPane = new JPanel();
	JPanel optionsPane = new JPanel();
	File eventuellesxml;
	
public void main()
	{
		PropertyManager pm = new PropertyManager();
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		final File[] files = pm.propertiesLaden();
		modulPane.setLayout(new BorderLayout());
		
		File[] modulesdirs = (new File(files[0], "modules").listFiles(new FilenameFilter() {
			public boolean accept(File f, String s) {
				if (s.endsWith(".svn"))
				{
					return false;
				}
				return new File(f,s).isDirectory();
				}
			}));
				
		final JComboBox moduleslist = new JComboBox(modulesdirs);
		moduleslist.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent event){
				modelsPane.removeAll();
				File[] models = (new File((File)moduleslist.getSelectedItem(), "models").listFiles(new FilenameFilter() {
					public boolean accept (File f, String s) {
						return s.toLowerCase().endsWith(".mesh");
					}
				}));
				final JComboBox modellist = new JComboBox(models);
				modellist.addActionListener(new ActionListener(){
					public void actionPerformed(ActionEvent event){
						if (eventuellesxml!=null)
						{eventuellesxml.delete();}
// Hier hängt sich das Teil bei geänderter Auswahl auf, anstatt, wie erhofft, das nicht mehr benötigte XML zu entfernen und ein neues anzulegen.
// Das könnte am noch bestehenden Zugriff des Converters liegen.
						eventuellesxml = new File(files[1], ((File)modellist.getSelectedItem()).getName()+".xml");
						frame.getContentPane().add(modulPane, BorderLayout.NORTH);
						frame.getContentPane().add(modelsPane, BorderLayout.WEST );
						optionsPane.removeAll();
						optionsPane = new MaterialsAendern((File)modellist.getSelectedItem());
						frame.getContentPane().add(optionsPane,
								BorderLayout.CENTER);
						frame.pack();
					}});
				modelsPane.add(modellist);
				frame.getContentPane().add(modelsPane, BorderLayout.WEST);
				frame.pack();
			}
		});
		
		modulPane.add(moduleslist, BorderLayout.CENTER);
		
		
		frame.getContentPane().setLayout(new BorderLayout());
		frame.getContentPane().add(modulPane, BorderLayout.NORTH);
		frame.pack();
		frame.setVisible(true);
		
	}

}
