grammar LinearExpression;
import CommonLexerRules;


linearExpr
    : op=(PLUS|MINUS)? term (signTerm)*
    ;

signTerm : op=(PLUS|MINUS) term
         ;

term :  UNSIGNED_INT TIMES? ID   # intTimesVar
     |  ID                       # var
     |  UNSIGNED_INT             # int
     ;