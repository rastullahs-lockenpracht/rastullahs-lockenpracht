package meshhandle.moduleanalysis;

import java.awt.BorderLayout;
import java.awt.Menu;
import java.awt.MenuBar;
import java.awt.MenuItem;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;
import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTree;
import javax.swing.tree.DefaultTreeModel;
import meshhandle.io.plaintext.ConfigFileLoader;
import meshhandle.io.plaintext.RubyFileLoader;
import meshhandle.model.module.Module;
import meshhandle.model.module.ModuleConfig;
import meshhandle.model.module.ModuleLoader;
import meshhandle.model.module.RubyFile;

public class AnalysisWindow extends JFrame implements ActionListener
{
	private static final long serialVersionUID = 1L;
	
	private List<Module> mModules;
	private JTree mTree;
	private ModuleTreeBuilder mTreeBuilder = new ModuleTreeBuilder();

	private MenuItem mReloadItem;

	private MenuItem mQuitItem;

	public AnalysisWindow(Properties options)
	{
		setSize(500, 300);
		setTitle("Module analysis");
		
		MenuBar menubar = new MenuBar();
		Menu filemenu = new Menu("File");
		mReloadItem = new MenuItem("Reload");
		mReloadItem.addActionListener(this);
		filemenu.add(mReloadItem);
		mQuitItem = new MenuItem("Quit");
		mQuitItem.addActionListener(this);
		filemenu.add(mQuitItem);
		menubar.add(filemenu );
		setMenuBar(menubar);
		
		getContentPane().setLayout(new BorderLayout());
		mTree = new JTree();
		getContentPane().add(new JScrollPane(mTree), BorderLayout.CENTER);
		
		initialize(options);
	}

	private void initialize(Properties options)
	{
		mModules = new ArrayList<Module>();
		
		String moduleRoot = options.getProperty("moduleroot");
		ModuleConfig mc = ConfigFileLoader.loadModuleConfig(moduleRoot);
		for (String module : mc.getModules())
		{
			Module mod = new Module(module, moduleRoot + "/" + module);
			mModules.add(mod);
			
			RubyFileLoader.addSearchPath(moduleRoot + "/" + module + "/conf");
			RubyFileLoader.addSearchPath(moduleRoot + "/" + module + "/scripts");
			RubyFileLoader.addSearchPath(moduleRoot + "/" + module + "/scripts/maps");

			RubyFile rbfile = RubyFileLoader.loadRubyFile(
					moduleRoot + "/" + module + "/conf/moduleconfig.rb");
			mod.addReferencedRubyFile(rbfile);
			
			ModuleLoader.resolve(mod);
		}
		
		mTree.setModel(new DefaultTreeModel(mTreeBuilder.convert(mModules)));
	}

	public void actionPerformed(ActionEvent evt)
	{
		if (evt.getSource() == mQuitItem)
		{
			System.exit(0);
		}
	}
}
