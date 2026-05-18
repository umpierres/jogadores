#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

// definindo as creden iais do banco de dado
#define SERVER "127.0.0.1"
#define USER "admin_futebol"
#define PASSWORD "senha123"
#define DATABASE "db_futebol"

void finish_with_error(MYSQL *con) {
    fprintf(stderr, "Erro MySQL: %s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

int main() {
    MYSQL *con = mysql_init(NULL);

    if (con == NULL) {
        fprintf(stderr, "Erro na inicialização do MySQL: %s\n", mysql_error(con));
        exit(1);
    }

    // cnecta ao banco de dados
    if (mysql_real_connect(con, SERVER, USER, PASSWORD, DATABASE, 0, NULL, 0) == NULL) {
        finish_with_error(con);
    }
    printf("Conexão com o banco '%s' estabelecida com sucesso!\n\n", DATABASE);

    int opcao = 0;
    char query[512]; 
    char nome[100];
    char posicao[50];
    char data_nascimento[15];
    int gols_marcados;
    int id_alvo;

    do {
        printf("=================================\n");
        printf("        MENU DE JOGADORES        \n");
        printf("=================================\n");
        printf("1. Inserir Jogador (CREATE)\n");
        printf("2. Listar Jogadores (READ)\n");
        printf("3. Atualizar Gols (UPDATE)\n");
        printf("4. Deletar Jogador (DELETE)\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        
        if (scanf("%d", &opcao) != 1) {
            while(getchar() != '\n'); 
            printf("Entrada inválida!\n\n");
            continue;
        }

        switch(opcao) {
            case 1:
                // ==========================================
                // CREATE
                // ==========================================
                printf("\n--- CREATE ---\n");
                printf("Nome do jogador: ");
                scanf(" %[^\n]", nome); 
                
                printf("Posição (ex: Atacante): ");
                scanf(" %[^\n]", posicao);
                
                printf("Data de nascimento (YYYY-MM-DD): ");
                scanf(" %[^\n]", data_nascimento);
                
                printf("Gols marcados até hoje: ");
                scanf("%d", &gols_marcados);

                // monta a query dinamicamente com um buffer
                snprintf(query, sizeof(query), 
                         "INSERT INTO jogadores (nome, posicao, data_nascimento, gols_marcados) "
                         "VALUES ('%s', '%s', '%s', %d)", 
                         nome, posicao, data_nascimento, gols_marcados);
                
                if (mysql_query(con, query)) {
                    finish_with_error(con);
                }
                printf("\n>> Jogador '%s' inserido com sucesso!\n\n", nome);
                break;

            case 2:
                // ==========================================
                // READ 
                // ==========================================
                printf("\n--- READ ---\n");
                if (mysql_query(con, "SELECT id, nome, posicao, data_nascimento, gols_marcados FROM jogadores")) {
                    finish_with_error(con);
                }

                MYSQL_RES *result = mysql_store_result(con);
                if (result == NULL) {
                    finish_with_error(con);
                }

                int num_fields = mysql_num_fields(result);
                MYSQL_ROW row;

                printf("ID\t| Nome\t\t\t| Posição\t| Nascimento\t| Gols\n");
                printf("--------------------------------------------------------------------------------\n");
                while ((row = mysql_fetch_row(result))) {
                    for (int i = 0; i < num_fields; i++) {
                        printf("%s\t| ", row[i] ? row[i] : "NULL");
                    }
                    printf("\n");
                }
                printf("\n");
                mysql_free_result(result);
                break;

            case 3:
                // ==========================================
                // UPDATE 
                // ==========================================
                printf("\n--- UPDATE ---\n");
                printf("Digite o ID do jogador que deseja atualizar: ");
                scanf("%d", &id_alvo);
                
                printf("Qual a nova quantidade TOTAL de gols? ");
                scanf("%d", &gols_marcados);

                snprintf(query, sizeof(query), "UPDATE jogadores SET gols_marcados = %d WHERE id = %d", gols_marcados, id_alvo);

                if (mysql_query(con, query)) {
                    finish_with_error(con);
                }
                printf("\n>> Gols do jogador ID %d atualizados para %d!\n\n", id_alvo, gols_marcados);
                break;

            case 4:
                // ==========================================
                // DELETE
                // ==========================================
                printf("\n--- DELETE ---\n");
                printf("Digite o ID do jogador que deseja DELETAR: ");
                scanf("%d", &id_alvo);

                snprintf(query, sizeof(query), "DELETE FROM jogadores WHERE id = %d", id_alvo);
                
                if (mysql_query(con, query)) {
                    finish_with_error(con);
                }
                printf("\n>> Jogador ID %d deletado com sucesso!\n\n", id_alvo);
                break;

            case 5:
                printf("\nSaindo do sistema...\n");
                break;

            default:
                printf("\nOpção inválida! Tente novamente.\n\n");
        }

    } while (opcao != 5);

    // fechar a conexão com o banco
    mysql_close(con);
    printf("Programa finalizado. Conexão encerrada.\n");

    return 0;
}