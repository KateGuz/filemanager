package ua.guz.filemanager.service;

import ua.guz.filemanager.entity.FileSystemItem;

public class FileSystemService {
    static {
        System.loadLibrary("filemanager");
    }

    public static native int countFiles(String path);

    public static native int countDirectories(String path);

    public static native FileSystemItem[] getContent(String path);
}
