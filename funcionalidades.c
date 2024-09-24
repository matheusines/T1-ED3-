#include <ctype.h>
#include <string.h>
#include <math.h>
#include "funcoesFornecidas.h"
#include "cabecalho.h"
#include "registro.h"

#define DELIMITER '#'
#define TRASH '$'
#define PAGE_SIZE 1600
// Função principal para a funcionalidade 1
void funcionalidade1() {

   // Inicializa o cabeçalho do arquivo binário chamando uma função que retorna um objeto do tipo Cabecalho
    Cabecalho header = inicializarCabecalho();

    char srcFileName[30], destinyFileName[30]; // Declara duas strings para armazenar os nomes dos arquivos de origem e destino.
    scanf("%s %s", srcFileName, destinyFileName); // Lê os nomes dos arquivos fornecidos pelo usuário.

    FILE *sf = fopen(srcFileName, "r"); // Abre o arquivo de origem (CSV) no modo de leitura.
    if (sf == NULL) { // Verifica se o arquivo foi aberto com sucesso.
        printf("Falha no processamento do arquivo."); // Exibe mensagem de erro se o arquivo não foi aberto.
        return; // Encerra a função caso o arquivo não possa ser aberto.
    }

    FILE *df = fopen(destinyFileName, "wb+"); // Abre o arquivo de destino (binário) no modo de leitura e escrita binária.
    escreverCabecalhoBin(df, &header); // Escreve o cabeçalho inicial no arquivo binário.

    fscanf(sf, "%*[^\n]\n"); // Ignora a primeira linha de cabeçalho do CSV, normalmente contendo os títulos das colunas.

    Registro *s = lerRegistroCSV(sf); // Lê o primeiro registro do CSV e armazena na estrutura Registro.
    int contPags = 1; // Inicializa o contador de páginas em disco.
    int nBytes = 0; // Inicializa o contador de bytes.

    // Loop para ler, escrever e processar cada registro do arquivo CSV
    while (s != NULL) {
        escreverRegistro(df, s); // Escreve o registro lido no arquivo binário.
        liberarRegistro(s);  // Libera a memória do registro após a escrita.
        s = lerRegistroCSV(sf); // Lê o próximo registro do CSV.
        header.proxRRN++; // Incrementa o próximo RRN (Número de Registro Relativo) no cabeçalho.
        nBytes += 160; // Incrementa o contador de bytes com o tamanho fixo de um registro.

        if ((nBytes % 1600) == 0) { // Verifica se alcançou o limite de bytes para incrementar o número de páginas em disco.
            contPags++; // Incrementa o contador de páginas.
        }
    }   

    header.nroPagDisco = contPags + 1; // Atualiza o número total de páginas em disco no cabeçalho.
    fseek(df, 0, SEEK_SET); // Move o ponteiro do arquivo para o início para reescrever o cabeçalho atualizado.
    header.status = '1'; // Atualiza o status do cabeçalho para indicar que o arquivo foi processado corretamente.
    escreverCabecalhoBin(df, &header); // Reescreve o cabeçalho atualizado no arquivo binário.

    fclose(sf); // Fecha o arquivo de origem.
    fclose(df); // Fecha o arquivo de destino.

    // Exibe o conteúdo do arquivo binário na tela (função de depuração/visualização).
    binarioNaTela(destinyFileName);
}
void funcionalidade2() {
    // Inicializa o cabeçalho do arquivo binário chamando uma função que retorna um objeto do tipo Cabecalho
    Cabecalho header = inicializarCabecalho();
    
    char srcFileName[30]; // Declara uma string para armazenar o nome do arquivo de origem.
    scanf("%s", srcFileName); // Lê o nome do arquivo de origem fornecido pelo usuário.
    
    FILE *sf = fopen(srcFileName, "rb"); // Abre o arquivo binário no modo de leitura.

    if (sf == NULL) { // Verifica se o arquivo foi aberto com sucesso.
        printf("Falha no processamento do arquivo."); // Exibe uma mensagem de erro caso o arquivo não possa ser aberto.
        return; // Encerra a função se o arquivo não foi aberto corretamente.
    }

    // Lê o cabeçalho do arquivo binário para inicializar as informações do arquivo.
    lerCabecalhoBin(sf, &header);

    int flag = 0; // Variável para verificar se algum registro foi encontrado.
    Registro *r = lerRegistroBin(sf); // Lê o primeiro registro do arquivo binário.

    // Loop para percorrer todos os registros no arquivo binário.
    while (r != NULL) {
        if (r->removido == '0') { // Verifica se o registro não foi removido logicamente.
            flag = 1; // Define a flag para indicar que pelo menos um registro válido foi encontrado.
            
            // Imprime os campos do registro se eles não forem nulos ou removidos.
            if (r->nome[0] != '\0') {
                printf("Nome: %s\n", r->nome);
            }
            if (r->especie[0] != '\0') {
                printf("Especie: %s\n", r->especie);
            }
            if (r->tipo[0] != '\0') {
                printf("Tipo: %s\n", r->tipo);
            }
            if (r->dieta[0] != '\0') {
                printf("Dieta: %s\n", r->dieta);
            }
            if (r->habitat[0] != '\0') {
                printf("Lugar que habitava: %s\n", r->habitat);
            }
            if (r->tamanho != -1) {
                printf("Tamanho: %.1f m\n", r->tamanho); // Exibe o tamanho do registro, se válido.
            }
            if (r->velocidade != -1) {
                printf("Velocidade: %d %cm/h\n", r->velocidade, r->unidadeMedida); // Exibe a velocidade, se válida.
            }
            printf("\n"); // Imprime uma linha em branco para separar os registros.
        }
        // Libera a memória do registro após a leitura e processamento.
        liberarRegistro(r);

        // Lê o próximo registro do arquivo binário.
        r = lerRegistroBin(sf);
    }

    // Verifica se nenhum registro válido foi encontrado.
    if (!flag) {
        printf("Registro inexistente."); // Exibe mensagem se nenhum registro não removido foi encontrado.
    }

    // Exibe o número de páginas de disco ocupadas pelo arquivo.
    printf("Numero de paginas de disco: %d\n\n", header.nroPagDisco);

    fclose(sf); // Fecha o arquivo binário após a leitura.
}

