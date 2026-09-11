#include <stdio.h>
#include <ctype.h>
#include <string.h>

/*
 * Analisador lexico baseado em diagramas de transicao.
 * Le caracter por caracter do arquivo entrada.txt.
 */

/* ------------------------------------------------------------------ */
/* (1) Tipos de token                                                 */
/* ------------------------------------------------------------------ */

typedef enum {
    TK_IDENT,     /* identificador: letra (letra|digito)*            */
    TK_OUTRO,     /* fallback: qualquer simbolo ainda nao mapeado    */
    TK_BRANCO,    /* espaco/tab/\n: reconhecido e ignorado           */
    TK_EOF        /* fim do arquivo                                  */
} TokenTipo;

/* nome de cada token para impressao. MESMA ordem do enum. */
static const char *TOKEN_NOME[] = {
    "IDENT", "OUTRO",
    "BRANCO", "EOF"
};

/* (2 leve) token carrega tipo + lexema, para uso futuro do parser */
typedef struct {
    TokenTipo tipo;
    char lexema[64];
} TOKEN;

/* ------------------------------------------------------------------ */
/* Leitura de caracteres: nextChar() / retract()                      */
/* Centraliza o retrocesso de 1 caractere usado por todos scanners.   */
/* ------------------------------------------------------------------ */

static FILE *fonte;   /* arquivo de entrada */
static int   ultimo;  /* ultimo char lido, para retract() devolver    */
static int   voltou;  /* 1 = proximo nextChar() reusa 'ultimo'        */

static int nextChar(void) {
    if (voltou) {          /* ha um char devolvido: reaproveita */
        voltou = 0;
        return ultimo;
    }
    ultimo = getc(fonte);
    return ultimo;
}

static void retract(void) {
    voltou = 1;            /* devolve 1 char: proximo nextChar() o repete */
}

/* ------------------------------------------------------------------ */
/* Tabela de simbolos: char de 1 posicao -> token                     */
/* Adicionar um novo operador/pontuacao = 1 linha aqui.               */
/* O que nao estiver mapeado cai em TK_OUTRO (comportamento antigo).  */
/* ------------------------------------------------------------------ */

static TokenTipo simbolo_para_token(int c) {
    switch (c) {
        default:  return TK_OUTRO;   /* fallback = OUTRO */
    }
}

/* ------------------------------------------------------------------ */
/* Classes de caractere do identificador (alfabeto em um so lugar)    */
/* ------------------------------------------------------------------ */

static int eh_inicio_ident(int c) { return isalpha(c); }
static int eh_meio_ident(int c)   { return isalnum(c); }

/* ------------------------------------------------------------------ */
/* Scanner do IDENT: diagrama de transicao com switch(state)          */
/* Estilo getRelop(): entra ja com o 1o char lido em 'primeiro'.      */
/*   estado 0: consome (letra|digito)*                                */
/*   estado 1: aceita, retract() no char extra, retorna IDENT         */
/* ------------------------------------------------------------------ */

static TOKEN getIdent(int primeiro) {
    TOKEN ret;
    int state = 0;
    int n = 0;
    int c = primeiro;

    ret.tipo = TK_IDENT;
    ret.lexema[n++] = (char) c;   /* 1o char ja e letra */

    while (1) {
        switch (state) {
            case 0:
                c = nextChar();
                if (eh_meio_ident(c)) {
                    if (n < (int) sizeof(ret.lexema) - 1)
                        ret.lexema[n++] = (char) c;
                    /* permanece no estado 0 */
                } else {
                    state = 1;
                }
                break;
            case 1:
                retract();            /* char extra nao pertence ao ident */
                ret.lexema[n] = '\0';
                return ret;
        }
    }
}

/* ------------------------------------------------------------------ */
/* proximo_token(): decide qual scanner rodar e devolve 1 TOKEN       */
/* ------------------------------------------------------------------ */

static TOKEN proximo_token(void) {
    TOKEN ret;
    int c = nextChar();

    if (c == EOF) {
        ret.tipo = TK_EOF;
        ret.lexema[0] = '\0';
        return ret;
    }

    if (eh_inicio_ident(c)) {
        return getIdent(c);           /* diagrama do identificador */
    }

    if (c == ' ' || c == '\t' || c == '\n') {
        ret.tipo = TK_BRANCO;         /* ignorado pelo main */
        ret.lexema[0] = (char) c;
        ret.lexema[1] = '\0';
        return ret;
    }

    /* simbolo de 1 caractere: tabela decide o tipo (ou OUTRO) */
    ret.tipo = simbolo_para_token(c);
    ret.lexema[0] = (char) c;
    ret.lexema[1] = '\0';
    return ret;
}

/* ------------------------------------------------------------------ */
// main
/* ------------------------------------------------------------------ */

int main(void) {
    TOKEN t;

    fonte = fopen("entrada.txt", "r");
    if (fonte == NULL) {
        fprintf(stderr, "erro: nao foi possivel abrir entrada.txt\n");
        return 1;
    }

    while ((t = proximo_token()).tipo != TK_EOF) {
        if (t.tipo == TK_BRANCO) {
            /* BRANCO ignorado; \n so quebra linha para casar o layout */
            if (t.lexema[0] == '\n')
                printf("\n");
            continue;
        }
        printf("%s ", TOKEN_NOME[t.tipo]);
    }

    fclose(fonte);
    return 0;
}
