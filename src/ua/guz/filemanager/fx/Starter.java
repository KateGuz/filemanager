package ua.guz.filemanager.fx;

import javafx.application.Application;
import javafx.beans.property.BooleanProperty;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.scene.Scene;
import javafx.scene.control.TreeItem;
import javafx.scene.control.TreeView;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;
import ua.guz.filemanager.entity.FileSystemItem;
import ua.guz.filemanager.service.FileSystemService;

public class Starter extends Application {

    @Override
    public void start(Stage primaryStage) throws Exception {
        primaryStage.setTitle("File Manager");
        ImageView homeView = new ImageView(new Image(getClass().getResourceAsStream("/home_icon.png")));
        homeView.setFitHeight(15);
        homeView.setFitWidth(15);
        TreeView<String> tree = new TreeView<>();
        String rootPath = "/";
        TreeItem<String> rootItem = new TreeItem<>(rootPath, homeView);
        rootItem.setExpanded(true);
        buildTree(rootItem);
        tree.setRoot(rootItem);

        StackPane treePane = new StackPane();
        treePane.getChildren().add(tree);

        BorderPane root = new BorderPane();
        root.setLeft(treePane);
        primaryStage.setScene(new Scene(root, 600, 600));
        primaryStage.show();
    }


    public static void main(String[] args) {
        launch(args);
    }

    private String getPath(TreeItem<String> item) {
        String rootPath = "/";
        StringBuilder path = new StringBuilder(item.getValue());
        if (!rootPath.equals(item.getValue())) {
            TreeItem<String> temp = item.getParent();
            while (!rootPath.equals(temp.getValue())) {
                path.insert(0, "/");
                path.insert(0, temp.getValue());
                temp = temp.getParent();
            }
            path.insert(0, rootPath);
        }
        return path.toString();
    }

    private void buildTree(TreeItem<String> rootItem) {
        String rootPath = getPath(rootItem);
        FileSystemItem[] array = FileSystemService.getContent(rootPath);
        if(rootItem.getChildren().size() != 0){
            rootItem.getChildren().remove(0);
        }
        for (FileSystemItem fileSystemItem : array) {
            TreeItem<String> item;

            if ("dir".equals(fileSystemItem.getType())) {
                ImageView folderView = new ImageView(new Image(getClass().getResourceAsStream("/folder_icon.png")));
                folderView.setFitHeight(15);
                folderView.setFitWidth(13);
                item = new TreeItem<>(fileSystemItem.getName(), folderView);
                rootItem.getChildren().add(item);
                FileSystemItem[] content = FileSystemService.getContent(getPath(item));
                for (FileSystemItem systemItem : content) {
                    if ("dir".equals(systemItem.getType())) {
                        item.getChildren().add(new TreeItem<>());
                        break;
                    }
                }
                item.expandedProperty().addListener(new ChangeListener<Boolean>() {
                    @Override
                    public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue, Boolean newValue) {
                        BooleanProperty bb = (BooleanProperty) observable;
                        TreeItem<String> item = (TreeItem<String>) bb.getBean();
                        buildTree(item);
                    }
                });
            }
        }
    }
}
