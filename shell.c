//Julio Hebert Silva
//Tomaz Miranda de Oliveira
#include "shell.h"

int pos_vazia_fat(){
    for (int i = 10; i < NUM_CLUSTER; i++){
        if (fat[i] == 0x0000)
        {
            fat[i] = 1;
            return i-10;
        }
    }
    return -1;
}

int barras(char* destino){
    int aux = strlen(destino);
    int num_barras = 0;
    for(int i=0; i<aux;i++){
        if(destino[i] == '/'){
            num_barras++;
        }
    }
    return num_barras;
}

int pos_vazia_pai(dir_entry_t pai[ENTRY_BY_CLUSTER], char* destino){ 
    for(int i=0; i<ENTRY_BY_CLUSTER; i++){
        if(strcmp(pai[i].filename, destino) == 0){
            return -2;
        }
    }
    for(int i=0; i<ENTRY_BY_CLUSTER; i++){ 
        if(pai[i].size == 0){
            return i;
        }
    }
    return -1;
}

void arruma_destinos(char* destino, char* dir_pai, char* dir_filho){
    int tam = strlen(destino);
    int flag = 0;
    dir_pai[0] = '/';
    for(int i=1; i<tam; i++){
        if(destino[i] != '/'){
            dir_pai[i] = destino[i];
        }else{
            dir_pai[i] = '\0';
            flag = i;
            break;
        }
    }
    dir_filho[0] = '/';
    int j=1;
    for(int i=flag+1; i<tam; i++){
        dir_filho[j++] = destino[i];
    }
    dir_filho[j] = '\0';
}

int excluir(dir_entry_t pai[ENTRY_BY_CLUSTER], char* destino){
    int num_barras = barras(destino);
    if(num_barras==1){
        for(int i=0; i<ENTRY_BY_CLUSTER; i++){
            if(strcmp(pai[i].filename, destino) == 0){
                if(pai[i].size<=2){
                    int pos_fat = pai[i].first_block;
                    data_cluster cluster ;
                    fat[pos_fat] = 0x0000;
                    pai[i].filename[0] = '\0';
                    pai[i].size = 0;
                    clusters[pos_fat] = cluster;
                    clusters_atualizados[pos_fat] = 1;
                    integridade(pos_fat);
                    return 1;
                }else{
                    printf("Err: Erro ao apagar DIR %s, nao vazio\n", destino);
                    return 0;
                }
            }
        }
        printf("Err: Arquivo ou Diretorio inexistente.\n");
        return 0;
    }else{
        char dir_pai[100];
        char dir_filho[100];
        arruma_destinos(destino, dir_pai, dir_filho);
        for(int i = 0; i<ENTRY_BY_CLUSTER; i++){
            if(strcmp(pai[i].filename, dir_pai) == 0){
                int pos_pai_fat = pai[i].first_block ;
                data_cluster cluster = cluster_da_memoria(pos_pai_fat);
                int flag = excluir((cluster.dir), dir_filho);
                if(flag){
                    pai[i].size--;
                    clusters[pos_pai_fat] = cluster;
                    clusters_atualizados[pos_pai_fat] = 1;
                    integridade(pos_pai_fat);
                }
                return 1;
            }
        }
        printf("Err: Caminho inexistente.\n");
        return 0;
    }
    return 0;
}

void read(dir_entry_t pai[ENTRY_BY_CLUSTER], char* destino){
    int num_barras = barras(destino);
    if(num_barras==1){
        for(int i = 0; i<ENTRY_BY_CLUSTER; i++){
            if(strcmp(pai[i].filename, destino) == 0 ) {
                if(pai[i].attributes == 1){
                    int pos_fat = pai[i].first_block;
                    data_cluster cluster = cluster_da_memoria(pos_fat);
                    printf("/>%s: %s \n", pai[i].filename, cluster.data);
                    return;
                }else{
                    printf("Err: %s eh um DIRETORIO, nao ARQUIVO.\n", pai[i].filename);
                    return;
                }
            }
        }
        printf("Err: Arquivo inexistente.\n");
        return;
    }else{
        char dir_pai[100];
        char dir_filho[100];
        arruma_destinos(destino, dir_pai, dir_filho);
        for(int i = 0; i<ENTRY_BY_CLUSTER; i++){
            if(strcmp(pai[i].filename, dir_pai) == 0){
                int pos_pai_fat = pai[i].first_block ;
                data_cluster cluster = cluster_da_memoria(pos_pai_fat);
                read((cluster.dir), dir_filho);
                return;
            }
        }
        printf("Err: Caminho inexistente.\n");
        return;
    }
}

