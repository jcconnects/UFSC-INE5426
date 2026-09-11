#include <stdio.h>
#include <ctype.h>
#include <string.h>

/*
 * Analisador lexico baseado em diagramas de transicao (Lab 6).
 * Le caracter por caracter do arquivo entrada.txt e reconhece TODOS
 * os tokens da gramatica base.
 *
 * Cada scanner (getIdent, getNumber, getRelop, getString) implementa
 * um diagrama de transicao com switch(state), no estilo getRelop() do
 * livro do Dragao (Aho, Lam, Sethi, Ullman).
 *
 * Convencoes dos diagramas (slides 14-18):
 *   - estado final marcado com '*' faz LOOKAHEAD: leu 1 char a mais,
 *     devolve com retract() ao fluxo antes de aceitar.
 *   - a execucao para ao chegar em um estado final.
 */

/* ------------------------------------------------------------------ */
/* (1) Tipos de token                                                 */
/* ------------------------------------------------------------------ */

typedef enum {
    /* --- identificador --- */
    TK_IDENT,          /* letra (letra|digito)*                       */

    /* --- palavras reservadas (subconjunto do IDENT) --- */
    TK_DEF,            /* def                                         */
    TK_INT,            /* int                                         */
    TK_FLOAT,          /* float                                       */
    TK_STRING,         /* string                                      */
    TK_RETURN,         /* return                                      */
    TK_IF,             /* if                                          */
    TK_ELSE,           /* else                                        */
    TK_FOR,            /* for                                         */
    TK_NEW,            /* new                                         */
    TK_BREAK,          /* break                                       */
    TK_PRINT,          /* print                                       */
    TK_READ,           /* read                                        */

    /* --- constantes literais --- */
    TK_INT_CONSTANT,   /* 999                                         */
    TK_FLOAT_CONSTANT, /* 3.14, 1e10, 2.5E-3                          */
    TK_STRING_CONSTANT,/* "venceremos"                                */

    /* --- operadores aritmeticos (individuais) --- */
    TK_ADD,            /* +                                           */
    TK_SUB,            /* -                                           */
    TK_MUL,            /* *                                           */
    TK_DIV,            /* /                                           */

    /* --- atribuicao e relacionais --- */
    TK_AT,             /* =   (atribuicao)                            */
    TK_RELOP,          /* < <= == != > >=  (grupo relacional)         */

    /* --- pontuacao / delimitadores --- */
    TK_AP,             /* (                                           */
    TK_FP,             /* )                                           */
    TK_AC,             /* {                                           */
    TK_FC,             /* }                                           */
    TK_COL,            /* [                                           */
    TK_CR,             /* ]                                           */
    TK_PV,             /* ;                                           */
    TK_VIR,            /* ,                                           */

    /* --- controle --- */
    TK_BRANCO,         /* espaco/tab/\n: reconhecido e ignorado       */
    TK_ERRO,           /* caractere/cadeia invalida (erro lexico)     */
    TK_EOF             /* fim do arquivo                              */
} TokenTipo;

/* nome de cada token para impressao. MESMA ordem do enum. */
static const char *TOKEN_NOME[] = {
    "IDENT",
    "DEF", "INT", "FLOAT", "STRING", "RETURN",
    "IF", "ELSE", "FOR", "NEW", "BREAK", "PRINT", "READ",
    "INT_CONSTANT", "FLOAT_CONSTANT", "STRING_CONSTANT",
    "ADD", "SUB", "MUL", "DIV",
    "AT", "RELOP",
    "AP", "FP", "AC", "FC", "COL", "CR", "PV", "VIR",
    "BRANCO", "ERRO", "EOF"
};

/* token carrega tipo + lexema (atributo repassado ao parser) */
typedef struct {
    TokenTipo tipo;
    char lexema[128];
} TOKEN;

