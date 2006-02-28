package meshhandle;

import java.io.File;

import javax.swing.JFileChooser;
import javax.swing.filechooser.FileFilter;

public class MeshChooser extends JFileChooser {

	public MeshChooser() {
		this.setDialogType(JFileChooser.CUSTOM_DIALOG);

		this.setAcceptAllFileFilterUsed(false);
		this.setFileFilter(new FileFilter() {

			public String getDescription() {
				return ".mesh";
			}

			public boolean accept(File f) {
				return (f.getName().endsWith(".mesh") || f.isDirectory());
			}
		});

		this.setControlButtonsAreShown(false);

	}

}