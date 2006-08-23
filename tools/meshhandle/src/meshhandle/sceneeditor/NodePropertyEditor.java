package meshhandle.sceneeditor;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.List;

import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JPanel;

import meshhandle.model.scene.NodeUserData;
import meshhandle.model.scene.SceneNode;

public class NodePropertyEditor extends JDialog implements ActionListener 
{

	/**
	 * 
	 */
	private static final long serialVersionUID = 4774762763265921746L;
	private final List<SceneNode> mNodes;
	private JCheckBox _editPhysicsProxy;
	private JButton _okButton;
	private JButton _cancelButton;
	private JComboBox _physicsProxy;

	public NodePropertyEditor(List<SceneNode> nodes) 
	{
		mNodes = nodes;
		setSize(400, 300);
		setModal(true);
		setTitle("Edit Properties of "+nodes.size()+" Scene Nodes");
		
		initializeComponents();
		fillContent();
	}

	private void fillContent() 
	{
		for (int idx = 0; idx < NodeUserData.PHYSICAL_BODIES.length; idx++) 
		{
			_physicsProxy.addItem(NodeUserData.PHYSICAL_BODIES[idx]);		
		}
	}

	private void initializeComponents() 
	{
		JPanel labels = new JPanel(new GridLayout(1, 3));
		_editPhysicsProxy = new JCheckBox("Physics Proxy");
		labels.add(_editPhysicsProxy);
		_editPhysicsProxy.addActionListener(this);
		
		JPanel entries = new JPanel(new GridLayout(1, 3));
		_physicsProxy = new JComboBox();
		_physicsProxy.setEditable(false);
		_physicsProxy.setEnabled(false);
		entries.add(_physicsProxy);
		
		JPanel buttons = new JPanel();
		_okButton = new JButton("Okay");
		_okButton.addActionListener(this);
		buttons.add(_okButton);
		
		_cancelButton = new JButton("Cancel");
		_cancelButton.addActionListener(this);
		buttons.add(_cancelButton);
		getContentPane().setLayout(new BorderLayout());
		getContentPane().add(labels, BorderLayout.WEST);
		getContentPane().add(entries, BorderLayout.CENTER);
		getContentPane().add(buttons, BorderLayout.SOUTH);
	}

	public void actionPerformed(ActionEvent actEvt) 
	{
		if (actEvt.getSource() == _cancelButton)
		{
			setVisible(false);
		}
		else if (actEvt.getSource() == _okButton)
		{
			for (SceneNode curNode : mNodes) 
			{
				if (_editPhysicsProxy.isSelected())
				{
					curNode.setPhysicsProxy((String)_physicsProxy.getSelectedItem());
				}
			}
			setVisible(false);
		}
		else if (actEvt.getSource() == _editPhysicsProxy)
		{
			_physicsProxy.setEnabled(_editPhysicsProxy.isSelected());
		}
	}
	
	
}