/* ------------------------------------------------------------------ */
/* Tabela de palavras reservadas                                      */
/* Um IDENT reconhecido e conferido aqui; se casar, vira reservada.   */
/* Adicionar palavra reservada = 1 linha nesta tabela.                */
/* ------------------------------------------------------------------ */

typedef struct {
    const char *palavra;
    TokenTipo   tipo;
} Reservada;

static const Reservada RESERVADAS[] = {
    { "def",    TK_DEF    },
    { "int",    TK_INT    },
    { "float",  TK_FLOAT  },
    { "string", TK_STRING },
    { "return", TK_RETURN },
    { "if",     TK_IF     },
    { "else",   TK_ELSE   },
    { "for",    TK_FOR    },
    { "new",    TK_NEW    },
    { "break",  TK_BREAK  },
    { "print",  TK_PRINT  },
    { "read",   TK_READ   }
};

static TokenTipo classifica_ident(const char *lexema) {
    int i;
    int n = (int) (sizeof(RESERVADAS) / sizeof(RESERVADAS[0]));
    for (i = 0; i < n; i++) {
        if (strcmp(lexema, RESERVADAS[i].palavra) == 0)
            return RESERVADAS[i].tipo;   /* casou: palavra reservada */
    }
    return TK_IDENT;                      /* nao casou: identificador */
}

/* ------------------------------------------------------------------ */
/* Leitura de caracteres: nextChar() / retract()                      */
/* Centraliza o retrocesso de 1 caractere (lookahead dos diagramas).  */
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
/* Classes de caractere (alfabeto em um so lugar)                     */
/* ------------------------------------------------------------------ */

static int eh_inicio_ident(int c) { return isalpha(c) || c == '_'; }
static int eh_meio_ident(int c)   { return isalnum(c) || c == '_'; }
static int eh_digito(int c)       { return isdigit(c); }
static int eh_branco(int c)       { return c == ' ' || c == '\t' ||
                                           c == '\n' || c == '\r'; }

/* ------------------------------------------------------------------ */
/* Helper: acrescenta char ao lexema com protecao de limite           */
/* ------------------------------------------------------------------ */

static void push_lex(TOKEN *t, int *n, int c) {
    if (*n < (int) sizeof(t->lexema) - 1)
        t->lexema[(*n)++] = (char) c;
}

/* ================================================================== */
// DIAGRAMA 1 - IDENT
/* ================================================================== */

static TOKEN getIdent(int primeiro) {
    TOKEN ret;
    int state = 12;               /* ja consumimos a 1a letra (11->12) */
    int n = 0;
    int c = primeiro;

    push_lex(&ret, &n, c);

    while (1) {
        switch (state) {
            case 12:
                c = nextChar();
                if (eh_meio_ident(c)) {
                    push_lex(&ret, &n, c);
                    /* permanece em 12 */
                } else {
                    state = 13;
                }
                break;
            case 13:                          /* estado final com '*' */
                retract();                    /* lookahead: devolve char extra */
                ret.lexema[n] = '\0';
                ret.tipo = classifica_ident(ret.lexema);
                return ret;
        }
    }
}

/* ================================================================== */
// DIAGRAMA 2 - NUMERO
/* ================================================================== */

