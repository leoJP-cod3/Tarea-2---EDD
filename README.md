# StreamFinder 🍿 - Catálogo de Películas

## Descripción

StreamFinder, una herramienta de línea de comandos diseñada para gestionar y explorar un catálogo masivo de películas y series de manera eficiente. La aplicación permitirá cargar datos externos, realizar búsquedas complejas y gestionar una lista de favoritos personalizada.

## Cómo compilar y ejecutar

Este sistema ha sido desarrollado en lenguaje C y puede ejecutarse fácilmente utilizando Visual Studio Code junto con una extensión para C/C++, como C/C++ Extension Pack de Microsoft. Para comenzar a trabajar con el sistema en tu equipo local, sigue estos pasos:

### Requisitos previos:

1. Tener instalado Visual Studio Code.
2. Instalar la extensión C/C++ (Microsoft).
3. Tener instalado un compilador de C (como gcc). Si estás en Windows, se recomienda instalar MinGW o utilizar el entorno WSL.
4. Asegurarse de tener la carpeta `data` con el archivo `Top1500.csv` en la raíz del proyecto.

### Pasos para compilar y ejecutar:

1. **Descarga y descomprime** el archivo del proyecto en una carpeta.
2. **Abre el proyecto en Visual Studio Code**:
* Inicia Visual Studio Code.
* Selecciona Archivo > Abrir carpeta... y elige la carpeta del proyecto.


3. **Compila el código**:
* Abre la terminal integrada (Terminal > Nueva terminal).
* En la terminal, compila el programa con el siguiente comando para incluir las librerías de la carpeta `tdas`:
```bash
gcc tdas/*.c tarea2.c -Wno-unused-result -o tarea2

```


4. **Ejecuta el programa**:
* Una vez compilado, puedes ejecutar la aplicación con:
```bash
./tarea2

```

* Si se usa Windows, para ejecutar:
```bash
.\tarea2.exe

```


### Solución de problemas: Error de directorios

Si al intentar compilar el código la terminal arroja un error indicando que no encuentra los archivos de los TDAs (como `list.h`, `map.h`, etc.), se debe a que los archivos están separados o sueltos en el directorio principal y el código espera que estén agrupados.

Para solucionar esto de manera rápida:

1. Crea una nueva carpeta llamada `tdas` en el mismo lugar donde está el archivo principal (`tarea2.c`).
2. Mueve todos los archivos correspondientes a las herramientas (`list.c`, `list.h`, `map.c`, `map.h`, `extra.c`, `extra.h`) dentro de esa nueva carpeta `tdas`. El único archivo de código que debe quedar afuera es `tarea2.c`.
3. Una vez hecho esto, vuelve a intentar compilar utilizando el comando original:
```bash
gcc tdas/*.c tarea2.c -Wno-unused-result -o tarea2

```


4. **Ejecuta el programa**:
* Una vez compilado, puedes ejecutar la aplicación con:
```bash
./tarea2

```


* Si se usa Windows, para ejecutar:
```bash
.\tarea2.exe

```


## Funcionalidades

### Funcionando correctamente:

* **Cargar Catálogo:** Lectura y parseo del archivo `Top1500.csv`. Creación dinámica de structs en memoria e indexación simultánea en 4 mapas distintos (por ID, Género, Director y Década) para garantizar velocidad de respuesta.
* **Buscar por Género:** Búsqueda instantánea que despliega todos los títulos pertenecientes a una categoría específica con su información completa.
* **Buscar por Director:** Localización en $O(1)$ de todas las obras de un director, manejando correctamente entradas con espacios.
* **Buscar por Década:** Conversión automática de cualquier año ingresado a su década base matemática `(año / 10) * 10` y búsqueda directa a través de un mapa con claves numéricas enteras.
* **Búsqueda Avanzada (Género y Década):** Filtro de doble criterio optimizado. En lugar de iterar todo el catálogo, obtiene la lista de un género específico en $O(1)$ y filtra únicamente esos resultados por año, mejorando drásticamente el rendimiento.
* **Gestionar Mi Watchlist:** Sub-menú que permite:
* **Agregar:** Validación estricta e instantánea (mediante mapa de IDs) de que el ID ingresado existe realmente en el catálogo antes de agregarlo.
* **Mostrar:** Visualización de los títulos pendientes. Notifica si la lista está vacía.
* **Eliminar:** Borrado seguro de un nodo específico (por ID) sin romper la estructura de la lista principal.


### Posibles Mejoras Futuras:

* **Persistencia de la Watchlist:** Añadir la capacidad de exportar la lista de favoritos a un archivo de texto o CSV para no perderla al cerrar la sesión.
* **Ordenamiento de Resultados:** Implementar algoritmos para mostrar las búsquedas ordenadas por calificación (Rating) de mayor a menor.
* **Búsquedas Parciales:** Mejorar la flexibilidad de búsqueda permitiendo encontrar directores o películas ingresando solo parte del nombre (ej. buscar "Tarantino" en lugar de "Quentin Tarantino").
* **Ordenamiento por Años:** Implementar un algoritmo para mostrar las búsquedas por década se muestre ordenada de menor a mayor.

## Ejemplo de uso

### Paso 1: Intentar gestionar sin cargar datos

El usuario intenta agregar una película antes de inicializar el sistema.

* **Opción seleccionada:** `6) Gestionar Mi Watchlist` -> `1) Agregar pelicula`
* **Entrada:** `tt0111161`
* El sistema valida y arroja error indicando que el ID no existe en el catálogo actual, protegiendo la integridad de los datos.

### Paso 2: Cargar Catálogo

Se inicializan las estructuras y se lee el archivo base.

* **Opción seleccionada:** `1) Cargar Catálogo`
* El sistema lee los registros, crea los índices cruzados e indica: "¡Catálogo cargado exitosamente!".

### Paso 3: Búsqueda por Director

El usuario desea ver las obras de su director favorito.

* **Opción seleccionada:** `3) Buscar por Director`
* **Entrada:** `Steven Spielberg`
* El sistema encuentra el índice y despliega inmediatamente en pantalla todas sus películas, mostrando detalles como ID, Título y Año.

### Paso 4: Búsqueda Avanzada

El usuario busca algo específico para ver: ciencia ficción antigua.

* **Opción seleccionada:** `5) Búsqueda Avanzada`
* **Entrada:** `Sci-Fi, 1980` (separados por coma).
* El sistema busca rápidamente en la lista de Sci-Fi, filtra y muestra exclusivamente los títulos de esa categoría lanzados entre 1980 y 1989.

### Paso 5: Añadir a la Watchlist

El usuario copia el ID de una película que le interesó en el paso anterior y decide guardarla.

* **Opción seleccionada:** `6) Gestionar Mi Watchlist` -> `1) Agregar pelicula`
* **Entrada:** `tt0080684` (ID válido).
* El sistema corrobora la existencia en el mapa y confirma que la película ha sido añadida exitosamente a la lista por ver.

### Paso 6: Revisar y Limpiar la Watchlist

El usuario verifica sus pendientes y borra una película que ya vio.

* **Opción seleccionada:** `6) Gestionar Mi Watchlist` -> `2) Mostrar Watchlist`
* Se imprime en pantalla la película agregada en el paso anterior.
* **Opción seleccionada:** `3) Eliminar pelicula`
* **Entrada:** `tt0080684`
* El sistema elimina el registro de la lista personalizada. Si vuelve a presionar `Mostrar`, el sistema advertirá amigablemente que la Watchlist se encuentra vacía.