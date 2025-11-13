#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO "pessoas.bin"

typedef struct {
    char nome[100];
    char cpf[12];
    int idade;
    char email[100];
} Pessoa;

int cpfExiste(const char *cpf) {
    FILE *file = fopen(ARQUIVO, "rb");
    if (!file) return 0;

    Pessoa p;
    while (fread(&p, sizeof(Pessoa), 1, file)) {
        if (strcmp(p.cpf, cpf) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void cadastrarPessoa() {
    Pessoa p;
    FILE *file = fopen(ARQUIVO, "ab");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    printf("=== Cadastrar nova pessoa ===\n");
    printf("CPF (11 digitos): ");
    scanf(" %11s", p.cpf);

    if (cpfExiste(p.cpf)) {
        printf("CPF ja cadastrado!\n");
        fclose(file);
        return;
    }

    getchar();
    printf("Nome: ");
    fgets(p.nome, sizeof(p.nome), stdin);
    p.nome[strcspn(p.nome, "\n")] = '\0';

    printf("Idade: ");
    scanf("%d", &p.idade);

    printf("E-mail: ");
    scanf(" %99s", p.email);

    fwrite(&p, sizeof(Pessoa), 1, file);
    fclose(file);
    printf("Pessoa cadastrada com sucesso!\n");
}

void listarPessoas() {
    FILE *file = fopen(ARQUIVO, "rb");
    if (!file) {
        printf("Nenhum cadastro encontrado.\n");
        return;
    }

    Pessoa p;
    printf("\n=== Lista de Pessoas ===\n");
    while (fread(&p, sizeof(Pessoa), 1, file)) {
        printf("Nome: %s\nCPF: %s\nIdade: %d\nE-mail: %s\n-------------------------\n",
               p.nome, p.cpf, p.idade, p.email);
    }
    fclose(file);
}

void buscarPorCPF() {
    char cpf[12];
    printf("Digite o CPF a buscar: ");
    scanf(" %11s", cpf);

    FILE *file = fopen(ARQUIVO, "rb");
    if (!file) {
        printf("Arquivo inexistente.\n");
        return;
    }

    Pessoa p;
    int encontrado = 0;
    while (fread(&p, sizeof(Pessoa), 1, file)) {
        if (strcmp(p.cpf, cpf) == 0) {
            printf("\nPessoa encontrada:\n");
            printf("Nome: %s\nCPF: %s\nIdade: %d\nE-mail: %s\n",
                   p.nome, p.cpf, p.idade, p.email);
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) printf("CPF nao encontrado.\n");
    fclose(file);
}

void atualizarPessoa() {
    char cpf[12];
    printf("Digite o CPF da pessoa a atualizar: ");
    scanf(" %11s", cpf);

    FILE *file = fopen(ARQUIVO, "r+b");
    if (!file) {
        printf("Arquivo nao encontrado.\n");
        return;
    }

    Pessoa p;
    int encontrado = 0;
    while (fread(&p, sizeof(Pessoa), 1, file)) {
        if (strcmp(p.cpf, cpf) == 0) {
            encontrado = 1;

            getchar(); // limpar buffer
            printf("Novo nome: ");
            fgets(p.nome, sizeof(p.nome), stdin);
            p.nome[strcspn(p.nome, "\n")] = '\0';

            printf("Nova idade: ");
            scanf("%d", &p.idade);

            printf("Novo e-mail: ");
            scanf(" %99s", p.email);

            fseek(file, -sizeof(Pessoa), SEEK_CUR);
            fwrite(&p, sizeof(Pessoa), 1, file);
            printf("Dados atualizados com sucesso!\n");
            break;
        }
    }
    if (!encontrado) printf("CPF nao encontrado.\n");
    fclose(file);
}

void removerPessoa() {
    char cpf[12];
    printf("Digite o CPF da pessoa a remover: ");
    scanf(" %11s", cpf);

    FILE *file = fopen(ARQUIVO, "rb");
    FILE *temp = fopen("temp.bin", "wb");

    if (!file || !temp) {
        printf("Erro ao abrir arquivo.\n");
        return;
    }

    Pessoa p;
    int removido = 0;
    while (fread(&p, sizeof(Pessoa), 1, file)) {
        if (strcmp(p.cpf, cpf) == 0) {
            removido = 1;
            continue;
        }
        fwrite(&p, sizeof(Pessoa), 1, temp);
    }

    fclose(file);
    fclose(temp);
    remove(ARQUIVO);
    rename("temp.bin", ARQUIVO);

    if (removido)
        printf("Pessoa removida com sucesso!\n");
    else
        printf("CPF nao encontrado.\n");
}

int main() {
    int opcao;
    do {
        printf("\n===== MENU =====\n");
        printf("1. Cadastrar pessoa\n");
        printf("2. Listar pessoas\n");
        printf("3. Buscar por CPF\n");
        printf("4. Atualizar pessoa\n");
        printf("5. Remover pessoa\n");
        printf("6. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: cadastrarPessoa(); break;
            case 2: listarPessoas(); break;
            case 3: buscarPorCPF(); break;
            case 4: atualizarPessoa(); break;
            case 5: removerPessoa(); break;
            case 6: printf("Saindo...\n"); break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 6);

    return 0;
}
