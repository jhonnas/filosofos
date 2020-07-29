#include "rshmem.h"
#include <stdlib.h>
#define N 5
#define M 50

/* Declaracion de la funcion incrementa */

incrementa (int *mem, int k){
int i;
i=*mem;
TP ; TP ; TP TP ;
TP ; TP TP ; TP ;
TP TP ; TP ; TP
i=i+k;
TP ; TP ; TP TP ;
TP ; TP TP ; TP ;

TP TP ; TP ; TP

*mem=i;
}


/* Declaracion de la funcion filosofo: uso de semaforos */

void filosofo(FILE *pf, key_t *sclave, int *spalillo, int scomer, int i, int *comer, int *fin){
/* ABRIR SEMAFOROS */

/*Abrir semaforo de la izquierda del filosofo */

if (-1==(spalillo[i]=semOpen(sclave[i])))
fprintf(stderr,"no tengo el cualificador del semaforo palillo %d\n",i);

/*Abrir semaforo de la derecha del filosofo */

if (-1==(spalillo[(i+1)%N]=semOpen(sclave[(i+1)%N])))
fprintf(stderr,"no tengo el cualifacador semaforo palillo %d\n",(i+1)%N);

/*Abrir semaforo de la variable comer y variable fin:memoria compartida*/

if (-1==(scomer=semOpen(sclave[N])))
fprintf(stderr,"no tengo el cualifacador semaforo scomer\n");

/* COMER M VECES */

while(*comer<M){
semWait(spalillo[i]);
semWait(spalillo[(i+1)%N]);
semWait(scomer);

incrementa(comer,1);

(void) fprintf(pf,"[comer:%.4d] el filosofo %d ha comido\n", *comer, i);
fflush(pf); /* para sincronizar la escritura de datos en el fichero de saliadas */

semSignal(scomer);
semSignal(spalillo[(i+1)%N]);
semSignal(spalillo[i]);
}


/* CERRAR SEMAFOROS Y TERMINAR */

semClose(spalillo[i]);
semClose(spalillo[(i+1)%N]);
semClose(scomer);
(*fin)++;
exit(1);
}

int main(){

FILE *pf; /*puntero a fichero salidas */
key_t sclave[N+1]; /*array de claves para semaforos */
int scomer; /*semaforo para variable comer (s.c.) */
int spalillo[N]; /*un semaforo por palillo */
int *comer; /*variable de memoria compartida */
int *fin; /*variable de memoria compartida */
int i; /*contador */

/* Abrir fichero de salidas */

if((pf=fopen("fich", "w+"))==NULL){

fprintf(stderr,"error al abrir el fichero para salidas\n");
exit(-1);
}
/* SEMAFOROS PARA PALILLOS */

for(i=0; i<N; i++){

/*crear nombres claves palillos */

if((key_t)-1==(sclave[i]=ftok("filos",'s'+i))){
fprintf(stderr,"main: error crear clave palillo %d con ftok(%c)\n", i,'s'+i);
exit(1);
}

/*crear semaforos palillos */

if(-1==(spalillo[i]=semCreate(sclave[i],1))){
fprintf(stderr,"main: no pude crear semaforo palillo %d\n",i);
exit(1);
}
}

/* SEMAFORO PARA VARIABLE COMER */

if((key_t)-1==(sclave[N]=ftok("filos",'r'))){
fprintf(stderr,"main: error crear clave comer con ftok()\n");
exit(1);
}
if(-1==(scomer=semCreate(sclave[N],1))){

fprintf(stderr,"main: no pude crear semaforo para comer (s.c.)\n");
exit(1);
}

/* CREAR ZONA DE MEMORIA COMPARTIDA: VARIABLE COMER */

if(!crearMemoria())
fprintf(stderr,"error de crearMemoria()\n");

/*inicializar variable comer y variable fin */

comer = (int *) memoria;
*comer = 0;
fin = (int *) comer + sizeof(int);
*fin = 0;

/* PROCESO PADRE */

for(i=0; i<N; i++){
if(0==fork())/* PROCESOS HIJOS */
filosofo(pf, sclave, spalillo, scomer, i, comer, fin);
}

while(*fin<5); /*espera a que los filosofos coman M veces */

fprintf(pf,"no habia comido ningun filososo y ahora han comido %d", *comer);
fclose(pf);

/* TERMINA */

/* eliminar memoria de las variables compartidas */

if(!eliminarMemoria())
fprintf(stderr,"error de eliminarMemoria()\n");

/* cerrar semaforos */

semClose(scomer);
for(i=0; i<N; i++)
semClose(spalillo[i]);
exit(0);
} /*fin proceso padre, fin main */
