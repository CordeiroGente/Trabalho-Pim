#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <conio.h>
#define MAX_SENHA 10
#define MAX_LOGIN 50
#define CLEAN_BUFF do{ int c; while((c = getchar()) != '\n' && c != EOF);}while(0)

int autenticado = 0;

typedef struct{
    int id[20];
    int quantidade[20];
    char nome[20];
} item; item it[20];

int Usuario( FILE* file, char* user, char* senha ) //valida usuario
{
	char tmpLogin[MAX_LOGIN];
	char tmpSenha[MAX_SENHA];

	fscanf(file, "%s", tmpLogin);

	while( !feof(file) )
	{
		if( !strcmp(tmpLogin, user))
		{
			fscanf(file, "%s", tmpSenha);

			if( !strcmp(tmpSenha, senha))
				return 1;
		}
		else
		{
			fscanf(file, "%*s");
		}

		fscanf(file, "%s", tmpLogin);
	}

	return 0;
}

char* CriaSenha() {
    char *senha = (char *)malloc(MAX_LOGIN * sizeof(char));
    int i = 0;
    char ch;

    while (1) {
        ch = getch(); // Captura o caractere sem exibi-lo

        if (ch == 8 || ch == 127) { // Tratamento para backspace
            if (i > 0) {
                i--; // Remove o último caractere da senha
                printf("\b \b"); // Apaga o '*' anterior na tela
            }
        } else if (ch == '\r') { // Pressiona Enter para finalizar
            senha[i] = '\0';
            printf("\n"); // Pula para a próxima linha
            break;
        } else if (i < MAX_LOGIN - 1 && ch >= 32 && ch <= 126) { 
            // Adiciona apenas caracteres válidos
            senha[i] = ch;
            printf("*"); // Mostra '*' na tela
            i++;
        }
    }
    return senha;
}

