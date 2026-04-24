/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package model;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.List;

/**
 * Handles file operations for music catalog (JSON serialization/deserialization).
 * 
 * @author Kacper Kostanek
 * @version 3.0
 */
public class FileManager {
    
    private static final Gson gson = new GsonBuilder().setPrettyPrinting().create();
    
    /**
     * Saves a collection to a JSON file.
     * 
     * @param collection collection to save
     * @param filePath   file path to save to
     * @throws Exception if save fails
     */
    public static void saveCollectionToFile(Collection collection, String filePath) throws Exception {
        if (collection == null) {
            throw new IllegalArgumentException("Collection cannot be null");
        }
        
        try (FileWriter writer = new FileWriter(filePath)) {
            // CHANGED IN EX3: Directly use List from collection
            List<Album> albums = collection.getAlbumList();
            String json = gson.toJson(albums);
            writer.write(json);
        }
    }
    
    /**
     * Loads a collection from a JSON file.
     * 
     * @param filePath file path to load from
     * @return loaded Collection
     * @throws Exception if load fails
     */
    public static Collection loadCollectionFromFile(String filePath) throws Exception {
        try (FileReader reader = new FileReader(filePath)) {
            Album[] albumsArray = gson.fromJson(reader, Album[].class);
            
            // Convert array to List for generic collection
            List<Album> albumList = new ArrayList<>();
            if (albumsArray != null) {
                for (Album album : albumsArray) {
                    // Ensure songList is initialized
                    if (album.getSongList() == null) {
                        album.setSongList(new ArrayList<>());
                    }
                    albumList.add(album);
                }
            }
            
            Collection collection = new Collection("Loaded Collection");
            collection.setAlbumList(albumList);
            return collection;
        }
    }
    
    /**
     * Exports collection as JSON string.
     * CHANGED IN EX3: Works with List.
     * 
     * @param collection collection to export
     * @return JSON string
     */
    public static String exportToJson(Collection collection) {
        if (collection == null) {
            return "{}";
        }
        // CHANGED IN EX3: Use List directly
        return gson.toJson(collection.getAlbumList());
    }
    
    /**
     * Imports collection from JSON string.
     * CHANGED IN EX3: Works with List.
     * 
     * @param jsonString JSON string to import
     * @return imported Collection
     */
    public static Collection importFromJson(String jsonString) {
        if (jsonString == null || jsonString.isEmpty()) {
            return new Collection("Imported Collection");
        }
        
        Album[] albumsArray = gson.fromJson(jsonString, Album[].class);
        
        // CHANGED IN EX3: Convert to List
        List<Album> albumList = new ArrayList<>();
        if (albumsArray != null) {
            for (Album album : albumsArray) {
                if (album.getSongList() == null) {
                    album.setSongList(new ArrayList<>());
                }
                albumList.add(album);
            }
        }
        
        Collection collection = new Collection("Imported Collection");
        collection.setAlbumList(albumList);
        return collection;
    }
}
