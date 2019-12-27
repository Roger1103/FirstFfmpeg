package com.aiot.device.firstffmpeg;

public class FFmpegUtil {

    static {
        System.loadLibrary("native-lib");
    }
    public static native void init();

    public static native void open(String url);

    public static native void read();

}
