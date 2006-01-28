package meshhandle;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Properties;

import javax.swing.JOptionPane;

public class PropertyManager {
    Properties pfade = new Properties();

    public File[] propertiesLaden() {
        File propFile = new File(System.getProperty("user.dir"), "properties");
        try {
            FileInputStream propStream = new FileInputStream(propFile);
            pfade.load(propStream);
        } catch (FileNotFoundException e) {
            JOptionPane.showMessageDialog(null,
                    "Sie haben kein Property-File.",
                    "Fehler beim Laden der Properties",
                    JOptionPane.ERROR_MESSAGE);
            e.printStackTrace();
        } catch (IOException e) {
            JOptionPane.showMessageDialog(null,
                    "Keine Ahnung, was das schon wieder soll.",
                    "Fehler beim Laden der Properties",
                    JOptionPane.ERROR_MESSAGE);
            e.printStackTrace();
        }
        File rl = new File(pfade.getProperty("RL-PFAD"));
        if (!rl.isDirectory()) {
            JOptionPane
                    .showMessageDialog(
                            null,
                            "Geben Sie in den Properties ihr korrektes RL-Verzeichnis an.",
                            "Fehler beim Laden der Properties",
                            JOptionPane.ERROR_MESSAGE);
        }
        File ogre = new File(pfade.getProperty("CONVERTER"));
        if (!rl.isDirectory()) {
            JOptionPane
                    .showMessageDialog(
                            null,
                            "Geben Sie in den Properties ihr Ogre-Tools-Verzeichnis korrekt an.",
                            "Fehler beim Laden der Properties",
                            JOptionPane.ERROR_MESSAGE);
        }
        File[] files = { rl, ogre };
        return files;
    }
}