// Função principal da funcionalidade 3
void funcionalidade3() {
    Cabecalho header; // Declara uma variável para armazenar o cabeçalho do arquivo.
    char srcFileName[30]; // Declara uma string para armazenar o nome do arquivo binário.
    int n; // Variável para armazenar o número de tentativas de busca.

    scanf("%s", srcFileName); // Lê o nome do arquivo binário fornecido pelo usuário.
    FILE *sf = fopen(srcFileName, "rb"); // Abre o arquivo binário no modo de leitura.
    if (sf == NULL) { // Verifica se o arquivo foi aberto com sucesso.
        printf("Falha no processamento do arquivo."); // Exibe uma mensagem de erro se o arquivo não foi aberto.
        return; // Encerra a função se não conseguiu abrir o arquivo.
    }

    lerCabecalhoBin(sf, &header); // Lê o cabeçalho do arquivo binário.
    scanf("%d", &n); // Lê o número de tentativas de busca que o usuário deseja realizar.

    char campo[30], valor[30]; // Declara strings para armazenar o campo de busca e o valor a ser buscado.

    // Loop para executar 'n' buscas.
    for (int i = 0; i < n; i++) {
        // Lê o campo que será usado na busca.
        scanf("%s", campo);

        // Verifica se o campo é uma string que precisa ser lida entre aspas.
        if (strcmp(campo, "nome") == 0 || strcmp(campo, "especie") == 0 || strcmp(campo, "habitat") == 0 ||
            strcmp(campo, "dieta") == 0 || strcmp(campo, "tipo") == 0 || strcmp(campo, "alimento") == 0) {
            scan_quote_string(valor); // Lê strings entre aspas duplas.
        } else {
            scanf("%s", valor); // Lê o valor sem aspas, que pode ser string ou numérico.
        }

        // Reposiciona o ponteiro de leitura do arquivo para começar logo após o cabeçalho.
        fseek(sf, PAGE_SIZE, SEEK_SET);

        int registrosEncontrados = 0; // Inicializa o contador de registros encontrados na busca.
        Registro *r = lerRegistroBin(sf); // Lê o primeiro registro do arquivo binário.

        // Exibe o número da busca, ajustando o formato de impressão para a primeira busca.
        if (i == 0) {
            printf("Busca %d\n", i + 1);
        } else {
            printf("\nBusca %d\n", i + 1);
        }

        // Itera sobre todos os registros do arquivo binário.
        while (r != NULL) {
            // Verifica se o registro não foi removido logicamente e se corresponde ao critério de busca.
            if (r->removido == '0' && buscarRegistro(r, campo, valor)) {
                // Exibe os campos do registro encontrado, ignorando campos vazios.
                if (r->nome[0] != '\0') printf("Nome: %s\n", r->nome);
                if (r->especie[0] != '\0') printf("Especie: %s\n", r->especie);
                if (r->tipo[0] != '\0') printf("Tipo: %s\n", r->tipo);
                if (r->dieta[0] != '\0') printf("Dieta: %s\n", r->dieta);
                if (r->habitat[0] != '\0') printf("Lugar que habitava: %s\n", r->habitat);
                if (r->tamanho != -1) printf("Tamanho: %.1f m\n", r->tamanho);
                if (r->velocidade != -1) printf("Velocidade: %d %cm/h\n", r->velocidade, r->unidadeMedida);
                printf("\n"); // Exibe uma linha em branco após cada registro.
                registrosEncontrados++; // Incrementa o contador de registros encontrados.
            }
            liberarRegistro(r); // Libera a memória do registro atual.
            r = lerRegistroBin(sf); // Lê o próximo registro do arquivo.
        }

        // Caso nenhum registro correspondente tenha sido encontrado.
        if (registrosEncontrados == 0) {
            printf("Registro inexistente.\n\n");
        }

        // Exibe o número de páginas de disco ocupadas pelo arquivo.
        printf("Numero de paginas de disco: %d\n", header.nroPagDisco);
    }
    printf("\n"); // Exibe uma linha em branco ao final de todas as buscas.
    fclose(sf); // Fecha o arquivo binário após as buscas.
}

