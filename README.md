# ASSQL
Angelscript SQLite and MySQL APIs. Includes datetime API.

# MariaDB-Connector
The mariadbclient.lib and dll needs to be recompiled with the proper configurations.  

Required MariaDB-Connector version: **[3.0.2](https://downloads.mariadb.org/connector-c/3.0.2/)**

# Required Libraries (Windows)
Windows compile will fail if the compile configuration does not contain the following included libraries:
```
wsock32.lib
shlwapi.lib
ws2_32.lib
```
