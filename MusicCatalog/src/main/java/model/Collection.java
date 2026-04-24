package model;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

/**
 * Represents a collection of albums in the music catalog.
 * 
 * @author Kacper Kostanek
 * @version 3.0
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
public class Collection {
    
    /**
     * Name of the collection.
     */
    private String collectionName;
    
    /**
     * List of albums in the collection.
     * CHANGED IN EX3: Now uses List<Album> (generic collection) instead of array.
     */
    private List<Album> albumList;
    
    /**
     * Constructor with collection name.
     * Initializes empty album list.
     * 
     * @param collectionName name of the collection
     */
    public Collection(String collectionName) {
        this.collectionName = collectionName;
        this.albumList = new ArrayList<>();  // Initialize with empty generic list
    }
    
    /**
     * Adds an album to the collection.
     * 
     * @param album album to add
     * @throws MusicCatalogException if album is invalid or already exists
     */
    public void addAlbumToCollection(Album album) throws MusicCatalogException {
        if (album == null) {
            throw new MusicCatalogException("Album cannot be null");
        }
        album.validate();
        
        // Check if album already exists using Stream API + Lambda
        boolean exists = albumList.stream()
            .anyMatch(a -> a.getId() == album.getId());
        
        if (exists) {
            throw new MusicCatalogException("Album with ID " + album.getId() + " already exists");
        }
        
        albumList.add(album);
    }
    
    /**
     * Removes an album from the collection by ID.
     * CHANGED IN EX3: Uses Stream API + Lambda for filtering.
     * 
     * @param albumId ID of album to remove
     * @throws MusicCatalogException if album not found
     */
    public void removeAlbumFromCollection(int albumId) throws MusicCatalogException {
        Album albumToRemove = albumList.stream()
            .filter(a -> a.getId() == albumId)
            .findFirst()
            .orElse(null);
        
        if (albumToRemove == null) {
            throw new MusicCatalogException("Album with ID " + albumId + " not found");
        }
        
        albumList.remove(albumToRemove);
    }
    
    /**
     * Finds an album by ID using Stream API.
     * CHANGED IN EX3: Uses Stream API + Lambda instead of for loop.
     * This is PRIMARY requirement - demonstrating Stream API usage.
     * 
     * @param albumId ID to search for
     * @return Album if found, null otherwise
     */
    public Album findAlbumById(int albumId) {
        return albumList.stream()
            .filter(a -> a.getId() == albumId)
            .findFirst()
            .orElse(null);
    }
    
    /**
     * Gets all albums as DTOs using Stream API.
     * CHANGED IN EX3: Uses Stream API + map() + collect().
     * 
     * @return List of AlbumDTOs
     */
    public List<AlbumDTO> getAlbumDTOs() {
        return albumList.stream()
            .map(Album::toDTO)
            .collect(Collectors.toList());
    }
    
    /**
     * Searches albums by title using Stream API + Lambda.
     * CHANGED IN EX3: NEW METHOD - demonstrates Stream API filtering.
     * 
     * @param titlePart substring to search for
     * @return List of matching albums
     */
    public List<Album> searchAlbumsByTitle(String titlePart) {
        return albumList.stream()
            .filter(a -> a.getTitle().toLowerCase().contains(titlePart.toLowerCase()))
            .collect(Collectors.toList());
    }
    
    /**
     * Gets albums sorted by title using Stream API + Lambda.
     * CHANGED IN EX3: NEW METHOD - demonstrates Stream API sorting.
     * 
     * @return sorted list of albums
     */
    public List<Album> getAlbumsSortedByTitle() {
        return albumList.stream()
            .sorted((a1, a2) -> a1.getTitle().compareTo(a2.getTitle()))
            .collect(Collectors.toList());
    }
    
    /**
     * Gets albums with more than N songs using Stream API + Lambda.
     * CHANGED IN EX3: NEW METHOD - demonstrates Stream API filtering.
     * 
     * @param minSongs minimum number of songs
     * @return filtered list of albums
     */
    public List<Album> getAlbumsWithMoreThanSongs(int minSongs) {
        return albumList.stream()
            .filter(a -> a.getSongCount() >= minSongs)
            .collect(Collectors.toList());
    }
    
    /**
     * Gets all songs from all albums using Stream API + Lambda.
     * CHANGED IN EX3: NEW METHOD - demonstrates Stream API flatMap().
     * 
     * @return flattened list of all songs from all albums
     */
    public List<Song> getAllSongs() {
        return albumList.stream()
            .flatMap(a -> a.getSongList().stream())
            .collect(Collectors.toList());
    }
    
    /**
     * Counts total songs across all albums.
     * CHANGED IN EX3: Uses Stream API count() instead of for loop.
     * 
     * @return total song count
     */
    public int getTotalSongCount() {
        return (int) albumList.stream()
            .flatMap(a -> a.getSongList().stream())
            .count();
    }
    
    /**
     * Searches for a specific song across all albums.
     * CHANGED IN EX3: Uses Stream API + Lambda.
     * 
     * @param songTitle song title to search for
     * @return List of albums containing this song
     */
    public List<Album> findAlbumsContainingSong(String songTitle) {
        return albumList.stream()
            .filter(a -> a.getSongList().stream()
                .anyMatch(s -> s.getSongTitle().equalsIgnoreCase(songTitle)))
            .collect(Collectors.toList());
    }
    
    /**
     * Updates album title.
     * 
     * @param albumId  album ID to update
     * @param newTitle new title
     * @throws MusicCatalogException if album not found
     */
    public void updateAlbumTitle(int albumId, String newTitle) throws MusicCatalogException {
        Album album = findAlbumById(albumId);
        if (album == null) {
            throw new MusicCatalogException("Album with ID " + albumId + " not found");
        }
        album.setTitle(newTitle);
    }
    
    /**
     * Gets album count.
     * CHANGED IN EX3: Uses Stream API count().
     * 
     * @return number of albums
     */
    public int getAlbumCount() {
        return (int) albumList.stream().count();
    }
    
    /**
     * Validates collection data.
     * 
     * @throws MusicCatalogException if invalid
     */
    public void validate() throws MusicCatalogException {
        if (collectionName == null || collectionName.trim().isEmpty()) {
            throw new MusicCatalogException("Collection name cannot be empty");
        }
    }
    
    /**
     * Returns string representation.
     * 
     * @return formatted string with collection info
     */
    @Override
    public String toString() {
        return String.format("Collection{name='%s', albums=%d, totalSongs=%d}", 
            collectionName, getAlbumCount(), getTotalSongCount());
    }
}