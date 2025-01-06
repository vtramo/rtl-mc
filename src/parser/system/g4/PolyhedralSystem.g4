grammar PolyhedralSystem;
import LinearExpression;

system: atom+ flow  atom* inv atom*
      | atom* flow  atom+ inv atom*
      | atom* flow  atom* inv atom+

      | atom+ inv  atom* flow atom*
      | atom* inv  atom+ flow atom*
      | atom* inv  atom* flow atom+

      | atom+ flow atom*  { notifyErrorListeners("Inv is missing!"); }
      | atom* flow atom+  { notifyErrorListeners("Inv is missing!"); }
      | atom+ inv atom*   { notifyErrorListeners("Flow is missing!"); }
      | atom* inv atom+   { notifyErrorListeners("Flow is missing!"); }
      | atom+             { notifyErrorListeners("Flow and Inv are missing!"); }
      | flow inv          { notifyErrorListeners("You must define at least one atomic proposition!"); }
      | inv flow          { notifyErrorListeners("You must define at least one atomic proposition!"); }
      | WS*               { notifyErrorListeners("Empty input!"); }
      ;

inv: INV powerset
   | INV poly* { notifyErrorListeners("Invariant must be defined by a powerset!"); }
   ;

flow: FLOW poly
    | FLOW poly+    { notifyErrorListeners("Flow must be defined by a single polyhedron, not multiple!"); }
    | FLOW powerset { notifyErrorListeners("Flow must be defined by a single polyhedron, not multiple!"); }
    ;

atom: VARID powerset                                                                    # atomPowerset
    | VARID '{' '}'                                                                     # atomEmpty
    | VARID poly                                                                        # atomPoly

    | VARID '{' '}' '}'+        { notifyErrorListeners("Too many parentheses"); }   # atomError
    | VARID '{'+ '{' '}'        { notifyErrorListeners("Too many parentheses"); }   # atomError
    | VARID '{'+ '{' '}' '}'+   { notifyErrorListeners("Too many parentheses"); }   # atomError
    | VARID poly+                   { notifyErrorListeners("Only one polyhedron is allowed! If you want a set of polyhedra, use a powerset."); }  # atomError
    ;

powerset: '(' poly* ')'     # powersetEmptyOrNotEmpty
        | '(' 'true' ')'    # powersetTrue

        | '(' poly* ')' ')'+        { notifyErrorListeners("Too many parentheses"); }  # powersetError
        | '('+ '(' poly* ')' ')'+   { notifyErrorListeners("Too many parentheses"); }  # powersetError
        | '('+ '(' poly* ')'+       { notifyErrorListeners("Too many parentheses"); }  # powersetError
        | '(' poly*                 { notifyErrorListeners("Missing closing ')'");  }  # powersetError
        | poly* ')'                 { notifyErrorListeners("Missing opening '('");  }  # powersetError
        ;

poly: '{' constr ('&' constr)* '}'  # polyAtLeastOneConstr
    | '{' 'true' '}'                # polyTrue

    | '{' constr ('&' constr)* '}' '}'+         { notifyErrorListeners("Too many parentheses"); }  # polyError
    | '{'+ '{' constr ('&' constr)* '}' '}'+    { notifyErrorListeners("Too many parentheses"); }  # polyError
    | '{'+ '{' constr ('&' constr)* '}'+        { notifyErrorListeners("Too many parentheses"); }  # polyError
    | constr ('&' constr)* '}'                  { notifyErrorListeners("Missing closing '}'"); }   # polyError
    | constr ('&' constr)* '}'                  { notifyErrorListeners("Missing opening '{'"); }   # polyError
    | '{' (constr '&' constr)* constr '}'       { notifyErrorListeners("Constraints must be concatenated with '&' operator."); } # polyError
    | '{' constr ('&' constr)* '&' '}'          { notifyErrorListeners("Missing right constraint."); } # polyError
    | '{' '&' constr ('&' constr)* '}'          { notifyErrorListeners("Missing left constraint."); }  # polyError
    ;

constr: linearExpr op=(LE|LT|GT|GE|EQ) linearExpr

      | linearExpr linearExpr { notifyErrorListeners("You must specify a valid operator (<=, <, >, >=, =) between the linear expressions."); }
      | linearExpr op=(LE|LT|GT|GE|EQ) { notifyErrorListeners("Missing right linear expression"); }
      | op=(LE|LT|GT|GE|EQ) linearExpr { notifyErrorListeners("Missing left linear expression"); }
      | linearExpr op=~(LE|LT|GT|GE|EQ) { notifyErrorListeners("Invalid operator '" + $op.text + "'. Valid operators are: <=, <, >, >=, =."); }
      ;
