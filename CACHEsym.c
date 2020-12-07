#include <stdio.h>
#include <stdlib.h>

// Mascaras de Bits
#define BLOQ_C 0b1111111000 // Bitmask para el Bloque
#define ETQ_C 0b1111100000 // Bitmask para la ETQ
#define LIN_C 0b0000011000 // Bitmask para la linea
#define PALABRA_C 0b0000000111 // Bitmask para la palabra

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

void comparator(FILE* f_memoria, char* RAM){
    int dir=0,bloq_dir=0,etq_dir=0,linea_dir=0,palabra_dir=0;

    // Recogemos la primera linea de accesos_memoria.txt
    
    // TODO : ¡SOLO ES DE UNA LINEA! Hacerlo para todas las lineas
    fscanf(f_memoria,"%x",&dir);
    
    // Separamos los valores de la dir mediante un bitmask
    
    bloq_dir= dir & BLOQ_C;
    etq_dir= dir & ETQ_C;
    linea_dir= dir & LIN_C;
    palabra_dir= dir & PALABRA_C;
    
    // PRINTS TEMPORALES
    printf("\nDireccion de linea: %x",dir);
    printf("\nRecoger Bloque: %x",bloq_dir);
    printf("\nRecoger ETQ: %x",etq_dir);
    printf("\nRecoger Linea: %x",linea_dir);
    printf("\nRecoger Palabra: %x",palabra_dir);
    printf("\n");
    // PRINTS TEMPORALES
    
    if(etq_dir == proceso.t_linea_cache[linea_dir].ETQ){
        printf("\nSon iguales");
    }
    else{
        proceso.numfallos++;
        // TODO : Colocar bien el valor de ADDR
        printf("\nT: %d, Fallo de CACHE %d, ADDR %04x ETQ %x linea %02x palabra %02x bloque %02x",proceso.tiempoglobal,proceso.numfallos,0,etq_dir,linea_dir,palabra_dir,bloq_dir);
        proceso.tiempoglobal += 10;
        
        // TODO : Crear funcion
        actualizarLinea();
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
	
	f_ram = fopen("RAM.txt","r");
	
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
	
	f_memoria = fopen("accesos_memoria.txt","r");
	
	if(f_memoria == NULL){
		printf("El archivo accesos_memoria.txt no se ha podido abrir\n");
		exit(-1);
	}

	// Circuito comparador de la correspondencia directa
	
	comparator(f_memoria, RAM);
	
	fclose(f_ram);
	fclose(f_memoria);
	
	return 0;
}

