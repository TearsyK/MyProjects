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
import model.Song;

@WebServlet("/albums/*")
public class SongServlet extends HttpServlet {

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        response.setContentType("text/html;charset=UTF-8");
        PrintWriter out = response.getWriter();
        Collection collection = (Collection) getServletContext().getAttribute("collection");
        String pathInfo = request.getPathInfo();
        int albumId = Integer.parseInt(pathInfo.substring(1));

        Album album = collection.findAlbumById(albumId);
        if (album == null) {
            out.println("<!DOCTYPE html><html><body>");
            out.println("<h1>Album nie znaleziony</h1>");
            out.println("<a href='/musicCatalog/albums'>Powrót do albumów</a>");
            out.println("</body></html>");
            return;
        }

        out.println("<!DOCTYPE html><html><body>");
        out.println("<h1>Piosenki w: " + album.getTitle() + "</h1>");
        out.println("<a href='/musicCatalog/albums'>Albumy</a>");
        out.println("<hr>");

        out.println("<h2>Dodaj piosenkę</h2>");
        out.println("<form method='POST'>");
        out.println("<input type='hidden' name='action' value='add'>");
        out.println("<input type='hidden' name='albumId' value='" + albumId + "'>");
        out.println("<input name='title' placeholder='Tytuł' required>");
        out.println("<button>Dodaj</button>");
        out.println("</form>");

        List<Song> songs = album.getSongList();
        out.println("<h2>Piosenki (" + songs.size() + ")</h2>");
        if (songs != null && !songs.isEmpty()) {
            for (Song song : songs) {
                out.println("<p><b>" + song.getSongTitle() + "</b> (ID: " + song.getSongId() + ") " +
                            "<form method='POST' style='display:inline'>" +
                            " <input type='hidden' name='action' value='delete'>" +
                            " <input type='hidden' name='songId' value='" + song.getSongId() + "'>" +
                            " <button onclick='return confirm(\"Usunąć?\")'>Usuń</button>" +
                            " </form> " +
                            "<form method='POST' style='display:inline'>" +
                            " <input type='hidden' name='action' value='edit'>" +
                            " <input type='hidden' name='songId' value='" + song.getSongId() + "'>" +
                            " <input name='newTitle' placeholder='Nowy tytuł'>" +
                            " <button>Edytuj</button>" +
                            " </form></p>");
            }
        } else {
            out.println("<p>Brak piosenek w tym albumie.</p>");
        }
        out.println("</body></html>");
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        Collection collection = (Collection) getServletContext().getAttribute("collection");
        String action = request.getParameter("action");
        int albumId = Integer.parseInt(request.getParameter("albumId"));
        Album album = collection.findAlbumById(albumId);

        try {
            if ("add".equals(action)) {
                String title = request.getParameter("title");
                int newSongId = generateSongId(album);
                Song song = new Song(newSongId, title);
                album.addSongToList(song);
                logEdit("Dodano piosenkę do albumu " + albumId + ": " + title, request);
            } else if ("delete".equals(action)) {
                int songId = Integer.parseInt(request.getParameter("songId"));
                Song song = album.findSongById(songId);
                String title = song.getSongTitle();
                album.removeSongById(songId);
                logEdit("Usunięto piosenkę ID " + songId + " z albumu " + albumId + ": " + title, request);
            } else if ("edit".equals(action)) {
                int songId = Integer.parseInt(request.getParameter("songId"));
                String newTitle = request.getParameter("newTitle");
                if (newTitle != null && !newTitle.isEmpty()) {
                    album.updateSongTitle(songId, newTitle);
                    logEdit("Edytowano piosenkę ID " + songId + " w albumie " + albumId, request);
                }
            }
            response.sendRedirect(request.getRequestURI());
        } catch (Exception e) {
            response.getWriter().println("<h1>Błąd: " + e.getMessage() + "</h1>");
        }
    }

    private int generateSongId(Album album) {
        int maxId = 0;
        for (Song song : album.getSongList()) {
            if (song.getSongId() > maxId) maxId = song.getSongId();
        }
        return maxId + 1;
    }

    private void logEdit(String message, HttpServletRequest request) {
        String history = (String) request.getSession().getAttribute("editHistory");
        if (history == null) history = "";
        history = message + "|" + history;
        if (history.split("\\|").length > 10) history = String.join("|", history.split("\\|", 11));
        request.getSession().setAttribute("editHistory", history);
    }
}