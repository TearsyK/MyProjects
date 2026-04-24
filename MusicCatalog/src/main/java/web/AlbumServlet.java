package web;

import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.List;
import model.Album;
import model.Collection;
import model.MusicCatalogException;

/**
 * Servlet handling album management operations (CRUD).
 * Provides access to the shared Collection model for album operations.
 * Displays album list, handles search, and processes add/edit/delete operations.
 * 
 * Implements unified GET/POST handling without code duplication.
 * Reports errors to client in case of invalid data or model exceptions.
 * 
 * 
 * @author Kacper Kostanek
 * @version 2.0
 */
@WebServlet("/albums")
public class AlbumServlet extends HttpServlet {


    /**
     * Handles GET and POST requests using unified method.
     * Delegates to common processing method to avoid code duplication.
     * 
     * @param request the HTTP request
     * @param response the HTTP response
     * @throws ServletException if a servlet error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    protected void service(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        String action = request.getParameter("action");
        
        if ("POST".equals(request.getMethod()) && action != null) {
            handlePostAction(request, response);
        } else {
            displayAlbumList(request, response);
        }
    }

    /**
     * Displays the album management page with list, search, and add forms.
     * Shows all albums or filtered results based on search term.
     * Displays validation errors if any.
     * 
     * @param request the HTTP request containing search parameters
     * @param response the HTTP response for HTML output
     * @throws ServletException if a servlet error occurs
     * @throws IOException if an I/O error occurs
     */
    private void displayAlbumList(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        response.setContentType("text/html;charset=UTF-8");
        PrintWriter out = response.getWriter();
        Collection collection = (Collection) getServletContext().getAttribute("collection");
        
        if (collection == null) {
            out.println("<!DOCTYPE html><html><body>");
            out.println("<h1>BLAD SYSTEMOWY</h1>");
            out.println("<p>Model kolekcji nie zostal zainicjalizowany.</p>");
            out.println("<a href='/musicCatalog/home'>Home</a>");
            out.println("</body></html>");
            return;
        }

        List<Album> albums = collection.getAlbumsSortedByTitle();
        String searchTerm = request.getParameter("searchTerm");
        String errorMessage = (String) request.getSession().getAttribute("errorMessage");
        String successMessage = (String) request.getSession().getAttribute("successMessage");

        if (errorMessage != null) request.getSession().setAttribute("errorMessage", null);
        if (successMessage != null) request.getSession().setAttribute("successMessage", null);

        if (searchTerm != null && !searchTerm.trim().isEmpty()) {
            albums = collection.searchAlbumsByTitle(searchTerm);
        }

        out.println("<!DOCTYPE html><html><body>");
        out.println("<h1>Albumy</h1>");
        out.println("<a href='/musicCatalog/home'>Home</a> | <a href='/musicCatalog/history'>Historia</a>");
        out.println("<hr>");

        if (errorMessage != null && !errorMessage.isEmpty()) {
            out.println("<p style='color: red;'><b>Blad:</b> " + escapeHtml(errorMessage) + "</p>");
        }
        if (successMessage != null && !successMessage.isEmpty()) {
            out.println("<p style='color: green;'><b>Sukces:</b> " + escapeHtml(successMessage) + "</p>");
        }

        out.println("<h2>Dodaj album</h2>");
        out.println("<form method='POST'>");
        out.println("<input type='hidden' name='action' value='add'>");
        out.println("<input name='title' placeholder='Tytul albumu' required>");
        out.println("<button>Dodaj</button>");
        out.println("</form>");

        out.println("<h2>Wyszukaj</h2>");
        out.println("<form method='GET'>");
        out.println("<input name='searchTerm' placeholder='Tytul albumu'>");
        out.println("<button>Szukaj</button>");
        out.println("</form>");

        out.println("<h2>Albumy (" + albums.size() + ")</h2>");
        for (Album album : albums) {
            out.println("<p><b>" + escapeHtml(album.getTitle()) + "</b> (ID: " + album.getId() + ") - Piosenek: " + album.getSongCount() + 
                        " <a href='/musicCatalog/albums/" + album.getId() + "'>Piosenki</a> " +
                        "<form method='POST' style='display:inline'>" +
                        " <input type='hidden' name='action' value='delete'>" +
                        " <input type='hidden' name='id' value='" + album.getId() + "'>" +
                        " <button onclick='return confirm(\"Usunac?\")'>Usun</button>" +
                        " </form> " +
                        "<form method='POST' style='display:inline'>" +
                        " <input type='hidden' name='action' value='edit'>" +
                        " <input type='hidden' name='id' value='" + album.getId() + "'>" +
                        " <input name='newTitle' placeholder='Nowy tytul' required>" +
                        " <button>Edytuj tytul</button>" +
                        " </form></p>");
        }
        out.println("</body></html>");
    }

