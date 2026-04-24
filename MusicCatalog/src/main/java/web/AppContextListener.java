package web;

import jakarta.persistence.*;
import jakarta.servlet.ServletContext;
import jakarta.servlet.ServletContextEvent;
import jakarta.servlet.ServletContextListener;
import jakarta.servlet.annotation.WebListener;
import java.util.logging.Level;
import java.util.logging.Logger;
import model.Album;
import model.AlbumRepository;
import model.Collection;
import model.MusicCatalogException;
import model.Song;

/**
 * Initializes the application model on startup.
 * 
 * @author Student
 * @version 1.0
 */
@WebListener
public class AppContextListener implements ServletContextListener {

    @Override
    public void contextInitialized(ServletContextEvent sce) {
        // Create and initialize Collection with sample data
        Collection collection = new Collection("My Music");
        ServletContext servletContext = sce.getServletContext();

        EntityManagerFactory emf = Persistence.createEntityManagerFactory("musicCatalogPU");
        EntityManager em = emf.createEntityManager();
        
        AlbumRepository repo = new AlbumRepository(em);
        servletContext.setAttribute("albumRepository", repo);
        
        // Add sample albums
        Album album1 = new Album("Toxicity", 1);
        try {
            album1.addSongToAlbum(new Song(101, "Shop Suey!"));
            album1.addSongToAlbum(new Song(102, "Toxicity"));
            album1.addSongToAlbum(new Song(103, "Aerials"));
            collection.addAlbumToCollection(album1);
        } catch (MusicCatalogException ex) {
            Logger.getLogger(AppContextListener.class.getName()).log(Level.SEVERE, null, ex);
        }
        Album album2 = new Album("Vesica Piscis", 2);
        try {
            album2.addSongToAlbum(new Song(201, "Scape"));
            album2.addSongToAlbum(new Song(202, "Repugnant"));
            album2.addSongToAlbum(new Song(203, "Arbiter of Taste"));
            album2.addSongToAlbum(new Song(204, "The Great Gig in the Sky"));
            album2.addSongToAlbum(new Song(205, "Glowjaw"));
            collection.addAlbumToCollection(album2);
        } catch (MusicCatalogException ex) {
            Logger.getLogger(AppContextListener.class.getName()).log(Level.SEVERE, null, ex);
        }
        Album album3 = new Album("A Night at the Opera", 3);
        try {
            album3.addSongToAlbum(new Song(301, "Bohemian Rhapsody"));
            album3.addSongToAlbum(new Song(302, "You're My Best Friend"));
            album3.addSongToAlbum(new Song(303, "I'm in Love with My Car"));
            album3.addSongToAlbum(new Song(304, "Lazing on a Sunday Afternoon"));
            collection.addAlbumToCollection(album3);
        } catch (MusicCatalogException ex) {
            Logger.getLogger(AppContextListener.class.getName()).log(Level.SEVERE, null, ex);
        }
        // Store in ServletContext
        sce.getServletContext().setAttribute("collection", collection);
        
        System.out.println("[Music Catalog] Initialized with " + collection.getAlbumCount() + " albums");
    }
}