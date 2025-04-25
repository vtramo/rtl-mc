#pragma once
#include "ppl_aliases.h"

class SolverResult
{
public:
    SolverResult(const bool isIncompleteResult, PowersetSharedPtr result)
        : m_isIncompleteResult { isIncompleteResult }
        , m_result{ result }
    {
    }

    [[nodiscard]] bool isIncompleteResult() const { return m_isIncompleteResult; }
    [[nodiscard]] PowersetSharedPtr result() const { return m_result; }

private:
    bool m_isIncompleteResult{};
    PowersetSharedPtr m_result{};
};
