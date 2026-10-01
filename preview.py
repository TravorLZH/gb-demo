"""Serve the built ROM to a browser emulator until Ctrl-C is pressed."""

from functools import partial
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
import webbrowser


project_dir = Path(__file__).resolve().parent
handler = partial(SimpleHTTPRequestHandler, directory=str(project_dir))

# Port 0 asks the OS for a free localhost port, avoiding port conflicts.
with ThreadingHTTPServer(("127.0.0.1", 0), handler) as server:
    url = f"http://127.0.0.1:{server.server_port}/preview.html"
    print(f"Opening {url} (press Ctrl-C here to stop)", flush=True)
    webbrowser.open(url)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        pass
