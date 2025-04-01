#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONSULTAS 100
#define FILE_NAME "consultas.dat"

typedef struct {
    int id;
    char nomePaciente[50];
    char data[20];  // formato: dd/mm/aaaa
    char hora[10];  // formato: hh:mm
    char tipoConsulta[50];  // Ex: Consulta Geral, Oftalmologia, etc.
} Consulta;

void salvarConsultas(Consulta consultas[], int count) {
    FILE *file = fopen(FILE_NAME, "wb");
    if (file == NULL) {
        perror("Erro ao salvar as consultas");
        return;
    }
    fwrite(&count, sizeof(int), 1, file);
    fwrite(consultas, sizeof(Consulta), count, file);
    fclose(file);
}

int carregarConsultas(Consulta consultas[]) {
    FILE *file = fopen(FILE_NAME, "rb");
    int count = 0;
    if (file != NULL) {
        fread(&count, sizeof(int), 1, file);
        fread(consultas, sizeof(Consulta), count, file);
        fclose(file);
    }
    return count;
}

void agendarConsulta(Consulta consultas[], int *count) {
    if (*count >= MAX_CONSULTAS) {
        printf("Agenda cheia!\n");
        return;
    }
    Consulta c;
    c.id = *count + 1;
    printf("Nome do Paciente: ");
    scanf(" %[^\n]s", c.nomePaciente);
    printf("Data da consulta (dd/mm/aaaa): ");
    scanf(" %[^\n]s", c.data);
    printf("Hora da consulta (hh:mm): ");
    scanf(" %[^\n]s", c.hora);
    printf("Tipo de consulta: ");
    scanf(" %[^\n]s", c.tipoConsulta);
    
    consultas[*count] = c;
    (*count)++;
    salvarConsultas(consultas, *count);
    printf("Consulta agendada com sucesso!\n");
}

void listarConsultas(Consulta consultas[], int count) {
    if (count == 0) {
        printf("Nenhuma consulta agendada.\n");
        return;
    }
    printf("\nConsultas Agendadas:\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d | Paciente: %s | Data: %s | Hora: %s | Tipo: %s\n",
               consultas[i].id, consultas[i].nomePaciente, consultas[i].data, consultas[i].hora, consultas[i].tipoConsulta);
    }
}

void cancelarConsulta(Consulta consultas[], int *count) {
    if (*count == 0) {
        printf("Nenhuma consulta para cancelar.\n");
        return;
    }
    int id;
    printf("Informe o ID da consulta a ser cancelada: ");
    if (scanf("%d", &id) != 1 || id <= 0 || id > *count) {
        printf("ID inválido!\n");
        return;
    }
    
    for (int i = 0; i < *count; i++) {
        if (consultas[i].id == id) {
            for (int j = i; j < *count - 1; j++) {
                consultas[j] = consultas[j + 1];
            }
            (*count)--;
            salvarConsultas(consultas, *count);
            printf("Consulta cancelada com sucesso!\n");
            return;
        }
    }
    printf("Consulta não encontrada!\n");
}

int main() {
    Consulta consultas[MAX_CONSULTAS];
    int count = carregarConsultas(consultas);
    int opcao;

    do {
        printf("\nMenu:\n");
        printf("1. Agendar Consulta\n");
        printf("2. Listar Consultas\n");
        printf("3. Cancelar Consulta\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1 || opcao < 1 || opcao > 4) {
            printf("Entrada inválida!\n");
            while (getchar() != '\n');
            continue;
        }
        switch (opcao) {
            case 1:
                agendarConsulta(consultas, &count);
                break;
            case 2:
                listarConsultas(consultas, count);
                break;
            case 3:
                cancelarConsulta(consultas, &count);
                break;
            case 4:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 4);

    return 0;
}
