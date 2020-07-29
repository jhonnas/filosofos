
/*
* rshmem.h
* * Fichero de cabecera con definiciones y declaraciones para usar
* memoria compartida.
*/

#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define ARRAY_SIZE 4000
#define MALLOC_SIZE 10000
#define SHM_SIZE 10000
#define SHM_MODE (SHM_R | SHM_W) /* read/write */

#define TRUE 1
#define FALSE 0

#ifdef RUTINAS_SHMEM
static int shmid; /* handler de memoria compartida */
char *memoria; /* puntero a zona de memoria compartida */
#else
extern char *memoria;
#endif

/* Prototipos de funciones de memoria compartida */

void origenTiempo();
void tiempoPasa();
int crearMemoria() ;
int eliminarMemoria() ;
#define TP tiempoPasa();
