#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

// Definição da estrutura Cliente
struct Cliente {
    char nome[50];
    char telefone[20];
    char plano[20];
    char status; // 'A' para ativo, 'I' para inativo
};

// Prototipação das funções
int tamanho(FILE *arq);
void cadastrar(FILE *arq);
void consultar(FILE *arq);
void geraRelatorio(FILE *arq);
void excluir(FILE *arq);

int main() {
    setlocale(LC_ALL, "Portuguese");
    int op;
    FILE *arq;

    if ((arq = fopen("clientes.dat", "rb+")) == NULL) {
        if ((arq = fopen("clientes.dat", "wb+")) == NULL) {
            printf("Erro ao abrir ou criar o arquivo!\n");
            return 1;
        }
    }

    do {
        system("CLS"); // Para Windows; use system("clear") em Linux/macOS
        printf("\n===== Gerenciador de Clientes de Academia =====\n");
        printf("1. Cadastrar Cliente\n");
        printf("2. Consultar Cliente\n");
        printf("3. Gerar Relatório\n");
        printf("4. Excluir Cliente\n");
        printf("5. Sair\n");
        printf("===============================================\n");
        printf("Clientes ativos cadastrados: %d\n", tamanho(arq));
        printf("Escolha uma opção: ");
        scanf("%d", &op);

        switch (op) {
            case 1:
                cadastrar(arq);
                break;
            case 2:
                consultar(arq);
                break;
            case 3:
                geraRelatorio(arq);
                break;
            case 4:
                excluir(arq);
                break;
            case 5:
                fclose(arq);
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
                system("pause");
                break;
        }
    } while (op != 5);

    return 0;
}

// Função corrigida para contar apenas clientes ativos
int tamanho(FILE *arq) {
    struct Cliente cliente;
    int count = 0;

    rewind(arq); // Volta ao início do arquivo
    while (fread(&cliente, sizeof(struct Cliente), 1, arq)) {
        if (cliente.status == 'A') {
            count++;
        }
    }

    return count;
}

void cadastrar(FILE *arq) {
    struct Cliente cliente;
    char confirma;

    printf("\nCadastro de Novo Cliente:\n");
    printf("Nome: ");
    fflush(stdin);
    gets(cliente.nome);
    printf("Telefone: ");
    gets(cliente.telefone);
    printf("Plano (mensal/semestral/anual): ");
    gets(cliente.plano);
    cliente.status = 'A';

    printf("\nConfirmar cadastro? (s/n): ");
    scanf(" %c", &confirma);

    if (toupper(confirma) == 'S') {
        fseek(arq, 0, SEEK_END);
        fwrite(&cliente, sizeof(struct Cliente), 1, arq);
        printf("\nCliente cadastrado com sucesso!\n");
    } else {
        printf("\nCadastro cancelado.\n");
    }
    system("pause");
}

void consultar(FILE *arq) {
    struct Cliente cliente;
    int id;

    printf("\nConsulta por Código:\n");
    printf("Digite o código do cliente: ");
    scanf("%d", &id);

    if (id > 0 && id <= tamanho(arq)) {
        fseek(arq, (id - 1) * sizeof(struct Cliente), SEEK_SET);
        fread(&cliente, sizeof(struct Cliente), 1, arq);

        if (cliente.status == 'A') {
            printf("\n--- Dados do Cliente ---\n");
            printf("Nome: %s\n", cliente.nome);
            printf("Telefone: %s\n", cliente.telefone);
            printf("Plano: %s\n", cliente.plano);
            printf("Status: Ativo\n");
        } else {
            printf("\nCliente inativo ou inexistente.\n");
        }
    } else {
        printf("\nCódigo inválido.\n");
    }
    system("pause");
}

void geraRelatorio(FILE *arq) {
    FILE *arqtxt = fopen("relatorio_clientes.txt", "w");
    struct Cliente cliente;
    int i;

    if (!arqtxt) {
        printf("Erro ao criar arquivo de relatório!\n");
        return;
    }

    fprintf(arqtxt, "Relatório de Clientes Ativos\n");
    fprintf(arqtxt, "-------------------------------------------\n");
    fprintf(arqtxt, "Nome                Telefone         Plano\n");
    fprintf(arqtxt, "-------------------------------------------\n");

    rewind(arq); // Volta ao início do arquivo
    while (fread(&cliente, sizeof(struct Cliente), 1, arq)) {
        if (cliente.status == 'A') {
            fprintf(arqtxt, "%-20s %-15s %-10s\n", cliente.nome, cliente.telefone, cliente.plano);
        }
    }

    fclose(arqtxt);
    printf("Relatório gerado com sucesso: relatorio_clientes.txt\n");
    system("pause");
}

void excluir(FILE *arq) {
    struct Cliente cliente;
    int id;
    char confirma;

    printf("\nExcluir Cliente:\n");
    printf("Digite o código do cliente: ");
    scanf("%d", &id);

    if (id > 0 && id <= tamanho(arq)) {
        fseek(arq, (id - 1) * sizeof(struct Cliente), SEEK_SET);
        fread(&cliente, sizeof(struct Cliente), 1, arq);

        if (cliente.status == 'A') {
            printf("\n--- Dados do Cliente ---\n");
            printf("Nome: %s\n", cliente.nome);
            printf("Telefone: %s\n", cliente.telefone);
            printf("Plano: %s\n", cliente.plano);
            printf("\nConfirmar exclusão? (s/n): ");
            getchar();
            scanf(" %c", &confirma);

            if (toupper(confirma) == 'S') {
                cliente.status = 'I';
                fseek(arq, (id - 1) * sizeof(struct Cliente), SEEK_SET);
                fwrite(&cliente, sizeof(struct Cliente), 1, arq);
                printf("\nCliente excluído com sucesso!\n");
            } else {
                printf("\nExclusão cancelada.\n");
            }
        } else {
            printf("\nCliente já está inativo.\n");
        }
    } else {
        printf("\nCódigo inválido.\n");
    }
    system("pause");
}
