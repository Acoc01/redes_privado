# redes_privado

## Compilar
Para compilar el código se necesita tener la biblioteca ***SODIUM*** y también contar con ***CMAKE*** instalados.
Primero se debe crear una carpeta "build" en el directorio donde se clonó el repositorio y ejecutar los siguientes comandos en orden.
```
cmake ..
cmake --build .

```
Luego de haber creado esto se crearán cuatro directorios de los cuales tres cuentan con ejecutables: "keygen", "server", "client".

## Ejecutar codigo
Para ejecutar el código primero se debe generar una clave con el ejecutable keygen en el directorio keygen de la siguiente forma:
```
./keygen llave.key
```
Lo que generará la llave. Esta llave se debe copiar en el directorio de server y client de la carpeta build ya que tanto client como server utilizan la llave.

Luego se debe mover el archivo a enviar al directorio ./build/client.

Posteriormente se ejecuta primero el servidor y luego el cliente en terminales distintas.
```
./client

Ingrese nombre del archivo:
archivo.txt

```
Para ejecutar el cliente, se debe ingresar el nombre del archivo a transferir cuando se pida por terminal.
