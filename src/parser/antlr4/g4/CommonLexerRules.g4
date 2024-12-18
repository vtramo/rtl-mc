lexer grammar CommonLexerRules;

fragment DIGIT:[0-9];
fragment LETTER:[a-zA-Z];
fragment A:[aA];
fragment B:[bB];
fragment C:[cC];
fragment D:[dD];
fragment E:[eE];
fragment F:[fF];
fragment G:[gG];
fragment H:[hH];
fragment I:[iI];
fragment J:[jJ];
fragment K:[kK];
fragment L:[lL];
fragment M:[mM];
fragment N:[nN];
fragment O:[oO];
fragment P:[pP];
fragment Q:[qQ];
fragment R:[rR];
fragment S:[sS];
fragment T:[tT];
fragment U:[uU];
fragment V:[vV];
fragment W:[wW];
fragment X:[xX];
fragment Y:[yY];
fragment Z:[zZ];

LINE_COMMENT: '//' .*? NEWLINE -> skip ;
COMMENT: '/*' .*? '*/' -> skip ;
WS: [ \t\r\n]+ -> skip ;

UNSIGNED_INT:   (DIGIT|[1-9]DIGIT*)   ;
NEWLINE:   '\r'? '\n'  ;

TIMES: '*' ;
PLUS : '+' ;
MINUS: '-' ;

LE: '<=' ;
LT: '<'  ;
GT: '>'  ;
GE: '>=' ;
EQ: '='  ;

FLOW: F L O W;
INV: I N V;
ID: LETTER DIGIT? ;