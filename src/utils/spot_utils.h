#ifndef SPOT_UTILS_H
#define SPOT_UTILS_H

#include <spot/tl/formula.hh>

namespace SpotUtils
{
    spot::formula top();
    spot::formula ap(std::string_view ap);
    spot::formula F(spot::formula&& formula);
    spot::formula X(spot::formula&& formula);
    spot::formula X(const spot::formula& formula);
    spot::formula G(spot::formula&& formula);
    spot::formula G(const spot::formula& formula);
    spot::formula W(spot::formula&& formula1, spot::formula&& formula2);
    spot::formula U(spot::formula&& formula1, spot::formula&& formula2);
    spot::formula U(const spot::formula& formula1, spot::formula&& formula2);
    spot::formula R(spot::formula&& formula1, spot::formula&& formula2);
    spot::formula S(spot::formula&& formula1, spot::formula&& formula2);
    spot::formula Equiv(const spot::formula& formula1, const spot::formula& formula2);
    spot::formula And(std::vector<spot::formula>&& formulas);
    spot::formula Or(std::vector<spot::formula>&& formulas);
    spot::formula Not(spot::formula&& formula);
    spot::formula Not(const spot::formula& formula);
    spot::formula sing();
    spot::formula alive();
    spot::formula notSing();
    spot::formula singOr(spot::formula&& formula);
    spot::formula singOr(const spot::formula& formula);
    spot::formula singOrNot(spot::formula&& formula);
    spot::formula singAndAliveAnd(spot::formula&& formula);

spot::formula generateRtlf(int atomicPropSetSize, int formulaSize, spot::op replaceXWith = spot::op::F);
}

#endif //SPOT_UTILS_H
