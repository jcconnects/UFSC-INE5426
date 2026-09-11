Construa um analisador léxico baseado em diagramas de transição que identifica todos os tokens da gramática base. O seu analisador deverá ler caracter por caracter da entrada. No fim do processo escreva a lista de tokens obtida na tela do computador.

O analisador léxico precisa ler caracter por caracter da entrada e precisa ser baseado em diagrama de transição. As palavras reservadas podem ser tratadas através do token IDENT. Os operadores aritméticos podem ser agrupados. Os operadores relacionais podem ser agrupados.

Entrada:

def f (int a) {

x = 999 + "venceremos";

return;

}

Saída:

DEF IDENT AP INT IDENT FP AC

IDENT AT INT_CONSTANT ADD STRING_CONSTANT PV

RETURN PV

FC