void funcionalidade4() {
    Cabecalho header; // Declara uma variável para armazenar o cabeçalho do arquivo.
    char srcFileName[30]; // Declara uma string para armazenar o nome do arquivo binário.
    int n; // Variável para armazenar o número de registros a serem removidos.

    // Lê o nome do arquivo binário fornecido pelo usuário.
    scanf("%s", srcFileName);
    FILE *sf = fopen(srcFileName, "rb+"); // Abre o arquivo binário no modo de leitura e escrita.
    if (sf == NULL) { // Verifica se o arquivo foi aberto com sucesso.
        printf("Falha no processamento do arquivo."); // Exibe mensagem de erro se o arquivo não puder ser aberto.
        return; // Encerra a função se o arquivo não foi aberto corretamente.
    }

    // Lê o cabeçalho do arquivo binário.
    lerCabecalhoBin(sf, &header);

    // Define o status como '0' (inconsistente) enquanto o arquivo está sendo modificado.
    header.status = '0';

    // Lê o número de registros que o usuário deseja remover.
    scanf("%d", &n);

    char campo[30], valor[50]; // Declara strings para armazenar o campo de busca e o valor.

    // Loop para executar 'n' remoções.
    for (int i = 0; i < n; i++) {
        // Lê o campo a ser buscado para remoção.
        scanf("%s", campo);

        // Verifica se o campo é uma string (dentro de aspas) ou um valor numérico.
        if (strcmp(campo, "nome") == 0 || strcmp(campo, "especie") == 0 || strcmp(campo, "habitat") == 0 ||
            strcmp(campo, "dieta") == 0 || strcmp(campo, "tipo") == 0 || strcmp(campo, "alimento") == 0) {
            scan_quote_string(valor); // Lê strings entre aspas.
        } else {
            scanf("%s", valor); // Lê valores numéricos ou strings sem aspas.
        }

        // Reposiciona o ponteiro do arquivo para começar após o cabeçalho.
        fseek(sf, PAGE_SIZE, SEEK_SET);

        long int byteOffset; // Variável para armazenar a posição do ponteiro do arquivo.
        Registro *r = lerRegistroBin(sf); // Lê o primeiro registro do arquivo.
        int RRN = 0; // RRN começa no primeiro registro após o cabeçalho.

        // Itera sobre todos os registros do arquivo.
        while (r != NULL) {
            byteOffset = ftell(sf); // Armazena a posição do ponteiro do arquivo após a leitura do registro.

            // Verifica se o registro não foi removido e atende ao critério de busca.
            if (r->removido == '0' && buscarRegistro(r, campo, valor)) {
                // Marca o registro como removido e atualiza a lista encadeada de registros removidos.
                removeRegistro(r, header.topo);
                header.topo = ((byteOffset - PAGE_SIZE - 160) / 160);  // Atualiza o topo da lista de removidos (RRN).

                // Reescreve o registro no arquivo marcando-o como removido.
                fseek(sf, byteOffset - 160, SEEK_SET);
                fwrite(&r->removido, sizeof(char), 1, sf);  // Escreve o campo 'removido'.
                fwrite(&r->encadeamento, sizeof(int), 1, sf);  // Escreve o campo 'encadeamento'.

                // Preenche o restante do registro com um caractere de lixo para indicar que está removido.
                char trash = TRASH;
                for (int j = 0; j < 155; j++) {
                    fwrite(&trash, sizeof(char), 1, sf);
                }

                // Incrementa o contador de registros removidos no cabeçalho.
                header.nroRegRem++;
            }

            liberarRegistro(r); // Libera a memória do registro atual.
            r = lerRegistroBin(sf); // Lê o próximo registro do arquivo.
        }
        liberarRegistro(r); // Libera o último registro lido após o loop.
    }

    // Atualiza o status para '1' (consistente) após as modificações.
    header.status = '1';

    // Atualiza o número de páginas de disco com base na posição atual do ponteiro do arquivo.
    header.nroPagDisco = (ftell(sf) + PAGE_SIZE - 1) / PAGE_SIZE;

    // Reescreve o cabeçalho atualizado no início do arquivo.
    fseek(sf, 0, SEEK_SET);
    escreverCabecalhoBin(sf, &header);

    fclose(sf); // Fecha o arquivo binário após as modificações.

    // Exibe o arquivo binário na tela para verificar o resultado das remoções.
    binarioNaTela(srcFileName);
}



