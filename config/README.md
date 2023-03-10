# Config file structure
## HTTP
List all options that are related to the web server as a whole.
As of now, we use global defined variable, and no options are available in this section.

## Server
List options that are related to one server. These include:
- `root`: default root for the server ressources. (1 argument)
- `server_name`: name of the server to be used during communication and logs. (1 argument)
- `index`: Default fallback file for the server. (1 argument)
- `listen_port`: pair of `IPv4:port` to communicate with the server, defaults to `127.0.0.1:8080`.
- `error_page`: pair of `status` and `path` for a specific status code html page.
- `location`: `path` to a location (optional `=` for exact location matching). Introduce a location specific set of rules. see below.

## Location
List options that are related to one specific location for server. These include:
- `redirect`: indicates the new URI for the location. In case redirect is defined, `301 Moved Permanently` is returned.
- `methods`: list of upper case methods to be accepted by the location (include `GET`, `POST` and `DELETE`).
- `root`: location specific root to replace the server default root. (1 argument)
- `index`: ressource to serve if URI is directory. (1 argument)
- `autoindex`: bool to enable directory listing for given location. (on/off or 0/1)

## Example
```
http {
  server {
    listen 127.0.0.1:8080;
    index index.html;
    server_name toto.42.fr;
    root /var/www;
    error_page 404 404.html;
    
    location / {
       autoindex 1; /* or `on`*/
       root www;
    }

    location = /toto/index.html {
      methods GET;
      root /var/toto;
      index 404.html;
    }
  }
}
