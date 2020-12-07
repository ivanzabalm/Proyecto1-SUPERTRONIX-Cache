#include <stdio.h>
#include <stdlib.h>

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


//borrar int repet, solo para pruebas
int repet = 0;

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

void actualizarLinea(){
    
    
}

int charToInt(char dirChar[]){
    int dir = 0;
    
    
    
    return dir;
}


void comparator(FILE* f_memoria, char* RAM, int linea_act){
    int dir=0,bloq_dir=0,etq_dir=0,linea_dir=0,palabra_dir=0;

    // Recogemos la primera linea de accesos_memoria.txt
    
    // TODO : ¡SOLO ES DE UNA LINEA! Hacerlo para todas las lineas
    
    //borrar
    repet++;
    
    fscanf(f_memoria,"%x",&dir);
    
    //dir = 0x02F4;
    
    //if(feof(f_memoria)||dir ==0){printf("\nFIN");exit(-1);}
    // Separamos los valores de la dir mediante un bitmask
    
    bloq_dir= (dir >>3) & BLOQ_C;
    etq_dir= (dir >> 5) & ETQ_C;
    linea_dir= (dir >> 3) & LIN_C;
    palabra_dir= dir & PALABRA_C;
    
    // PRINTS TEMPORALES
    printf("\nDireccion de linea: %x",dir);
    printf("\nRecoger Bloque: %x",bloq_dir);
    printf("\nRecoger ETQ: %x",etq_dir);
    printf("\nRecoger Linea: %x",linea_dir);
    printf("\nRecoger Palabra: %x",palabra_dir);
    printf("\n");
    // PRINTS TEMPORALES

    
    int direccion[16];
    
    for(int i = 0; i<4; i++){
        
        direccion[i] = 0;
        
    }
    printf("\n");

    if(etq_dir == proceso.t_linea_cache[linea_act].ETQ){
        printf("\nSon iguales");
    }
    else{
        proceso.numfallos++;
        // TODO : Colocar bien el valor de ADDR
        printf("\nT: %d, Fallo de CACHE %d, ADDR %04x ETQ %x linea %02x palabra %02x bloque %02x",proceso.tiempoglobal,proceso.numfallos,0,etq_dir,linea_dir,palabra_dir,bloq_dir);
        proceso.tiempoglobal += 10;
        
        // TODO : Crear funcion
        printf("\nCargando el bloque %02d en la linea %02x",bloq_dir,linea_dir);
        printf("\nT: %d, Acierto de CACHE, ADDR %04x ETQ %x linea %02x palabra %02x DATO %02x",proceso.tiempoglobal,0,etq_dir,linea_dir,palabra_dir,RAM[bloq_dir]);
        proceso.t_linea_cache[linea_act].ETQ = etq_dir;
        
        //asignamos uno a uno los valores correspondientes a los datos de la etiqueta que hemos leido
        //para ello, buscamos en el array RAM el bloque dedirección + i
        //ya que hemos de buscar 8 datos
        

        for(int i = 0; i<8;i++){
            
            proceso.t_linea_cache[linea_act].Datos[i] = RAM[(bloq_dir+i)];
            
        }
        
        for(int i=0; i<4; i++){
            
            printf("\nETQ:%x    Datos ", proceso.t_linea_cache[i].ETQ);
            
            for(int j=0; j<8; j++){
                printf("%x ", proceso.t_linea_cache[i].Datos[j]);
            }
        }
        printf("\n");
        
        
        actualizarLinea();
    }
    if(linea_act<4){linea_act++;}
    
    //borrar
    if(repet <2){
        comparator(f_memoria, RAM,linea_act);
}

    
}


int main(){
    int i=0;
    unsigned char RAM[1024];
    
    FILE *f_ram,*f_memoria;
    char c;
    
    // Arranque de CACHE con sus valores por defecto
    
    cacheDefault();
    
    // Lectura de fichero RAM.bin
    
    //f_ram = fopen("RAM.bin","r");
    f_ram = fopen("/Users/mario/Desktop/U-TAD/proyectos Xcode/C/proyecto_ssoo_2/proyecto_ssoo_2/RAM.bin","r");
    
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
    
    //f_ram = fopen("accesos_memoria.txt","r");
    f_memoria = fopen("/Users/mario/Desktop/U-TAD/proyectos Xcode/C/proyecto_ssoo_2/proyecto_ssoo_2/accesos_memoria.txt","r");
    
    if(f_memoria == NULL){
        printf("El archivo accesos_memoria.txt no se ha podido abrir\n");
        exit(-1);
    }

    // Circuito comparador de la correspondencia directa
    
    comparator(f_memoria, RAM,0);
    
    fclose(f_ram);
    fclose(f_memoria);
    
    return 0;
}

