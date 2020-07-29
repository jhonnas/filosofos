
#define RUTINAS_SHMEM

#include "rshmem.h"

/* Crea memoria compartida.
* - el manejador de memoria es interno
* - manda mensajes de error por salida de error estándar.
*/
int crearMemoria() {
char *funcName = "crearMemoria";
if ((shmid=shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE))<0) {
fprintf(stderr, "%s: error de shmget()\n", funcName);
} else if ((memoria=shmat(shmid, 0, 0)) == (void *) -1) {
fprintf(stderr, "%s: error de shmat()\n", funcName);
} else {
return TRUE;
}
return FALSE;
}

/* Destruye la memoria compartida creada por crearMemoria()
*/
int eliminarMemoria() {
char *funcName = "eliminarMemoria";
if (shmctl(shmid, IPC_RMID, 0) < 0) {
fprintf(stderr,"%s: error de shmctl()\n", funcName);
return FALSE ;
} else
return TRUE ;
}

/* Coloca una semilla en el temporizador del bucle de
* tiempoPasa()
*/
void origenTiempo(){
srand((unsigned int) time(NULL)) ;
}

/* Rutina que hace pasar un poco de tiempo con un bucle
* sencillo
*/
void tiempoPasa() {
unsigned int i;
int a=3;

/* Los parametros "50" y "2" dependen mucho de la velocidad
* de la computadora y de la configuracion del SO. Espero que
* funcionen bien
*/
for (i=rand()/50; i>0; i--)
a = a%3 + i;
}
