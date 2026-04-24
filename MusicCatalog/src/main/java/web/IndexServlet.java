package web;

import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;

/**
 * Main page servlet.
 * 
 * @author Student
 * @version 1.0
 */
@WebServlet("/")
public class IndexServlet extends HttpServlet {

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        
        response.setContentType("text/html;charset=UTF-8");
        PrintWriter out = response.getWriter();
        
        out.println("<html><body>");
        out.println("<h1>Music Catalog</h1>");
        out.println("<p>Welcome!</p>");
        out.println("<a href='/musicCatalog/albums'>Albums</a> | ");
        out.println("<a href='/musicCatalog/history'>History</a>");
        out.println("</body></html>");
    }
}
