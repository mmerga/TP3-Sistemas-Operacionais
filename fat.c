//Julio Hebert Silva
//Tomaz Miranda de Oliveira
#include "fat.h"

void init(void)
{
	FILE* ptr_file;
	int i;
	ptr_file = fopen(fat_name,"wb");
	for (i = 0; i < 2; ++i)
		boot_block[i] = 0xbb;
	fat[0] = 0xfffd;	// boot block
	for (i = 1; i < 9; ++i)
		fat[i] = 0xfffe;	// FAT
	fat[9] = 0xffff;	// fim do arquivo 
	for (i = 10; i < NUM_CLUSTER; ++i)
		fat[i] = 0x0000;	// cluster livre
	fwrite(&boot_block, sizeof(boot_block), 1,ptr_file);
	fwrite(&fat, sizeof(fat), 1, ptr_file);
	for(int i = 0 ; i < ENTRY_BY_CLUSTER; i++){
		root_dir[i].size = 0;
	}
	root_dir->size=1;
	for(int i =0; i <ENTRY_BY_CLUSTER; i++){
		root_dir[i].size = 0;
		root_dir[i].filename[0] = '\0';
	}
	fwrite(&root_dir, sizeof(root_dir), 1,ptr_file);
	for (i = 0; i < 4086; ++i)
		fwrite(&clusters, sizeof(data_cluster), 1, ptr_file);
	fclose(ptr_file);
}

void load()
{
	FILE* ptr_file;
	int i;
	ptr_file = fopen(fat_name, "rb");
	fseek(ptr_file, sizeof(boot_block), SEEK_SET);
	fread(fat, sizeof(fat), 1, ptr_file);
	fread(root_dir, sizeof(root_dir), 1, ptr_file);
	fclose(ptr_file);
}

void fechar_salvar(){
	FILE* ptr_file;
	int i;
	ptr_file = fopen(fat_name, "r+b");
	fwrite(&boot_block, sizeof(boot_block), 1,ptr_file);
	fwrite(&fat, sizeof(fat), 1, ptr_file);
	fwrite(&root_dir, sizeof(root_dir), 1,ptr_file);
	for (i = 0; i < 4086; ++i)
		if(clusters_atualizados[i] == 1){
			fwrite(&clusters[i], sizeof(data_cluster), 1, ptr_file);
		}else{
			fseek(ptr_file, sizeof(data_cluster)*(i), SEEK_CUR);
		}
	fclose(ptr_file);
}

void integridade(int index){
	FILE* ptr_file;
    ptr_file = fopen(fat_name, "r+b");
    fseek(ptr_file, sizeof(boot_block), SEEK_SET);
	fwrite(&fat, sizeof(fat), 1, ptr_file);
    fseek(ptr_file, sizeof(root_dir), SEEK_CUR);
    fseek(ptr_file, sizeof(data_cluster)*(index), SEEK_CUR); 
    fwrite(&clusters[index], sizeof(data_cluster), 1, ptr_file);
    fclose(ptr_file);
}

void existe_fat(){
    FILE *fp;
	fp=fopen(fat_name,"rb");
	if(fp){ // Fat Existe
		fclose(fp);
		load();
	}else{	// Fat Nao Existe
		init();
		load();
	}
}

data_cluster cluster_da_memoria(int index){
    FILE* ptr_file;
    ptr_file = fopen(fat_name, "rb");
    fseek(ptr_file, sizeof(boot_block), SEEK_SET);
    fseek(ptr_file, sizeof(fat), SEEK_CUR);
    fseek(ptr_file, sizeof(root_dir), SEEK_CUR);
    fseek(ptr_file, sizeof(data_cluster)*(index), SEEK_CUR); 
    data_cluster cluster;
    fread(&cluster, sizeof(data_cluster), 1, ptr_file);
    fclose(ptr_file);
    return cluster;
}