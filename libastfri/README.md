# TODO
nullptr je bug, pouzivat prislusny nullobject typ


Header dependencies:
```
  Kind <--+
          \
ExprFwd <-+ ExprKind <-+ ExprDef <-- ExprFactory
                       /
           ASTNode <--+
```
