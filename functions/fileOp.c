//
// Created by dio on 11/12/23.
//

#include "funcs.h"
#include "../defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char TableNames[MAX_TABLE_NAME][MAX_NUM_TABLES];

Type stringToType(char *type) {
    if (strcmp(type, "INT") == 0) {
        return INT;
    } else if (strcmp(type, "FLOAT") == 0) {
        return FLOAT;
    } else if (strcmp(type, "DOUBLE") == 0) {
        return DOUBLE;
    } else if (strcmp(type, "CHAR") == 0) {
        return CHAR;
    } else if (strcmp(type, "STRING") == 0) {
        return STRING;
    }
    return -1;  // Se o tipo de dado não for reconhecido
}

void readMain() {
    FILE *file = fopen("main.txt", "r");
    char tableName[MAX_TABLE_NAME];
    int i = 0;

    while (fscanf(file, "%s", tableName) != EOF) {
        strcpy(TableNames[i], tableName);
        i++;
    }

    fclose(file);
}

void readTable(Table *table, char *tableName) {
    char filename[MAX_TABLE_NAME + 4];
    sprintf(filename, "%s.txt", tableName);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", filename);
        return;
    }

    // Leitura do cabeçalho
    fscanf(file, "Número de Colunas: %d\n", &(table->numColumns));
    fscanf(file, "Número de Registros: %d\n", &(table->numRows));
    fscanf(file, "Chave Primária: %d\n", &(table->primaryKeyIndex));

    // Leitura do nome das colunas
    for (int i = 0; i < table->numColumns; i++) {
        fscanf(file, "%s", table->columns[i].name);
    }
    // Leitura do tipo das colunas
    for (int i = 0; i < table->numColumns; i++) {
        char type[MAX_COLUMN_NAME];
        fscanf(file, "%s", type);
        table->columns[i].type = stringToType(type);
        if (table->columns[i].type == -1) {
            printf("Tipo de dado não reconhecido na coluna %d.\n", i);
            return;
        }
    }

    // Leitura dos dados com base no modelo atual de colunas
    for (int i = 0; i < table->numRows; i++) {
        char line[MAX_STRING_SIZE];
        fgets(line, MAX_STRING_SIZE, file);

        char *token = strtok(line, ",");
        for (int j = 0; j < table->numColumns; j++) {
            switch (table->columns[j].type) {
                case INT:
                    table->columns[j].Data.intData[i] = atoi(token);
                    break;
                case FLOAT:
                    table->columns[j].Data.floatData[i] = atof(token);
                    break;
                case DOUBLE:
                    table->columns[j].Data.doubleData[i] = atof(token);
                    break;
                case CHAR:
                    table->columns[j].Data.charData[i] = token[0];
                    break;
                case STRING:
                    strcpy(table->columns[j].Data.stringData[i], token);
                    break;
            }
            token = strtok(NULL, ",");
        }
    }

    fclose(file);
}