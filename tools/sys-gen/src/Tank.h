#pragma once

#include "ClosedInterval.h"
#include "ppl_aliases.h"

class Tank
{
public:
    Tank() = default;

    Tank(PPL::Variable variable, ClosedInterval<int> inputPump, ClosedInterval<int> outputPump)
        : m_variable { variable }
        , m_inputPump { inputPump }
        , m_outputPump { outputPump }
    {
        m_variability = ClosedInterval<int> {
            m_inputPump.min() - m_outputPump.max(),
            m_inputPump.max() - m_outputPump.min()
        };

        m_variabilityConstraints.insert(m_variable >= m_variability.min());
        m_variabilityConstraints.insert(m_variable <= m_variability.max());
    }

    [[nodiscard]] const PPL::Variable& variable() const { return m_variable; }
    [[nodiscard]] const ClosedInterval<int>& variability() const { return m_variability; }
    [[nodiscard]] const PPL::Constraint_System& variabilityConstraints() const { return m_variabilityConstraints; }
    [[nodiscard]] const ClosedInterval<int>& inputPumpInterval() const { return m_inputPump; }
    [[nodiscard]] const ClosedInterval<int>& outputPumpInterval() const { return m_outputPump; }

private:
    PPL::Variable m_variable { 0 };
    ClosedInterval<int> m_inputPump {};
    ClosedInterval<int> m_outputPump {};
    ClosedInterval<int> m_variability {};
    PPL::Constraint_System m_variabilityConstraints {};
};