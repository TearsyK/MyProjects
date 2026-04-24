package web;

import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.Cookie;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;

/**
 * Servlet for displaying history and statistics.
 * Displays edit logs from session attribute and cookie information to the client.
 * Allows clearing edit history on request.
 * 
 * @author Kacper Kostanek
 * @version 2.0
 */
@WebServlet("/history")
public class HistoryServlet extends HttpServlet {

    /**
     * Handles GET requests to display history and cookie information.
     * 
     * @param request the HTTP request
     * @param response the HTTP response
     * @throws ServletException if a servlet error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        response.setContentType("text/html;charset=UTF-8");
        PrintWriter out = response.getWriter();

        out.println("<!DOCTYPE html><html><body>");
        out.println("<h1>Historia Edycji</h1>");
        out.println("<a href='/musicCatalog/home'>Home</a> | <a href='/musicCatalog/albums'>Albumy</a>");
        out.println("<hr>");

        String history = (String) request.getSession().getAttribute("editHistory");
        if (history != null && !history.isEmpty()) {
            String[] logs = history.split("\\|");
            out.println("<h2>Ostatnie edycje (max 10):</h2>");
            out.println("<ul>");
            for (int i = 0; i < Math.min(10, logs.length); i++) {
                if (!logs[i].isEmpty()) {
                    out.println("<li>" + escapeHtml(logs[i]) + "</li>");
                }
            }
            out.println("</ul>");
        } else {
            out.println("<p>Brak historii edycji.</p>");
        }

        out.println("<h2>Ciastka (Cookies):</h2>");
        Cookie[] cookies = request.getCookies();
        if (cookies != null && cookies.length > 0) {
            out.println("<table border='1'>");
            out.println("<tr><th>Nazwa</th><th>Wartosc</th><th>Sciezka</th></tr>");
            for (Cookie cookie : cookies) {
                out.println("<tr>");
                out.println("<td>" + escapeHtml(cookie.getName()) + "</td>");
                out.println("<td>" + escapeHtml(cookie.getValue()) + "</td>");
                out.println("<td>" + escapeHtml(cookie.getPath() != null ? cookie.getPath() : "/") + "</td>");
                out.println("</tr>");
            }
            out.println("</table>");
        } else {
            out.println("<p>Brak ciastek.</p>");
        }

        out.println("<h2>Zarzadzanie:</h2>");
        out.println("<form method='POST'>");
        out.println("<button type='submit' name='action' value='clear' onclick='return confirm(\"Usunac cala historie?\")'>Wyczysc historie</button>");
        out.println("</form>");

        out.println("</body></html>");
    }

    /**
     * Handles POST requests for clearing history.
     * 
     * @param request the HTTP request
     * @param response the HTTP response
     * @throws ServletException if a servlet error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        String action = request.getParameter("action");
        
        if ("clear".equals(action)) {
            request.getSession().setAttribute("editHistory", null);
            response.sendRedirect("/musicCatalog/history");
        } else {
            doGet(request, response);
        }
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