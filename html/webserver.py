import http.server
import socketserver

# Define the port you want the server to run on
PORT = 8000
HOSTNAME = "localhost"

# Handler to serve files from the current directory
Handler = http.server.SimpleHTTPRequestHandler

with socketserver.TCPServer(("", PORT), Handler) as httpd:
    print(f"Serving at port http://{HOSTNAME}:{PORT}")
    httpd.serve_forever()