static TOKEN getNumber(int primeiro) {
    TOKEN ret;
    int state = 18;               /* ja consumimos o 1o digito (17->18) */
    int n = 0;
    int c = primeiro;

    push_lex(&ret, &n, c);

    while (1) {
        switch (state) {
            case 18:                              /* parte inteira */
                c = nextChar();
                if (eh_digito(c))      { push_lex(&ret, &n, c); }
                else if (c == '.')     { push_lex(&ret, &n, c); state = 19; }
                else if (c == 'E' || c == 'e') { push_lex(&ret, &n, c); state = 21; }
                else                   { state = 25; }
                break;

            case 19:                              /* apos '.', exige digito */
                c = nextChar();
                if (eh_digito(c))      { push_lex(&ret, &n, c); state = 20; }
                else {
                    /* '.' sem digito -> numero mal formado (erro lexico) */
                    retract();
                    ret.lexema[n] = '\0';
                    ret.tipo = TK_ERRO;
                    return ret;
                }
                break;

            case 20:                              /* parte fracionaria */
                c = nextChar();
                if (eh_digito(c))      { push_lex(&ret, &n, c); }
                else if (c == 'E' || c == 'e') { push_lex(&ret, &n, c); state = 21; }
                else                   { state = 26; }
                break;

            case 21:                              /* apos E: sinal ou digito */
                c = nextChar();
                if (c == '+' || c == '-') { push_lex(&ret, &n, c); state = 22; }
                else if (eh_digito(c))    { push_lex(&ret, &n, c); state = 23; }
                else {
                    /* expoente sem digito -> erro lexico */
                    retract();
                    ret.lexema[n] = '\0';
                    ret.tipo = TK_ERRO;
                    return ret;
                }
                break;

            case 22:                              /* apos sinal, exige digito */
                c = nextChar();
                if (eh_digito(c))      { push_lex(&ret, &n, c); state = 23; }
                else {
                    retract();
                    ret.lexema[n] = '\0';
                    ret.tipo = TK_ERRO;
                    return ret;
                }
                break;

            case 23:                              /* laco do expoente */
                c = nextChar();
                if (eh_digito(c))      { push_lex(&ret, &n, c); }
                else                   { state = 24; }
                break;

            case 24:                              /* final* FLOAT (com expoente) */
                retract();
                ret.lexema[n] = '\0';
                ret.tipo = TK_FLOAT_CONSTANT;
                return ret;

            case 25:                              /* final* INT */
                retract();
                ret.lexema[n] = '\0';
                ret.tipo = TK_INT_CONSTANT;
                return ret;

            case 26:                              /* final* FLOAT (so fracao) */
                retract();
                ret.lexema[n] = '\0';
                ret.tipo = TK_FLOAT_CONSTANT;
                return ret;
        }
    }
}

/* ================================================================== */
// DIAGRAMA 3 - RELOP + ATRIBUICAO
/* ================================================================== */

static TOKEN getRelop(int primeiro) {
    TOKEN ret;
    int n = 0;
    int c = primeiro;
    int state;

    push_lex(&ret, &n, c);

    /* estado inicial escolhido pelo 1o caractere */
    if      (c == '<') state = 1;
    else if (c == '=') state = 4;
    else if (c == '>') state = 7;
    else               state = 9;   /* '!' */

    while (1) {
        switch (state) {
            case 1:                               /* leu '<' */
                c = nextChar();
                if (c == '=')      { push_lex(&ret, &n, c); ret.lexema[n]='\0'; ret.tipo=TK_RELOP; return ret; } /* <= */
                else if (c == '>') { push_lex(&ret, &n, c); ret.lexema[n]='\0'; ret.tipo=TK_RELOP; return ret; } /* <> */
                else { retract(); ret.lexema[n]='\0'; ret.tipo=TK_RELOP; return ret; }                          /* <  */

            case 4:                               /* leu '=' */
                c = nextChar();
                if (c == '=') { push_lex(&ret, &n, c); ret.lexema[n]='\0'; ret.tipo=TK_RELOP; return ret; }     /* == */
                else { retract(); ret.lexema[n]='\0'; ret.tipo=TK_AT; return ret; }                             /* =  (atribuicao) */

            case 7:                               /* leu '>' */
                c = nextChar();
                if (c == '=') { push_lex(&ret, &n, c); ret.lexema[n]='\0'; ret.tipo=TK_RELOP; return ret; }     /* >= */
                else { retract(); ret.lexema[n]='\0'; ret.tipo=TK_RELOP; return ret; }                          /* >  */

            case 9:                               /* leu '!' */
                c = nextChar();
                if (c == '=') { push_lex(&ret, &n, c); ret.lexema[n]='\0'; ret.tipo=TK_RELOP; return ret; }     /* != */
                else {
                    /* '!' isolado nao pertence a gramatica base -> erro */
                    retract(); ret.lexema[n]='\0'; ret.tipo=TK_ERRO; return ret;
                }
        }
    }
}