// Função para inserir um registro reutilizando registros logicamente removidos ou ao final do arquivo
void inserirRegistro(FILE *df, Registro *r, Cabecalho *header) {
    if (header->topo != -1) {
        // Reutiliza registros logicamente removidos
        int RRNRemovido = header->topo;  // RRN do topo da pilha de removidos
        long int byteOffset = PAGE_SIZE + (RRNRemovido * 160);  // Calcula o byte offset correto

        // Posiciona o ponteiro no arquivo para o registro removido
        fseek(df, byteOffset, SEEK_SET);

        // Lê o registro removido
        char removido;
        int encadeamento;
        fread(&removido, sizeof(char), 1, df);
        fread(&encadeamento, sizeof(int), 1, df);

        // Atualiza o topo da pilha para o próximo registro removido
        header->topo = encadeamento;  // Atualiza para o próximo removido na lista

        // Reposiciona o ponteiro para o início do registro para reescrever o novo
        fseek(df, byteOffset, SEEK_SET);

        // Marca o registro como válido
        r->removido = '0';  // Agora é um registro válido
        r->encadeamento = -1;  // Não há encadeamento para um registro válido

        // Escreve o novo registro no lugar do registro removido
        escreverRegistro(df, r);

        // Atualiza o número de registros removidos
        header->nroRegRem--;
        // O proxRRN NÃO deve ser atualizado aqui
    } else {
        // Caso não haja registros removidos, insere ao final do arquivo
        long int byteOffset = PAGE_SIZE + header->proxRRN * 160;  // Próximo byte offset
        fseek(df, byteOffset, SEEK_SET);

        // Marca o registro como válido
        r->removido = '0';
        r->encadeamento = -1;

        // Escreve o novo registro no final do arquivo
        escreverRegistro(df, r);

        // Atualiza o proxRRN para o próximo registro a ser inserido ao final do arquivo
        header->proxRRN++;
    }
}


