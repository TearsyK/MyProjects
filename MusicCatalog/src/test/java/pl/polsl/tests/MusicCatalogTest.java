/**
 * Unit tests for the model classes in the music catalog application.
 * Tests cover Song, Album, and Collection classes with various scenarios.
 * 
 * @author Kacper Kostanek
 * @version 1.0
 */
package model;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.DisplayName;
import static org.junit.jupiter.api.Assertions.*;
import java.util.List;

@DisplayName("Music Catalog Model Tests")
public class MusicCatalogTest {

    private Collection collection;
    private Album album1;
    private Album album2;
    private Song song1;
    private Song song2;

    /**
     * Set up test fixtures before each test.
     * Initializes collection, albums, and songs for testing.
     */
    @BeforeEach
    void setUp() {
        collection = new Collection("My Music Library");
        album1 = new Album("Pink Floyd - The Wall", 1);
        album2 = new Album("Led Zeppelin IV", 2);
        try {
            song1 = new Song(1, "In the Flesh?");
            song2 = new Song(2, "Comfortably Numb");
        } catch (MusicCatalogException e) {
            fail("Setup failed: " + e.getMessage());
        }
    }

    // ==================== Song Tests ====================

    /**
     * Test Song creation with valid parameters.
     */
    @Test
    @DisplayName("Create Song with valid data")
    void testCreateValidSong() throws MusicCatalogException {
        Song song = new Song(1, "Test Song");
        assertEquals(1, song.getSongId());
        assertEquals("Test Song", song.getSongTitle());
    }

    /**
     * Test Song creation with negative ID throws exception.
     */
    @Test
    @DisplayName("Song creation with negative ID should throw exception")
    void testCreateSongWithNegativeId() {
        assertThrows(MusicCatalogException.class, () -> {
            new Song(-1, "Invalid Song");
        });
    }

    /**
     * Test Song creation with null title throws exception.
     */
    @Test
    @DisplayName("Song creation with null title should throw exception")
    void testCreateSongWithNullTitle() {
        assertThrows(MusicCatalogException.class, () -> {
            new Song(1, null);
        });
    }

    /**
     * Test Song creation with empty title throws exception.
     */
    @Test
    @DisplayName("Song creation with empty title should throw exception")
    void testCreateSongWithEmptyTitle() {
        assertThrows(MusicCatalogException.class, () -> {
            new Song(1, "   ");
        });
    }

    /**
     * Test Song title update.
     */
    @Test
    @DisplayName("Update Song title")
    void testUpdateSongTitle() {
        song1.setSongTitle("Updated Title");
        assertEquals("Updated Title", song1.getSongTitle());
    }

    /**
     * Test Song equality based on ID and title.
     */
    @Test
    @DisplayName("Song equality comparison")
    void testSongEquality() throws MusicCatalogException {
        Song songA = new Song(1, "Same Song");
        Song songB = new Song(1, "Same Song");
        Song songC = new Song(2, "Different Song");
        
        assertEquals(songA, songB);
        assertNotEquals(songA, songC);
    }

    // ==================== Album Tests ====================

    /**
     * Test Album creation with valid parameters.
     */
    @Test
    @DisplayName("Create Album with valid data")
    void testCreateValidAlbum() {
        Album album = new Album("Test Album", 1);
        assertEquals("Test Album", album.getTitle());
        assertEquals(1, album.getId());
        assertEquals(0, album.getSongCount());
    }

    /**
     * Test Album creation with zero ID throws exception.
     */
    @Test
    @DisplayName("Album creation with zero ID should throw exception")
    void testCreateAlbumWithZeroId() {
        assertThrows(MusicCatalogException.class, () -> {
            Album album = new Album("Test", 0);
            album.validate();
        });
    }

    /**
     * Test Album creation with null title throws exception.
     */
    @Test
    @DisplayName("Album creation with null title should throw exception")
    void testCreateAlbumWithNullTitle() {
        assertThrows(MusicCatalogException.class, () -> {
            Album album = new Album(null, 1);
            album.validate();
        });
    }

    /**
     * Test adding Song to Album.
     */
    @Test
    @DisplayName("Add Song to Album")
    void testAddSongToAlbum() throws MusicCatalogException {
        album1.addSongToList(song1);
        assertEquals(1, album1.getSongCount());
        assertEquals(song1, album1.findSongById(1));
    }

    /**
     * Test adding duplicate Song to Album throws exception.
     */
    @Test
    @DisplayName("Adding duplicate Song should throw exception")
    void testAddDuplicateSong() throws MusicCatalogException {
        album1.addSongToList(song1);
        assertThrows(MusicCatalogException.class, () -> {
            album1.addSongToList(song1);
        });
    }

    /**
     * Test adding null Song to Album throws exception.
     */
    @Test
    @DisplayName("Adding null Song should throw exception")
    void testAddNullSong() {
        assertThrows(MusicCatalogException.class, () -> {
            album1.addSongToList(null);
        });
    }

