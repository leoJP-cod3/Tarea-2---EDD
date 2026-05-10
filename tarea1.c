#include "tdas/list.h"
#include "tdas/extra.h"
#include "tdas/queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct {
  char nombre[150];
} Categoria;

typedef struct{
  char descripcion[300];
  char categoria[150];
  time_t hora_registro;
} Tarea;


// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("======= Planificador de Tareas =======");
  puts("1) Nueva Categoría");
  puts("2) Eliminar Categoría");
  puts("3) Mostrar Categorías");
  puts("4) Registrar Pendiente");
  puts("5) Atender Siguiente");
  puts("0) Salir");
}

void agregar_categoria(List *categorias) {
  Categoria *nueva_categoria = (Categoria*) malloc(sizeof(Categoria));
  
  printf("Ingrese el nombre de la nueva categoria: ");
  
  scanf(" %149[^\n]", nueva_categoria->nombre);
  list_pushBack(categorias, nueva_categoria);
  
  printf("Nueva categoria agregada. ");
}


void mostrar_categoria(List *categorias) {
  Categoria *cat_actual = (Categoria *) list_first(categorias);

  if (cat_actual == NULL){
    printf("\nLa lista de categorias esta vacía.\n");
    return;
  }
  printf("------- Lista de Categorias -------\n");
  while (cat_actual != NULL){
    printf("~ %s\n", cat_actual->nombre);
    cat_actual = (Categoria*) list_next(categorias);
  }
  printf("\n");
}


void eliminar_categoria(List *categorias, Queue *fila_pendiente) {
  char cat_a_eliminar[150];

  printf("Categorias actuales\n");
  mostrar_categoria(categorias);

  printf("Ingrese el nombre de la categoria que desea eliminar: ");
  scanf(" %149[^\n]", cat_a_eliminar);
  
  int encontrada = 0;
  Categoria *cat_actual = (Categoria *) list_first(categorias);

  while(cat_actual != NULL){
    if (strcmp(cat_actual->nombre, cat_a_eliminar) == 0){
      list_popCurrent(categorias);
      encontrada = 1;
      printf("Categoria '%s' eliminada de la lista.\n", cat_a_eliminar);
      break;
    }
    cat_actual = (Categoria *) list_next(categorias);
  }
  
  if (encontrada == 0){
    printf("Aviso: No se encontro la categoria.\n");
    return;
  }
  Queue *fila_auxiliar = queue_create(NULL);
  Tarea *tarea_actual = (Tarea *) queue_remove(fila_pendiente);
  
  int tareas_borradas = 0;
  while(tarea_actual != NULL){
    if (strcmp(tarea_actual->categoria, cat_a_eliminar) == 0){
      free(tarea_actual);
      tareas_borradas++;
    } else {
      queue_insert(fila_auxiliar, tarea_actual);
    }
    tarea_actual = (Tarea *) queue_remove(fila_pendiente);
  }
  tarea_actual = (Tarea *) queue_remove(fila_auxiliar);
  while(tarea_actual != NULL){
    queue_insert(fila_pendiente, tarea_actual);
    tarea_actual = (Tarea *) queue_remove(fila_auxiliar);
  }
  free(fila_auxiliar);
  printf("Se eliminaron %d tareas de la categoria '%s'", tareas_borradas, cat_a_eliminar);
}



void registrar_pendiente(Queue *fila_pendiente, List *categorias) {
  Tarea *nueva_tarea = (Tarea*) malloc(sizeof(Tarea));

  printf("Ingrese la descripcion de la tarea: ");
  scanf(" %299[^\n]", nueva_tarea->descripcion);

  Categoria *primer_cat = (Categoria*) list_first(categorias);

  if(primer_cat == NULL){
    printf("\nActualmente no hay categorias registradas.\n");
    printf("La categoria que escriba a continuacion, se creara automaticamente.\n");
  } else {
    printf("Seleccione una de las siguientes categorias disponibles\n");
    mostrar_categoria(categorias);
  }
  
  printf("Ingrese el nombre de la categoria: ");
  scanf(" %149[^\n]", nueva_tarea->categoria);

  int cat_existe = 0;
  Categoria *cat_actual = (Categoria *) list_first(categorias);
  
  while(cat_actual != NULL){
    if (strcmp(cat_actual->nombre, nueva_tarea->categoria) == 0){
      cat_existe = 1;
      break;
    }
    cat_actual = (Categoria *) list_next(categorias);
  }

  if (cat_existe == 0){
    printf("Aviso: La categoria '%s' se ha creado y agregado a la lista.", nueva_tarea->categoria);
    Categoria *nueva_cat = (Categoria *) malloc(sizeof(Categoria));
    strcpy(nueva_cat->nombre, nueva_tarea->categoria);
    list_pushBack(categorias, nueva_cat);
  }
  
  nueva_tarea->hora_registro = time(NULL);
  queue_insert(fila_pendiente, nueva_tarea);
  
  printf("\nNueva Tarea agregada.\n");
}

void atender_siguiente(Queue *fila_pendiente) {
  Tarea *tarea_atendida = (Tarea*) queue_remove(fila_pendiente);

  if (tarea_atendida == NULL){
    printf("Libre de tareas.\n");
    return;
  }

  printf("Detalles: %s |\n", tarea_atendida->descripcion);
  printf("Categoria: %s |\n", tarea_atendida->categoria);
  printf("Hora de registro: %s \n", ctime(&tarea_atendida->hora_registro));
  
  free(tarea_atendida);
}

int main() {
  char opcion;
  List *categorias = list_create(); // Lista para almacenar categorías
  Queue *fila_pendiente = queue_create(NULL);
  

  do {
    mostrarMenuPrincipal();
    printf("\nIngrese su opción: ");
    scanf(" %c", &opcion); 

    switch (opcion) {
    case '1':
      agregar_categoria(categorias);
      break;
    case '2':
      eliminar_categoria(categorias, fila_pendiente);
      break;
    case '3':
      mostrar_categoria(categorias);
      break;
    case '4':
      registrar_pendiente(fila_pendiente, categorias);
      break;
    case '5':
      atender_siguiente(fila_pendiente);
      break;
    case '0':
      puts("Saliendo del planificador.");
      break;
    default:
      puts("Opción no válida. Por favor, intente de nuevo.");
    }
    presioneTeclaParaContinuar();

  } while (opcion != '0');

  list_clean(categorias);

  return 0;
}