// Função principal para a funcionalidade 5 (inserção de registros)
void funcionalidade5() {
    Cabecalho header = inicializarCabecalho();
    char fileName[30];
    scanf("%s", fileName);

    FILE *df = fopen(fileName, "rb+");
    if (df == NULL) {
        printf("Falha no processamento do arquivo.");
        return;
    }

    lerCabecalhoBin(df, &header);

    int numInsertions;
    scanf("%d", &numInsertions);

    Registro *r;

    for (int i = 0; i < numInsertions; i++) {
        r = (Registro *)malloc(sizeof(Registro));
        r->removido = '0'; // Define como não removido
        r->encadeamento = -1; // Define o encadeamento como -1, pois o registro não está removido

        // Leitura dos campos do registro
        scanf("%d", &r->populacao); // Leitura da população da espécie

        scanf("%f %c", &r->tamanho, &r->unidadeMedida); // Leitura do tamanho e da unidade de medida

        scanf("%d", &r->velocidade); // Leitura da velocidade do indivíduo

        r->nome = (char *)malloc(50 * sizeof(char));
        scan_quote_string(r->nome); // Leitura do nome da espécie

        r->especie = (char *)malloc(50 * sizeof(char));
        scan_quote_string(r->especie); // Leitura do nome científico da espécie

        r->habitat = (char *)malloc(50 * sizeof(char));
        scan_quote_string(r->habitat); // Leitura do habitat da espécie

        r->tipo = (char *)malloc(50 * sizeof(char));
        scan_quote_string(r->tipo); // Leitura do tipo da espécie

        r->dieta = (char *)malloc(50 * sizeof(char));
        scan_quote_string(r->dieta); // Leitura da dieta da espécie

        r->alimento = (char *)malloc(50 * sizeof(char));
        scan_quote_string(r->alimento); // Leitura do alimento da espécie

        // Inserção do registro no arquivo binário
            inserirRegistro(df, r, &header);


        // Libera a memória alocada para os campos de string
        free(r->nome);
        free(r->especie);
        free(r->habitat);
        free(r->tipo);
        free(r->dieta);
        free(r->alimento);
        free(r);
    }

    // Atualiza o cabeçalho do arquivo
    fseek(df, 0, SEEK_SET);
    header.status = '1'; // Marca o arquivo como consistente
    escreverCabecalhoBin(df, &header);

    fclose(df);
    binarioNaTela(fileName);
}

