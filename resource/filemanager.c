#include "ua_guz_filemanager_service_FileSystemService.h"
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

struct FS_item {
    char name[256];
    char type[10];
};

JNIEXPORT jint JNICALL Java_ua_guz_filemanager_service_NativeService_countFiles (JNIEnv * env, jclass myClass, jstring path) {
    const char *nativeString = (*env)->GetStringUTFChars(env, path, 0);
    int result = listfile(nativeString);
    return result;
}

JNIEXPORT jint JNICALL Java_ua_guz_filemanager_service_NativeService_countDirectories (JNIEnv * env, jclass myClass, jstring path) {
    const char *nativeString = (*env)->GetStringUTFChars(env, path, 0);
    int result = listdir(nativeString);
    return result;
}

JNIEXPORT jobjectArray JNICALL Java_ua_guz_filemanager_service_FileSystemService_getContent (JNIEnv * env, jclass myClass, jstring path){
    const char *nativeString = (*env)->GetStringUTFChars(env, path, 0);
    int count = getFSItemsCount(nativeString);
    struct FS_item items[count];
    get_content(nativeString, items);
    jclass cls = (*env)->FindClass(env, "ua/guz/filemanager/entity/FileSystemItem");
    jvalue args[2];
    jobject object;
    jobjectArray array = (jobjectArray)((*env)->NewObjectArray(env, count, cls, NULL));
    jmethodID constructor = (*env)->GetMethodID(env, cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");
    for(int i = 0; i < count; i++){
        args[0].l = (*env)->NewStringUTF(env, items[i].type);
        args[1].l = (*env)->NewStringUTF(env, items[i].name);
        object = (*env)->NewObjectA(env, cls, constructor, args);
        (*env)->SetObjectArrayElement(env,array,i,object);
    }
    return array;
}

int listdir(const char *path)
{
    DIR *dir;
    struct dirent *entry;
    int count = 0;
    if (!(dir = opendir(path)))
        return 0;
    if (!(entry = readdir(dir)))
        return 0;
    do {
        if (entry->d_type == DT_DIR) {
            char subpath[1024];
            int len = snprintf(subpath, sizeof(subpath)-1, "%s/%s", path, entry->d_name);
            subpath[len] = 0;
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            printf("%s\n", entry->d_name);
            count++;
            count += listdir(subpath);
        }
    } while (entry = readdir(dir));
    closedir(dir);
    return count;
}

int listfile(const char *path)
{
    DIR *dir;
    struct dirent *entry;
    int count = 0;
    if (!(dir = opendir(path)))
        return 0;
    if (!(entry = readdir(dir)))
        return 0;
    do {
        if (entry->d_type == DT_DIR) {
            char subpath[1024];
            int len = snprintf(subpath, sizeof(subpath)-1, "%s/%s", path, entry->d_name);
            subpath[len] = 0;
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            count += listfile(subpath);
        } else {
           printf("%s\n", entry->d_name);
           count++;
        }
    } while (entry = readdir(dir));
    closedir(dir);
    return count;
}

int getFSItemsCount(const char *path){
    DIR *dir;
    struct dirent *entry;
    int count = 0;
    if (!(dir = opendir(path)))
        return NULL;
    if (!(entry = readdir(dir)))
        return NULL;
    do {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        count ++;
    } while (entry = readdir(dir));
    return count;
}

void get_content(const char *path, struct FS_item items[])
{
    int count = getFSItemsCount(path);
    DIR *dir;
    struct dirent *entry;
    if (!(dir = opendir(path)))
        return NULL;
    if (!(entry = readdir(dir)))
        return NULL;
    int i = 0;
    do {
        struct FS_item item;
        strcpy(item.name, entry->d_name);
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            strcpy(item.type, "dir");

        } else {
            strcpy(item.type, "file");
        }
        items[i++] = item;
    } while (entry = readdir(dir));
    closedir(dir);
}