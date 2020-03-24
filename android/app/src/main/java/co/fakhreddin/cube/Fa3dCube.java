package co.fakhreddin.cube;

public class Fa3dCube {

    static {
        System.loadLibrary("3d-cube");
    }

    /**
     * @param physicalDeviceWidth the current view width
     * @param physicalDeviceHeight the current view height
     */
    public static native void init(int physicalDeviceWidth, int physicalDeviceHeight,String applicationAbsolutePath);
    public static native void step(long deltaTime);
}