void Menu_de_itens() {
    int opcao;
    do {
        system("cls");
        printf("\n\tModificacao de Itens:\n");
        printf("1. Adicionar item.\n");
        printf("2. Excluir item.\n");
        printf("3. Abrir lista de itens.\n");
        printf("4. Retornar ao Menu.\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do teclado

        switch (opcao) {
            case 1: { // Adicionar item
                FILE *file = fopen("Itens.txt", "a");
                if (file == NULL) {
                    printf("Erro ao abrir o arquivo.\n");
                    break;
                }

                int id;
                char nome[50];
                float preco;
                int estoque; // Nova variável para estoque

                printf("Digite o ID do novo item: ");
                scanf("%d", &id);
                getchar();

                printf("Digite o nome do novo item: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';

                printf("Digite o preco do novo item: ");
                scanf("%f", &preco);

                printf("Digite o estoque do novo item: "); // Solicitar estoque
                scanf("%d", &estoque);

                // Adiciona o estoque no arquivo
                fprintf(file, "%d,%s,%.2f,%d\n", id, nome, preco, estoque);
                fclose(file);
                printf("Item adicionado com sucesso!\n");
                sleep(2);
                break;
            }

            case 2: { // Excluir item
                FILE *file = fopen("Itens.txt", "r");
                if (file == NULL) {
                    printf("Erro ao abrir o arquivo.\n");
                    break;
                }

                FILE *temp = fopen("Temp.txt", "w");
                if (temp == NULL) {
                    printf("Erro ao criar o arquivo temporario.\n");
                    fclose(file);
                    break;
                }

                int id, idExcluir, encontrado = 0;
                char nome[50];
                float preco;
                int estoque;

                printf("Digite o ID do item a ser excluido: ");
                scanf("%d", &idExcluir);

                while (fscanf(file, "%d,%49[^,],%f,%d\n", &id, nome, &preco, &estoque) == 4) {
                    if (id == idExcluir) {
                        encontrado = 1;
                        continue; // Pula o item a ser excluído
                    }
                    fprintf(temp, "%d,%s,%.2f,%d\n", id, nome, preco, estoque);
                }

                fclose(file);
                fclose(temp);

                remove("Itens.txt");
                rename("Temp.txt", "Itens.txt");

                if (encontrado)
                    printf("Item excluido com sucesso!\n");
                else
                    printf("Item não encontrado.\n");

                break;
            }

            case 3: { // Abrir lista de itens
                FILE *file = fopen("Itens.txt", "r");
                if (file == NULL) {
                    printf("Erro ao abrir o arquivo.\n");
                    break;
                }

                int id;
                char nome[50];
                float preco;
                int estoque; // Variável para estoque
                char linha[100]; // Buffer para armazenar cada linha

                printf("\nLista de Itens:\n");
                while (fgets(linha, sizeof(linha), file) != NULL) {
                    // Usando strtok para dividir os valores separados por vírgulas
                    char *token = strtok(linha, ",");
                    
                    if (token != NULL) {
                        id = atoi(token); // Primeiro token é o ID
                        token = strtok(NULL, ",");
                    }

                    if (token != NULL) {
                        strncpy(nome, token, sizeof(nome) - 1); // Segundo token é o nome
                        nome[sizeof(nome) - 1] = '\0'; // Garante o término da string
                        token = strtok(NULL, ",");
                    }

                    if (token != NULL) {
                        preco = atof(token); // Terceiro token é o preco
                        token = strtok(NULL, ",");
                    }

                    if (token != NULL) {
                        estoque = atoi(token); // Quarto token é o estoque
                    }

                    printf("ID: %d, Nome: %s, Preco: %.2f, Estoque: %d\n", id, nome, preco, estoque);
                }

                fclose(file);
                
                // Mantém a lista aberta até que o usuário digite algo
                printf("\nPressione Enter para retornar ao menu...");
                getchar(); // Aguarda o Enter do usuário

                break;
            }

            default:
                printf("Opcao inválida. Tente novamente.\n");
        }
    } while (opcao != 4);
}


void Menu_de_estoque() {
    int opcao;
    do {
        system("cls");
        printf("\n\tMenu de Estoque:\n");
        printf("1. Alterar estoque.\n");
        printf("2. Menu de fornecedores.\n");
        printf("3. Modificar Nome do Item.\n");
        printf("4. Retornar ao Menu\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do teclado

        switch (opcao) {
            case 1: { // Alterar estoque
                FILE *file = fopen("Itens.txt", "r");
                if (file == NULL) {
                    printf("Erro ao abrir o arquivo.\n");
                    break;
                }

                FILE *temp = fopen("Temp.txt", "w");
                if (temp == NULL) {
                    printf("Erro ao criar o arquivo temporario.\n");
                    fclose(file);
                    break;
                }

                int id, idAlterar, novoEstoque, encontrado = 0;
                char nome[50];
                float preco;
                int estoque;

                printf("Digite o ID do item para alterar o estoque: ");
                scanf("%d", &idAlterar);

                while (fscanf(file, "%d,%49[^,],%f,%d\n", &id, nome, &preco, &estoque) == 4) {
                    if (id == idAlterar) {
                        encontrado = 1;
                        printf("Estoque atual: %d\n", estoque);
                        printf("Digite o novo estoque: ");
                        scanf("%d", &novoEstoque);
                        estoque = novoEstoque;
                    }
                    fprintf(temp, "%d,%s,%.2f,%d\n", id, nome, preco, estoque);
                }

                fclose(file);
                fclose(temp);

                remove("Itens.txt");
                rename("Temp.txt", "Itens.txt");

                if (encontrado)
                    printf("Estoque alterado com sucesso!\n");
                else
                    printf("Item não encontrado.\n");

                sleep(2);
                break;
            }

           case 2: { // Lista de fornecedores com submenu
    int subOpcao;
    do {
        system("cls");
        printf("\n\tLista de Fornecedores:\n");
        printf("1. Adicionar fornecedor.\n");
        printf("2. Excluir fornecedor.\n");
        printf("3. Exibir lista de fornecedores.\n");
        printf("4. Retornar ao Menu de Estoque.\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &subOpcao);
        getchar(); // Limpa o buffer do teclado

        switch (subOpcao) {
            case 1: { // Adicionar fornecedor
                FILE *file = fopen("Fornecedores.txt", "a");
                if (file == NULL) {
                    printf("Erro ao abrir o arquivo de fornecedores.\n");
                    break;
                }

                char nome[50];
                char contato[50];

                printf("Digite o nome do fornecedor: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0'; // Remove o \n

                printf("Digite o contato do fornecedor: ");
                fgets(contato, sizeof(contato), stdin);
                contato[strcspn(contato, "\n")] = '\0'; // Remove o \n

                fprintf(file, "%s,%s\n", nome, contato);
                fclose(file);

                printf("Fornecedor adicionado com sucesso!\n");
                sleep(2);
                break;
            }

            case 2: { // Excluir fornecedor
                FILE *file = fopen("Fornecedores.txt", "r");
                if (file == NULL) {
                    printf("Erro ao abrir o arquivo de fornecedores.\n");
                    break;
                }

                FILE *temp = fopen("TempFornecedores.txt", "w");
                if (temp == NULL) {
                    printf("Erro ao criar o arquivo temporario.\n");
                    fclose(file);
                    break;
                }

                char nome[50], contato[50], linha[100];
                char nomeExcluir[50];
                int encontrado = 0;

                printf("Digite o nome do fornecedor a ser excluido: ");
                fgets(nomeExcluir, sizeof(nomeExcluir), stdin);
                nomeExcluir[strcspn(nomeExcluir, "\n")] = '\0'; // Remove o \n

                while (fgets(linha, sizeof(linha), file) != NULL) {
                    sscanf(linha, "%49[^,],%49[^\n]", nome, contato);
                    if (strcmp(nome, nomeExcluir) == 0) {
                        encontrado = 1;
                        continue; // Pula o fornecedor a ser excluído
                    }
                    fprintf(temp, "%s,%s\n", nome, contato);
                }

                fclose(file);
                fclose(temp);

                remove("Fornecedores.txt");
                rename("TempFornecedores.txt", "Fornecedores.txt");

                if (encontrado)
                    printf("Fornecedor excluido com sucesso!\n");
                else
                    printf("Fornecedor não encontrado.\n");

                sleep(2);
                break;
            }

            case 3: { // Exibir lista de fornecedores
                FILE *file = fopen("Fornecedores.txt", "r");
                if (file == NULL) {
                    printf("Erro ao abrir o arquivo de fornecedores.\n");
                    break;
                }

                char linha[100];
                printf("\nLista de Fornecedores:\n");
                while (fgets(linha, sizeof(linha), file) != NULL) {
                    printf("%s", linha);
                }

                fclose(file);
                printf("\nPressione Enter para retornar ao submenu...");
                getchar(); // Aguarda o Enter do usuário
                break;
            }

            case 4:
                printf("Voltando ao Menu de Estoque...\n");
                sleep(1);
                break;

            default:
                printf("Opcao inválida. Tente novamente.\n");
                sleep(2);
        }
    } while (subOpcao != 4);
    break;
}

            case 3: { // Modificar nome do item
                FILE *file = fopen("Itens.txt", "r");
                if (file == NULL) {
                    printf("Erro ao abrir o arquivo.\n");
                    break;
                }

                FILE *temp = fopen("Temp.txt", "w");
                if (temp == NULL) {
                    printf("Erro ao criar o arquivo temporario.\n");
                    fclose(file);
                    break;
                }

                int id, idAlterar, encontrado = 0;
                char nome[50], novoNome[50];
                float preco;
                int estoque;

                printf("Digite o ID do item para alterar o nome: ");
                scanf("%d", &idAlterar);
                getchar();

                while (fscanf(file, "%d,%49[^,],%f,%d\n", &id, nome, &preco, &estoque) == 4) {
                    if (id == idAlterar) {
                        encontrado = 1;
                        printf("Nome atual: %s\n", nome);
                        printf("Digite o novo nome: ");
                        fgets(novoNome, sizeof(novoNome), stdin);
                        novoNome[strcspn(novoNome, "\n")] = '\0'; // Remove o \n
                        strncpy(nome, novoNome, sizeof(nome) - 1);
                    }
                    fprintf(temp, "%d,%s,%.2f,%d\n", id, nome, preco, estoque);
                }

                fclose(file);
                fclose(temp);

                remove("Itens.txt");
                rename("Temp.txt", "Itens.txt");

                if (encontrado)
                    printf("Nome do item alterado com sucesso!\n");
                else
                    printf("Item não encontrado.\n");

                sleep(2);
                break;
            }

            case 4:
                printf("Voltando ao Menu Principal...\n");
                sleep(1);
                break;

            default:
                printf("Opcao inválida. Tente novamente.\n");
                sleep(2);
        }
    } while (opcao != 4);
}


void Caixa_registradora() {
    float fundos = 0.0; // Variável para armazenar os fundos
    int opcao;

    do {
        system("cls");
        printf("\n\tMenu Monetario:\n");
        printf("1. Realizar Compra.\n");
        printf("2. Alterar Preco de Produto.\n");
        printf("3. Exibir Fundos.\n");
        printf("4. Retornar ao Menu\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do teclado

        switch (opcao) {
            case 1: { // Realizar Compra
                FILE *file = fopen("Itens.txt", "r");
                if (file == NULL) {
                    printf("Erro ao abrir o arquivo de itens.\n");
                    break;
                }

                FILE *temp = fopen("TempItens.txt", "w");
                if (temp == NULL) {
                    printf("Erro ao criar arquivo temporario.\n");
                    fclose(file);
                    break;
                }

                int id, estoque, idCompra, quantidadeCompra, encontrado;
                char nome[50];
                float preco, total = 0.0;
                char linha[100];

                do {
                    encontrado = 0;

                    // Solicitar o ID do item a ser comprado
                    printf("Digite o ID do item a ser comprado (ou -1 para finalizar): ");
                    scanf("%d", &idCompra);

                    if (idCompra == -1) break; // Finalizar a compra

                    // Solicitar a quantidade a ser comprada
                    printf("Digite a quantidade: ");
                    scanf("%d", &quantidadeCompra);

                    // Resetando o ponteiro do arquivo para leitura correta
                    rewind(file);

                    while (fgets(linha, sizeof(linha), file) != NULL) {
                        sscanf(linha, "%d,%49[^,],%f,%d", &id, nome, &preco, &estoque);

                        if (id == idCompra) {
                            encontrado = 1;

                            if (quantidadeCompra > estoque) {
                                printf("Estoque insuficiente para o item: %s\n", nome);
                                fprintf(temp, "%d,%s,%.2f,%d\n", id, nome, preco, estoque); // Mantém o mesmo item no arquivo
                            } else {
                                estoque -= quantidadeCompra; // Atualiza o estoque
                                total += quantidadeCompra * preco; // Calcula o preco total
                                printf("Adicionado: %s (Qtd: %d, Total: %.2f)\n", nome, quantidadeCompra, quantidadeCompra * preco);
                                fprintf(temp, "%d,%s,%.2f,%d\n", id, nome, preco, estoque); // Atualiza o arquivo com o novo estoque
                            }
                        } else {
                            fprintf(temp, "%d,%s,%.2f,%d\n", id, nome, preco, estoque);
                        }
                    }

                    if (!encontrado) {
                        printf("Item com ID %d não encontrado.\n", idCompra);
                    }

                } while (1);

                fclose(file);
                fclose(temp);

                // Substitui o arquivo original pelo temporario
                remove("Itens.txt");
                rename("TempItens.txt", "Itens.txt");

                fundos += total; // Adiciona o total da compra aos fundos
                printf("\nCompra finalizada! Total: %.2f\n", total);
                printf("Pressione Enter para continuar...");
                getchar(); // Aguarda o Enter
                break;
            }

            case 2: { // Alterar Preco de Produto
                FILE *file = fopen("Itens.txt", "r");
                if (file == NULL) {
                    printf("Erro ao abrir o arquivo de itens.\n");
                    break;
                }

                FILE *temp = fopen("TempItens.txt", "w");
                if (temp == NULL) {
                    printf("Erro ao criar arquivo temporario.\n");
                    fclose(file);
                    break;
                }

                int id, idAlterar, encontrado;
                char nome[50];
                float preco;
                int estoque;
                char linha[100];

                printf("Digite o ID do item que deseja alterar o preco: ");
                scanf("%d", &idAlterar);

                encontrado = 0;
                rewind(file); // Resetando o ponteiro do arquivo para leitura correta

                while (fgets(linha, sizeof(linha), file) != NULL) {
                    sscanf(linha, "%d,%49[^,],%f,%d", &id, nome, &preco, &estoque);

                    if (id == idAlterar) {
                        encontrado = 1;
                        printf("Item encontrado: %s\n", nome);
                        printf("Preco atual: %.2f\n", preco);

                        printf("Digite o novo preco: ");
                        scanf("%f", &preco);

                        fprintf(temp, "%d,%s,%.2f,%d\n", id, nome, preco, estoque); // Atualiza o preco no arquivo temporario
                    } else {
                        fprintf(temp, "%d,%s,%.2f,%d\n", id, nome, preco, estoque); // Mantém os outros itens sem alteração
                    }
                }

                if (!encontrado) {
                    printf("Item com ID %d não encontrado.\n", idAlterar);
                }

                fclose(file);
                fclose(temp);

                // Substitui o arquivo original pelo temporario
                remove("Itens.txt");
                rename("TempItens.txt", "Itens.txt");

                if (encontrado)
                    printf("Preco do item atualizado com sucesso!\n");
                else
                    printf("Falha ao alterar o preco. Item não encontrado.\n");

                printf("Pressione Enter para continuar...");
                getchar(); // Aguarda o Enter
                break;
            }

            case 3: { // Exibir Fundos
                printf("Fundos disponíveis: %.2f\n", fundos);
                printf("Pressione Enter para continuar...");
                getchar(); // Aguarda o Enter
                break;
            }

            case 4:
                printf("Voltando ao Menu Principal...\n");
                break;

            default:
                printf("Opcao inválida. Tente novamente.\n");
        }
    } while (opcao != 4);
}


int main() { //inicio da main!!!
    FILE *fpIN, *fpADM;
    int option = 0;
    char *user = (char *)malloc(sizeof *user * MAX_LOGIN);
    char *senha, *confirmaSenha, *admSenha;
    admSenha = (char *)malloc(sizeof *admSenha * MAX_LOGIN); // Alocação para a senha do ADM

veruser:
    if (autenticado != 1) {
        do {
            printf("Menu de login\n");
            printf("1- Login\n2- Cadastrar\n3- Sair\n");
            scanf("%d", &option);
            CLEAN_BUFF;

            switch (option) {
                case 1:
                    printf("Usuario: ");
                    gets(user);
                    printf("Senha: ");
                    senha = CriaSenha();

                    fpIN = fopen("usuarios.txt", "a+");
                    if (Usuario(fpIN, user, senha)) {
                        printf("\nUsuario registrado.\n");
                        autenticado = 1;
                        fclose(fpIN);
                        free(senha);
                        goto veruser;
                    } else {
                        printf("\nUsuario nao registrado\n");
                        fclose(fpIN);
                        free(senha);
                        exit(0);
                    }
                    break;

                case 2:
                    printf("Usuario: ");
                    gets(user);

                    // Verificação da senha de administrador
                    printf("Digite a senha de administrador para prosseguir: ");
                    senha = CriaSenha();
                    printf("\n");

                    fpADM = fopen("ADM.txt", "r");
                    if (fpADM == NULL) {
                        printf("Erro ao abrir o arquivo ADM.txt.\n");
                        free(senha);
                        exit(1);
                    }

                    fgets(admSenha, MAX_LOGIN, fpADM);
                    admSenha[strcspn(admSenha, "\n")] = '\0'; // Remover o \n do final da linha
                    fclose(fpADM);

                    if (strcmp(senha, admSenha) != 0) {
                        printf("Senha de administrador incorreta.\n");
                        free(senha);
                        break;
                    }
                    free(senha);

                    // Processo de criação de usuário
                    do {
                        printf("Senha: ");
                        senha = CriaSenha();
                        printf("\nConfirmacao de senha: ");
                        confirmaSenha = CriaSenha();
                        printf("\n");

                        if (!strcmp(senha, confirmaSenha))
                            break;
                        else
                            printf("As senhas nao sao iguais. Tente novamente.\n");
                    } while (1);

                    fpIN = fopen("usuarios.txt", "a+");
                    fprintf(fpIN, "%s %s\n", user, senha);
                    fclose(fpIN);

                    free(senha);
                    free(confirmaSenha);
                    break;

                case 3:
                    free(admSenha);
                    free(user);
                    return 0;

                default:
                    printf("Escolha 1, 2 ou 3...\n");
                    break;
            }
        } while (1);
    }

    printf("Usuario autenticado");
    sleep(2);
    int opcao;
    do {
        system("cls");
        printf("\n\tMenu:\n");
        printf("1. Adicionar Produto\n");
        printf("2. Exibir e Modificar estoque\n");
        printf("3. Caixa Registradora\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do teclado

        switch (opcao) {
            case 1:
                Menu_de_itens();
                break;

            case 2:
                Menu_de_estoque();
                break;

            case 3:
                Caixa_registradora();
                break;

            case 4:
                printf("Saindo do Sistema...");
                free(admSenha);
                free(user);
                exit(0);
                break;

            default:
                printf("Opcao invalida, tente novamente.\n");
                break;
        }
    } while (opcao != 4);

    // Liberar memória caso o loop principal termine
    free(admSenha);
    free(user);
    return 0;
}


    

                /*system("cls");
                printf("\n\tSistema de Caixa: \n");
                printf("1. Realizar Compra\n");
                printf("2. Realizar devolução\n");
                printf("3. Retornar ao Menu\n");
                printf("Escolha uma opcao; ");
                scanf("%d", &opcao2);

lista de produtos com id
calculadora para soma dos produtos
descontar unidade do estoque
notificação para estoque baixo
usar setinhas para coordenar 
Sistema de Login mesmo que temporário
*/
