Construa um analisador léxico baseado em diagramas de transição que identifica os tokens IDENT (identificador), BRANCO e OUTRO. Qualquer token BRANCO deve ser ignorado. O seu analisador deverá receber um código fonte e devolver uma lista de tokens contendo somente os tokens IDENT e OUTRO. O seu analisador deverá ler caracter por caracter da entrada. No fim do processo escreva a lista de tokens obtida na tela do computador.

O analisador léxico precisa ler caracter por caracter da entrada e precisa ser baseado em diagrama de transição.

Entrada:

def f (int a) {

x = x + a;

return;

}

Saída:

IDENT IDENT OUTRO IDENT IDENT OUTRO OUTRO

IDENT OUTRO IDENT OUTRO IDENT OUTRO

IDENT OUTRO

OUTRO
