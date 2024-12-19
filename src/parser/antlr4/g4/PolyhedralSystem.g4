grammar PolyhedralSystem;
import LinearExpression;

system: atom+ flow  atom* inv atom*
      | atom* flow  atom+ inv atom*
      | atom* flow  atom* inv atom+

      | atom+ inv  atom* flow atom*
      | atom* inv  atom+ flow atom*
      | atom* inv  atom* flow atom+
      ;

inv: INV powerset
   ;

flow: FLOW poly
    ;

atom: ID powerset               # atomPowerset
    | ID '{'WS*'}'              # atomEmpty
    | ID poly                   # atomPoly
    ;

powerset: '(' poly* ')'
        ;

poly: '{' constr ('&' constr)* '}'
    ;

constr: linearExpr op=(LE|LT|GT|GE|EQ) linearExpr
      ;

