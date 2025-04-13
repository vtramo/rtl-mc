grammar Polyhedra;
import LexerRules;

polyhedra: atom+
         | WS*               { notifyErrorListeners("Empty input!"); }
         ;

atom: VARID powerset HEX_COLOR_CODE?                                                                    # atomPowerset
    | VARID '{' '}' HEX_COLOR_CODE?                                                                     # atomEmpty
    | VARID poly HEX_COLOR_CODE?                                                                        # atomPoly

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

linearExpr
    : op=(PLUS|MINUS)? term (signTerm)*
    ;

signTerm : op=(PLUS|MINUS) term
         ;

term :  UNSIGNED_INT TIMES? VARID   # intTimesVar
     |  VARID                       # var
     |  UNSIGNED_INT                # int

     | UNSIGNED_INT op=~TIMES VARID   { notifyErrorListeners("Invalid operator '" + $op.text + "'. Only '*' is allowed."); }  # termError
     | FLOAT                          { notifyErrorListeners("Only integer values are allowed."); }  # termError
     | FLOAT TIMES? VARID             { notifyErrorListeners("Only integer values are allowed."); }  # termError
     ;