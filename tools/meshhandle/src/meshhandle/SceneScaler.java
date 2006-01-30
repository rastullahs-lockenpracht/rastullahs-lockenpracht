package meshhandle;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.RandomAccessFile;

import javax.xml.parsers.ParserConfigurationException;

import meshhandle.scene.Scene;
import meshhandle.xml.SceneLoader;

import org.xml.sax.SAXException;

public class SceneScaler {

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

            System.out.print("Input-Scene  :");
            inputFile = reader.readLine();
            System.out.print("Faktor          :");
            factor = Float.parseFloat(reader.readLine());
            System.out.print("Output-Scene :");
            outputFile = reader.readLine();
        } else {
            inputFile = args[0];
            factor = Float.parseFloat(args[1]);
            outputFile = args[2];
        }

        System.out.println("processing " + inputFile);
        Scene scene = SceneLoader.readScene(inputFile);
        scene.scale(factor);
        RandomAccessFile outFile = new RandomAccessFile(outputFile, "rw");
        outFile.writeBytes(scene.toXML());

    }
}
