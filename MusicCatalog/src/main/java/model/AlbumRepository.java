package model;

import jakarta.persistence.*;
import java.util.List;

public class AlbumRepository {
    private EntityManager em;
    
    public AlbumRepository(EntityManager em) {
        this.em = em;
    }
    
    // Znajdź album po ID
    public Album findById(int id) {
        return em.find(Album.class, id);
    }
    
    // Wszystkie albumy
    public List<Album> findAll() {
        return em.createQuery("SELECT a FROM Album a", Album.class).getResultList();
    }
    
    // Zapisz album
    public Album save(Album album) {
        em.persist(album);
        return album;
    }
    
    // Usuń album
    public void delete(int id) {
        Album album = findById(id);
        if (album != null) {
            em.remove(album);
        }
    }
}