    /**
     * Test removing Song from Album.
     */
    @Test
    @DisplayName("Remove Song from Album")
    void testRemoveSongFromAlbum() throws MusicCatalogException {
        album1.addSongToList(song1);
        album1.addSongToList(song2);
        
        album1.removeSongById(1);
        assertEquals(1, album1.getSongCount());
        assertNull(album1.findSongById(1));
        assertEquals(song2, album1.findSongById(2));
    }

    /**
     * Test removing non-existent Song from Album throws exception.
     */
    @Test
    @DisplayName("Removing non-existent Song should throw exception")
    void testRemoveNonExistentSong() throws MusicCatalogException {
        album1.addSongToList(song1);
        assertThrows(MusicCatalogException.class, () -> {
            album1.removeSongById(999);
        });
    }

    /**
     * Test finding Song by ID.
     */
    @Test
    @DisplayName("Find Song by ID")
    void testFindSongById() throws MusicCatalogException {
        album1.addSongToList(song1);
        album1.addSongToList(song2);
        
        assertEquals(song1, album1.findSongById(1));
        assertEquals(song2, album1.findSongById(2));
        assertNull(album1.findSongById(999));
    }

    /**
     * Test searching Songs by title substring.
     */
    @Test
    @DisplayName("Search Songs by title")
    void testSearchSongsByTitle() throws MusicCatalogException {
        album1.addSongToList(song1);
        album1.addSongToList(song2);
        
        List<Song> results = album1.searchSongsByTitle("Comfortably");
        assertEquals(1, results.size());
        assertEquals(song2, results.get(0));
    }

    /**
     * Test searching Songs with empty results.
     */
    @Test
    @DisplayName("Search Songs with no results")
    void testSearchSongsNoResults() throws MusicCatalogException {
        album1.addSongToList(song1);
        
        List<Song> results = album1.searchSongsByTitle("NonExistent");
        assertEquals(0, results.size());
    }

    /**
     * Test updating Album title.
     */
    @Test
    @DisplayName("Update Album title")
    void testUpdateAlbumTitle() throws MusicCatalogException {
        collection.addAlbumToCollection(album1);
        collection.updateAlbumTitle(1, "New Title");
        
        assertEquals("New Title", collection.findAlbumById(1).getTitle());
    }

    /**
     * Test Album to DTO conversion.
     */
    @Test
    @DisplayName("Album to DTO conversion")
    void testAlbumToDTO() {
        AlbumDTO dto = album1.toDTO();
        assertEquals(1, dto.albumId());
        assertEquals("Pink Floyd - The Wall", dto.albumTitle());
    }

    // ==================== Collection Tests ====================

    /**
     * Test Collection creation with valid name.
     */
    @Test
    @DisplayName("Create Collection with valid name")
    void testCreateValidCollection() {
        Collection coll = new Collection("Test Collection");
        assertEquals("Test Collection", coll.getCollectionName());
        assertEquals(0, coll.getAlbumCount());
    }

    /**
     * Test Collection creation with null name throws exception.
     */
    @Test
    @DisplayName("Collection creation with null name should throw exception")
    void testCreateCollectionWithNullName() {
        assertThrows(MusicCatalogException.class, () -> {
            Collection coll = new Collection(null);
            coll.validate();
        });
    }

    /**
     * Test adding Album to Collection.
     */
    @Test
    @DisplayName("Add Album to Collection")
    void testAddAlbumToCollection() throws MusicCatalogException {
        collection.addAlbumToCollection(album1);
        assertEquals(1, collection.getAlbumCount());
        assertEquals(album1, collection.findAlbumById(1));
    }

    /**
     * Test adding duplicate Album to Collection throws exception.
     */
    @Test
    @DisplayName("Adding duplicate Album should throw exception")
    void testAddDuplicateAlbum() throws MusicCatalogException {
        collection.addAlbumToCollection(album1);
        assertThrows(MusicCatalogException.class, () -> {
            collection.addAlbumToCollection(album1);
        });
    }

    /**
     * Test adding null Album to Collection throws exception.
     */
    @Test
    @DisplayName("Adding null Album should throw exception")
    void testAddNullAlbum() {
        assertThrows(MusicCatalogException.class, () -> {
            collection.addAlbumToCollection(null);
        });
    }

    /**
     * Test removing Album from Collection.
     */
    @Test
    @DisplayName("Remove Album from Collection")
    void testRemoveAlbumFromCollection() throws MusicCatalogException {
        collection.addAlbumToCollection(album1);
        collection.addAlbumToCollection(album2);
        
        collection.removeAlbumFromCollection(1);
        assertEquals(1, collection.getAlbumCount());
        assertNull(collection.findAlbumById(1));
        assertEquals(album2, collection.findAlbumById(2));
    }

    /**
     * Test removing non-existent Album from Collection throws exception.
     */
    @Test
    @DisplayName("Removing non-existent Album should throw exception")
    void testRemoveNonExistentAlbum() {
        assertThrows(MusicCatalogException.class, () -> {
            collection.removeAlbumFromCollection(999);
        });
    }