void funcionalidade6() {
    Cabecalho header = inicializarCabecalho(); // Inicializa o cabeçalho do arquivo binário.
    char srcFileName[30]; // Declara uma string para armazenar o nome do arquivo binário de entrada.

    // Lê o nome do arquivo binário de entrada (arquivo original).
    scanf("%s", srcFileName);
    FILE *sf = fopen(srcFileName, "rb"); // Abre o arquivo original no modo de leitura.
    if (sf == NULL) { // Verifica se o arquivo foi aberto com sucesso.
        printf("Falha no processamento do arquivo.\n"); // Exibe mensagem de erro se o arquivo não puder ser aberto.
        return; // Encerra a função se o arquivo não foi aberto corretamente.
    }

    // Define o nome do arquivo compactado.
    char *compactFileName = "compactado.bin";
    FILE *compactFile = fopen(compactFileName, "wb"); // Cria o arquivo compactado no modo de escrita.
    if (compactFile == NULL) { // Verifica se o arquivo compactado foi criado com sucesso.
        printf("Falha ao criar o arquivo compactado.\n"); // Exibe mensagem de erro se o arquivo não puder ser criado.
        fclose(sf); // Fecha o arquivo original.
        return; // Encerra a função se o arquivo compactado não foi criado.
    }

    // Lê o cabeçalho do arquivo original.
    lerCabecalhoBin(sf, &header);

    // Atualiza o status do arquivo compactado para '0' (inconsistente) enquanto está sendo escrito.
    header.status = '0';
    escreverCabecalhoBin(compactFile, &header); // Escreve o cabeçalho no arquivo compactado.

    // Posiciona o ponteiro no primeiro registro do arquivo original, após o cabeçalho.
    fseek(sf, PAGE_SIZE, SEEK_SET);
    Registro *r; // Declara um ponteiro para o registro.
    long int byteOffset; // Variável para armazenar o byte offset.
    int RRN = 0; // Inicializa o RRN (Registro Relativo Número) como 0.

    // Itera sobre os registros do arquivo original.
    while (1) {
        byteOffset = ftell(sf); // Captura o byte offset atual (posição do ponteiro no arquivo).
        r = lerRegistroBin(sf); // Lê o próximo registro do arquivo original.

        if (r == NULL) {
            break;  // Sai do loop quando não há mais registros para ler.
        }

        if (r->removido == '0') {  // Verifica se o registro não está logicamente removido.
            // Reescreve o registro não removido no arquivo compactado.
            fseek(compactFile, 0, SEEK_END);  // Move o ponteiro para o final do arquivo compactado.
            escreverRegistro(compactFile, r); // Escreve o registro no arquivo compactado.
        }

        RRN++;  // Incrementa o RRN.
        liberarRegistro(r);  // Libera a memória do registro atual.
    }

    // Atualiza o status do arquivo compactado para '1' (consistente).
    header.status = '1';
    header.topo = -1;  // Define o topo como -1, pois o arquivo compactado não possui registros removidos.
    header.nroRegRem = 0; // Define o número de registros removidos como 0.
    header.qttCompacta++; // Incrementa o contador de operações de compactação.
    header.proxRRN = RRN; // Define o próximo RRN como o último valor de RRN.
    
    // Atualiza o número de páginas de disco com base no tamanho do arquivo compactado.
    fseek(compactFile, 0, SEEK_END); // Move o ponteiro para o final do arquivo compactado.
    long tamanhoArquivo = ftell(compactFile);  // Obtém o tamanho do arquivo compactado.
    header.nroPagDisco = (tamanhoArquivo + PAGE_SIZE - 1) / PAGE_SIZE; // Calcula o número de páginas ocupadas.

    // Reescreve o cabeçalho atualizado no início do arquivo compactado.
    fseek(compactFile, 0, SEEK_SET);
    escreverCabecalhoBin(compactFile, &header);

    // Fecha os arquivos.
    fclose(sf);
    fclose(compactFile);

    // Exibe o conteúdo do arquivo compactado na tela para verificação.
    binarioNaTela(compactFileName);
}
