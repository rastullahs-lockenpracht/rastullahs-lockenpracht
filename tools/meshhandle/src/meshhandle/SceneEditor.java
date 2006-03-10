package meshhandle;

import java.awt.BorderLayout;
import java.awt.Menu;
import java.awt.MenuBar;
import java.awt.MenuItem;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTree;
import javax.swing.filechooser.FileFilter;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import javax.xml.parsers.ParserConfigurationException;

import meshhandle.scene.Scene;
import meshhandle.scene.SceneNode;
import meshhandle.sceneeditor.MeshFile;
import meshhandle.sceneeditor.SceneTreeCreator;
import meshhandle.xml.SceneLoader;

import org.xml.sax.SAXException;

public class SceneEditor extends JFrame implements ActionListener {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		(new SceneEditor()).setVisible(true);
	}

	private MenuItem mOpenItem;
	private MenuItem mSaveItem;
	private Scene mScene;
	private JTree mSceneTree;
	private MenuItem mGroupByEntitiesItem;
	private MenuItem mGroupByNodesItem;
	private JButton mRemoveButton;
	private JFileChooser mDotsceneChooser;
	private MenuItem mQuitItem;
	
	public SceneEditor()
	{
		super(".scene-Editor");
		setSize(800, 600);
		
		MenuBar menubar = createMenubar();
		setMenuBar(menubar);
		
		JPanel commandBar = new JPanel();
		mRemoveButton = new JButton("Delete");
		mRemoveButton.addActionListener(this);
		commandBar.add(mRemoveButton);

		mSceneTree = new JTree();
		
		getContentPane().setLayout(new BorderLayout());
		getContentPane().add(new JScrollPane(mSceneTree), BorderLayout.CENTER);
		getContentPane().add(commandBar, BorderLayout.NORTH);
		
		mDotsceneChooser = createDotSceneChooser();
	}

	private JFileChooser createDotSceneChooser() {
		JFileChooser fileChooser = new JFileChooser();
		fileChooser.setFileFilter(new FileFilter() {
		
			@Override
			public String getDescription() {
				return "dotScene file (*.scene)";
			}
		
			@Override
			public boolean accept(File file) {
				return file.isDirectory() || file.getName().endsWith(".scene");
			}
		
		});

		return fileChooser;
	}

	private MenuBar createMenubar() {
		Menu sceneMenu = new Menu("File");
		mOpenItem = new MenuItem("Open...");
		mOpenItem.addActionListener(this);
		sceneMenu.add(mOpenItem);
		mSaveItem = new MenuItem("Save...");
		mSaveItem.addActionListener(this);
		sceneMenu.add(mSaveItem);
		sceneMenu.addSeparator();
		mQuitItem = new MenuItem("Quit");
		mQuitItem.addActionListener(this);
		sceneMenu.add(mQuitItem);
		
		Menu groupMenu = new Menu("Group");
		mGroupByNodesItem = new MenuItem("Nodes");
		mGroupByNodesItem.addActionListener(this);
		groupMenu.add(mGroupByNodesItem);
		mGroupByEntitiesItem = new MenuItem("Entities");
		mGroupByEntitiesItem.addActionListener(this);
		groupMenu.add(mGroupByEntitiesItem);
		
		MenuBar menubar = new MenuBar();
		menubar.add(sceneMenu);
		menubar.add(groupMenu);
		return menubar;
	}

	public void actionPerformed(ActionEvent evt) 
	{
		if (evt.getSource() == mRemoveButton)
		{
			DefaultMutableTreeNode selectedNode = (DefaultMutableTreeNode) mSceneTree.getLastSelectedPathComponent();
			System.out.println(selectedNode);
			
			if (selectedNode.getUserObject() instanceof SceneNode)
			{
				mScene.removeNode((SceneNode)selectedNode.getUserObject());
			}
			else if (selectedNode.getUserObject() instanceof String)
			{
				int res = JOptionPane.showConfirmDialog(
						this, 
						"Delete multiple nodes", 
						"Do you really want to delete all nodes with mesh '"
						+ selectedNode + "'?", 
						JOptionPane.YES_NO_OPTION);
				
				if (res == JOptionPane.YES_OPTION)
				{
					for (int childIdx = 0; childIdx < selectedNode.getChildCount(); childIdx++)
					{
						mScene.removeNode((SceneNode)((DefaultMutableTreeNode)selectedNode.getChildAt(childIdx)).getUserObject());
						
					}
				}
			}
		}
		else if (evt.getSource() == mGroupByEntitiesItem)
		{
			if (mScene != null)
				mSceneTree.setModel(new DefaultTreeModel(SceneTreeCreator.createEntityTree(mScene)));
		}
		else if (evt.getSource() == mGroupByNodesItem)
		{
			if (mScene != null)
				mSceneTree.setModel(new DefaultTreeModel(SceneTreeCreator.createSceneTree(mScene)));
		}
		else if (evt.getSource() == mOpenItem)
		{
			if (JFileChooser.APPROVE_OPTION == mDotsceneChooser.showOpenDialog(this))
			{
				try 
				{
					mScene = SceneLoader.readScene(mDotsceneChooser.getSelectedFile().getAbsolutePath());
					mSceneTree.setModel(new DefaultTreeModel(SceneTreeCreator.createSceneTree(mScene)));
				} 
				catch (ParserConfigurationException e) 
				{
					e.printStackTrace();
				} 
				catch (SAXException e) 
				{
					e.printStackTrace();
				} 
				catch (IOException e) 
				{
					e.printStackTrace();
				}
			}
		}
		else if (evt.getSource() == mSaveItem)
		{
			if (JFileChooser.APPROVE_OPTION == mDotsceneChooser.showSaveDialog(this))
			{
				try 
				{
					FileWriter outFile = new FileWriter(mDotsceneChooser.getSelectedFile(), false);
			        outFile.write(mScene.toXML());
			        outFile.flush();
			        outFile.close();
				}
				catch (IOException e) 
				{
					e.printStackTrace();
				}
			}
		}
		else if (evt.getSource() == mQuitItem)
		{
			System.exit(0);
		}
	}

}
