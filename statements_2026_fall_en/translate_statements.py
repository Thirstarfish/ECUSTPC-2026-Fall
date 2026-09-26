"""Build the English booklet from the Chinese TeX sources.

The source's TeX structure, formulae, figures, examples, and flavor text remain
in place. Translations below are keyed by source line number for review.
"""
from pathlib import Path
import re
import shutil

DST = Path(__file__).parent
SRC = DST.parent / "statements_2026_fall"
TITLES = [
    "Synecdoche, New York", "AIMC Lab", "Split in Two 2", "Unit Distance!?",
    "Eradication", "Convex Hull", "Beyond the Stars", "Ordinary Offensive Magic",
    "Shooting Stars", "Time Trouble 2", "Aeonglass", "In Filtration 3", "Witch Trial",
]

def entries(raw):
    return {int(a): b for a, b in (line.split("|", 1) for line in raw.strip().splitlines())}

TR = {
"A": entries(r"""
22|Caden has come to understand ``fate'' in his own way. Leaving theater behind, he wants to use a string to represent his life through synecdoche. But in seeking a pure, original expression true to himself, he again falls into an infinite loop of self-reference. This time, Caden leaves the choice to you: portray his life for him.
24|Caden's synecdoche is a string $T$ of length $L$. We call $T$ \BF{valid} if and only if all three conditions below hold:
27|\t\item $T$ consists only of lowercase English letters.
28|\t\item For every $i = 1, 2, \ldots, K$, the character of $T$ at position $p_i$ must equal the given character $c_i$, where $1 \le p_i \le L$ and $c_i$ is a lowercase letter.
29|\t\item For every $i = 1, 2, \ldots, N$, the given string $s_i$ must \BF{not} occur as a contiguous substring of $T$.
32|Given $L$, $K$ position constraints $(p_i,c_i)$, and $N$ forbidden substrings $s_i$, count all valid strings $T$. Output the answer modulo $998\,244\,353$.
36|The first line contains three integers $L$, $K$, and $N$ ($1 \le L \le 10^{18}$, $0 \le K \le 500$, $0 \le N \le 50$): the string length, the number of position constraints, and the number of forbidden substrings.
38|Each of the next $K$ lines contains an integer $p_i$ and a lowercase letter $c_i$ ($1 \le p_i \le L$), describing a position constraint. All $p_i$ are distinct.
40|Each of the next $N$ lines contains a nonempty string $s_i$, describing a forbidden substring.
42|The sum of the lengths of all $s_i$ does not exceed $100$; that is, $\sum_{i=1}^N \vert{}s_i\vert{} \le 100$.
46|Output one integer for each test case: the number of valid strings modulo $998\,244\,353$.
87|The string has length $3$, its second character must be \TT{b}, and it must not contain \TT{ab}. Valid strings have the form \TT{?b?}; the first character can be any lowercase letter except \TT{a}. There are $25 \times 26 = 650$ such strings.
"""),
"B": entries(r"""
18|Your AIMC Lab has developed an entirely new three-dimensional quantum matrix!
20|Consider a three-dimensional grid of size $N \times M \times H$, with coordinates $1 \le x \le N$, $1 \le y \le M$, and $1 \le z \le H$. A qubit occupies every integer grid point. Let $(x_u,y_u,z_u)$ be the coordinates of qubit $u$.
22|For any two qubits $u$ and $v$, define their \BF{squared spatial distance} as
27|The system has eight energy matrices $A_{D,c}$, each of size $N \times M \times H$, indexed by integers $0 \le D \le 3$ and $0 \le c \le 1$.
29|You may initialize each qubit to either basis state $|0\rangle$ or $|1\rangle$. Its corresponding basis-state value is $c_P=0$ or $c_P=1$.
31|After initialization, the total energy is calculated as follows:
34|\t\item For each integer pair $0 \le D \le 3$, $0 \le c \le 1$, qubit $P$ gains $A_{D,c}(x_P,y_P,z_P)$ energy if and only if:
36|\t\t\item The basis-state value of $P$ satisfies $c_P=c$.
37|\t\t\item For every qubit $Q$ with $d^2(P,Q)=D$, the value $c_P+c_Q+D$ is even.
41|As the chief scientist of AIMC Lab, initialize every qubit to a basis state so that the system's total energy is maximized. Output this maximum energy.
45|The first line contains three integers $N$, $M$, and $H$ ($1 \le N,M,H \le 20$), the grid dimensions.
47|The data for the eight energy matrices $A_{D,c}$ follow in lexicographic order of $(D,c)$.
49|Each matrix consists of $N$ blocks. Each block has $M$ lines, each containing $H$ integers. Blocks are separated by blank lines.
51|The $z$-th number in the $y$-th line of the $x$-th block is the energy at $(x,y,z)$. All energy values satisfy $A_{D,c}(x_P,y_P,z_P) \in [0,10^9]$.
55|Output one integer: the maximum total energy the system can achieve.
90|The grid contains only one qubit, at $(1,1,1)$.
92|Initializing it to $|1\rangle$ gives $A_{0,1}(1,1,1)+A_{1,1}(1,1,1)+A_{2,1}(1,1,1)+A_{3,1}(1,1,1)=5+200+2000+60=2265$, which is optimal.
159|Initialize all $8$ qubits of the $2 \times 2 \times 2$ grid to $|0\rangle$.
162|\t\item Each qubit gains $1$ energy from $A_{0,0}$, for a total of $8$.
163|\t\item All qubits at squared distance $2$ are also in state $|0\rangle$, so each qubit gains $10$ from $A_{2,0}$, for a total of $80$.
166|The total energy is $8+80=88$. This can be shown to be the maximum.
"""),
"C": entries(r"""
18|You are given two arrays $a$ and $b$, each of length $n$.
20|Choose any $k \in [0,n]$ and concatenate the first $k$ elements of $a$ with the last $n-k$ elements of $b$ to form a new array of length $n$.
22|Select exactly $m$ pairwise nonadjacent elements from the new array. Let $p$ be the smallest selected element. Maximize $p$.
26|The first line contains an integer $T$ ($1 \le T \le 10^4$), the number of test cases.
28|The first line of each test case contains two integers $n$ and $m$ ($1 \le m \le \lceil \frac n2 \rceil \le n \le 2 \times 10^5$): the array length and the number of elements to select.
30|The second line contains $n$ integers $a_1,a_2,\ldots,a_n$ ($-10^9 \le a_i \le 10^9$).
32|The third line contains $n$ integers $b_1,b_2,\ldots,b_n$ ($-10^9 \le b_i \le 10^9$).
34|The sum of $n$ over all test cases does not exceed $2 \times 10^5$.
38|For each test case, output one integer: the maximum possible value of $p$.
64|In the first test case, choosing $k=4$ gives $[5,4,3,2,5]$. Select the first and fifth elements; their minimum is $5$, which is optimal.
"""),
"D": entries(r"""
26|Construct a set of $n$ points in the Euclidean plane such that:
29|\t\item The absolute values of both coordinates of every point are at most $10^5$;
30|\t\item Each coordinate of every point becomes an integer when multiplied by $10^{13}$;
31|\t\item The distance between any two points is at least $10^{-4}$;
32|\t\item At least $\left\lfloor \frac12 n\log_2 n-n \right\rfloor$ distinct unordered pairs of points have distance in $[1-10^{-9},1+10^{-9}]$.
38|The first line contains an integer $T$ ($1 \le T \le 100$), the number of test cases.
40|Each test case contains one integer $n$ ($2 \le n \le 5000$), the number of points to construct.
42|The sum of $n$ over all test cases does not exceed $5000$.
46|For each test case, output $n$ lines. The $i$-th line contains two integers $X_i,Y_i$ ($-10^{18} \le X_i,Y_i \le 10^{18}$), representing the actual point $\left(\frac{X_i}{10^{13}},\frac{Y_i}{10^{13}}\right)$.
48|Your output is accepted if and only if it satisfies this format and all four conditions above.
50|If several valid constructions exist, output any one of them.
73|In the first test case, the actual coordinates are:
79|The first three conditions are easy to verify. The pairwise distances are:
83|Thus every pairwise distance is within the allowed tolerance of $1$, satisfying the fourth condition.
"""),
"E": entries(r"""
18|We often simplify the square root of a large integer into radical form; for example, $\sqrt{432}=12\sqrt3$.
20|Define the number-theoretic function $f(n)=\min\{a \mid n=ab^2,\ a,b\in\mathbb N\}$ on $\N$. In other words, $f(n)$ is the smallest natural number $a$ satisfying the equation. Such an $a$ always exists.
22|You are given $q$ queries, each specifying $l$ and $r$. For each query, find $\sum_{i=l}^r f(i)$ modulo $998\,244\,353$.
26|The first line contains an integer $q$ ($1 \le q \le 500$), the number of queries.
28|Each of the next $q$ lines contains two integers $l$ and $r$ ($1 \le l \le r \le 10^{12}$), specifying a query interval.
32|For each query, output one integer: $\sum_{i=l}^r f(i)$ modulo $998\,244\,353$.
59|For the second and third queries, the corresponding values of $f$ are:
71|In particular, the third query gives
"""),
"F": entries(r"""
18|You are given $n$ quadratic inequalities, each of the form
24|Determine whether there is an \BF{integer} $x$ satisfying all $n$ inequalities simultaneously.
28|The first line contains an integer $T$ ($1 \le T \le 10^4$), the number of test cases.
30|The first line of each test case contains an integer $n$ ($1 \le n \le 2 \times 10^5$), the number of inequalities.
32|Each of the next $n$ lines contains three integers $a_i,b_i,c_i$ ($-10^5 \le a_i,b_i,c_i \le 10^5$, $a_i\ne0$), the coefficients of the $i$-th inequality.
34|The sum of $n$ over all test cases does not exceed $2 \times 10^5$.
38|For each test case, output \TT{YES} if such an integer $x$ exists, or \TT{NO} otherwise.
40|The answer is case-insensitive; for example, \TT{yes}, \TT{YES}, \TT{Yes}, and \TT{YeS} are all accepted as affirmative answers.
81|In the second test case, $x=100$ satisfies all three inequalities, so an integer solution exists.
"""),
"G": entries(r"""
16|\BF{This is a communication problem. Pay attention to its unusual constraints.}
20|That was long ago, in a past forgotten by all of ECUST\ldots
22|In that forgotten age, Zhulin and Qianwai fought side by side, lived together, and gazed up at the same stars. Later, sand buried their language, oceans severed their roads, and even their names faded from history. Only the vast, silent star chart above them remained: public, radiant, and unchanging. Yet nobody knew which stars they had once quietly joined into a message.
24|This problem has $20$ test cases.
26|You are given an undirected graph $G=(V,E)$ with $|V|=2000$ and $|E|=43\,600$. It has no multiple edges or self-loops and is generated as follows. Initially, $G$ has $|V|$ vertices and no edges. Then, $|E|$ times, the generator uniformly chooses one of the vertex pairs not yet joined by an edge and adds an undirected edge between them.
28|Your program will be run twice:
30|\BF{First run}
32|In the first run, you play Zhulin. Choose and output $n$ induced four-cycles in $G$. They may share vertices.
35|\t\item \BF{Definition of an induced four-cycle:} Choose $4$ vertices of $G$ that have \BF{exactly} $4$ edges among them, forming a simple cycle. In particular, there are no diagonals.
38|\BF{Second run}
40|In the second run, you play Qianwai. The judge has your complete output from the first run. For each of $q$ queries, it selects one of your four-cycles, deletes one of its vertices, and gives you the other three vertex numbers. Report the index of the four-cycle in Zhulin's output from which they came.
44|\BF{First run}
46|The first line contains the string \TT{send}.
48|The second line contains an integer $n$ ($n=200\,000$), the number of induced four-cycles you must output.
50|The third line contains two integers $|V|$ and $|E|$.
52|Each of the next $|E|$ lines contains two integers $u,v$, indicating an undirected edge between vertices $u$ and $v$ ($1 \le u,v \le |V|$).
54|\BF{Second run}
56|The first line contains the string \TT{receive}.
58|The second line contains two integers $|V|$ and $|E|$.
60|Each of the next $|E|$ lines contains two integers $u,v$, indicating an undirected edge between vertices $u$ and $v$ ($1 \le u,v \le |V|$).
62|Line $|E|+3$ contains an integer $q$ ($q=4n$), the number of received vertex sets.
64|Each of the next $q$ lines contains $3$ integers: the vertices remaining after the judge removes one vertex from a four-cycle in your first-run output.
68|\BF{First run}
70|Output $n$ lines. Line $i$ contains four integers $v_1,v_2,v_3,v_4$, the vertices of the $i$-th induced four-cycle.
72|\BF{Second run}
74|Output $q$ lines. For each received vertex set, output an integer $i$ ($1 \le i \le n$), the index of its four-cycle.
78|\BF{First run}
102|\BF{Second run}
133|Parts of the sample are omitted because the complete data is too large.
"""),
"H": entries(r"""
18|Consider an $n \times m$ grid. Each cell has one of three states:
21|\t\item \TT{/} (forward-slash mirror): reflects on both sides. A ray turns $90^\circ$, connecting the top with the left and the bottom with the right.
22|\t\item \TT{\textbackslash} (backslash mirror): reflects on both sides. A ray turns $90^\circ$, connecting the top with the right and the bottom with the left.
23|\t\item \TT{.} (empty cell): contains no obstacle. A ray passes straight through, connecting the top with the bottom and the left with the right.
26|Initially, some cells contain fixed \TT{/} or \TT{\textbackslash} mirrors; the rest are empty. You may \BF{leave each initially empty cell empty}, \BF{replace it with a \TT{/} mirror}, or \BF{replace it with a \TT{\textbackslash} mirror}.
28|The outside boundary of the grid consists of $2(n+m)$ unit-length segments. A ray may enter or leave only perpendicularly through the midpoint of one of these segments.
30|Given an entry point and a target exit point, determine whether the grid can be filled legally so that a ray entering at the specified point, after straight-line travel and reflections, exits at exactly the target point.
32|If possible, output any valid completed grid. Otherwise, report that it is impossible.
36|The first line contains an integer $T$ ($1 \le T \le 100$), the number of test cases.
38|The first line of each test case contains two integers $n,m$ ($1 \le n \times m \le 2 \times 10^5$), the numbers of rows and columns.
40|Each of the next $n$ lines contains a string of length $m$ describing the initial grid. Here \TT{/} is a forward-slash mirror, \TT{\textbackslash} is a backslash mirror, and \TT{.} is an empty cell.
42|The next line contains four space-separated values $d_1,p_1,d_2,p_2$. The first pair specifies the entry point, and the second specifies the target exit point:
45|\t\item Each of $d_1,d_2$ is one of U, D, L, R, denoting the top, bottom, left, or right boundary, respectively.
46|\t\item Each of $p_1,p_2$ is a positive integer specifying the midpoint of the outer edge of the $p$-th boundary cell. On U or D, $p$ is a column number ($1 \le p \le m$); on L or R, it is a row number ($1 \le p \le n$).
49|It is guaranteed that $(d_1,p_1) \ne (d_2,p_2)$.
51|The sum of $n \times m$ over all test cases does not exceed $2 \times 10^5$.
55|For each test case, if a valid completion exists, output $n$ lines of length $m$ describing it. Otherwise, output \TT{IMPOSSIBLE}.
57|A backslash must usually be escaped: \TT{\textbackslash\textbackslash} outputs \TT{\textbackslash}.
99|In the first test case, the ray enters from the left edge of row $1$ and travels right. It passes through the empty cell $(1,1)$, turns upward at the \TT{/} mirror in $(1,2)$, and exits through the top edge of column $2$.
"""),
"I": entries(r"""
20|There are $n$ crystals numbered $1$ through $n$ from left to right. Crystal $i$ initially contains $a_i$ units of energy. You may collect all $n$ crystals in any order.
22|When you collect crystal $i$:
25|\t\item You add its current energy to your total score.
26|\t\item If crystals $i-1$ and $i+1$ exist and have not yet been collected, their energies each change sign.
29|The crystals' numbers do not change as you collect them.
31|What is the maximum total score after collecting all $n$ crystals?
35|The first line contains an integer $T$ ($1 \le T \le 10^4$), the number of test cases.
37|The first line of each test case contains an integer $n$ ($1 \le n \le 2 \times 10^5$), the number of crystals.
39|The second line contains $n$ integers $a_1,a_2,\ldots,a_n$ ($-10^9 \le a_i \le 10^9$), their initial energies.
41|The sum of $n$ over all test cases does not exceed $2 \times 10^5$.
45|For each test case, output one integer: the maximum possible total score.
69|In the first test case, collecting crystals in the order $3 \to 5 \to 1 \to 2 \to 4$ gives a score of $15$. This is optimal.
"""),
"J": entries(r"""
18|To prove that he is never late for class, Xingyu insists that he entered the classroom at $7{:}110$ for a class beginning at $8{:}00$. This inspires a ``generalized time notation.''
20|In generalized time notation, a time is written $A{:}B$, where $A,B$ are nonnegative integers.
22|We define two \BF{equivalence rules} for generalized times:
25|\t\item For any $B \ge 60$, $A{:}B$ and $(A+1){:}(B-60)$ represent the same instant.
26|\t\item For any $A \ge 24$, $A{:}B$ and $(A-24){:}B$ represent the same instant.
29|A time $h{:}m$ is a ``standard time'' if and only if $0 \le h < 24$ and $0 \le m < 60$.
31|Given a generalized time, find the unique equivalent standard time.
35|The first line contains an integer $T$ ($1 \le T \le 10^5$), the number of test cases.
37|Each test case contains two integers $A,B$ ($0 \le A,B \le 10^9$), representing the generalized time $A{:}B$.
41|For each test case, output two integers $h,m$ representing the equivalent standard time $h{:}m$.
70|In the first test case, $7{:}110 \to 8{:}50$ by rule 1, since $110=60+50$.
72|In the second test case, $25{:}30 \to 1{:}30$ by rule 2, since $25=24+1$.
74|In the third test case, $0{:}0$ is already a standard time.
"""),
"K": entries(r"""
18|In the Euclidean plane, you are given a polygon $P=P_0P_1\cdots P_{n-1}$ whose vertices are listed in counterclockwise order.
20|An angle $\alpha$ ($0 \le \alpha < \pi$) is a \BF{stable direction} if and only if there exists a $c \in \R$ such that, for every point $X$ in polygon $P$ (including its boundary), the perpendicular segment from $X$ to the line
24|lies entirely in $P$ (including its boundary). Here $\alpha$ is a direction of the line's normal vector.
27|Find the number of \BF{stable directions}. This number can be shown to be finite.
31|The first line contains an integer $T$ ($1 \le T \le 100$), the number of test cases.
33|The first line of each test case contains an integer $n$ ($3 \le n \le 10^5$), the number of polygon vertices.
35|Each of the next $n$ lines contains two integers $x_i,y_i$ ($-10^9 \le x_i,y_i \le 10^9$), the coordinates of the $i$-th vertex, listed in counterclockwise order.
37|Within a test case, all vertices are distinct. Adjacent edges intersect only at their shared endpoint, and nonadjacent edges do not intersect.
39|The sum of $n$ over all test cases does not exceed $10^5$.
43|For each test case, output one integer: the number of \BF{stable directions}.
146|\t\t{\scriptsize Illustration for test case 1}
215|\t\t{\scriptsize Illustration for test case 2}
271|\t\t{\scriptsize Illustration for test case 3}
278|The lines and \BF{stable directions} for test case 1 are
293|The lines and \BF{stable directions} for test case 2 are
308|The line and \BF{stable direction} for test case 3 are
316|The corresponding line is not uniquely determined.
"""),
"L": entries(r"""
18|Maddy is still purging Baddy's evil army\ldots for the third time!
20|The game takes place on an infinite Cartesian plane, which you may imagine as an infinite chessboard. Baddy controls $n$ stationary \BF{cannons}, placed at $(x_1,y_1),(x_2,y_2),\ldots,(x_n,y_n)$.
22|Under Chinese chess rules, a cannon attacks by jumping over a screen. If cannons $A$ and $B$ share a row or column and \BF{exactly one} other cannon $C$ lies between them, $A$ attacks $B$ (and $B$ attacks $A$). Cannon $C$ is their ``screen.''
24|At each step, Maddy may destroy a cannon \BF{that is not currently attacked by any other cannon}. The destroyed cannon is immediately removed; it can no longer attack or serve as a screen.
26|Maddy wants to destroy all $n$ cannons. Does there exist an order of destruction that clears the board?
30|The first line contains an integer $T$ ($1 \le T \le 10^5$), the number of test cases.
32|The first line of each test case contains an integer $n$ ($1 \le n \le 2 \times 10^5$), the number of cannons.
34|Each of the next $n$ lines contains two integers $x_i,y_i$ ($-10^9 \le x_i,y_i \le 10^9$), the position of cannon $i$.
36|The sum of $n$ over all test cases does not exceed $2 \times 10^5$. All cannon positions within a test case are distinct.
40|For each test case, output \TT{YES} if some valid destruction order clears the board, or \TT{NO} otherwise.
42|The answer is case-insensitive; for example, \TT{yes}, \TT{YES}, \TT{Yes}, and \TT{YeS} are all accepted as affirmative answers.
79|In the first test case, the three cannons share a column. Initially, $(0,0)$ and $(0,2)$ attack each other over the screen at $(0,1)$, while $(0,1)$ is unattacked. Destroy $(0,1)$ first. Then the other two are unattacked and may be destroyed in either order.
81|In the third test case, four cannons share a row. Initially, $(0,0)$ and $(2,0)$ attack each other over $(1,0)$; $(1,0)$ and $(3,0)$ attack each other over $(2,0)$. Thus every cannon is attacked, none can be destroyed, and the answer is \TT{NO}.
"""),
"M": entries(r"""
18|Your trident has struck a villager again. This time, when the trial ends, you discover that you can no longer find it.
20|You know that the trident landed on a rooted tree $T$ with $n$ vertices, rooted at vertex $1$. Each vertex has a weight $a_i$, and at every moment these weights form a permutation of $1 \sim n$. For vertices $u,v$, let $P(u,v)$ be their simple path, including both $u$ and $v$.
22|A set $S$ of tree vertices is a \textbf{trident} if and only if there exist three vertices $F,x,y$ (which may be equal) satisfying:
25|\t\item $F$ is an ancestor of $x$, or $F=x$;
26|\t\item $F$ is an ancestor of $y$, or $F=y$;
32|Process $q$ operations in order. There are three types:
34|\textbf{Operation 1: Lightning.} Given vertices $x$ and $y$, swap their current weights.
36|\textbf{Operation 2: Judgment.} Given two distinct weights $l<r$, reverse the weights within $[l,r]$.
38|Formally, if vertex $x$ has weight $a_x$ before this operation and $a'_x$ afterward, define
47|Then set $a_x\leftarrow a'_x$ simultaneously for every integer $1 \le x \le n$. The weights remain a permutation of $1 \sim n$.
49|\textbf{Operation 3: Find the trident.} Given $l \le r$, determine whether $S=\{x\in\N,\ 1\le x\le n\mid l\le a_x\le r\}$ is a \textbf{trident}.
51|Answer all questions from \textbf{Operation 3} in order.
55|The first line contains an integer $T$ ($1 \le T \le 10^3$), the number of test cases.
57|The first line of each test case contains two integers $n,q$ ($1 \le n,q \le 10^5$), the numbers of tree vertices and operations.
59|Each of the next $n-1$ lines contains two integers $u,v$ ($1 \le u,v \le n$, $u\ne v$), describing a tree edge.
61|The next line contains $n$ integers $a_1,a_2,\ldots,a_n$ ($1 \le a_i \le n$), the initial vertex weights.
63|Each of the next $q$ lines describes an operation. It starts with an integer $\text{op}_i$ ($1 \le \text{op}_i \le 3$), followed by:
66|\t\item If $\text{op}_i=1$, two integers $x,y$ ($1 \le x,y \le n$): perform \textbf{Operation 1}, swapping their weights.
67|\t\item If $\text{op}_i=2$, two integers $l,r$ ($1 \le l<r \le n$): perform \textbf{Operation 2}, reversing the weights in $[l,r]$.
68|\t\item If $\text{op}_i=3$, two integers $l,r$ ($1 \le l \le r \le n$): perform \textbf{Operation 3}, asking whether the vertices with weights in $[l,r]$ form a \textbf{trident}.
71|Within each test case, the initial weights form a permutation of $1\sim n$, and the given edges form a tree with $n$ vertices.
73|Across all test cases, $\sum n \le 3 \times 10^5$ and $\sum q \le 3 \times 10^5$.
77|For each \textbf{Operation 3}, output one line:
80|\t\item Output \TT{YES} if $S=\{x\in\N,\ 1\le x\le n\mid l\le a_x\le r\}$ is a \textbf{trident};
81|\t\item Otherwise, output \TT{NO}.
84|The answer is case-insensitive; for example, \TT{yes}, \TT{YES}, \TT{Yes}, and \TT{YeS} are all accepted as affirmative answers.
167|\caption{Initial state}
211|\caption{After Judgment $(1,7)$}
255|\caption{After Lightning $(4,5)$}
260|For the third trident query in the first test case, after the preceding Judgment operation,
261|the current weights are
266|Weights $4,5,6,7$ are at vertices $1,5,2,4$, respectively, so $S=\{1,2,4,5\}$.
268|Choose $F=1$, $x=4$, and $y=5$. Then:
271|\item Vertex $1$ is an ancestor of vertex $4$.
272|\item Vertex $1$ is an ancestor of vertex $5$.
276|Therefore, $S$ is a \textbf{trident}.
"""),
}

