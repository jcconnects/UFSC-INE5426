# Diagramas de Transição — Analisador Léxico (Lab 6)

Um arquivo `.puml` **por função** do `lexico.c`, para editar/paralelizar cada
diagrama de forma independente. PNGs pré-renderizados ao lado.

| Arquivo | Função em `lexico.c` | Tokens reconhecidos | Estados (slide) |
|---------|----------------------|---------------------|-----------------|
| `1_ident.puml`      | `getIdent()`   | IDENT + reservadas (DEF, INT, FLOAT, STRING, RETURN, IF, ELSE, FOR, NEW, BREAK, PRINT, READ) | 11–13\* (slide 17) |
| `2_number.puml`     | `getNumber()`  | INT_CONSTANT, FLOAT_CONSTANT (real + notação científica) | 17–26\* (slide 18) |
| `3_relop.puml`      | `getRelop()`   | RELOP (`< <= == != > >=`), AT (`=`) | 0–11\* (slide 14) |
| `4_string.puml`     | `getString()`  | STRING_CONSTANT (`"..."`) | — |
| `5_dispatcher.puml` | `proximo_token()` | despacho para os diagramas acima | — |

## Convenções (slides 14–18)

- Estado final com **`*`** = *lookahead*: o scanner leu 1 caractere a mais e o
  devolve ao fluxo com `retract()` antes de aceitar o token.
- A execução do diagrama **para** ao chegar num estado final (`<<accepting>>`).
- Palavras reservadas são tratadas como IDENT no diagrama e depois
  reclassificadas pela tabela de reservadas (`classifica_ident()`).
- Operadores aritméticos são individuais (ADD, SUB, MUL, DIV); relacionais são
  agrupados em RELOP; `=` isolado vira AT (atribuição).

## Renderizar

Não há JRE local (o `/usr/bin/java` é um stub). Opções:

```bash
# 1) VS Code: extensão "PlantUML" (Alt+D para preview)

# 2) Servidor web PlantUML (sem instalar nada):
#    cole o conteúdo do .puml em https://www.plantuml.com/plantuml

# 3) Com Java + plantuml.jar instalados:
java -jar plantuml.jar docs/diagrams/*.puml   # gera os .png
```
