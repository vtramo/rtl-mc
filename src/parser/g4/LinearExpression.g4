grammar LinearExpression;
import CommonLexerRules;

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