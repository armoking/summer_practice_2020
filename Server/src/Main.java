import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.*;

import static java.lang.Thread.sleep;

public class Main {

    public static String PATH_TO_IMAGE = "C:\\Users\\User\\Desktop\\github\\summer_practice_2020\\photo.jpg";
    public static String PATH_TO_SOURCE = "C:\\Users\\User\\Desktop\\github\\summer_practice_2020\\source.txt";
    public static String PATH_TO_RESULT = "C:\\Users\\User\\Desktop\\github\\summer_practice_2020\\result.txt";
    public static String PATH_TO_EXECUTE = "C:\\Users\\User\\Desktop\\github\\summer_practice_2020\\Parser\\main.exe";

    public static BufferedImage ReadImage(String filename) {
        BufferedImage img = null;
        File f;
        for (String format : new String[]{"", ".png ", ".jpg ", ".jpeg ", ".bmp "}) {
            try {
                String fullPath = filename + format;
                f = new File(fullPath);
                return ImageIO.read(f);
            } catch (Exception ignored) {
            }
        }
        return null;
    }

    public static void main(String[] args) throws IOException, InterruptedException {
        BufferedImage image = ReadImage(PATH_TO_IMAGE);
        BufferedWriter out = new BufferedWriter(new FileWriter(PATH_TO_SOURCE));

        assert image != null;
        int w = image.getWidth();
        int h = image.getHeight();
        out.write(w + " " + h + "\n");
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int color = image.getRGB(x, y) & 0xffffff;
                out.write(color + " ");
            }
            out.write('\n');
        }
        out.close();

        Runtime runtime = Runtime.getRuntime();
        Process process = runtime.exec(PATH_TO_EXECUTE);
        while (process.isAlive()) {
            sleep(150);
        }

        BufferedReader in = new BufferedReader(new FileReader(PATH_TO_RESULT));



    }
}
