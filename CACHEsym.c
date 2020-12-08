#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Mascaras de Bits
#define BLOQ_C 0b1111111111111 // Bitmask para el Bloque
#define ETQ_C 0b11111111111 // Bitmask para la ETQ
#define LIN_C 0b11 // Bitmask para la linea
#define PALABRA_C 0b111 // Bitmask para la palabra

typedef struct {
    short int ETQ;
    short int Datos[8];
}T_LINEA_CACHE;

typedef struct {
    T_LINEA_CACHE t_linea_cache[4];
    int tiempoglobal;
    int numfallos;
}PROCESO;

PROCESO proceso;


//numero de direcciones y caracteres leidos
float Naddr = 0;
char CharMemoria[100];

void cacheDefault(){
    const short int ETQ_Default = 255;
    
    // Asignamos los valores iniciales de la CACHE
    
    printf("Contenido de la CACHE por defecto:");
    
    for(int i=0; i<4; i++){
        
        proceso.t_linea_cache[i].ETQ = ETQ_Default;
        printf("\nETQ:%x    Datos ", proceso.t_linea_cache[i].ETQ);
        
        for(int j=0; j<8; j++){
            proceso.t_linea_cache[i].Datos[j] = 0;
            printf("%x ", proceso.t_linea_cache[i].Datos[j]);
        }
    }
    printf("\n");
}

void printCache(){
    
    for(int i=0; i<4; i++){
        
        printf("\nETQ:%02x    Datos ", proceso.t_linea_cache[i].ETQ);
        
        for(int j=0; j<8; j++){
            printf("%02x ", proceso.t_linea_cache[i].Datos[j]);
            CharMemoria[j+(i*8)] = proceso.t_linea_cache[i].Datos[j];
        }
    }
    printf("\n");
    
}

void comparator(FILE* f_memoria, char* RAM){
    
    int addr=0,bloq_addr=0,etq_addr=0,linea_addr=0,palabra_addr=0;
    
    //Recogemos la siguiente línea linea de accesos_memoria.txt y la guardaos en addr
    fscanf(f_memoria,"%x",&addr);
        
    //Comprobamos que no es el final del fichero, si lo es terminamos la función
    if(feof(f_memoria)){
        
        printf("\n-FIN DE DIRECCIONES DE MEMORIA-\n");
        
    }else{
        
        //incrementamos el número de addrecciones leídas
        Naddr++;

        // Separamos los valores de la addr mediante un bitmask
        bloq_addr= (addr >>3) & BLOQ_C;
        etq_addr= (addr >> 5) & ETQ_C;
        linea_addr= (addr >> 3) & LIN_C;
        palabra_addr= addr & PALABRA_C;
        
        //Mostramos los datos de la dirección
        printf("\naddreccion de linea: %x",addr);
        printf("\nRecoger Bloque: %x",bloq_addr);
        printf("\nRecoger ETQ: %x",etq_addr);
        printf("\nRecoger Linea: %x",linea_addr);
        printf("\nRecoger Palabra: %x",palabra_addr);
        printf("\n");

        //Comprobamos si la etiqueta de la addrección está cargada en caché en la linea
        if(etq_addr == proceso.t_linea_cache[linea_addr].ETQ){
            
            // si es así, mostramos un acierto y la caché cargada
            
            printf("\nT: %d, Acierto de CACHE, ADDR %04x ETQ %02x linea %02x palabra %02x DATO %02x",proceso.tiempoglobal,addr,etq_addr,linea_addr,palabra_addr,RAM[addr]);
            printf("\n");

            printCache();

        }
        else{
            
            //de lo contrario, incrementamos el número de fallos
            proceso.numfallos++;

            //mostramos el tiempo, número de fallos y datos de la addrección
            printf("\nT: %d, Fallo de CACHE %d, ADDR %04x ETQ %02x linea %02x palabra %02x bloque %02x",proceso.tiempoglobal,proceso.numfallos,addr,etq_addr,linea_addr,palabra_addr,bloq_addr);
            
            //e incrementamos en 10 el tiempo global
            proceso.tiempoglobal += 10;
            
            //cargamos la addrección en su correspondiente línea de caché
            printf("\nCargando el bloque %02x en la linea %02x",bloq_addr,linea_addr);
            printf("\nT: %d, Acierto de CACHE, ADDR %04x ETQ %02x linea %02x palabra %02x DATO %02x",proceso.tiempoglobal,addr,etq_addr,linea_addr,palabra_addr,RAM[addr]);
            
            proceso.t_linea_cache[linea_addr].ETQ = etq_addr;
            
            //y cargamos en caché el bloque de la addrección en su correspondiene línea
            //para ello, buscamos en el array RAM el bloque deaddrección + i
            //ya que hemos de buscar 8 datos
            for(int i = 0; i<8;i++){
                
                proceso.t_linea_cache[linea_addr].Datos[i] = RAM[((addr-palabra_addr)+7-i)];
                
            }
            
            printf("\n");

            //mostramos la caché actual
            
            printCache();
            
        }
        
        //hacemos un sleep de 2 segundos y lo incrementamos en el tiempo global
        sleep(2);
        proceso.tiempoglobal += 2;
        
        //llamamos de nuevo a esta misma función para vargar la siguiente addrección
        comparator(f_memoria, RAM);
        
    }
    
}

void dataPrint(){
    
    printf("Se han leído un total de %.0f direcciones de memoria.\nHay un total de %d fallos, dando una media de %.2f fallos por dirección.\nSe ha tardado un total de %d segundos a una media de %.2f segundos por cada dirección.\n",Naddr,proceso.numfallos,(proceso.numfallos/Naddr),proceso.tiempoglobal,(proceso.tiempoglobal/Naddr));
    
    
    //mostramos todos los caracteres guardados en caché
    
    printf("\nValores guardados en caché:\n");
    int salt = 0;

    for(int i = 0; i < 32; i++){
        
        if(salt == 8){salt = 0; printf("\n");}
        printf("%c ",CharMemoria[i]);
        salt++;
        
    }
    printf("\n\n");
 
}

int main(){
    int i=0;
    unsigned char RAM[1024];
    
    FILE *f_ram,*f_memoria;
    char c;
    
    // Arranque de CACHE con sus valores por defecto
    
    cacheDefault();
    
    // Lectura de fichero RAM.bin
    
    f_ram = fopen("RAM.bin","r");
    
    if(f_ram == NULL){
        printf("El archivo RAM.bin no se ha podido abrir\n");
        exit(-1);
    }
    
    while((c = getc(f_ram)) != EOF){
        RAM[i]=c;
        i++;
    }
    RAM[i] = '\0';
    i=0;
    
    // Lectura de fichero accesos_memoria.txt
    
    f_ram = fopen("accesos_memoria.txt","r");
    
    if(f_memoria == NULL){
        printf("El archivo accesos_memoria.txt no se ha podido abrir\n");
        exit(-1);
    }

    // Circuito comparador de la correspondencia addrecta
    comparator(f_memoria, RAM);
    printf("\n");
    
    //Cerramos lo ficheros
    fclose(f_ram);
    fclose(f_memoria);
        
    dataPrint();
    
    return 0;
}