    /**
     * Processes POST requests for album CRUD operations.
     * Handles: add, delete, edit actions with proper error handling.
     * 
     * @param request the HTTP request containing action and parameters
     * @param response the HTTP response for redirect
     * @throws ServletException if a servlet error occurs
     * @throws IOException if an I/O error occurs
     */
    private void handlePostAction(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        Collection collection = (Collection) getServletContext().getAttribute("collection");
        String action = request.getParameter("action");
        String message = "";
        String messageType = "error";

        try {
            if (collection == null) {
                throw new MusicCatalogException("Model kolekcji nie jest dostepny");
            }

            if ("add".equals(action)) {
                String title = request.getParameter("title");
                if (title == null || title.trim().isEmpty()) {
                    throw new MusicCatalogException("Tytul albumu nie moze byc pusty");
                }
                int newId = generateAlbumId(collection);
                Album newAlbum = new Album(title, newId);
                collection.addAlbumToCollection(newAlbum);
                logEdit("Dodano album: " + title, request);
                message = "Album '" + title + "' zostal pomyslnie dodany";
                messageType = "success";
                
            } else if ("delete".equals(action)) {
                String idStr = request.getParameter("id");
                if (idStr == null || idStr.isEmpty()) {
                    throw new MusicCatalogException("Brak ID albumu do usunicia");
                }
                int id = Integer.parseInt(idStr);
                Album album = collection.findAlbumById(id);
                if (album == null) {
                    throw new MusicCatalogException("Album o ID " + id + " nie zostal znaleziony");
                }
                String title = album.getTitle();
                collection.removeAlbumFromCollection(id);
                logEdit("Usunieto album ID " + id + ": " + title, request);
                message = "Album '" + title + "' zostal usuniety";
                messageType = "success";
                
            } else if ("edit".equals(action)) {
                String idStr = request.getParameter("id");
                if (idStr == null || idStr.isEmpty()) {
                    throw new MusicCatalogException("Brak ID albumu do edycji");
                }
                int id = Integer.parseInt(idStr);
                String newTitle = request.getParameter("newTitle");
                if (newTitle == null || newTitle.trim().isEmpty()) {
                    throw new MusicCatalogException("Nowy tytul albumu nie moze byc pusty");
                }
                Album album = collection.findAlbumById(id);
                if (album == null) {
                    throw new MusicCatalogException("Album o ID " + id + " nie zostal znaleziony");
                }
                String oldTitle = album.getTitle();
                collection.updateAlbumTitle(id, newTitle);
                logEdit("Edytowano album ID " + id + ": '" + oldTitle + "' -> '" + newTitle + "'", request);
                message = "Album zmieniony: '" + oldTitle + "' -> '" + newTitle + "'";
                messageType = "success";
            }
            
        } catch (NumberFormatException e) {
            message = "Blad: Nieprawidlowy format ID albumu";
        } catch (MusicCatalogException e) {
            message = "Blad modelu: " + e.getMessage();
        } catch (Exception e) {
            message = "Nieoczekiwany blad: " + e.getMessage();
        }

        if ("success".equals(messageType)) {
            request.getSession().setAttribute("successMessage", message);
        } else {
            request.getSession().setAttribute("errorMessage", message);
        }
        response.sendRedirect("/musicCatalog/albums");
    }

    /**
     * Generates a new unique album ID based on highest existing ID.
     * 
     * @param collection the album collection
     * @return the next available ID
     */
    private int generateAlbumId(Collection collection) {
        int maxId = 0;
        for (Album album : collection.getAlbumsSortedByTitle()) {
            if (album.getId() > maxId) {
                maxId = album.getId();
            }
        }
        return maxId + 1;
    }

    /**
     * Logs an edit action to the session history.
     * Maintains up to 10 most recent edits in the session.
     * 
     * @param message the edit message to log
     * @param request the HTTP request containing session
     */
    private void logEdit(String message, HttpServletRequest request) {
        String history = (String) request.getSession().getAttribute("editHistory");
        if (history == null) {
            history = "";
        }
        history = message + "|" + history;
        
        String[] parts = history.split("\\|");
        if (parts.length > 10) {
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < 10; i++) {
                if (i > 0) sb.append("|");
                sb.append(parts[i]);
            }
            history = sb.toString();
        }
        
        request.getSession().setAttribute("editHistory", history);
    }

    /**
     * Escapes HTML special characters to prevent XSS attacks.
     * 
     * @param text the text to escape
     * @return the escaped text safe for HTML output
     */
    private String escapeHtml(String text) {
        if (text == null) {
            return "";
        }
        return text.replace("&", "&amp;")
                   .replace("<", "&lt;")
                   .replace(">", "&gt;")
                   .replace("\"", "&quot;")
                   .replace("'", "&#39;");
    }
}