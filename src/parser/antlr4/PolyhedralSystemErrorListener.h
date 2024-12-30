//
// Created by vincenzo on 28/12/24.
//

#ifndef POLYHEDRALSYSTEMERRORLISTENER_H
#define POLYHEDRALSYSTEMERRORLISTENER_H

#include "antlr4-runtime.h"
#include "PolyhedralSystemParserError.h"


class PolyhedralSystemErrorListener final: public antlr4::BaseErrorListener {
public:
    void syntaxError(
      antlr4::Recognizer *recognizer,
      antlr4::Token* offendingSymbol,
      size_t line,
      size_t charPositionInLine,
      const std::string &msg,
      std::exception_ptr e) override;

    [[nodiscard]] bool hasErrors() const;
    [[nodiscard]] std::vector<PolyhedralSystemParserError> getErrors() const;
private:
    std::vector<PolyhedralSystemParserError> m_errors {};

    void addError(PolyhedralSystemParserError&& error);
};
#endif //POLYHEDRALSYSTEMERRORLISTENER_H
