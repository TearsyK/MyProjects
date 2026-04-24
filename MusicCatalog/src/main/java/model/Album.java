package model;

import jakarta.persistence.*;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

/**
 * Represents an album in the music catalog.
 * 
 * @author Kacper Kostanek
 * @version 3.0
 */
@Entity
@Table(name = "ALBUM")
@Data
@NoArgsConstructor
@AllArgsConstructor
public class Album {
    
    /**
     * Title of the album.
     */
    private String title;
    
    /** 
     * Identifier for the album.
     */
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private int id;
    
    /**
     * List of songs in the album.
     */
    @OneToMany(mappedBy = "album", cascade = CascadeType.ALL)
    private List<Song> songList;
    
    /**
     * Constructor with title and ID.
     * Initializes empty song list.
     * 
     * @param title album title
     * @param id    album ID
     */
    public Album(String title, int id) {
        this.title = title;
        this.id = id;
        this.songList = new ArrayList<>();  // Initialize with empty generic list
    }
    
    /**
 * Returns list of songs in album.
 * @return List of Song objects
 */
public List<Song> getSongList() {
    return new ArrayList<>(songList); // songs to Twoje pole z piosenkami
}
    
    /**
     * Adds a song to the album.
     * 
     * @param song song to add
     * @throws MusicCatalogException if song is invalid
     */
    public void addSongToList(Song song) throws MusicCatalogException {
        if (song == null) {
            throw new MusicCatalogException("Song cannot be null");
        }
        
        // Check if song already exists (Stream API)
        boolean exists = songList.stream()
            .anyMatch(s -> s.getSongId() == song.getSongId());
        
        if (exists) {
            throw new MusicCatalogException("Song with ID " + song.getSongId() + " already exists");
        }
        
        songList.add(song);
    }
    
    /**
     * Removes a song from the album by ID.
     * CHANGED IN EX3: Uses Stream API + Lambda for filtering.
     * 
     * @param songId ID of song to remove
     * @throws MusicCatalogException if song not found
     */
    public void removeSongById(int songId) throws MusicCatalogException {
        Song songToRemove = songList.stream()
            .filter(s -> s.getSongId() == songId)
            .findFirst()
            .orElse(null);
        
        if (songToRemove == null) {
            throw new MusicCatalogException("Song with ID " + songId + " not found");
        }
        
        songList.remove(songToRemove);
    }
    
    /**
     * Finds a song by ID using Stream API.
     * CHANGED IN EX3: Uses Stream API instead of for loop.
     * 
     * @param songId ID to search for
     * @return Song if found, null otherwise
     */
    public Song findSongById(int songId) {
        return songList.stream()
            .filter(s -> s.getSongId() == songId)
            .findFirst()
            .orElse(null);
    }
       
    /**
     * Searches songs by title substring using Stream API.
     * CHANGED IN EX3: New method using Stream API + Lambda.
     * 
     * @param titlePart substring to search for
     * @return filtered list of songs
     */
    public List<Song> searchSongsByTitle(String titlePart) {
        return songList.stream()
            .filter(s -> s.getSongTitle().toLowerCase().contains(titlePart.toLowerCase()))
            .collect(Collectors.toList());
    }
    
    /**
     * Updates song title.
     * 
     * @param songId   song ID to update
     * @param newTitle new title
     * @throws MusicCatalogException if song not found
     */
    public void updateSongTitle(int songId, String newTitle) throws MusicCatalogException {
        Song song = findSongById(songId);
        if (song == null) {
            throw new MusicCatalogException("Song with ID " + songId + " not found");
        }
        song.setSongTitle(newTitle);
    }
    
    /**
     * Gets number of songs in album.
     * 
     * @return song count
     */
    public int getSongCount() {
        return (int) songList.stream().count();
    }
    
    /**
     * Validates album data.
     * 
     * @throws MusicCatalogException if invalid
     */
    public void validate() throws MusicCatalogException {
        if (title == null || title.trim().isEmpty()) {
            throw new MusicCatalogException("Album title cannot be empty");
        }
        if (id <= 0) {
            throw new MusicCatalogException("Album ID must be positive");
        }
    }
    
    /**
     * Converts Album to AlbumDTO record.
     * 
     * @return AlbumDTO with album data
     */
    public AlbumDTO toDTO() {
        return new AlbumDTO(this.id, this.title);
    }
    
    /**
     * Returns string representation.
     * 
     * @return formatted string with album info
     */
    @Override
    public String toString() {
        return String.format("Album{id=%d, title='%s', songs=%d}", id, title, getSongCount());
    }

    /**
 * Adds song to album (used by AppContextListener).
 * @param song Song to add
 */
public void addSongToAlbum(Song song) {
    try {
        addSongToList(song);  // Używa Twojej istniejącej metody
    } catch (MusicCatalogException e) {
        System.err.println("Error adding song: " + e.getMessage());
    }
}

}
