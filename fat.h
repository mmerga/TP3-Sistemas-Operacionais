//Julio Hebert Silva
//Tomaz Miranda de Oliveira
/*INCLUDE*/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*DEFINE*/
#define SECTOR_SIZE	512
#define CLUSTER_SIZE	2*SECTOR_SIZE 
#define ENTRY_BY_CLUSTER CLUSTER_SIZE /sizeof(dir_entry_t) 
#define NUM_CLUSTER	4096
#define fat_name	"fat.part"

struct _dir_entry_t
{
    char filename[18];
	uint8_t attributes; // 0 DIR 1 ARQ
	uint8_t reserved[7];
	uint16_t first_block;
	uint32_t size;
};

typedef struct _dir_entry_t  dir_entry_t;

struct _data_cluster
{
    dir_entry_t dir[ENTRY_BY_CLUSTER]; 
    char data[1000];
};

typedef struct _data_cluster data_cluster;

/*DATA DECLARATION*/
unsigned short fat[NUM_CLUSTER]; 
unsigned char boot_block[CLUSTER_SIZE]; 
dir_entry_t root_dir[ENTRY_BY_CLUSTER]; 
data_cluster clusters[4086];
unsigned short clusters_atualizados[4086];


// Inicia uma nova Fat, se já existir a antiga será apagada
void init();

// Carrega o arquivo da Fat na memoria, deve existir
void load();	

// Salvar a tabela de volta na memoria junto com os cluster alterados
void fechar_salvar();

// Salva no disco o cluster INDEX, atualizando o antigo
void integridade(int index);

// Verifica se a Fat existe, se existe sera carregada, se nao sera criada uma nova
void existe_fat();

// Retorna o Cluster INDEX da memória
data_cluster cluster_da_memoria(int index);