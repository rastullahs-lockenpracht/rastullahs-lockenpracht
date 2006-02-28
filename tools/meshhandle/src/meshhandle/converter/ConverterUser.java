package meshhandle.converter;

import java.io.IOException;

public class ConverterUser {
public static boolean fromMeshToXML(String converterpath, String filepath){
	String[] args = { converterpath,
			filepath, filepath + ".xml" };
	int exitVal = 1;
	// any error???
	try {
		Process proc = Runtime.getRuntime().exec(args);

		// any error message?
		StreamGobbler errorGobbler = new StreamGobbler(proc
				.getErrorStream(), "Add. inf.:");

		// any output?
		StreamGobbler outputGobbler = new StreamGobbler(proc
				.getInputStream(), "Output");

		// kick them off
		errorGobbler.start();
		outputGobbler.start();

		exitVal = proc.waitFor();

	} catch (IOException e) {
		System.out.println("Fehler beim Aufruf des Konverters!");
		e.printStackTrace();
	} catch (InterruptedException e) {
		System.out.println("Fehler bei der Ausführung des Konverters!");
		e.printStackTrace();
	}
	return (exitVal == 0);
}
public static boolean fromXMLToMesh(String converterpath, String xmlpath){
	int exitVal=1;
	String[] args = { converterpath,
			xmlpath, xmlpath.substring(0, xmlpath.length()-3) };
	try {
		Process proc = Runtime.getRuntime().exec(args);
		exitVal = proc.waitFor();
	} catch (IOException e) {
		System.out.println("Fehler beim Aufruf des Konverters!");
		e.printStackTrace();
	} catch (InterruptedException e) {
		System.out.println("Fehler bei der Ausführung des Konverters!");
		e.printStackTrace();
	}
	
	return (exitVal==0);

}
}