void append(dir_entry_t pai[ENTRY_BY_CLUSTER], char* destino, char* frase){
    int num_barras = barras(destino);
    if(num_barras==1){
        for(int i = 0; i<ENTRY_BY_CLUSTER; i++){
            if(strcmp(pai[i].filename, destino) == 0 ) {
                if(pai[i].attributes == 1){
                    int pos_fat = pai[i].first_block;
                    data_cluster cluster = cluster_da_memoria(pos_fat);
                    strcat(cluster.data, frase);           
                    clusters[pos_fat] = cluster;
                    clusters_atualizados[pos_fat] = 1;
                    integridade(pos_fat);
                    return;
                }else{
                    printf("Err: %s eh um DIRETORIO, nao ARQUIVO.\n", pai[i].filename);
                    return;
                }
            }
        }
        printf("Err: Arquivo inexistente.\n");
        return;
    }else{
        char dir_pai[100];
        char dir_filho[100];
        arruma_destinos(destino, dir_pai, dir_filho);
        for(int i = 0; i<ENTRY_BY_CLUSTER; i++){
            if(strcmp(pai[i].filename, dir_pai) == 0){
                int pos_pai_fat = pai[i].first_block ;
                data_cluster cluster = cluster_da_memoria(pos_pai_fat);
                append((cluster.dir), dir_filho, frase);
                return;
            }
        }
        printf("Err: Caminho inexistente.\n");
        return;
    }
}

void write(dir_entry_t pai[ENTRY_BY_CLUSTER], char* destino, char* frase){ 
    int num_barras = barras(destino);
    if(num_barras==1){
        for(int i = 0; i<ENTRY_BY_CLUSTER; i++){
            if(strcmp(pai[i].filename, destino) == 0 ) {
                if(pai[i].attributes == 1){
                    int pos_fat = pai[i].first_block;
                    data_cluster cluster = cluster_da_memoria(pos_fat);
                    strcpy(cluster.data, frase);                
                    clusters[pos_fat] = cluster;
                    clusters_atualizados[pos_fat] = 1;
                    integridade(pos_fat);
                    return;
                }else{
                    printf("Err: %s eh um DIRETORIO, nao ARQUIVO.\n", pai[i].filename);
                    return;
                }
            }
        }
        printf("Err: Arquivo inexistente.\n");
        return;
    }else{
        char dir_pai[100];
        char dir_filho[100];
        arruma_destinos(destino, dir_pai, dir_filho);
        for(int i = 0; i<ENTRY_BY_CLUSTER; i++){
            if(strcmp(pai[i].filename, dir_pai) == 0){
                int pos_pai_fat = pai[i].first_block ; 
                data_cluster cluster = cluster_da_memoria(pos_pai_fat);
                write((cluster.dir), dir_filho, frase);
                return;
            }
        }
        printf("Err: Caminho inexistente.\n");
    }
}

int mkdir_create(dir_entry_t pai[ENTRY_BY_CLUSTER], char* destino, int tipo){ 
    int num_barras = barras(destino);   
    if(num_barras==1){
        int pos_pai = pos_vazia_pai(pai, destino);
        if(pos_pai >= 0){
            int pos_fat = pos_vazia_fat();
            if(pos_fat != -1){
                strcpy(pai[pos_pai].filename, destino);
                pai[pos_pai].attributes = tipo ; 
                pai[pos_pai].reserved[0] = 0;
                pai[pos_pai].first_block = pos_fat;
                pai[pos_pai].size = 2;
                if(tipo == 0){
                    strcpy(clusters[pos_fat].dir[0].filename, ".\0"); 
                    clusters[pos_fat].dir[0].size = 2;
                    strcpy(clusters[pos_fat].dir[1].filename, "..\0");
                    clusters[pos_fat].dir[1].size = 2; 
                    for(int j=2 ; j<ENTRY_BY_CLUSTER; j++){
                        clusters[pos_fat].dir[j].size = 0;
                    }
                }
                clusters_atualizados[pos_fat] = 1;
                integridade(pos_fat);
                return 1;       
            }else{
                printf("Err: Fat cheia\n");
                return 0;
            }
        }else if(pos_pai == -1){
            printf("Err: Tamanho máximo do diretorio atingido, 32\n");
            return 0;
        }else if(pos_pai == -2){
            printf("Err: Ja existe arquivo ou diretorio com esse nome.\n");
            return 0;
        }
    }else{      
        char dir_pai[100];
        char dir_filho[100];
        arruma_destinos(destino, dir_pai, dir_filho);
        for(int i = 0; i<ENTRY_BY_CLUSTER; i++){
            if(strcmp(pai[i].filename, dir_pai) == 0){
                int pos_pai_fat = pai[i].first_block ;
                data_cluster cluster = cluster_da_memoria(pos_pai_fat); 
                int flag = mkdir_create((cluster.dir), dir_filho, tipo);
                if(flag){
                    pai[i].size++; 
                    clusters[pos_pai_fat] = cluster;
                    clusters_atualizados[pos_pai_fat] = 1;
                    integridade(pos_pai_fat);
                }
                return 1;
            }
        }
        printf("Err: Caminho inexistente.\n");
        return 0;
    }
    return 0;
}

