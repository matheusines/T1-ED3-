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
    Cabecalho header = inicializarCabecalho();

    char srcFileName[30], destinyFileName[30];
    scanf("%s %s", srcFileName, destinyFileName);

    FILE *sf = fopen(srcFileName, "r");
    if (sf == NULL) {
        printf("Falha no processamento do arquivo.");
        return;
    }

    FILE *df = fopen(destinyFileName, "wb+");
    escreverCabecalhoBin(df, &header);

    fscanf(sf, "%*[^\n]\n"); // Ignorar a primeira linha de cabeçalho do CSV

    Registro *s = lerRegistroCSV(sf);
    int contPags = 1;
    int nBytes = 0;
    while (s != NULL) {
        escreverRegistro(df, s);
        liberarRegistro(s);  // Libera o registro após escrever no arquivo
        s = lerRegistroCSV(sf);
        header.proxRRN++;
        nBytes+= 160;

        if ((nBytes % 1600) == 0){
            contPags++;
        }
    }   

    header.nroPagDisco = contPags +1;
    fseek(df, 0, SEEK_SET);
    header.status = '1';
    escreverCabecalhoBin(df, &header);

    fclose(sf);
    fclose(df);

    // Exibir o arquivo binário
    binarioNaTela(destinyFileName);
}

void funcionalidade2() {
    // Inicializa o cabeçalho do arquivo binário
    Cabecalho header = inicializarCabecalho();
    
    char srcFileName[30]; // Nome do arquivo de origem
    scanf("%s", srcFileName);
    
    FILE *sf = fopen(srcFileName, "rb"); // Abre o arquivo binário para leitura

    if (sf == NULL) {
        printf("Falha no processamento do arquivo.");
        return; // Erro ao abrir o arquivo
    }

    // Lê o cabeçalho do arquivo binário
    lerCabecalhoBin(sf, &header);

    int flag = 0; // Usado para verificar se algum registro foi encontrado
    Registro *r = lerRegistroBin(sf); // Lê o primeiro registro do arquivo

    // Enquanto houver registros no arquivo binário
    while (r != NULL) {
        if (r->removido == '0') { // Se o registro não foi removido logicamente
            flag = 1; // Define flag para indicar que pelo menos um registro foi lido
            // Imprime o registro (adaptado para ignorar campos nulos)
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
                printf("Tamanho: %.1f m\n", r->tamanho);
            }
            if (r->velocidade != -1) {
                printf("Velocidade: %d %cm/h\n", r->velocidade, r->unidadeMedida);
            }
            printf("\n");
        }
        // Libera o registro lido
        liberarRegistro(r);

        // Lê o próximo registro do arquivo
        r = lerRegistroBin(sf);
    }

    // Se nenhum registro foi encontrado
    if (!flag) {
        printf("Registro inexistente.");
    }

    // Número de páginas de disco ocupadas
    printf("Numero de paginas de disco: %d\n\n", header.nroPagDisco);

    fclose(sf); // Fecha o arquivo
}

