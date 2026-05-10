#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char id[100];
  char title[100];
  List *genres;
  char director[300];
  float rating;
  int year;
} Film;

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts(" StreamFinder - Catalogo de Peliculas");
  puts("========================================");
  
  puts("1) Cargar Películas");
  puts("2) Buscar por Género");
  puts("3) Buscar por Director");
  puts("4) Buscar por Década");
  puts("5) Busqueda Avanzada (Década y Género)");
  puts("6) Gestionar Mi Watchlist");
  puts("7) Salir");
}

/**
 * Compara dos claves de tipo string para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo string.
 *
 * @param key1 Primer puntero a la clave string.
 * @param key2 Segundo puntero a la clave string.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

/**
 * Compara dos claves de tipo entero para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo entero.
 *
 * @param key1 Primer puntero a la clave entera.
 * @param key2 Segundo puntero a la clave entera.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2; // Compara valores enteros directamente
}

/**
 * Carga películas desde un archivo CSV y las almacena en un mapa por ID.
 */
void cargar_peliculas(Map *pelis_byid, Map *pelis_bygenres, Map *pelis_bydirector, Map *pelis_bydecada) {
  // Intenta abrir el archivo CSV que contiene datos de películas
  FILE *archivo = fopen("data/Top1500.csv", "r");
  if (archivo == NULL) {
    perror(
        "Error al abrir el archivo"); // Informa si el archivo no puede abrirse
    return;
  }

  char **campos;
  // Leer y parsear una línea del archivo CSV. La función devuelve un array de
  // strings, donde cada elemento representa un campo de la línea CSV procesada.
  campos = leer_linea_csv(archivo, ','); // Lee los encabezados del CSV

  // Lee cada línea del archivo CSV hasta el final
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    // Crea una nueva estructura Film y almacena los datos de cada película
    Film *peli = (Film *)malloc(sizeof(Film));
    strcpy(peli->id, campos[1]);        // Asigna ID
    strcpy(peli->title, campos[5]);     // Asigna título
    strcpy(peli->director, campos[14]); // Asigna director
    peli->genres = split_string(campos[11], ",");       // Inicializa la lista de géneros
    peli->year = atoi(campos[10]); // Asigna año, convirtiendo de cadena a entero
    peli->rating = atoi(campos[7]); // Agrego rating 

    
    // Inserta la película en el mapa usando el ID como clave
    map_insert(pelis_byid, peli->id, peli);

    // Código generado con ayuda de chatgpt3.5
    // conversación: https://chat.openai.com/share/5f0643ad-e8f5-4fb7-a0fa-2d2f92408429
    
    // Obtiene el primer género de la lista de géneros de la película
    char *genre = list_first(peli->genres);
    // Itera sobre cada género de la película
    while (genre != NULL) {
        // Busca el género en el mapa pelis_bygenres
        MapPair *genre_pair = map_search(pelis_bygenres, genre);
        // Si el género no existe en el mapa, crea una nueva lista y agrégala al mapa
        if (genre_pair == NULL) {
            List *new_list = list_create();
            list_pushBack(new_list, peli);
            map_insert(pelis_bygenres, genre, new_list);
        } else {
            // Si el género ya existe en el mapa, obtén la lista y agrega la película
            List *genre_list = (List *)genre_pair->value;
            list_pushBack(genre_list, peli);
        }
        // Avanza al siguiente género en la lista
        genre = list_next(peli->genres);
    }
    MapPair *director_pair = map_search(pelis_bydirector, peli->director);

    if (director_pair == NULL){
      List*director_list = list_create();
      list_pushBack(director_list, peli);
      map_insert(pelis_bydirector, peli->director, director_list);
    } else{
      List *director_list = (List*) director_pair->value;
      list_pushBack(director_list, peli);
    }
    int decada_calculada = (peli->year/10) * 10;

    MapPair *decada_pair = map_search(pelis_bydecada, &decada_calculada);

    if (decada_pair == NULL){
      int *nueva_decada = (int *) malloc(sizeof(int));
      *nueva_decada = decada_calculada;
      
      List*decada_list = list_create();
      list_pushBack(decada_list, peli);
      
      map_insert(pelis_bydecada, nueva_decada, decada_list);
    } else{
      List *decada_list = (List*) decada_pair->value;
      list_pushBack(decada_list, peli);
    }
    
  }
  fclose(archivo); // Cierra el archivo después de leer todas las líneas


  // Itera sobre el mapa para mostrar las películas cargadas
  MapPair *pair = map_first(pelis_byid);
  while (pair != NULL) {
    Film *peli = pair->value;
    printf("ID: %s, Título: %s, Director: %s, Año: %d\n", peli->id, peli->title,
           peli->director, peli->year);

    printf("Géneros: ");
    for(char *genre = list_first(peli->genres); genre != NULL; genre = list_next(peli->genres))
      printf("%s, ", genre);
    printf("\n");
    
    pair = map_next(pelis_byid); // Avanza al siguiente par en el mapa
  }
}

