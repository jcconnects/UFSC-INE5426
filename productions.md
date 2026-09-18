# Demonstração LL(1) das produções com alternativa vazia

Para uma produção da forma:

\[
A \rightarrow \alpha_1 \mid \alpha_2 \mid \lambda
\]

devem ser verificadas as seguintes condições:

\[
FIRST(\alpha_1)\cap FIRST(\alpha_2)=\varnothing
\]

\[
FIRST(\alpha_1)\cap FOLLOW(A)=\varnothing
\]

\[
FIRST(\alpha_2)\cap FOLLOW(A)=\varnothing
\]

Quando existe somente uma alternativa não vazia:

\[
A\rightarrow\alpha\mid\lambda
\]

basta verificar:

\[
FIRST(\alpha)\cap FOLLOW(A)=\varnothing
\]

Para simplificar a escrita, considere:

\[
T=\{\texttt{int},\texttt{float},\texttt{string}\}
\]

\[
R=\{\texttt{<},\texttt{>},\texttt{<=},
\texttt{>=},\texttt{==},\texttt{!=}\}
\]

\[
S=
\{
\texttt{int},\texttt{float},\texttt{string},
\texttt{ident},\texttt{print},\texttt{read},
\texttt{return},\texttt{if},\texttt{for},
\texttt{\{},\texttt{break},\texttt{;}
\}
\]

em que:

\[
FIRST(STATEMENT)=S
\]

---

## 1. `PROGRAM`

A produção é:

\[
PROGRAM\rightarrow
STATEMENT\mid FUNCLIST\mid\lambda
\]

Os conjuntos envolvidos são:

\[
FIRST(STATEMENT)=S
\]

\[
FIRST(FUNCLIST)=\{\texttt{def}\}
\]

Como `PROGRAM` é o símbolo inicial:

\[
FOLLOW(PROGRAM)=\{\texttt{\$}\}
\]

Primeiro, verifica-se a interseção entre as duas alternativas não vazias:

\[
FIRST(STATEMENT)\cap FIRST(FUNCLIST)
=
S\cap\{\texttt{def}\}
=
\varnothing
\]

Como existe a alternativa \(\lambda\), também são necessárias as interseções com `FOLLOW(PROGRAM)`:

\[
FIRST(STATEMENT)\cap FOLLOW(PROGRAM)
=
S\cap\{\texttt{\$}\}
=
\varnothing
\]

\[
FIRST(FUNCLIST)\cap FOLLOW(PROGRAM)
=
\{\texttt{def}\}\cap\{\texttt{\$}\}
=
\varnothing
\]

Portanto, `PROGRAM` não possui conflito LL(1).

---

## 2. `FUNCLIST_TAIL`

A produção obtida pela fatoração direta é:

\[
FUNCLIST_TAIL\rightarrow
FUNCLIST\mid\lambda
\]

Temos:

\[
FIRST(FUNCLIST)=\{\texttt{def}\}
\]

Como `FUNCLIST_TAIL` aparece no final de `FUNCLIST`:

\[
FOLLOW(FUNCLIST_TAIL)=FOLLOW(FUNCLIST)
\]

Como `FUNCLIST` aparece no final de `PROGRAM`:

\[
FOLLOW(FUNCLIST)=FOLLOW(PROGRAM)=\{\texttt{\$}\}
\]

Logo:

\[
FOLLOW(FUNCLIST_TAIL)=\{\texttt{\$}\}
\]

A interseção é:

\[
FIRST(FUNCLIST)\cap FOLLOW(FUNCLIST_TAIL)
=
\{\texttt{def}\}\cap\{\texttt{\$}\}
=
\varnothing
\]

Portanto, `FUNCLIST_TAIL` não possui conflito LL(1).

---

## 3. `PARAMLIST`

A produção é:

\[
PARAMLIST\rightarrow
TYPE\ ident\ PARAMLIST_TAIL
\mid\lambda
\]

Temos:

\[
FIRST(TYPE\ ident\ PARAMLIST_TAIL)
=
FIRST(TYPE)
=
T
\]

Como `PARAMLIST` aparece em:

\[
FUNCDEF\rightarrow
\texttt{def}\ ident\ \texttt{(}\ PARAMLIST\ \texttt{)}
\ \texttt{\{}\ STATELIST\ \texttt{\}},
\]

temos:

\[
FOLLOW(PARAMLIST)=\{\texttt{)}\}
\]

