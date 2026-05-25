#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Mantida a estrutura e dimensão original do seu template
#define VERTICES 4
#define ARESTAS 6

// Converte o índice numérico da coluna (0 a 5) em letra correspondente
char obterLetraAresta(int indice) {
    return 'a' + indice; // 'a', 'b', 'c'... em minúsculo como na imagem
}

// Converte o texto do grafo em formato compatível com link URL
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
    // Matriz de Incidência original do seu modelo
    int M[VERTICES][ARESTAS] = {
        {1, 1, 0, 0, 0, 0}, 
        {1, 0, 1, 0, 0, 1}, 
        {0, 1, 1, 1, 0, 0}, 
        {0, 0, 0, 1, 2, 1}  
    };

    // IMPRESSÃO DA MATRIZ NA TELA
    printf("=== MATRIZ DE INCIDENCIA FORNECIDA ===\n      a   b   c   d   e   f\n");
    for (int i = 0; i < VERTICES; i++) {
        printf("V%d: ", i + 1);
        for (int j = 0; j < ARESTAS; j++) {
            printf("%3d ", M[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // LÓGICA DE DETECÇÃO: Se houver valor negativo (-1), o grafo é dirigido
    int ehDirigido = 0;
    for (int i = 0; i < VERTICES; i++) {
        for (int j = 0; j < ARESTAS; j++) {
            if (M[i][j] < 0) {
                ehDirigido = 1;
                break;
            }
        }
    }

    printf("=== QUESTAO 1: MAPEAMENTO LOGICO DAS ARESTAS ===\n");
    printf("Tipo de Grafo Identificado: %s\n\n", ehDirigido ? "DIRIGIDO (-1 e +1)" : "NAO DIRIGIDO (1 e 2)");

    char dotCode[1024] = "";
    char urlFinal[2048] = "https://dreampuf.github.io/GraphvizOnline/#";
    char encodedDot[1024] = "";
    char comandoSistema[2100] = "";

    // CONFIGURAÇÃO VISUAL DO GRAFO
    if (ehDirigido) {
        strcat(dotCode, "digraph G {\n");
    } else {
        strcat(dotCode, "graph G {\n");
    }
    
    // REDIMENSIONAMENTO DE ESPAÇO E TAMANHO BASEADO NA REFERENCIA:
    // Nós brancos com bordas pretas finas (fillcolor=#FFFFFF, color=#000000)
    strcat(dotCode, "    node [shape=circle,style=filled,fillcolor=\"#FFFFFF\",color=\"#000000\",fontname=\"Arial\",fontsize=12,width=0.5,fixedsize=true];\n");
    // Arestas pretas com tamanho de fonte reduzido para não poluir (fontsize=10)
    strcat(dotCode, "    edge [color=\"#000000\",penwidth=1.0,fontname=\"Arial\",fontsize=10];\n"); 
    // Separação geométrica controlada para manter os nós bem espaçados
    strcat(dotCode, "    graph [overlap=false, splines=true, sep=0.4];\n");
    strcat(dotCode, "    layout=neato;\n");

    // VARREDURA DA MATRIZ COLUNA POR COLUNA (ARESTA POR ARESTA)
    for (int j = 0; j < ARESTAS; j++) {
        int origem = -1, destino = -1;
        int v1 = -1, v2 = -1;
        int eh_loop = 0;
        char letra = obterLetraAresta(j);

        for (int i = 0; i < VERTICES; i++) {
            if (ehDirigido) {
                if (M[i][j] == -1) origem = i;
                if (M[i][j] == 1)  destino = i;
            } else {
                if (M[i][j] == 1) {
                    if (v1 == -1) v1 = i;
                    else v2 = i;
                } else if (M[i][j] == 2) {
                    v1 = i;
                    v2 = i;
                    eh_loop = 1;
                }
            }
        }

        // MAPEAMENTO TEXTUAL E CONSTRUÇÃO DAS ARESTAS DO DESENHO
        char linhaAresta[150];
        if (ehDirigido) {
            if (origem != -1 && destino != -1) {
                printf("Aresta %c: Sentido orientado V%d -> V%d\n", letra, origem + 1, destino + 1);
                sprintf(linhaAresta, "    V%d -> V%d [label=\" %c\"];\n", origem + 1, destino + 1, letra);
                strcat(dotCode, linhaAresta);
            }
        } else {
            if (v1 != -1 && v2 != -1) {
                if (eh_loop) {
                    printf("Aresta %c: LACO (loop) detectado no vertice V%d\n", letra, v1 + 1);
                    sprintf(linhaAresta, "    V%d -- V%d [label=\" %c\"];\n", v1 + 1, v2 + 1, letra);
                } else {
                    printf("Aresta %c: Conexao nao direcionada V%d <-> V%d\n", letra, v1 + 1, v2 + 1);
                    sprintf(linhaAresta, "    V%d -- V%d [label=\" %c\"];\n", v1 + 1, v2 + 1, letra);
                }
                strcat(dotCode, linhaAresta);
            }
        }
    }
    strcat(dotCode, "}\n");

    // INTERFACE COM O SISTEMA OPERACIONAL
    urlEncode(encodedDot, dotCode);
    strcat(urlFinal, encodedDot);

    printf("\n[INFO] Abrindo navegador com estilo e tamanhos calibrados...\n");

#if defined(_WIN32) || defined(_WIN64)
    sprintf(comandoSistema, "start \"\" \"%s\"", urlFinal);
#elif __APPLE__
    sprintf(comandoSistema, "open \"%s\"", urlFinal);
#else
    sprintf(comandoSistema, "xdg-open \"%s\"", urlFinal);
#endif

    system(comandoSistema);

    return 0;
}