    /**
     * Test finding Album by ID.
     */
    @Test
    @DisplayName("Find Album by ID")
    void testFindAlbumById() throws MusicCatalogException {
        collection.addAlbumToCollection(album1);
        collection.addAlbumToCollection(album2);
        
        assertEquals(album1, collection.findAlbumById(1));
        assertEquals(album2, collection.findAlbumById(2));
        assertNull(collection.findAlbumById(999));
    }

    /**
     * Test searching Albums by title substring.
     */
    @Test
    @DisplayName("Search Albums by title")
    void testSearchAlbumsByTitle() throws MusicCatalogException {
        collection.addAlbumToCollection(album1);
        collection.addAlbumToCollection(album2);
        
        List<Album> results = collection.searchAlbumsByTitle("Pink");
        assertEquals(1, results.size());
        assertEquals(album1, results.get(0));
    }

    /**
     * Test searching Albums with multiple results.
     */
    @Test
    @DisplayName("Search Albums with multiple results")
    void testSearchAlbumsMultipleResults() throws MusicCatalogException {
        Album album3 = new Album("Pink Floyd - Animals", 3);
        collection.addAlbumToCollection(album1);
        collection.addAlbumToCollection(album2);
        collection.addAlbumToCollection(album3);
        
        List<Album> results = collection.searchAlbumsByTitle("Pink");
        assertEquals(2, results.size());
    }

    /**
     * Test getting Albums sorted by title.
     */
    @Test
    @DisplayName("Get Albums sorted by title")
    void testGetAlbumsSortedByTitle() throws MusicCatalogException {
        collection.addAlbumToCollection(album2); // Led Zeppelin
        collection.addAlbumToCollection(album1); // Pink Floyd
        
        List<Album> sorted = collection.getAlbumsSortedByTitle();
        assertEquals(2, sorted.size());
        assertTrue(sorted.get(0).getTitle().compareTo(sorted.get(1).getTitle()) <= 0);
    }

    /**
     * Test getting Albums with minimum song count.
     */
    @Test
    @DisplayName("Get Albums with minimum song count")
    void testGetAlbumsWithMinSongs() throws MusicCatalogException {
        collection.addAlbumToCollection(album1);
        collection.addAlbumToCollection(album2);
        
        album1.addSongToList(song1);
        album1.addSongToList(song2);
        
        List<Album> results = collection.getAlbumsWithMoreThanSongs(1);
        assertEquals(1, results.size());
        assertEquals(album1, results.get(0));
    }

    /**
     * Test getting all Songs from all Albums using flatMap.
     */
    @Test
    @DisplayName("Get all Songs from Collection")
    void testGetAllSongs() throws MusicCatalogException {
        collection.addAlbumToCollection(album1);
        collection.addAlbumToCollection(album2);
        
        album1.addSongToList(song1);
        album1.addSongToList(song2);
        
        List<Song> allSongs = collection.getAllSongs();
        assertEquals(2, allSongs.size());
    }

    /**
     * Test getting total Song count across all Albums.
     */
    @Test
    @DisplayName("Get total Song count")
    void testGetTotalSongCount() throws MusicCatalogException {
        collection.addAlbumToCollection(album1);
        collection.addAlbumToCollection(album2);
        
        album1.addSongToList(song1);
        album1.addSongToList(song2);
        
        assertEquals(2, collection.getTotalSongCount());
    }

    /**
     * Test finding Albums containing specific Song.
     */
    @Test
    @DisplayName("Find Albums containing specific Song")
    void testFindAlbumsContainingSong() throws MusicCatalogException {
        collection.addAlbumToCollection(album1);
        collection.addAlbumToCollection(album2);
        
        album1.addSongToList(song1);
        
        List<Album> results = collection.findAlbumsContainingSong("In the Flesh?");
        assertEquals(1, results.size());
        assertEquals(album1, results.get(0));
    }

    /**
     * Test Album count in Collection.
     */
    @Test
    @DisplayName("Get Album count in Collection")
    void testGetAlbumCount() throws MusicCatalogException {
        collection.addAlbumToCollection(album1);
        collection.addAlbumToCollection(album2);
        
        assertEquals(2, collection.getAlbumCount());
    }

    /**
     * Test getting Album DTOs from Collection.
     */
    @Test
    @DisplayName("Get Album DTOs from Collection")
    void testGetAlbumDTOs() throws MusicCatalogException {
        collection.addAlbumToCollection(album1);
        collection.addAlbumToCollection(album2);
        
        List<AlbumDTO> dtos = collection.getAlbumDTOs();
        assertEquals(2, dtos.size());
        assertEquals(1, dtos.get(0).albumId());
    }

    /**
     * Test Collection validation with valid data.
     */
    @Test
    @DisplayName("Validate Collection with valid data")
    void testValidateCollectionValid() {
        assertDoesNotThrow(() -> {
            collection.validate();
        });
    }

    /**
     * Test Collection toString representation.
     */
    @Test
    @DisplayName("Collection toString")
    void testCollectionToString() throws MusicCatalogException {
        collection.addAlbumToCollection(album1);
        String str = collection.toString();
        assertNotNull(str);
        assertTrue(str.contains("My Music Library"));
    }
}