/**
 * Busca y muestra la información de una película por su ID en un mapa.
 */
void buscar_por_id(Map *pelis_byid) {
  char id[10]; // Buffer para almacenar el ID de la película

  // Solicita al usuario el ID de la película
  printf("Ingrese el id de la película: ");
  scanf("%s", id); // Lee el ID del teclado

  // Busca el par clave-valor en el mapa usando el ID proporcionado
  MapPair *pair = map_search(pelis_byid, id);

  // Si se encontró el par clave-valor, se extrae y muestra la información de la
  // película
  if (pair != NULL) {
    Film *peli =
        pair->value; // Obtiene el puntero a la estructura de la película
    // Muestra el título y el año de la película
    printf("Título: %s, Año: %d\n", peli->title, peli->year);
  } else {
    // Si no se encuentra la película, informa al usuario
    printf("La película con id %s no existe\n", id);
  }
}

void buscar_por_genero(Map *pelis_bygenres) {
  char genero[100];

  // Solicita al usuario el ID de la película
  printf("Ingrese el género de la película: ");
  scanf("%s", genero); // Lee el ID del teclado

  MapPair *pair = map_search(pelis_bygenres, genero);
  
  if (pair != NULL) {
      List* pelis = pair->value;
      Film *peli = list_first(pelis);
      
      while (peli != NULL) {
        printf("ID: %s | Título: %s | Director: %s | Año: %d\n", peli->id, peli->title,
           peli->director, peli->year);
        peli = list_next(pelis);
      }
  }
}


void buscar_por_director(Map *pelis_bydirector){
  char director[300];

  printf("Ingrese el nombre del director: ");
  scanf(" %[^\n]", director);

  MapPair *pair = map_search(pelis_bydirector, director);

  if(pair != NULL){
    List* pelis = (List*) pair->value;
    Film *peli = list_first(pelis);
    printf("\n --- Peliculas de %s --- \n", director);
    while(peli != NULL){
      printf("ID: %s | Titulo: %s | Año: %d\n", peli->id, peli->title, peli->year);
      peli = list_next(pelis);
    }
  } else printf("\nNo se encontraron peliculas del director %s\n", director);
}


void buscar_por_decada(Map *pelis_bydecada){
  int año_ingresado;

  printf("Ingresa un año para buscar su decada: ");
  scanf("%d", &año_ingresado);

  int decada_buscada = (año_ingresado / 10) * 10;

  MapPair *pair = map_search(pelis_bydecada, &decada_buscada);
  
  if(pair != NULL){
    List* pelis = (List*) pair->value;
    Film *peli = list_first(pelis);
    
    printf("\n --- Peliculas de la decada %d's --- \n", decada_buscada);
    while(peli != NULL){
      printf("ID: %s | Titulo: %s | Año: %d\n", peli->id, peli->title, peli->year);
      peli = list_next(pelis);
    }
  } else printf("\nNo se encontraron peliculas en la decada %d\n", decada_buscada);
}


