# pipeline
```mermaid
graph LR
    subgraph "Compiler"
        A[Source] --Driver--> B[flex/bison]
        B ---> C[AST]
        C ---> E[LLVM IR]
    end
    C -- output--> D[Mermaid]
```
