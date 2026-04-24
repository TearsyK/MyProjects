/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package model;

/**
 * Custom exception thrown when an invalid music catalog operation is performed.
 * This exception is used throughout the model layer to signal errors such as
 * invalid song IDs, duplicate entries, or empty collections.
 * 
 * @author Kacper Kostanek
 * @version 1.1 (compatibility with new GUI project)
 */
public class MusicCatalogException extends Exception {
    
    /**
     * Constructs a new MusicCatalogException with the specified detail message.
     * 
     * @param message the detail message explaining the reason for the exception
     */
    public MusicCatalogException(String message) {
        super(message);
    }
    
    /**
     * Constructs a new MusicCatalogException with the specified detail message
     * and cause.
     * 
     * @param message the detail message explaining the reason for the exception
     * @param cause the cause of this exception
     */
    public MusicCatalogException(String message, Throwable cause) {
        super(message, cause);
    }
}
