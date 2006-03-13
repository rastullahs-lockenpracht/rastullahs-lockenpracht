package meshhandle;

import javax.swing.JFrame;

public class StartKlasse {

    /**
     * @param args
     */
    public static void main(String[] args) {
    	JFrame frame = null;
    	try {
        	frame = new MeshHandlerWindow();    		
    		frame.setVisible(true);
    	} catch(Exception e) {
    		e.printStackTrace();
    		if (frame != null) {
    			frame.setVisible(false);
    			System.exit(0);
    		}
    	}
    	
    }
}
