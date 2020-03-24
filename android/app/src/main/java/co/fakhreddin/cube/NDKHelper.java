
package co.fakhreddin.cube;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.ByteBuffer;

public class NDKHelper {
    private static final String tagName = "3dCube-NDKHelper";
    public static class TextureInformation{
        boolean success;
        boolean alphaChannel;
        int originalWidth;
        int originalHeight;
        Object image;
    }
    public static class TextFileInformation{
        boolean success;
        Object content;
    }
    public static Object loadImage(String path) {
        Bitmap bitmap = null;
        TextureInformation object = new TextureInformation();
        object.success = false;

        try {
            bitmap = BitmapFactory.decodeStream(
                co.fakhreddin.cube.MainActivity.getInstance()
                .getResources()
                .getAssets()
                .open(path)
            );
        } catch (Exception e) {
            log(e.getMessage());
            log("Couldn't load file:" + path);
            return object;
        }

        object.alphaChannel = bitmap.hasAlpha();
        object.originalWidth = bitmap.getWidth();
        object.originalHeight = bitmap.getHeight();
        //Currently alpha is not working. My guess is that bitmap always have alpha
        int numberOfChannels = 4;
        int iBytes = bitmap.getWidth() * bitmap.getHeight() * numberOfChannels;
        ByteBuffer buffer = ByteBuffer.allocateDirect(iBytes);
        bitmap.copyPixelsToBuffer(buffer);
        object.image = buffer;
        object.success = true;

        return object;
    }
    public static Object loadText(String path){
        TextFileInformation textFileInformation = new TextFileInformation();
        textFileInformation.success = false;
        try {
            InputStream file = co.fakhreddin.cube.MainActivity.getInstance().getAssets().open(path);
            //Read text from file
            StringBuilder text = new StringBuilder();
            BufferedReader br = new BufferedReader(new InputStreamReader(file));
            String line;
            while ((line = br.readLine()) != null) {
                text.append(line);
                text.append('\n');
            }
            br.close();
            //We allocate byteBuffer directly to be accessible by c++ thread
            ByteBuffer buffer =  ByteBuffer.allocateDirect(text.toString().getBytes().length);
            buffer.put(text.toString().getBytes());
            textFileInformation.content = buffer;
        }catch (Exception exception){
            log(exception.getMessage());
            return textFileInformation;
        }
        textFileInformation.success = true;
        return textFileInformation;
    }
    public static void log(String log){
        Log.d(NDKHelper.tagName,log);
    }
}
