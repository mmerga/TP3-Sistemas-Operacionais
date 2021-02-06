//Julio Hebert Silva
//Tomaz Miranda de Oliveira
#include "fat.h"



// Retorna qual posicao da Fat - qual cluster - esta vazio
int pos_vazia_fat();

// Conta quantas barras tem no Caminho lido do shell, cada barra significa um diretorio
// Caso seja passado " /src/home"  sera retornado o valor 2
int barras(char* destino);

// Retorna qual posicao do diretorio Pai esta vazia, valores variam de -2 a 31 
//-2 Existe ARQ ou DIR com mesmo nome ; -1 Esta cheio ; 0 ~ 31 posicao vazia
int pos_vazia_pai(dir_entry_t pai[ENTRY_BY_CLUSTER], char* destino);

// Divide a String Destino em outras duas, dir_pai recebe do começo ate antes da primeira / e o dir_filho recebe o restante
// Caso seja passado " /novo/nome", dir_pai = /novo  e  dir_filho = /nome 
void arruma_destinos(char* destino, char* dir_pai, char* dir_filho);

// Excluir da Fat Destino, fucao recursiva, navega na tabela ate achar o diretorio pai do destino
// Caso seja lido do terminal " unlink novo/nome", o ARQ (ou DIR) /nome sera apagado da memoria, caso seja um DIR e nao estiver vazio, o mesmo nao ser apagado
int excluir(dir_entry_t pai[ENTRY_BY_CLUSTER], char* destino);

// Escreve no terminal o que esta escrito no arquivo passado, fucao recursiva, navega na tabela ate achar o diretorio pai do destino
// Caso seja lido do terminal " read novo/nome", sera escrito os dados do arquivo /nome do diretorio  /novo se existir
void read(dir_entry_t pai[ENTRY_BY_CLUSTER], char* destino);

// Concatena a frase lida do terminal com o que ja estava no arquivo em questao, fucao recursiva, navega na tabela ate achar o diretorio pai do destino
// Caso seja lido do terminal " append Hebert novo/nome", sera feito uma (cat) concatenacao com o arquivo /nome do diretorio /novo, se existir, 
// e passa a conter o valor JulioHebert
void append(dir_entry_t pai[ENTRY_BY_CLUSTER], char* destino, char* frase);

// Escreve no arquivo a frase lida do terminal, fucao recursiva, navega na tabela ate achar o diretorio pai do destino
// Caso seja lido do terminal " write Julio novo/nome", sera colocado Julio no arquivo /nome do diretorio /novo se existir
void write(dir_entry_t pai[ENTRY_BY_CLUSTER], char* destino, char* frase);

// Cria diretorios ou arquivo no diretorio passado , fucao recursiva, navega na tabela ate achar o diretorio pai do destino
// Caso seja lido do terminal " mkdir novo", será criado a pasta /novo no diretorio raiz
// Caso seja lido do terminal " create novo/nome", será criado um arquivo nome no diretorio /novo se o diretorio /novo existir
int mkdir_create(dir_entry_t pai[ENTRY_BY_CLUSTER], char* destino, int tipo);

// Escreve no terminal todos os ARQ ou DIR da pasta passada como referencia, fucao recursiva, navega na tabela ate achar o diretorio pai do destino
// Caso seja lido do terminal " ls /" , ele ira imprimir os ARQ/DIR do diretorio raiz se existir algum
// Caso seja lido do terminal " ls novo", ele ira imprimir os ARQ/DIR do diretorio /novo se existir
void ls(dir_entry_t pai[ENTRY_BY_CLUSTER], char* destino);

//Escreve no terminais todos os possiveis comandos do shell
void help();