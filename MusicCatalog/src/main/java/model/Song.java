package model;

import jakarta.persistence.*;
import java.util.Objects;
import lombok.*;

/**
 * Represents a single song in the music catalog.
 * Contains song identification and title information.
 *
 * @author Kacper Kostanek
 * @version 1.1 (compatibility with new GUI project)
 */
@Entity
@NoArgsConstructor
@Table(name = "SONG")
public class Song {

    /**
     * Unique identifier for the song.
     */
    @Getter @Setter
    @Id
    private int songId;
    
    /**
     * Relation to Album table
     */
    @ManyToOne
    @JoinColumn(name = "album_id")
    private Album album;
    
    
    /**
     * Title of the song.
     */
    @Getter @Setter
    private String songTitle;

    /**
     * Constructs a new Song with the specified ID and title.
     *
     * @param id the unique identifier for the song
     * @param title the title of the song
     * @throws MusicCatalogException if the ID is negative or title is null/empty
     */
    public Song(int id, String title) throws MusicCatalogException {
        if (id < 0) {
            throw new MusicCatalogException("Song ID cannot be negative: " + id);
        }  // ← TERAZ JEST NAWIAS!

        if (title == null || title.trim().isEmpty()) {
            throw new MusicCatalogException("Song title cannot be null or empty");
        }  // ← TERAZ JEST NAWIAS!

        this.songId = id;
        this.songTitle = title;
    }

    
    
    /**
     * Constructs a new Song from a DTO.
     *
     * @param songDTO the data transfer object containing song data
     * @throws MusicCatalogException if the ID is negative or title is null/empty
     */
    public Song(SongDTO songDTO) throws MusicCatalogException {
        if (songDTO.songId() < 0) {
            throw new MusicCatalogException("Song ID cannot be negative: " + songDTO.songId());
        }

        if (songDTO.songTitle() == null || songDTO.songTitle().trim().isEmpty()) {
            throw new MusicCatalogException("Song title cannot be null or empty");
        }

        this.songId = songDTO.songId();
        this.songTitle = songDTO.songTitle();
    }

    /**
     * Generates a hash code for this song based on its ID and title.
     *
     * @return the hash code
     */
    @Override
    public int hashCode() {
        int hash = 7;
        hash = 89 * hash + this.songId;
        hash = 89 * hash + Objects.hashCode(this.songTitle);
        return hash;
    }

    /**
     * Compares this song to another object for equality.
     * Two songs are equal if they have the same ID and title.
     *
     * @param obj the object to compare with
     * @return true if the objects are equal, false otherwise
     */
    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }

        if (obj == null) {
            return false;
        }

        if (getClass() != obj.getClass()) {
            return false;
        }

        final Song other = (Song) obj;
        if (this.songId != other.songId) {
            return false;
        }

        return Objects.equals(this.songTitle, other.songTitle);
    }

    /**
     * Returns a string representation of this song.
     *
     * @return a string containing the song ID and title
     */
    @Override
    public String toString() {
        return "Song{" + "songId=" + songId + ", songTitle='" + songTitle + "'}";
    }

}
