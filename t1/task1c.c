#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;

typedef struct link link;
struct link {
    link *nextVirus;
    virus *vir;
};

struct fun_desc {
    char *name;
    link* (*fun)(link*, char*);
};


virus* readVirus(FILE* virFile){
    virus *newVirus=(virus*) calloc(1, sizeof(virus));
    if(fread(newVirus, 1, 18, virFile) <= 0){
        free(newVirus);
        return NULL;
    }
    newVirus->sig=(unsigned char*) calloc((newVirus->SigSize), sizeof(char));
    fread(newVirus->sig,1, newVirus->SigSize, virFile);
    return newVirus;
}


void printVirus(virus* virus, FILE* output){
    fprintf(output, "Virus name: %s\n", virus->virusName);
    fprintf(output, "Virus size: %d\n", virus->SigSize);
    fprintf(output, "signature:\n");
    for(int i = 0; i < virus->SigSize; i++)
        fprintf(output, "%02hhX ", virus->sig[i]);
    fprintf(output, "\n\n");
}

void list_print(link *virus_list, FILE* output){
    while(virus_list != NULL){
        printVirus(virus_list->vir, output);
        virus_list = virus_list->nextVirus;
    }
}

link* list_append(link* virus_list, virus* data){
    link* newVirus = calloc(sizeof(virus), 1);
    newVirus->vir = data;
    newVirus->nextVirus = NULL;
    if(virus_list == NULL){
        return newVirus;
    } else {
        newVirus->nextVirus = virus_list;
        return newVirus;
    }
}

void list_free(link *virus_list){
    if(virus_list != NULL){
        list_free(virus_list->nextVirus);
        free(virus_list->vir->sig);
        free(virus_list->vir);
        free(virus_list);
    }
}

void detect_virus(char *buffer, unsigned int size, link *virus_list){

    //loop over all viruses
    while(virus_list != NULL){
       //check which virus exists in the buffer
        for(int i = 0; i < size; i++){
            unsigned short virSize=virus_list->vir->SigSize;
            if(memcmp(virus_list->vir->sig, buffer+i, virSize) == 0){
                printf("Virus location: %d(0x%x)\n", i, i);
                printf("Virus name: %s\n", virus_list->vir->virusName);
                printf("Virus size: %d\n\n", virSize);
            }
        }
        virus_list = virus_list->nextVirus;
    } 
}

link* load_signatures(link* virus_list, char* fileName){
    FILE* file = fopen("signatures-L", "r");
    char buffer[4];
    fread(&buffer[0], 1, 4, file);
    virus* virus;
    while((virus= readVirus(file)) != NULL){
        virus_list = list_append(virus_list, virus);
    }
    fclose(file);
    return virus_list;
}

link* print_viruses(link* virus_list, char* fileName){
    FILE* output = fopen("outputF.txt", "w");
    list_print(virus_list, output);
    fclose(output);
    return virus_list;
}

link* detect_virus_func(link* virus_list, char* fileName){
    FILE* infectedFile = fopen(fileName, "r");
    char buffer[10000];
    int fileSize = fread(buffer, 1, 10000, infectedFile);
    detect_virus(buffer, fileSize, virus_list);
    fclose(infectedFile);
    return virus_list;
}

link* fix_file(link* virus_list, char* fileName){
    printf("Not implemented\n");
    return virus_list;
}

link* quit_menu(link* virus_list, char* fileName){
    list_free(virus_list);
    exit(1);
}

struct fun_desc menuOptions[] = { {"Load signatures", load_signatures}, {"Print viruses", print_viruses}
,{"Detect viruses",detect_virus_func},{"Fix File",fix_file},{"Quit",quit_menu},{NULL,NULL}};

int menuSize = sizeof(menuOpts) / sizeof(struct fun_desc) -1;

int main(int argc, char **argv){
    char input[100];
    int num;
    link* virus_list = NULL;
    while (1)
    {
        printf("Please choose a function:\n");
        for(int i = 1; i <= menuSize; i++){
            printf("%d) %s\n", i, menuOptions[i-1].name);
        }
        printf("Option: ");
        fgets(input, 100, stdin);
        sscanf(input, "%d\n", &num);
        if((num > menuSize)|(num < 1)){
            printf("Not within bounds\n");
            list_free(virus_list);
            exit(1);
        } 
        printf("Within bounds\n");
        virus_list = menuOptions[num-1].fun(virus_list, argv[1]);
    }
    return 0;
}
