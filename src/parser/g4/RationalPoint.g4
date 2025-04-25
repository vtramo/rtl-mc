grammar RationalPoint;
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

value: op=(PLUS|MINUS)? UNSIGNED_INT  # validIntegerValue
     | opNum=(PLUS|MINUS)? UNSIGNED_INT '/' opDen=(PLUS|MINUS)? denom=UNSIGNED_INT # validRationalValue

     | '-' '-'          { notifyErrorListeners("Unexpected second '-'"); }  # unexpectedDoubleMinus
     | '-'              { notifyErrorListeners("Missing number after '-'"); }  # missingNumberAfterMinusError
     | '-' op='/'       { notifyErrorListeners("Expected number after '-' but found operator"); }  # missingNumberAfterMinusOperator
     | '-' op='+'       { notifyErrorListeners("Expected number after '-' but found operator '+'"); }  # missingNumberAfterMinusPlus
     | '-'              { notifyErrorListeners("Missing number after '-'"); }  # missingNumberAfterMinusError

     | FLOAT                                                                    { notifyErrorListeners("Floating-point numbers are not allowed!"); }  # floatNotAllowedError
     | op=(PLUS|MINUS)? FLOAT                                                   { notifyErrorListeners("Floating-point numbers are not allowed!"); }  # floatNotAllowedError
     | op=(PLUS|MINUS)? '.' UNSIGNED_INT                                        { notifyErrorListeners("Floating-point numbers are not allowed!"); }  # floatNotAllowedError
     | op=(PLUS|MINUS)? UNSIGNED_INT '/' op=(PLUS|MINUS)? FLOAT                 { notifyErrorListeners("Floating-point numbers are not allowed!"); }  # floatNotAllowedError
     | op=(PLUS|MINUS)? FLOAT '/' op=(PLUS|MINUS)? UNSIGNED_INT                 { notifyErrorListeners("Floating-point numbers are not allowed!"); }  # floatNotAllowedError
     | op=(PLUS|MINUS)? '.' UNSIGNED_INT '/' op=(PLUS|MINUS)? UNSIGNED_INT      { notifyErrorListeners("Floating-point numbers are not allowed!"); }  # floatNotAllowedError
     | op=(PLUS|MINUS)? UNSIGNED_INT '/' op=(PLUS|MINUS)? '.' UNSIGNED_INT      { notifyErrorListeners("Floating-point numbers are not allowed!"); }  # floatNotAllowedError

     | op=(PLUS|MINUS)? UNSIGNED_INT '/' { notifyErrorListeners("Missing denominator after '/'"); }  # missingDenominatorError
     | op=(PLUS|MINUS)? '/' op=(PLUS|MINUS)? UNSIGNED_INT { notifyErrorListeners("Missing numerator before '/'"); }  # missingNumeratorError
     | op=(PLUS|MINUS)? UNSIGNED_INT '/' op=(PLUS|MINUS)? UNSIGNED_INT { notifyErrorListeners("Missing numerator before '/'"); }  # missingNumeratorError
     ;