Logo:

\[
FIRST(TYPE\ ident\ PARAMLIST_TAIL)
\cap FOLLOW(PARAMLIST)
\]

# \[

# T\cap\{\texttt{)}\}

\varnothing
\]

Portanto, `PARAMLIST` não possui conflito LL(1).

---

## 4. `PARAMLIST_TAIL`

A produção é:

\[
PARAMLIST_TAIL\rightarrow
\texttt{,}\ PARAMLIST
\mid\lambda
\]

Temos:

\[
FIRST(\texttt{,}\ PARAMLIST)=\{\texttt{,}\}
\]

Como `PARAMLIST_TAIL` aparece no final da alternativa não vazia de `PARAMLIST`:

\[
FOLLOW(PARAMLIST_TAIL)=FOLLOW(PARAMLIST)
\]

Assim:

\[
FOLLOW(PARAMLIST_TAIL)=\{\texttt{)}\}
\]

A interseção é:

\[
FIRST(\texttt{,}\ PARAMLIST)
\cap FOLLOW(PARAMLIST_TAIL)
\]

# \[

# \{\texttt{,}\}\cap\{\texttt{)}\}

\varnothing
\]

Portanto, `PARAMLIST_TAIL` não possui conflito LL(1).

---

## 5. `VARDECL_DIM`

A produção é:

\[
VARDECL_DIM\rightarrow
\texttt{[}\ int\_constant\ \texttt{]}\ VARDECL_DIM
\mid\lambda
\]

Temos:

\[
FIRST(
\texttt{[}\ int\_constant\ \texttt{]}\ VARDECL_DIM
)
=
\{\texttt{[}\}
\]

Como uma declaração de variável aparece em:

\[
STATEMENT\rightarrow VARDECL\ \texttt{;},
\]

temos:

\[
FOLLOW(VARDECL_DIM)=FOLLOW(VARDECL)=\{\texttt{;}\}
\]

Logo:

\[
\{\texttt{[}\}\cap\{\texttt{;}\}
=
\varnothing
\]

Portanto, `VARDECL_DIM` não possui conflito LL(1).

---

## 6. `PARAMLISTCALL`

A produção é:

\[
PARAMLISTCALL\rightarrow
ident\ PARAMLISTCALL_TAIL
\mid\lambda
\]

Temos:

\[
FIRST(ident\ PARAMLISTCALL_TAIL)
=
\{\texttt{ident}\}
\]

Como `PARAMLISTCALL` aparece entre parênteses em:

\[
FUNCCALL\rightarrow
\texttt{call}\ ident\ \texttt{(}\ PARAMLISTCALL\ \texttt{)},
\]

temos:

\[
FOLLOW(PARAMLISTCALL)=\{\texttt{)}\}
\]

Logo:

\[
\{\texttt{ident}\}\cap\{\texttt{)}\}
=
\varnothing
\]

Portanto, `PARAMLISTCALL` não possui conflito LL(1).

---

## 7. `PARAMLISTCALL_TAIL`

A produção é:

\[
PARAMLISTCALL_TAIL\rightarrow
\texttt{,}\ PARAMLISTCALL
\mid\lambda
\]

Temos:

\[
FIRST(\texttt{,}\ PARAMLISTCALL)
=
\{\texttt{,}\}
\]

Como `PARAMLISTCALL_TAIL` aparece no final da alternativa não vazia de `PARAMLISTCALL`:

\[
FOLLOW(PARAMLISTCALL_TAIL)
=
FOLLOW(PARAMLISTCALL)
=
\{\texttt{)}\}
\]

Logo:

\[
\{\texttt{,}\}\cap\{\texttt{)}\}
=
\varnothing
\]

Portanto, `PARAMLISTCALL_TAIL` não possui conflito LL(1).

---

## 8. `ELSE_OPT`

Após a inclusão do terminal `endif`, as produções são:

\[
IFSTAT\rightarrow
\texttt{if}\ \texttt{(}\ EXPRESSION\ \texttt{)}
\ STATEMENT\ ELSE_OPT\ \texttt{endif}
\]

\[
ELSE_OPT\rightarrow
\texttt{else}\ STATEMENT
\mid\lambda
\]

Temos:

\[
FIRST(\texttt{else}\ STATEMENT)
=
\{\texttt{else}\}
\]

