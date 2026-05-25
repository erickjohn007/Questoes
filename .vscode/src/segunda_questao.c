#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERTICES 4
#define ARESTAS 4

// Converte o índice numérico da aresta (0 a 3) em letras minúsculas (a a d) como na referência
char obterLetraAresta(int indice) {
    return 'a' + indice;
}

// Converte o texto do grafo em formato compatível com link URL (URL Encoding)
void urlEncode(char* dest, const char* src) {
    while (*src) {
        if (*src == ' ')  { strcat(dest, "%20"); }
        else if (*src == '\n') { strcat(dest, "%0A"); }
        else if (*src == '{')  { strcat(dest, "%7B"); }
        else if (*src == '}')  { strcat(dest, "%7D"); }
        else if (*src == '"')  { strcat(dest, "%22"); }
        else if (*src == '#')  { strcat(dest, "%23"); }
        else if (*src == '=')  { strcat(dest, "%3D"); }
        else if (*src == ';')  { strcat(dest, "%3B"); }
        else if (*src == '[')  { strcat(dest, "%5B"); }
        else if (*src == ']')  { strcat(dest, "%5D"); }
        else if (*src == '-')  { strcat(dest, "%2D"); }
        else if (*src == '>')  { strcat(dest, "%3E"); }
        else {
            char tmp[2] = {*src, '\0'};
            strcat(dest, tmp);
        }
        src++;
    }
}

int main() {
    // Representação dos pares de arestas originais do seu modelo
    int conexoes[ARESTAS][2] = {
        {0, 1}, // Aresta a: (x1, x2)
        {0, 2}, // Aresta b: (x1, x3)
        {1, 3}, // Aresta c: (x2, x4)
        {2, 3}  // Aresta d: (x3, x4)
    };

    // IMPRESSÃO DOS DADOS DE ENTRADA NA TELA
    printf("=== DADOS DE ENTRADA DO GRAFO ===\n");
    printf("Vertices: V1, V2, V3, V4\n");
    printf("Arestas fornecidas:\n");
    for (int j = 0; j < ARESTAS; j++) {
        printf("  Aresta %c: (V%d, V%d)\n", obterLetraAresta(j), conexoes[j][0] + 1, conexoes[j][1] + 1);
    }
    printf("\n");

    char dotCode[1024] = "";
    char urlFinal[2048] = "https://dreampuf.github.io/GraphvizOnline/#";
    char encodedDot[1024] = "";
    char comandoSistema[2100] = "";

    // CONFIGURAÇÃO VISUAL DO GRAFO
    strcat(dotCode, "graph G {\n");
    
    // REDIMENSIONAMENTO DE ESPAÇO E TAMANHO BASEADO NA REFERENCIA:
    // Nós brancos com contorno preto (fillcolor=#FFFFFF, color=#000000)
    strcat(dotCode, "    node [shape=circle,style=filled,fillcolor=\"#FFFFFF\",color=\"#000000\",fontname=\"Arial\",fontsize=12,width=0.5,fixedsize=true];\n");
    // Linhas pretas finas e rótulos discretos (fontsize=10)
    strcat(dotCode, "    edge [color=\"#000000\",penwidth=1.0,fontname=\"Arial\",fontsize=10];\n"); 
    // Garante o distanciamento geométrico adequado para evitar distorções
    strcat(dotCode, "    graph [overlap=false, splines=true, sep=0.4];\n");
    strcat(dotCode, "    layout=neato;\n");

    // MAPEAMENTO TEXTUAL E CONSTRUÇÃO DAS ARESTAS DO DESENHO
    printf("=== PROCESSAMENTO E MONTAGEM DO GRAFO ===\n");
    for (int j = 0; j < ARESTAS; j++) {
        int v1 = conexoes[j][0] + 1;
        int v2 = conexoes[j][1] + 1;
        char letra = obterLetraAresta(j);

        printf("Mapeando Aresta %c: Conexao nao direcionada V%d <-> V%d\n", letra, v1, v2);

        // Adiciona a linha estruturada no formato DOT do Graphviz
        char linhaAresta[150];
        sprintf(linhaAresta, "    V%d -- V%d [label=\" %c\"];\n", v1, v2, letra);
        strcat(dotCode, linhaAresta);
    }
    strcat(dotCode, "}\n");

    // INTERFACE COM O SISTEMA OPERACIONAL (Abertura automática do link no navegador)
    urlEncode(encodedDot, dotCode);
    strcat(urlFinal, encodedDot);

    printf("\n[INFO] Abrindo navegador para exibir o grafo resultante...\n");

#if defined(_WIN32) || defined(_WIN64)
    sprintf(comandoSistema, "start \"\" \"%s\"", urlFinal); // Comando Windows
#elif __APPLE__
    sprintf(comandoSistema, "open \"%s\"", urlFinal);       // Comando macOS
#else
    sprintf(comandoSistema, "xdg-open \"%s\"", urlFinal);   // Comando Linux
#endif

    system(comandoSistema);

    return 0;
}