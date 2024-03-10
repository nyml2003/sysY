```mermaid
graph TD
6[CompUnit]--0--->5
5[FuncDef]
5--retType-->0
0[Type:int]
5--ident-->1
1[main]
5--block-->4
4[Block]
4--0-->3
3[ReturnStmt]
3--expr-->2
2[0]
```