Como `endif` aparece imediatamente depois de `ELSE_OPT`:

\[
FOLLOW(ELSE_OPT)=\{\texttt{endif}\}
\]

Logo:

\[
FIRST(\texttt{else}\ STATEMENT)
\cap FOLLOW(ELSE_OPT)
\]

# \[

# \{\texttt{else}\}\cap\{\texttt{endif}\}

\varnothing
\]

Portanto, `ELSE_OPT` não possui conflito LL(1).

---

## 9. `STATELIST_TAIL`

A produção obtida pela fatoração direta é:

\[
STATELIST_TAIL\rightarrow
STATELIST\mid\lambda
\]

Temos:

\[
FIRST(STATELIST)=FIRST(STATEMENT)=S
\]

Como `STATELIST_TAIL` aparece no final de `STATELIST`:

\[
FOLLOW(STATELIST_TAIL)=FOLLOW(STATELIST)
\]

`STATELIST` aparece antes do terminal `}` nas produções de `FUNCDEF` e de `STATEMENT`. Portanto:

\[
FOLLOW(STATELIST)=\{\texttt{\}}\}
\]

Consequentemente:

\[
FOLLOW(STATELIST_TAIL)=\{\texttt{\}}\}
\]

A interseção é:

\[
FIRST(STATELIST)\cap FOLLOW(STATELIST_TAIL)
\]

# \[

# S\cap\{\texttt{\}}\}

\varnothing
\]

Portanto, `STATELIST_TAIL` não possui conflito LL(1).

---

## 10. `ALLOC_DIM_TAIL`

A produção é:

\[
ALLOC_DIM_TAIL\rightarrow
\texttt{[}\ NUMEXPRESSION\ \texttt{]}\ ALLOC_DIM_TAIL
\mid\lambda
\]

Temos:

\[
FIRST(
\texttt{[}\ NUMEXPRESSION\ \texttt{]}\ ALLOC_DIM_TAIL
)
=
\{\texttt{[}\}
\]

Uma expressão de alocação aparece no final de `ATRIBVALUE`, que aparece no final de `ATRIBSTAT`. Um comando de atribuição pode ser seguido por `;` ou por `)` quando aparece no último campo de um `for`. Portanto:

\[
FOLLOW(ALLOC_DIM_TAIL)
=
\{\texttt{;},\texttt{)}\}
\]

Logo:

\[
\{\texttt{[}\}
\cap
\{\texttt{;},\texttt{)}\}
=
\varnothing
\]

Portanto, `ALLOC_DIM_TAIL` não possui conflito LL(1).

---

## 11. `EXPRESSION_REL`

A produção é:

\[
EXPRESSION_REL\rightarrow
RELOP\ NUMEXPRESSION
\mid\lambda
\]

Temos:

\[
FIRST(RELOP\ NUMEXPRESSION)
=
FIRST(RELOP)
=
R
\]

Uma expressão completa pode ser seguida por `;` ou por `)`. Portanto:

\[
FOLLOW(EXPRESSION_REL)
=
FOLLOW(EXPRESSION)
=
\{\texttt{;},\texttt{)}\}
\]

Logo:

\[
R\cap\{\texttt{;},\texttt{)}\}
=
\varnothing
\]

Isto é:

\[
\{
\texttt{<},\texttt{>},\texttt{<=},
\texttt{>=},\texttt{==},\texttt{!=}
\}
\cap
\{\texttt{;},\texttt{)}\}
=
\varnothing
\]

Portanto, `EXPRESSION_REL` não possui conflito LL(1).

---

## 12. `NUMEXPRESSION_TAIL`

A produção é:

\[
NUMEXPRESSION_TAIL\rightarrow
\texttt{+}\ TERM\ NUMEXPRESSION_TAIL
\mid
\texttt{-}\ TERM\ NUMEXPRESSION_TAIL
\mid\lambda
\]

Os conjuntos `FIRST` das alternativas não vazias são:

\[
FIRST(
\texttt{+}\ TERM\ NUMEXPRESSION_TAIL
)
=
\{\texttt{+}\}
\]

\[
FIRST(
\texttt{-}\ TERM\ NUMEXPRESSION_TAIL
)
=
\{\texttt{-}\}
\]

A primeira condição é:

\[
\{\texttt{+}\}\cap\{\texttt{-}\}
=
\varnothing
\]

