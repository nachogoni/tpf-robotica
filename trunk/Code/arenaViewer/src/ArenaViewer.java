import java.awt.Color;
import java.awt.Dimension;
import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.JFrame;
import javax.swing.JScrollPane;


public class ArenaViewer {

	protected static final int SCALE = 3;
	public static void main(String[] args) throws IOException {
		File worldInfo = new File("../controllers/Tcleaner/worldInfo.cfg");
		final Dimension d = getArenaDimension(worldInfo);
		
		final BufferedImage image = new BufferedImage(d.height*SCALE, d.width*SCALE, BufferedImage.TYPE_INT_RGB);
		
		final ImagePanel contentPane = new ImagePanel(image);
		final JScrollPane jsp = new JScrollPane(contentPane);
		jsp.setVisible(true);
		final JFrame f = new JFrame();
		f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		f.setContentPane(jsp);
		f.setTitle("Arena Viewer");
		f.setSize(image.getWidth()+40,image.getHeight()+40);
		f.setLocation(200,200);
		f.setVisible(true);

		File changesFile = new File("../controllers/Tcleaner/changes.tmp");
		FileWatcher fw = new FileWatcher(changesFile) {
			int iter = 0;
			@Override
			protected void onChange(File file) {
				BufferedReader br;
				try {
					br = new BufferedReader(new FileReader(file));
					String line = br.readLine();
					if ( line != null && !line.isEmpty() ){
						Matcher m = cellPattern.matcher(line);
						iter++;
						while( m.find() ){
//							Color color = new Color(255-((iter*5)%255),255-((iter*10)%255),255-((iter*20)%255));
							Color color = new Color(255,255,255);
							setPixelColor(image,d.height-1-Integer.valueOf(m.group(2)), Integer.valueOf(m.group(1)), color.getRGB());
						}
						contentPane.repaint();
						jsp.repaint();
						f.repaint();
					}
				} catch (FileNotFoundException e) {
					e.printStackTrace();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
			
			private void setPixelColor(BufferedImage image, int z, int x, int rgb) {
				for(int k = 0; k < SCALE ; k++){
					for(int l = 0; l < SCALE ; l++){
//						System.out.println("To be set i: "+z+" , j:"+x+" , k:"+k+" , l:"+l+" , ik:"+(z*SCALE+k)+" , jl:"+(x*SCALE+l));
						image.setRGB(z*SCALE+k,x*SCALE+l,rgb);
					}
				}
			}
		};
		new Thread(fw).start();
	}

	private static Dimension getArenaDimension(File worldInfo) throws IOException {
		BufferedReader br = new BufferedReader(new FileReader(worldInfo));
		String readLine = br.readLine();
		String lastLine = "";
		while( !readLine.trim().isEmpty() ){
			lastLine = readLine;
			readLine = br.readLine();
		}
		Matcher m = dimensionPattern.matcher(lastLine);
		m.find();
		Dimension d = new Dimension(Integer.valueOf(m.group(1)), Integer.valueOf(m.group(2)));
		return d;
	}
	
	static Pattern dimensionPattern = Pattern.compile("(\\d+):(\\d+)");
	static Pattern cellPattern = Pattern.compile("\\((\\d+),(\\d+)\\)");
}
