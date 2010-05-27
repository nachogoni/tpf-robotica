import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.image.BufferedImage;

import javax.swing.JPanel;

public class ImagePanel extends JPanel {
    /**
	 * 
	 */
	private static final long serialVersionUID = 3752562361154053730L;
	BufferedImage image;

    public ImagePanel(BufferedImage image) {
        this.image = image;
        setOpaque(true);
    }

    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        // Draw image centered.
        int x = (getWidth() - image.getWidth())/2;
        int y = (getHeight() - image.getHeight())/2;
        g.drawImage(image, x, y, this);
        this.setPreferredSize(new Dimension(image.getWidth(),image.getHeight()));
    }

}