package meshhandle;

import java.io.BufferedReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;

import javax.xml.parsers.ParserConfigurationException;

import meshhandle.io.xml.MeshLoader;
import meshhandle.model.mesh.Mesh;

import org.xml.sax.SAXException;

public class MeshScaler {

    /**
     * @param args
     * @throws IOException
     * @throws ParserConfigurationException
     * @throws SAXException
     */
    public static void main(String[] args) throws IOException, SAXException,
            ParserConfigurationException {

        String inputFile;
        float factor;
        String outputFile;

        if (args.length < 3) {
            BufferedReader reader = new BufferedReader(new InputStreamReader(
                    System.in));

            System.out.print("Input-Mesh  :");
            inputFile = reader.readLine();
            System.out.print("Faktor      :");
            factor = Float.parseFloat(reader.readLine());
            System.out.print("Output-Mesh :");
            outputFile = reader.readLine();
        } else {
            inputFile = args[0];
            factor = Float.parseFloat(args[1]);
            outputFile = args[2];
        }

        System.out.println("processing " + inputFile);
        Mesh mesh = MeshLoader.readMesh(inputFile);
        mesh.scale(factor);
        FileWriter outFile = new FileWriter(outputFile, false);
        outFile.write(mesh.toXML());
        outFile.flush();
    }
}
