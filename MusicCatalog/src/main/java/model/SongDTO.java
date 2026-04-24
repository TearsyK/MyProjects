/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package model;

/**
 * Record type representing Song data transfer object.
 * Provides immutable data structure for song information.
 * 
 * @param songId    Unique song identifier
 * @param songTitle Title of the song
 * 
 * @author Kacper Kostanek
 * @version 1.0
 */
public record SongDTO(int songId, String songTitle) {
    // Record automatically generates:
    // - Constructor: SongDTO(int songId, String songTitle)
    // - Getters: songId(), songTitle()
    // - equals(), hashCode(), toString()
}