void buscar_por_decada_genero(Map *pelis_bygenres){
  char genero[100];
  int anio_ingresado;
  
  printf("Ingrese el género y decada de la película separados por una coma (EJ: Crime, 1983): ");
  scanf(" %[^,], %d", genero, &anio_ingresado);

  int decada_buscada = (anio_ingresado / 10) * 10;

  MapPair *pair = map_search(pelis_bygenres, genero);

  if (pair != NULL) {
    List* pelis = pair->value;
    Film *peli = list_first(pelis);
    int encontradas = 0;

    printf("\n--- Peliculas de %s en la decada de los %d's ---\n", genero, decada_buscada);
    
    while (peli != NULL) {
      int decada_peli = (peli->year / 10) * 10;
      
      if (decada_peli == decada_buscada) {
        printf("ID: %s | Título: %s | Director: %s | Año: %d\n",peli->id, peli->title, peli->director, peli->year);
        encontradas = 1;
      }
      peli = list_next(pelis);
    }
    if (!encontradas) printf("\nNo se encontraron peliculas de %s en los años %d\n", genero, decada_buscada);
  } else printf("\nEl genero %s no se encuentra en el catalogo\n", genero);
}


void gestionar_watchlist(List *mi_watchlist, Map *pelis_byid){
  char opcion;
  char id_buscado[100];

  printf("========================================\n");
  printf("          GESTIONAR WATCHLIST           \n");
  printf("========================================\n");

  printf("1) Agregar pelicula\n");
  printf("2) Mostrar Watchlist\n");
  printf("3) Eliminar Watchlist\n");
  printf("Elija una opción: ");
  scanf(" %c", &opcion);
  
  if (opcion == '1'){
    printf("Ingresa el ID para agregar: ");
    scanf(" %s", id_buscado);
    MapPair *pair = map_search(pelis_byid, id_buscado);
    if(pair != NULL){
      list_pushBack(mi_watchlist, pair->value);
      printf("Pelicula Agregada.\n");
    } else printf("El ID no existe en el catálogo\n");
  } else if (opcion == '2'){
    printf("\n--- Las peliculas de tu WatchList ---\n");
      Film *peli = list_first(mi_watchlist);
    if (peli == NULL){
      printf("La WatchList esta vacia\n");
    } else {
      while(peli != NULL){
        printf("ID: %s | Titulo: %s\n", peli->id, peli->title);
        peli = list_next(mi_watchlist);
      }
    }
  } else if (opcion == '3'){
    Film *peli = list_first(mi_watchlist); 
    
    if(peli == NULL) {
      printf("La WatchList esta vacia.\n");
    } else {
      printf("Ingresa el ID a eliminar: ");
      scanf(" %s", id_buscado);
      
      int eliminado = 0;
      while (peli != NULL){
        if(strcmp(peli->id, id_buscado) == 0){
          list_popCurrent(mi_watchlist);
          printf("Pelicula eliminada\n");
          eliminado = 1;
          break;
        }
        peli = list_next(mi_watchlist);
      }
      if(eliminado == 0) printf("ID no encontrado en la WatchList\n");
    }
  }
}

int main() {
  char opcion; // Variable para almacenar una opción ingresada por el usuario
               // (sin uso en este fragmento)

  // Crea un mapa para almacenar películas, utilizando una función de
  // comparación que trabaja con claves de tipo string.
  Map *pelis_byid = map_create(is_equal_str);
  Map *pelis_bygenres = map_create(is_equal_str);
  Map *pelis_bydirector = map_create(is_equal_str);
  Map *pelis_bydecada = map_create(is_equal_int);
  
  List *mi_watchlist  = list_create();
  
  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      cargar_peliculas(pelis_byid, pelis_bygenres, pelis_bydirector, pelis_bydecada);
      break;
    case '2':
      buscar_por_genero(pelis_bygenres);
      break;
    case '3':
      buscar_por_director(pelis_bydirector);
      break;
    case '4':
      buscar_por_decada(pelis_bydecada);
      break;
    case '5':
      buscar_por_decada_genero(pelis_bygenres);
      break;
    case '6':
      gestionar_watchlist(mi_watchlist, pelis_byid);
      break;
    }
    presioneTeclaParaContinuar();

  } while (opcion != '7');

  return 0;
}