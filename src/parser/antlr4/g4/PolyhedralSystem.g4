grammar PolyhedralSystem;
import LinearExpression;

system: flow  inv   atom+
      | inv   flow  atom+
      | atom+ inv   flow
      | atom+ flow  inv
      | inv   atom+ flow
      | flow  atom+ inv

      | atom+ flow  atom* inv atom*
      | atom* flow  atom+ inv atom*
      | atom* flow  atom* inv atom+

      | atom+ inv  atom* flow atom*
      | atom* inv  atom+ flow atom*
      | atom* inv  atom* flow atom+
      ;

inv: INV powerset NEWLINE?
   ;

flow: FLOW poly NEWLINE?
    ;

atom: ID powerset NEWLINE?      # atomPowerset
    | ID '{'WS*'}'              # atomEmpty
    | ID poly                   # atomPoly
    ;

powerset: '(' poly* ')'
        ;

poly: '{' constr ('&' NEWLINE? constr)* '}'
    ;

constr: linearExpr op=(LE|LT|GT|GE|EQ) linearExpr
      ;