// Função principal da funcionalidade 3
void funcionalidade3() {
    Cabecalho header;
    char srcFileName[30];
    int n;
    
    scanf("%s", srcFileName); // Nome do arquivo binário
    FILE *sf = fopen(srcFileName, "rb"); // Abre o arquivo binário
    if (sf == NULL) {
        printf("Falha no processamento do arquivo.");
        return;
    }

    lerCabecalhoBin(sf, &header); // Lê o cabeçalho do arquivo
    scanf("%d", &n); // Lê o número de tentativas de busca

    char campo[30], valor[30];

    // Loop para executar 'n' buscas
    for (int i = 0; i < n; i++) {
        // Lê o campo a ser buscado
        scanf("%s", campo);

        // Verifica se o campo deve ser uma string (dentro de aspas)
        if (strcmp(campo, "nome") == 0 || strcmp(campo, "especie") == 0 || strcmp(campo, "habitat") == 0 ||
            strcmp(campo, "dieta") == 0 || strcmp(campo, "tipo") == 0 || strcmp(campo, "alimento") == 0) {
            scan_quote_string(valor); // Lê strings entre aspas
        } else {
            scanf("%s", valor); // Lê o valor (pode ser numérico ou string sem aspas)
        }

        // Reposiciona o ponteiro no arquivo para começar após o cabeçalho
        fseek(sf, PAGE_SIZE, SEEK_SET);

        int registrosEncontrados = 0; // Inicializa o contador de registros encontrados
        Registro *r = lerRegistroBin(sf); // Lê o primeiro registro

        if (i==0){
            printf("Busca %d\n", i + 1); // Exibe o número da busca
        }else{
            printf("\nBusca %d\n", i + 1); // Exibe o número da busca
        }
        

        // Itera sobre os registros do arquivo
        while (r != NULL) {
            // Verifica se o registro não foi removido e atende ao critério da busca
            if (r->removido == '0' && buscarRegistro(r, campo, valor)) {
                // Exibe os campos do registro encontrado
                if (r->nome[0] != '\0') printf("Nome: %s\n", r->nome);
                if (r->especie[0] != '\0') printf("Especie: %s\n", r->especie);
                if (r->tipo[0] != '\0') printf("Tipo: %s\n", r->tipo);
                if (r->dieta[0] != '\0') printf("Dieta: %s\n", r->dieta);
                if (r->habitat[0] != '\0') printf("Lugar que habitava: %s\n", r->habitat);
                if (r->tamanho != -1) printf("Tamanho: %.1f m\n", r->tamanho);
                if (r->velocidade != -1) printf("Velocidade: %d %cm/h\n", r->velocidade, r->unidadeMedida);
                printf("\n");
                registrosEncontrados++;
            }
            liberarRegistro(r); // Libera o registro atual
            r = lerRegistroBin(sf); // Lê o próximo registro
        }

        // Caso nenhum registro tenha sido encontrado
        if (registrosEncontrados == 0) {
            printf("Registro inexistente.\n\n");
        }

        printf("Numero de paginas de disco: %d\n", header.nroPagDisco); // Mostra o número de páginas de disco
    }
    printf("\n");
    fclose(sf); // Fecha o arquivo
}


