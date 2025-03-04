#pragma once

#include <optional>
#include <stdexcept>

class SingOpenStatePair
{
public:
    SingOpenStatePair(const unsigned state, const bool isSing)
    {
        if (isSing)
        {
            m_singState = state;
        }
        else
        {
            m_openState = state;
        }
    }

    SingOpenStatePair(unsigned singState, unsigned openState)
        : m_singState { singState }
        , m_openState { openState }
    {
    }

    SingOpenStatePair() = default;

    bool containsSingState() const
    {
        return m_singState.has_value();
    }

    bool containsOpenState() const
    {
        return m_openState.has_value();
    }

    unsigned singState() const
    {
        if (!m_singState.has_value())
        {
            throw std::logic_error("SingOpenStatePair: singState is not set");
        }

        return *m_singState;
    }

    unsigned openState() const
    {
        if (!m_openState.has_value())
        {
            throw std::logic_error("SingOpenStatePair: openState is not set");
        }

        return *m_openState;
    }

private:
    std::optional<unsigned> m_singState {};
    std::optional<unsigned> m_openState {};
};
