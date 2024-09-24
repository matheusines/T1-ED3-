

# Trabalho de Manipulação de Arquivos Binários

## Descrição

Este projeto implementa diversas funcionalidades para manipulação de arquivos binários que armazenam registros de dados relacionados a diferentes entidades. As funcionalidades permitem a leitura, escrita, remoção e compactação de registros, além de outras operações sobre o arquivo binário. O programa é dividido em seis funcionalidades principais, que realizam operações específicas conforme a entrada do usuário.

## Funcionalidades

1. **Funcionalidade 1: Criação de Arquivo Binário**
   - Lê dados de um arquivo CSV e cria um arquivo binário com os registros lidos.
   - Ignora os registros removidos e atualiza o cabeçalho do arquivo com as informações corretas.

2. **Funcionalidade 2: Exibição de Registros**
   - Exibe todos os registros não removidos de um arquivo binário.
   - Imprime cada campo do registro e indica o número de páginas de disco ocupadas pelo arquivo.

3. **Funcionalidade 3: Busca de Registros**
   - Permite a busca de registros com base em critérios específicos.
   - Os campos de busca podem incluir nome, espécie, habitat, dieta, tipo, e outros.
   - Exibe os registros encontrados e o número de páginas de disco ocupadas.

4. **Funcionalidade 4: Remoção Lógica de Registros**
   - Realiza a remoção lógica de registros, marcando-os como removidos sem alterar fisicamente os dados.
   - Atualiza a lista encadeada de registros removidos no cabeçalho do arquivo.

5. **Funcionalidade 5: Inserção de Registros**
   - Insere novos registros no arquivo binário, reutilizando espaços de registros removidos ou inserindo no final do arquivo.
   - Trata campos nulos e atualiza o cabeçalho do arquivo com o novo status.

6. **Funcionalidade 6: Compactação de Arquivo**
   - Compacta o arquivo binário, removendo fisicamente os registros marcados como removidos.
   - Cria um novo arquivo compactado com apenas os registros válidos, atualizando o cabeçalho com as novas informações.

## Estrutura do Projeto

- `main.c`: Contém a função principal que gerencia a execução do programa e chama as funcionalidades conforme a entrada do usuário.
- `funcionalidades.c`: Implementa todas as funcionalidades descritas, incluindo leitura, escrita, inserção, remoção e compactação de registros.
- `funcionalidades.h`: Declaração das funções utilizadas nas funcionalidades principais do programa.
- `funcoesFornecidas.c`: Implementa funções fornecidas pelo projeto, que podem incluir operações de entrada e saída específicas, como leitura e escrita de registros na tela.
- `funcoesFornecidas.h`: Declaração das funções fornecidas, utilizadas para manipulações adicionais dos dados ou interface com o usuário.
- `utils.c`: Contém funções auxiliares para leitura, escrita e manipulação de registros e do cabeçalho do arquivo binário.
- `utils.h`: Declaração das funções auxiliares utilizadas em `utils.c`.
- `cabecalho.c`: Implementa funções específicas para manipulação do cabeçalho dos arquivos binários, incluindo leitura e escrita do cabeçalho.
- `cabecalho.h`: Declaração das funções relacionadas à manipulação do cabeçalho do arquivo binário.
- `registro.c`: Implementa funções para manipulação dos registros, como criação, leitura, escrita e liberação de memória.
- `registro.h`: Declaração das funções utilizadas para manipulação dos registros individuais do arquivo binário.
- `funcoesUteis.c`: Contém funções de suporte e utilitárias que são usadas em diversas partes do programa, auxiliando nas operações gerais.
- `funcoesUteis.h`: Declaração das funções utilitárias definidas em `funcoesUteis.c`.


## Como Executar

1. Compile o programa usando o GCC ou qualquer outro compilador C:
   ```bash
    make
   ```
   
2. Execute o programa:
   ```bash
   ./programaTrab
   ```

3. Siga as instruções na tela para escolher a funcionalidade desejada e fornecer as entradas necessárias.

## Entrada

- O programa recebe entradas por meio de arquivos CSV e binários. Para inserção e busca de registros, os valores são lidos via terminal conforme o formato esperado.

## Saída

- A saída do programa é exibida no terminal e, em alguns casos, os resultados são refletidos diretamente nos arquivos binários, como atualizações no cabeçalho ou registros removidos/inseridos.

## Observações

- **Tratamento de Erros:** O programa verifica falhas de abertura de arquivos e operações de leitura/escrita, exibindo mensagens de erro quando necessário.
- **Consistência dos Arquivos:** Antes de qualquer operação que modifique o arquivo, o status é alterado para inconsistente, retornando ao status consistente ao final da operação.

## Autores

- Matheus Ines 
- Bruno Tomé Rosa

Aqui está uma seção de Links Úteis que você pode adicionar ao seu README:

## Links Úteis

- [Apresentação do Trabalho](https://docs.google.com/presentation/d/1XyUVv3M26B43VlJZXYALKOFSn1I3_NTLpv6hP0_3Tn8/edit?usp=sharing) - Apresentação utilizada no vídeo 

## Licença

Este projeto é distribuído sob a licença [Escolha sua licença].

---