Como `NUMEXPRESSION_TAIL` aparece no final de `NUMEXPRESSION`:

\[
FOLLOW(NUMEXPRESSION_TAIL)
=
FOLLOW(NUMEXPRESSION)
\]

Uma expressão numérica pode ser seguida por um operador relacional, `;`, `)`, ou `]`. Portanto:

\[
FOLLOW(NUMEXPRESSION_TAIL)
=
R\cup
\{\texttt{;},\texttt{)},\texttt{]}\}
\]

Logo:

\[
\{\texttt{+}\}
\cap
FOLLOW(NUMEXPRESSION_TAIL)
=
\varnothing
\]

\[
\{\texttt{-}\}
\cap
FOLLOW(NUMEXPRESSION_TAIL)
=
\varnothing
\]

Equivalentemente:

\[
\{\texttt{+},\texttt{-}\}
\cap
\left(
R\cup\{\texttt{;},\texttt{)},\texttt{]}\}
\right)
=
\varnothing
\]

Portanto, `NUMEXPRESSION_TAIL` não possui conflito LL(1).

---

## 13. `TERM_TAIL`

A produção é:

\[
TERM_TAIL\rightarrow
\texttt{\*}\ UNARYEXPR\ TERM_TAIL
\mid
\texttt{/}\ UNARYEXPR\ TERM_TAIL
\mid
\texttt{\%}\ UNARYEXPR\ TERM_TAIL
\mid\lambda
\]

Os conjuntos `FIRST` das alternativas não vazias são:

\[
\{\texttt{\*}\},
\qquad
\{\texttt{/}\},
\qquad
\{\texttt{\%}\}
\]

As interseções entre essas alternativas são:

\[
\{\texttt{\*}\}\cap\{\texttt{/}\}
=
\varnothing
\]

\[
\{\texttt{\*}\}\cap\{\texttt{\%}\}
=
\varnothing
\]

\[
\{\texttt{/}\}\cap\{\texttt{\%}\}
=
\varnothing
\]

Como `TERM_TAIL` aparece no final de `TERM`:

\[
FOLLOW(TERM_TAIL)=FOLLOW(TERM)
\]

Um termo pode ser seguido por `+`, `-`, por um operador relacional, por `;`, por `)` ou por `]`. Portanto:

\[
FOLLOW(TERM_TAIL)
=
\{\texttt{+},\texttt{-}\}
\cup R
\cup
\{\texttt{;},\texttt{)},\texttt{]}\}
\]

Logo:

\[
\{\texttt{\*},\texttt{/},\texttt{\%}\}
\cap FOLLOW(TERM_TAIL)
=
\varnothing
\]

Portanto, `TERM_TAIL` não possui conflito LL(1).

---

## 14. `LVALUE_TAIL`

A produção é:

\[
LVALUE_TAIL\rightarrow
\texttt{[}\ NUMEXPRESSION\ \texttt{]}\ LVALUE_TAIL
\mid\lambda
\]

Temos:

\[
FIRST(
\texttt{[}\ NUMEXPRESSION\ \texttt{]}\ LVALUE_TAIL
)
=
\{\texttt{[}\}
\]

`LVALUE` pode ser seguido pelo operador de atribuição, por operadores aritméticos, por operadores relacionais ou por delimitadores. Assim:

\[
\begin{aligned}
FOLLOW(LVALUE_TAIL)=\{&
\texttt{=},
\texttt{+},\texttt{-},
\texttt{\*},\texttt{/},\texttt{\%},\\
&\texttt{<},\texttt{>},\texttt{<=},
\texttt{>=},\texttt{==},\texttt{!=},\\
&\texttt{;},\texttt{)},\texttt{]}
\}
\end{aligned}
\]

Como `[` não pertence a esse conjunto:

\[
\{\texttt{[}\}
\cap FOLLOW(LVALUE_TAIL)
=
\varnothing
\]

Portanto, `LVALUE_TAIL` não possui conflito LL(1).

---

# Conclusão

Para todas as produções com alternativa vazia, o conjunto `FIRST` de cada alternativa não vazia possui interseção vazia com o `FOLLOW` da respectiva variável.

Além disso, nas produções com mais de uma alternativa não vazia, os conjuntos `FIRST` dessas alternativas são dois a dois disjuntos.

Portanto, nenhuma das produções analisadas apresenta conflito LL(1).