void ls(dir_entry_t pai[ENTRY_BY_CLUSTER], char* destino){ 
    int num_barras = barras(destino);
    if(num_barras==1 && destino[1] == '\0'){
        for(int i=0; i < ENTRY_BY_CLUSTER; i++){
            if(pai[i].size >= 2){
                if(pai[i].attributes == 0){
                    printf("<DIR> %s \n", pai[i].filename);
                }else{
                    printf("<ARQ> %s \n", pai[i].filename);
                }
            }
        }
    }else if(num_barras==1 && destino[1] != '\0'){
        for(int i=0; i<ENTRY_BY_CLUSTER; i++){
            if(strcmp(pai[i].filename, destino) == 0){
                int pos_pai_fat = pai[i].first_block ;
                data_cluster cluster = cluster_da_memoria(pos_pai_fat);
                ls(cluster.dir, "/\0");
                return;
            }
        }
        printf("Err: Caminho inexistente.\n");
        return;
    }else if(num_barras>1){
        char dir_pai[100];
        char dir_filho[100];
        arruma_destinos(destino, dir_pai, dir_filho);
        for(int i = 0; i<ENTRY_BY_CLUSTER; i++){
            if(strcmp(pai[i].filename, dir_pai) == 0){
                int pos_pai_fat = pai[i].first_block ;
                data_cluster cluster = cluster_da_memoria(pos_pai_fat);
                ls(cluster.dir, dir_filho);
                return;
            }
        }
        printf("Err: Caminho inexistente.\n");
        return;
    }
}

void help(){
    printf("\n\n\n");
    printf("        Possiveis comandos do shell:\n\n");
    printf("Cria uma nova tabela FAT, excluindo os dados da antiga.\n       init\n\n");
    printf("Carrega a tabela FAT.\n       load\n\n");
    printf("Cria o diretorio NovaPasta no diretorio RAIZ.\n        mkdir /NovaPasta\n\n");
    printf("Cria o diretorio SistemasOperacionais no diretorio  NovaPasta.\n        mkdir /NovaPasta/SistemasOperacionais\n\n");
    printf("Cria arquivo Nome no diretorio RAIZ.\n        create /Nome \n\n");
    printf("Cria arquivo Nome do diretorio NovaPasta.\n       create /NovaPasta/Nome\n\n");
    printf("Lista os diretorios e arquivos do diretorio RAIZ.\n       ls /\n\n");
    printf("Lista os diretorios e arquivos do diretorio SistemasOperacionais.\n       ls /NovaPasta/SistemasOperacionais\n\n");
    printf("Escreve \"string\"  no arquivo Nome.\n      write \"string\" /NovaPasta/Nome\n\n");
    printf("Anexa \"string\"  no arquivo Nome.\n        append \"string\" /NovaPasta/Nome\n\n");
    printf("Exclui o diretorio(ou arquivo) SistemasOperacionais. OBS: Se for diretorio, deve estar vazio.\n       unlink /NovaPasta/SistemasOperacionais\n\n");
    printf("Escreve no terminal os dados do arquivo Nome.\n       read /NovaPasta/Nome\n\n");
    printf("Encerra o processo.\n        exit\n\n");
    printf("    \n\n");
}