/* ================================================================== */
/* DIAGRAMA 4 - STRING_CONSTANT  "..."                                 */
/* ================================================================== */

static TOKEN getString(void) {
    TOKEN ret;
    int n = 0;
    int c;

    push_lex(&ret, &n, '"');       /* aspa de abertura */

    while (1) {
        c = nextChar();
        if (c == '"') {                       /* fecha string: aceita */
            push_lex(&ret, &n, c);
            ret.lexema[n] = '\0';
            ret.tipo = TK_STRING_CONSTANT;
            return ret;
        }
        if (c == EOF || c == '\n') {          /* string nao fechada: erro */
            retract();
            ret.lexema[n] = '\0';
            ret.tipo = TK_ERRO;
            return ret;
        }
        push_lex(&ret, &n, c);                /* char comum: acumula */
    }
}

/* ------------------------------------------------------------------ */
/* Tabela de simbolos de 1 caractere -> token                         */
/* Adicionar pontuacao/operador simples = 1 linha aqui.               */
/* ------------------------------------------------------------------ */

static TokenTipo simbolo_para_token(int c) {
    switch (c) {
        case '+': return TK_ADD;
        case '-': return TK_SUB;
        case '*': return TK_MUL;
        case '/': return TK_DIV;
        case '(': return TK_AP;
        case ')': return TK_FP;
        case '{': return TK_AC;
        case '}': return TK_FC;
        case '[': return TK_COL;
        case ']': return TK_CR;
        case ';': return TK_PV;
        case ',': return TK_VIR;
        default:  return TK_ERRO;   /* nao mapeado = erro lexico */
    }
}

/* ------------------------------------------------------------------ */
/* proximo_token(): decide qual diagrama rodar e devolve 1 TOKEN      */
/* ------------------------------------------------------------------ */

static TOKEN proximo_token(void) {
    TOKEN ret;
    int c = nextChar();

    if (c == EOF) {
        ret.tipo = TK_EOF;
        ret.lexema[0] = '\0';
        return ret;
    }

    /* brancos: reconhece e sinaliza (ignorados pelo main) */
    if (eh_branco(c)) {
        ret.tipo = TK_BRANCO;
        ret.lexema[0] = (char) c;
        ret.lexema[1] = '\0';
        return ret;
    }

    /* diagrama 1: identificador / palavra reservada */
    if (eh_inicio_ident(c)) return getIdent(c);

    /* diagrama 2: numero (inteiro / real / cientifico) */
    if (eh_digito(c))       return getNumber(c);

    /* diagrama 3: relacionais e atribuicao */
    if (c == '<' || c == '=' || c == '>' || c == '!') return getRelop(c);

    /* diagrama 4: string constante */
    if (c == '"')           return getString();

    /* simbolo simples de 1 caractere (ou erro) */
    ret.tipo = simbolo_para_token(c);
    ret.lexema[0] = (char) c;
    ret.lexema[1] = '\0';
    return ret;
}

/* ------------------------------------------------------------------ */
/* main                                                               */
/* ------------------------------------------------------------------ */

int main(void) {
    TOKEN t;
    int erros = 0;

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
        if (t.tipo == TK_ERRO) {
            fprintf(stderr, "\n[ERRO LEXICO] cadeia invalida: \"%s\"\n", t.lexema);
            erros++;
            continue;
        }
        printf("%s ", TOKEN_NOME[t.tipo]);
    }
    printf("\n");

    fclose(fonte);

    if (erros > 0) {
        fprintf(stderr, "\n%d erro(s) lexico(s) encontrado(s).\n", erros);
        return 1;
    }
    return 0;
}