void funcionalidade4() {
    Cabecalho header;
    char srcFileName[30];
    int n;

    // Lê o nome do arquivo binário
    scanf("%s", srcFileName);
    FILE *sf = fopen(srcFileName, "rb+"); // Abre o arquivo para leitura e escrita
    if (sf == NULL) {
        printf("Falha no processamento do arquivo.");
        return;
    }

    // Lê o cabeçalho do arquivo
    lerCabecalhoBin(sf, &header);

    // Define o status como inconsistente ao abrir para modificações
    header.status = '0';

    // Lê o número de registros a serem removidos
    scanf("%d", &n);

    char campo[30], valor[50];

    // Executa 'n' remoções
    for (int i = 0; i < n; i++) {
        // Lê o campo a ser buscado
        scanf("%s", campo);

        // Verifica se o campo é uma string ou valor numérico
        if (strcmp(campo, "nome") == 0 || strcmp(campo, "especie") == 0 || strcmp(campo, "habitat") == 0 ||
            strcmp(campo, "dieta") == 0 || strcmp(campo, "tipo") == 0 || strcmp(campo, "alimento") == 0) {
            scan_quote_string(valor); // Leitura de strings entre aspas
        } else {
            scanf("%s", valor); // Leitura de valores numéricos
        }

        // Reposiciona o ponteiro no arquivo para começar após o cabeçalho
        fseek(sf, PAGE_SIZE, SEEK_SET);

        long int byteOffset;
        Registro *r = lerRegistroBin(sf); // Lê o primeiro registro
        int RRN = 0; // RRN começa no primeiro registro após o cabeçalho

        // Itera sobre os registros
        while (r != NULL) {
            byteOffset = ftell(sf);  // Armazena o offset do próximo registro
        

            if (r->removido == '0' && buscarRegistro(r, campo, valor)) {
                // Chama a função removeRegistro para marcar o registro como removido e atualizar a lista encadeada
                removeRegistro(r, header.topo);
                header.topo = ((byteOffset - PAGE_SIZE -160) / 160) ;  // Atualiza o topo da lista de removidos (RRN)

                // Reescreve o registro no arquivo
                fseek(sf, byteOffset - 160, SEEK_SET);
                fwrite(&r->removido, sizeof(char), 1, sf);  // Escreve campo 'removido'
                fwrite(&r->encadeamento, sizeof(int), 1, sf);  // Escreve campo 'encadeamento'

                // Preenche o restante com lixo
                char trash = TRASH;
                for (int j = 0; j < 155; j++) {
                    fwrite(&trash, sizeof(char), 1, sf);
                }

                // Atualiza o cabeçalho com o número de registros removidos
                header.nroRegRem++;  // Incrementa o número de registros removidos
                
                
            }
            
            liberarRegistro(r);  // Libera o registro atual
            r = lerRegistroBin(sf);  // Lê o próximo registro
        }
        liberarRegistro(r);  // Libera o último registro lido
    }

    // Atualiza o status para consistente
    header.status = '1';


    // Atualiza o número de páginas de disco
    header.nroPagDisco = (ftell(sf) + PAGE_SIZE - 1) / PAGE_SIZE;

    // Atualiza o cabeçalho no arquivo
    fseek(sf, 0, SEEK_SET);
    escreverCabecalhoBin(sf, &header);

    // Exibe o arquivo binário na tela
    
    fclose(sf); // Fecha o arquivo
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
    float floatValues;
    int intValues;

    // Lê o nome do arquivo binário
    scanf("%s", fileName);

    FILE *df = fopen(fileName, "rb+"); // Abre o arquivo para leitura e escrita
    if (df == NULL) {
        printf("Falha no processamento do arquivo.");
        return;
    }

    // Lê o cabeçalho do arquivo binário
    lerCabecalhoBin(df, &header);

    // Define o status como inconsistente
    header.status = '0';

    int numInsertions;
    scanf("%d", &numInsertions);  // Número de registros a serem inseridos

    for (int i = 0; i < numInsertions; i++) {
        Registro *r = criarRegistro();  // Cria um novo registro com valores padrão
        char fieldValue[50];  // Buffer para valores numéricos e strings

        // Leitura e tratamento do campo 'nome'
        scan_quote_string(fieldValue);
        if (strcmp(fieldValue, "NULO") == 0) {
            strcpy(r->nome, "#");  // Trata "NULO" com '#'
        } else {
            strcpy(r->nome, fieldValue);  // Copia a string lida
        }
        

        // Leitura e tratamento do campo 'dieta'
        scan_quote_string(fieldValue);
        if (strcmp(fieldValue, "NULO") == 0) {
            strcpy(r->dieta, "#");  // Trata "NULO" com '#'
        } else {
            strcpy(r->dieta, fieldValue);  // Copia a string lida
        }

        // Leitura e tratamento do campo 'habitat'
        scan_quote_string(fieldValue);
        if (strcmp(fieldValue, "NULO") == 0) {
            strcpy(r->habitat, "#");  // Trata "NULO" com '#'
        } else {
            strcpy(r->habitat, fieldValue);  // Copia a string lida
        }

        // Leitura e tratamento do campo 'população'
        scanf("%s", fieldValue);
        if (fieldValue == "NULO"){
            r->populacao = -1;
        }else{
            r->populacao = atoi(fieldValue);
        }
        // Leitura e tratamento do campo 'tipo'
        scan_quote_string(fieldValue);
        if (strcmp(fieldValue, "NULO") == 0) {
            strcpy(r->tipo, "#");  // Trata "NULO" com '#'
        } else {
            strcpy(r->tipo, fieldValue);  // Copia a string lida
        }

        // Leitura e tratamento do campo 'velocidade'
        scanf("%s", fieldValue);
        if (fieldValue == "NULO"){
            r->velocidade = -1;
        }else{
            r->velocidade = atoi(fieldValue);
        }
        

        // Leitura do campo 'unidadeMedida'
        scanf("%s", fieldValue);
        if (strcmp(fieldValue, "NULO") == 0) {
            r->unidadeMedida = '$';  // Atribui '$' para valores nulos
        } else {
            r->unidadeMedida = fieldValue[1];  // Atribui o primeiro caractere da string
        }
    

        // Leitura e tratamento do campo 'tamanho'
        scanf("%s", fieldValue);
        if (fieldValue == "NULO"){
            r->tamanho = -1;
        }else{
            r->tamanho = atoi(fieldValue);
        }
        // Leitura e tratamento do campo 'especie'
        scan_quote_string(fieldValue);
        if (strcmp(fieldValue, "NULO") == 0) {
            strcpy(r->especie, "#");  // Trata "NULO" com '#'
        } else {
            strcpy(r->especie, fieldValue);  // Copia a string lida
        }

        // Leitura e tratamento do campo 'alimento'
        scan_quote_string(fieldValue);
        if (strcmp(fieldValue, "NULO") == 0) {
            strcpy(r->alimento, "#");  // Trata "NULO" com '#'
        } else {
            strcpy(r->alimento, fieldValue);  // Copia a string lida
        }

        // Insere o registro reutilizando o espaço dos registros removidos ou ao final
        inserirRegistro(df, r, &header);
        liberarRegistro(r);  // Libera o registro após a escrita
    }


    int tamanhoArquivo = ftell(df);
    
    if (tamanhoArquivo % PAGE_SIZE == 0) {
        header.nroPagDisco = tamanhoArquivo / PAGE_SIZE;
    } else {
        header.nroPagDisco = (tamanhoArquivo / PAGE_SIZE) + 1;
    }

    header.status = '1';  // Marca o arquivo como consistente   

    // Reescreve o cabeçalho
    fseek(df, 0, SEEK_SET);
    escreverCabecalhoBin(df, &header);

    fclose(df);  // Fecha o arquivo
    binarioNaTela(fileName);  // Exibe o arquivo binário na tela
}




