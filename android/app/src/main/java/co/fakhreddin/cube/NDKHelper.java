package co.fakhreddin.cube;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

import java.io.File;
import java.io.FileInputStream;
import java.nio.ByteBuffer;

public class NDKHelper {
    private static final String tagName = "NDKHelper";
    public static class TextureInformation{
        boolean success;
        boolean alphaChannel;
        int originalWidth;
        int originalHeight;
        Object image;
    }
    public static Object loadImage(String path) {
        Bitmap bitmap = null;
        TextureInformation object = new TextureInformation();
        object.success = false;

        try {
            String str = path;
            if (!path.startsWith("/")) {
                str = "/" + path;
            }

            File file = new File(co.fakhreddin.cube.MainActivity.getInstance().getExternalFilesDir(null), str);
            if (file.canRead()) {
                bitmap = BitmapFactory.decodeStream(new FileInputStream(file));
            } else {
                bitmap = BitmapFactory.decodeStream(
                        co.fakhreddin.cube.MainActivity.getInstance()
                                .getResources()
                                .getAssets()
                                .open(path)
                );
            }
        } catch (Exception e) {
            Log.w(NDKHelper.tagName, "Couldn't load a file:" + path);
            return object;
        }

        object.alphaChannel = bitmap.hasAlpha();
        object.originalWidth = bitmap.getWidth();
        object.originalHeight = bitmap.getHeight();
        int numberOfChannels = 4;//bitmap.hasAlpha() ? 4 : 3;
        int iBytes = bitmap.getWidth() * bitmap.getHeight() * numberOfChannels;
        ByteBuffer buffer = ByteBuffer.allocateDirect(iBytes);
        bitmap.copyPixelsToBuffer(buffer);
        object.image = buffer;
        object.success = true;

        return object;
    }
}
