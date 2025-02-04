grammar McPoint;
import CommonLexerRules;

array: '[' pair (',' pair)* ']'  # validArray

     | '[' ']'                   { notifyErrorListeners("Empty array is not allowed!"); }    # emptyArrayError
     | '[' pair (',' pair)*      { notifyErrorListeners("Missing closing ']'"); }            # missingClosingBracket
     | pair (',' pair)* ']'      { notifyErrorListeners("Missing opening '['"); }            # missingOpeningBracket
     | '[' pair (',' pair)* ','  { notifyErrorListeners("Trailing comma is not allowed!"); } # trailingCommaError
     ;

pair: VARID '=' value  # validPair

    | VARID '='        { notifyErrorListeners("Missing value after '='"); }  # missingValueError
    | '=' value        { notifyErrorListeners("Missing identifier before '='"); }  # missingIdentifierError
    | VARID value      { notifyErrorListeners("Missing '=' between identifier and value"); }  # missingEqualSignError
    | VARID            { notifyErrorListeners("Missing '=' and value after identifier"); }  # missingEqualAndValueError
    ;

value: op='-'? UNSIGNED_INT  # validValue

     | '-'       { notifyErrorListeners("Missing number after '-'"); }  # missingNumberAfterMinusError
     | FLOAT     { notifyErrorListeners("Floating-point numbers are not allowed"); }  # floatNotAllowedError
     ;