void funcionalidade6() {
    Cabecalho header = inicializarCabecalho();
    char srcFileName[30];

    // Lê o nome do arquivo binário de entrada (arquivo original)
    scanf("%s", srcFileName);
    FILE *sf = fopen(srcFileName, "rb"); // Abre o arquivo original para leitura
    if (sf == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Nome do arquivo compactado
    char *compactFileName = "compactado.bin";
    FILE *compactFile = fopen(compactFileName, "wb"); // Cria o arquivo compactado
    if (compactFile == NULL) {
        printf("Falha ao criar o arquivo compactado.\n");
        fclose(sf);
        return;
    }

    // Lê o cabeçalho do arquivo original
    lerCabecalhoBin(sf, &header);

    // Atualiza o status do arquivo compactado para inconsistente enquanto é escrito
    header.status = '0';
    escreverCabecalhoBin(compactFile, &header);

    // Posiciona o ponteiro no primeiro registro do arquivo original, após o cabeçalho
    fseek(sf, PAGE_SIZE, SEEK_SET);
    Registro *r;
    long int byteOffset;
    int RRN = 0;

    // Itera sobre os registros do arquivo original
    while (1) {
        byteOffset = ftell(sf); // Captura o byteOffset atual (posição do ponteiro)
        r = lerRegistroBin(sf); // Lê o próximo registro

        if (r == NULL) {
            break;  // Sai do loop quando não há mais registros
        }

        if (r->removido == '0') {  // Apenas registra os que NÃO estão removidos
            // Reescreve o registro não removido no arquivo compactado
            fseek(compactFile, 0, SEEK_END);  // Coloca o ponteiro no final do arquivo compactado
            escreverRegistro(compactFile, r); // Escreve o registro no arquivo compactado
        }

        RRN++;  // Incrementa o RRN
        liberarRegistro(r);  // Libera o registro atual
    }

    // Atualiza o status do arquivo compactado para consistente
    header.status = '1';
    header.topo = -1;  // O arquivo compactado não possui registros removidos
    header.nroRegRem = 0;
    header.qttCompacta++;
    header.proxRRN = RRN;
    // Atualiza o número de páginas de disco
    fseek(compactFile, 0, SEEK_END); // Vai para o final do arquivo compactado
    long tamanhoArquivo = ftell(compactFile);  // Tamanho do arquivo compactado
    header.nroPagDisco = (tamanhoArquivo + PAGE_SIZE - 1) / PAGE_SIZE;

    // Reescreve o cabeçalho atualizado no arquivo compactado
    fseek(compactFile, 0, SEEK_SET);
    escreverCabecalhoBin(compactFile, &header);

    // Fecha os arquivos
    fclose(sf);
    fclose(compactFile);

    // Exibe o conteúdo do arquivo compactado na tela
    binarioNaTela(compactFileName);
}
