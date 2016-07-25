package ua.guz.filemanager.entity;

public class FileSystemItem {
    private String type;
    private String name;

    public FileSystemItem(String type, String name) {
        this.type = type;
        this.name = name;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getType() {

        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    @Override
    public String toString() {
        return "FileSystemItem{" +
                "type='" + type + '\'' +
                ", name='" + name + '\'' +
                '}';
    }
}
