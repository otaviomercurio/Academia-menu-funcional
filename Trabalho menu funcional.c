#include <stdio.h>      
#include <stdlib.h>     
#include <string.h>     
#include <ctype.h>
#include <locale.h>

// Defini��o da estrutura Cliente
struct Cliente {
    char nome[50];
    char telefone[20];
    char plano[20]; // Exemplo: mensal, semestral ou anual
    char status;    // 'A' para ativo, 'I' para inativo
};

// Prototipa��o das fun��es
int tamanho(FILE *arq);
void cadastrar(FILE *arq);
void consultar(FILE *arq);
void geraRelatorio(FILE *arq);
void excluir(FILE *arq);

int main() {
	setlocale(LC_ALL, "Portuguese");
    int op;
    FILE *arq;

    // Abrir o arquivo bin�rio, criar caso n�o exista
    if ((arq = fopen("clientes.dat", "rb+")) == NULL) {
        if ((arq = fopen("clientes.dat", "wb+")) == NULL) {
            printf("Erro ao abrir ou criar o arquivo!\n");
            return 1;
        }
    }

    do {
        system("CLS");
        printf("\n===== Gerenciador de Clientes de Academia =====\n");
        printf("1. Cadastrar Cliente\n");
        printf("2. Consultar Cliente\n");
        printf("3. Gerar Relat�rio\n");
        printf("4. Excluir Cliente\n");
        printf("5. Sair\n");
        printf("===============================================\n");
        printf("Clientes cadastrados: %d\n", tamanho(arq));
        printf("Escolha uma op��o: ");
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
                printf("Op��o inv�lida! Tente novamente.\n");
                break;
        }
    } while (op != 5);

    return 0;
}

// Retorna o n�mero de registros no arquivo
int tamanho(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    return ftell(arq) / sizeof(struct Cliente);
}

// Fun��o para cadastrar um cliente
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
    cliente.status = 'A'; // Cliente come�a como ativo

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

// Fun��o para consultar um cliente por c�digo
void consultar(FILE *arq) {
    struct Cliente cliente;
    int id;

    printf("\nConsulta por C�digo:\n");
    printf("Digite o c�digo do cliente: ");
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
        printf("\nC�digo inv�lido.\n");
    }
    system("pause");
}

// Fun��o para gerar um relat�rio
void geraRelatorio(FILE *arq) {
    FILE *arqtxt = fopen("relatorio_clientes.txt", "w");
    struct Cliente cliente;

    if (!arqtxt) {
        printf("Erro ao criar arquivo de relat�rio!\n");
        return;
    }

    fprintf(arqtxt, "Relat�rio de Clientes Ativos\n");
    fprintf(arqtxt, "-------------------------------------------\n");
    fprintf(arqtxt, "Nome                Telefone         Plano\n");
    fprintf(arqtxt, "-------------------------------------------\n");

    int i; // Declara��o da vari�vel fora do loop
    for (i = 0; i < tamanho(arq); i++) { // Usando a vari�vel declarada anteriormente
        fseek(arq, i * sizeof(struct Cliente), SEEK_SET);
        fread(&cliente, sizeof(struct Cliente), 1, arq);

        if (cliente.status == 'A') {
            fprintf(arqtxt, "%-20s %-15s %-10s\n", cliente.nome, cliente.telefone, cliente.plano);
        }
    }

    fclose(arqtxt);
    printf("Relat�rio gerado com sucesso: relatorio_clientes.txt\n");
    system("pause");
}

// Fun��o para excluir um cliente (marcar como inativo)
void excluir(FILE *arq) {
    struct Cliente cliente;
    int id;
    char confirma;

    printf("\nExcluir Cliente:\n");
    printf("Digite o c�digo do cliente: ");
    scanf("%d", &id);

    if (id > 0 && id <= tamanho(arq)) {
        fseek(arq, (id - 1) * sizeof(struct Cliente), SEEK_SET);
        fread(&cliente, sizeof(struct Cliente), 1, arq);

        if (cliente.status == 'A') {
            printf("\n--- Dados do Cliente ---\n");
            printf("Nome: %s\n", cliente.nome);
            printf("Telefone: %s\n", cliente.telefone);
            printf("Plano: %s\n", cliente.plano);
            printf("\nConfirmar exclus�o? (s/n): ");
            getchar();
            scanf(" %c", &confirma);

            if (toupper(confirma) == 'S') {
                cliente.status = 'I';
                fseek(arq, (id - 1) * sizeof(struct Cliente), SEEK_SET);
                fwrite(&cliente, sizeof(struct Cliente), 1, arq);
                printf("\nCliente exclu�do com sucesso!\n");
            } else {
                printf("\nExclus�o cancelada.\n");
            }
        } else {
            printf("\nCliente j� est� inativo.\n");
        }
    } else {
        printf("\nC�digo inv�lido.\n");
    }
    system("pause");
}