HEADINGS = {"题目描述": "Problem Statement", "输入格式": "Input", "输出格式": "Output"}
HAN = re.compile(r"[\u3400-\u9fff]")
files = sorted(SRC.glob("Problem_*.tex"))
assert len(files) == 13
DST.mkdir(exist_ok=True)
shutil.copy2(SRC / "ecustpc.sty", DST / "ecustpc.sty")
style_path = DST / "ecustpc.sty"
style = style_path.read_text(encoding="utf-8")
style = style.replace(r"\renewcommand{\thesection}{试题 \hspace{-0.5em}}", r"\renewcommand{\thesection}{Problem \hspace{-0.5em}}")
style = style.replace("此页为保证阅读顺畅，刻意留白", "This page is intentionally blank")
style = style.replace(r"第 \thepage 页 \quad 共 \pageref{LastPage} 页", r"Page \thepage\ of \pageref{LastPage}")
style_path.write_text(style, encoding="utf-8")

for file in files:
    letter = file.name[8]
    lines = file.read_text(encoding="utf-8").splitlines()
    out = []
    in_quote = False
    used = set()
    for number, line in enumerate(lines, 1):
        if line == r"\begin{quote}":
            in_quote = True
        if number in TR[letter]:
            line = TR[letter][number]
            indent = ""
            while line.startswith(r"\t") and (line[2:].startswith(r"\t") or line[2:].startswith(r"\item")):
                indent += "\t"
                line = line[2:]
            line = indent + line
            used.add(number)
        elif line.startswith(r"\section["):
            title = TITLES[ord(letter)-65]
            line = re.sub(r"^\\section\[[^]]*\]", lambda _: rf"\section[{letter}. {title}]", line)
            line = re.sub(r"(\\BF\{\\TT\{"+letter+r"\}\}\})\s+.*$", lambda m: m.group(1)+" "+title+"}", line)
        elif line.startswith(r"\subsection{"):
            match = re.fullmatch(r"\\subsection\{(.+)\}", line)
            label = match.group(1)
            if label in HEADINGS:
                line = r"\subsection{" + HEADINGS[label] + "}"
            elif (m := re.fullmatch(r"样例 (\d+)( 解释)?", label)):
                line = r"\subsection{" + ("Sample " + m.group(1) + (" Explanation" if m.group(2) else "")) + "}"
        elif line.startswith(r"\textit{运行时间限制}"):
            line = line.replace("运行时间限制", "Time limit")
        elif line.startswith(r"\textit{内存使用限制}"):
            line = line.replace("内存使用限制", "Memory limit")
        elif line.startswith(r"\textit{读入文件}"):
            line = r"\textit{Input} \qquad standard input"
        elif line.startswith(r"\textit{输出文件}"):
            line = r"\textit{Output} \qquad standard output"
        if HAN.search(line) and not in_quote and not line.lstrip().startswith("%"):
            raise ValueError(f"Untranslated Chinese in {file.name}:{number}: {line}")
        out.append(line)
        if line == r"\end{quote}":
            in_quote = False
    assert used == TR[letter].keys(), (letter, TR[letter].keys()-used)
    (DST / file.name).write_text("\n".join(out)+"\n", encoding="utf-8")

print(f"Wrote {len(files)} English problem statements to {DST}")
