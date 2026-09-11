# Exercício - Analisador sintático LL(1)

Considere a seguinte gramática $G = (V, T, R, S)$ sendo $V = \{S, B, B'\}$ o conjunto das variáveis, $T = \{0M, 1M, 0m, 1m, 0, 1\}$ o conjunto dos terminais, $R$ o conjunto de produções (abaixo), e $S$ a variável inicial.

$$
\begin{aligned}
R = \{\; S  &\rightarrow B, \\
B  &\rightarrow 0MB' \mid 1MB' \mid 0 \mid 1, \\
B' &\rightarrow 0mB \mid 1mB \mid 0 \mid 1 \;\}.
\end{aligned}
$$

A gramática $G$ está em LL(1). Uma palavra derivada pela gramática $G$ é a seguinte $0M1m1M0$. A palavra possui o valor binário $0110$ sendo cada bit seguido por uma letra $M$ ou $m$ de forma alternada (primeiro $M$ e depois $m$), exceto pelo último bit. Desenvolva um analisador sintático LL(1) que tem como objetivo demonstrar que uma dada palavra $w$ está ou não em $L(G)$.
