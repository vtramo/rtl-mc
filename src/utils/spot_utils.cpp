#include "spot_constants.h"
#include "spot_utils.h"

spot::formula top()
{
    return spot::formula::tt();
}

spot::formula ap(const std::string_view ap)
{
    return spot::formula::ap(std::string { ap });
}

spot::formula F(spot::formula&& formula)
{
    return spot::formula::F(std::move(formula));
}

spot::formula X(spot::formula&& formula)
{
    return spot::formula::X(std::move(formula));
}

spot::formula X(const spot::formula& formula)
{
    return spot::formula::X(formula);
}

spot::formula G(spot::formula&& formula)
{
    return spot::formula::G(std::move(formula));
}

spot::formula G(const spot::formula& formula)
{
    return spot::formula::G(formula);
}

spot::formula W(spot::formula&& formula1, spot::formula&& formula2)
{
    return spot::formula::W(std::move(formula1), std::move(formula2));
}

spot::formula U(spot::formula&& formula1, spot::formula&& formula2)
{
    return spot::formula::U(std::move(formula1), std::move(formula2));
}

spot::formula U(const spot::formula& formula1, spot::formula&& formula2)
{
    return spot::formula::U(formula1, std::move(formula2));
}

spot::formula R(spot::formula&& formula1, spot::formula&& formula2)
{
    return spot::formula::R(std::move(formula1), std::move(formula2));
}

spot::formula S(spot::formula&& formula1, spot::formula&& formula2)
{
    return spot::formula::S(std::move(formula1), std::move(formula2));
}

spot::formula Equiv(const spot::formula& formula1, const spot::formula& formula2)
{
    return spot::formula::Equiv(formula1, formula2);
}

spot::formula And(std::vector<spot::formula>&& formulas)
{
    return spot::formula::And(std::move(formulas));
}

spot::formula Or(std::vector<spot::formula>&& formulas)
{
    return spot::formula::Or(std::move(formulas));
}

spot::formula Not(const spot::formula& formula)
{
    return spot::formula::Not(formula);
}

spot::formula Not(spot::formula&& formula)
{
    return spot::formula::Not(std::move(formula));
}

spot::formula sing()
{
    return spot::constants::g_sing;
}

spot::formula singOr(spot::formula&& formula)
{
    return Or({ spot::constants::g_sing, std::move(formula) });
}

spot::formula singOr(const spot::formula& formula)
{
    return Or({ spot::constants::g_sing, formula });
}

spot::formula singOrNot(spot::formula&& formula)
{
    return Or({ spot::constants::g_sing, Not(std::move(formula))});
}

spot::formula alive()
{
    return spot::constants::g_alive;
}

spot::formula singAndAlive()
{
    return And({ spot::constants::g_sing, spot::constants::g_alive });
}

spot::formula singAndAliveAnd(spot::formula&& formula)
{
    return And({ singAndAlive(), std::move(formula) });
}

spot::formula singAndAliveAnd(const spot::formula& formula)
{
    return And({ singAndAlive(), formula });
}

spot::formula notSing()
{
    return Not(spot::constants::g_sing);
}

