#include <stdio.h>         // Inclui a biblioteca padrão de entrada e saída para usar funções como printf() e scanf().
#include <stdlib.h>        // Inclui a biblioteca padrão para uso de funções como malloc(), free(), etc.
#include "funcionalidades.h" // Inclui o arquivo de cabeçalho "funcionalidades.h" onde as funções `funcionalidade1()`, `funcionalidade2()`, etc., estão declaradas.

int main() {
    int opt;              // Declara a variável 'opt' para armazenar a opção do usuário.
    scanf("%d", &opt);    // Lê a entrada do usuário e armazena o valor na variável 'opt'.

    // Estrutura switch para executar a função correspondente com base na entrada do usuário.
    switch (opt) {
        case 1:                    // Caso o usuário insira 1
            funcionalidade1();     // Chama a função `funcionalidade1()`
            break;                 // Encerra o case para evitar a execução de outros casos.

        case 2:                    // Caso o usuário insira 2
            funcionalidade2();     // Chama a função `funcionalidade2()`
            break;                 // Encerra o case.

        case 3:                    // Caso o usuário insira 3
            funcionalidade3();     // Chama a função `funcionalidade3()`
            break;                 // Encerra o case.
            
        case 4:                    // Caso o usuário insira 4
            funcionalidade4();     // Chama a função `funcionalidade4()`
            break;                 // Encerra o case.

        case 5:                    // Caso o usuário insira 5
            // Não há nenhuma ação definida para o case 5.
            break;                 // Encerra o case.

        case 6:                    // Caso o usuário insira 6
            funcionalidade6();     // Chama a função `funcionalidade6()`
            break;                 // Encerra o case.

        default:                   // Caso o usuário insira qualquer outro valor que não seja 1-6
            break;                 // Nenhuma ação é realizada.
    }
}
