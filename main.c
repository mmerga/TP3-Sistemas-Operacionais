//Julio Hebert Silva
//Tomaz Miranda de Oliveira

#include "shell.h"

// Apenas para apagar a variavel destino, pois estavam sempre pegando lixo do comando anterior 
// Coloca \0 em todas as posicoes da string
void lixo(char* destino){
    for(int i=0; i<=100; i++)
        destino[i] = '\0';
}

// Caso o Caminho lido do shell falte a primeira barra ele ainda sera aceito, essa funcao coloca a barra na primeira posição da string,
// Caso seja passado " src" sera retornado " /src"
void arruma_destino_lido(char* destino){
    int tam = strlen(destino);
    char desti[tam+1] ;
    for(int i=1; i<=tam; i++){
        desti[i] = destino[i-1];
    }
    desti[0] = '/';
    desti[tam+1] = '\0';
    for(int i=0; i<=tam ; i++){
        destino[i] = desti[i];
    }
}

int main(int argc, char **argv){
    printf("Shell\n");
    char comando[10];
    existe_fat();
    do{
        printf("/> ");
        scanf("%s", comando);
        if (strcmp(comando, "init") == 0) {
            init();
            for(int i=0; i<4086; i++)
                clusters_atualizados[i] = 0;
        }else if(strcmp(comando, "load") == 0){
            load();
            for(int i=0; i<4086; i++)
                clusters_atualizados[i] = 0;
        }else if(strcmp(comando, "mkdir") == 0){
            char destino[100];
            scanf("%s", destino);
            if(destino[0] != '/'){
                arruma_destino_lido(destino);
            }
            int temp = mkdir_create(root_dir, destino, 0);
            lixo(destino);
        }else if(strcmp(comando, "ls") == 0){
            char destino[100];
            scanf("%s", destino);
            if(destino[0] != '/'){
                arruma_destino_lido(destino);
            }
            ls(root_dir, destino);
            lixo(destino);
        }else if(strcmp(comando, "create") == 0){
            char destino[100];
            scanf("%s", destino);
            if(destino[0] != '/'){
                arruma_destino_lido(destino);
            }
            int temp = mkdir_create(root_dir, destino, 1);
            lixo(destino);
        }else if(strcmp(comando, "unlink") == 0){
            char destino[100];
            scanf("%s", destino);
            if(destino[0] != '/'){
                arruma_destino_lido(destino);
            }
            int temp = excluir(root_dir, destino);
            lixo(destino);
        }else if(strcmp(comando, "read") == 0){
            char destino[100];
            scanf("%s", destino);
            if(destino[0] != '/'){
                arruma_destino_lido(destino);
            }
            read(root_dir, destino);
            lixo(destino);
        }else if(strcmp(comando, "write") == 0){
            char destino[100];
            char frase[100];
            scanf("%s", frase);
            scanf("%s", destino);
            if(destino[0] != '/'){
                arruma_destino_lido(destino);
            }
            write(root_dir, destino, frase);
            lixo(destino);
        }else if(strcmp(comando, "append") == 0){
            char destino[100];
            char frase[100];
            scanf("%s", frase);
            scanf("%s", destino);
            if(destino[0] != '/'){
                arruma_destino_lido(destino);
            }
            append(root_dir, destino, frase);
            lixo(destino);
        }else if(strcmp(comando, "exit") == 0 ){
            fechar_salvar();
            break;
        }else if(strcmp(comando, "help") == 0){
            help();
        }else{
            printf("/> Err: Comando invalido, para ajuda digite help\n");
        }
    }while(1);
    return 0;
}