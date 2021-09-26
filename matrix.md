# Phase 0 (Extending SimpleRA to include Matrices)

### Team 9
### Vasu Singhal (2018101074), Daksh Rawat (2018101087)

## Dense Matrices

### Page Layout

We decompose the original $(N * N)$ into smaller $(M * M)$ matrix and store each submatrix in its separate page. The motivation to form submatrices like this was that this format allows for easy TRANSPOSE operation with better time complexity as compared to other formats. (If the original matrix is not perfectly decomposed into smaller $M * M$ matrices, then we pad the original matrices with $-1$. This does not increase space consumption as it does not increase the number of pages that are used).
We took $M$ to be 43 (to ensure that the size of each page does not exceed 8kB).

### Transpose

To perform in place Transpose, we load two disk blocks into memory, Transpose them and write them back onto the disk.

Specically, for a block $(i, j)$ where $i$ not equal to $j$, we perform the following operation:
&nbsp;&nbsp; $1.$ We load blocks $(i, j)$ and $(j, i)$ into main memory.
&nbsp;&nbsp; $2.$ Transpose both of them in place.
&nbsp;&nbsp; $3.$ Write the block $(i, j)$ in the black where $(j, i)$ was stored and $(j, i)$ in the block where $(i, j)$ was stored previously.

For a block $(i, i)$:
&nbsp;&nbsp; 1. We load block $(i, i)$ into memory
&nbsp;&nbsp; 2. Transpose the block in place.
&nbsp;&nbsp; 3. Write it back in the same page.

Transpose operation in sparse matrices is performed in a different way and is discussed later.

## Sparse Matrices
### Compresssion technique and page layout

We store the matrix in compressed form if the number of zeros in the matrix is greater than 60% to save space.

To compress the matrix, instead storing the entire matrix, we only store information about cells which have non-zero numbers in them. Specifically, if there is a zero at cell $(i, j)$, then we simply ignore it. Otherwise we store a tuple of the form $(i, j, val)$, where val is the non-zero value at cell $(i, j)$.

Thus, the page layout for non sparse matrices, is a list of tuples storing the location as well as value of each non-zero cell.

### Compression ratio

Let the input matrix be of size $N * N$ and let the number of zeros in the matrix be $p * (N * N)$ where $0$ $\le$ $p$ $\le$ $1$.
Number of non-zero elements than become $(1 - p) * (N * N)$

If we dont store the matrix in compressed state, spaces used up will be $4 * N * N$ bytes (4 bytes for every integer)

In the compressed format, for each non-zero cell we store 3 integers. Thus space used will be $4 * 3 * (1 - p) * (N * N)$

Compression ratio is thus: $\frac{4 * (N * N)}{4 * 3 * (1 - p) * (N * N)}$ = $\frac{1}{3 * (1 - p)}$

For a 90% sparse matrix, $p$ = $0.9$, the compression ratio comes out to be $3.333$

### Transpose
The way transpose operation is performed for a sparse matrix is significantly different than the way it was performed for non sparce matrices.

In the original matrix, if there was a non zero value at location $(i, j)$, then in the transpose matrix, that same non zero value will be at $(j, i)$. Thus we iterate through each page, and for every tuple $(i, j, val)$, we replace it by $(j, i, val)$.
However, earlier the entries in the page were sorted by rows and then by columns (which is perferred as it makes life easier for performing EXPORT and PRINT operations). However, after doing this operation, the entries in page are first sorted by column and then by rows. Thus there is a need to sort the entries.
For sorting, if there are $N$ blocks in total, then we perform $N$ iterations, and in each iteration $i$, we write the page that should come in the $i$th location in sorted order (the alogrithm works similar to selection sort. In iteration $i$, we pick up the $i$ the page and then for each $j$ such that $i$ $\lt$ $j$ $\le$ $N$ we  concatenate the entries in page $i$ and $j$, sort them, and place the first half entries in page $i$ and the second half in page $j$. This ensures that in iteration $i$, page $i$ is correctly sorted and is in its correct place)