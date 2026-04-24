/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package model;

/**
 * Record type representing Album data transfer object.
 * Provides immutable data structure for album information.
 * 
 * @param albumId    Unique album identifier
 * @param albumTitle Title of the album
 * 
 * @author Kacper Kostanek
 * @version 1.0
 */
public record AlbumDTO(int albumId, String albumTitle) {
    // Record automatically generates:
    // - Constructor: AlbumDTO(int albumId, String albumTitle)
    // - Getters: albumId(), albumTitle()
    // - equals(), hashCode(), toString()
}

