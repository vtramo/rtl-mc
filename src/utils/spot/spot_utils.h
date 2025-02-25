#ifndef SPOT_UTILS_H
#define SPOT_UTILS_H

#include <spot/tl/apcollect.hh>
#include <spot/tl/formula.hh>
#include <spot/twaalgos/postproc.hh>

namespace spot
{
    using atomic_prop_vector = std::vector<formula>;
}

namespace SpotUtils
{
    spot::formula top();
    spot::formula bottom();
    spot::atomic_prop_set AP(std::set<std::string>&& atoms);
    spot::formula ap(std::string_view ap);
    spot::formula F(spot::formula&& formula);
    spot::formula F(const spot::formula& formula);
    spot::formula X(spot::formula&& formula);
    spot::formula strongX(spot::formula&& formula);
    spot::formula strongX(const spot::formula& formula);
    spot::formula X(const spot::formula& formula);
    spot::formula G(spot::formula&& formula);
    spot::formula G(const spot::formula& formula);
    spot::formula W(spot::formula&& formula1, spot::formula&& formula2);
    spot::formula U(spot::formula&& formula1, spot::formula&& formula2);
    spot::formula U(const spot::formula& formula1, spot::formula&& formula2);
    spot::formula R(spot::formula&& formula1, spot::formula&& formula2);
    spot::formula Equiv(const spot::formula& formula1, const spot::formula& formula2);
    spot::formula And(std::vector<spot::formula>&& formulas);
    spot::formula Or(std::vector<spot::formula>&& formulas);
    spot::formula Not(spot::formula&& formula);
    spot::formula Not(const spot::formula& formula);
    spot::formula sing();
    spot::formula alive();
    spot::formula notAlive();
    spot::formula notSing();
    spot::formula singOr(spot::formula&& formula);
    spot::formula singOr(const spot::formula& formula);
    spot::formula singOrNot(spot::formula&& formula);
    spot::formula singAndAliveAnd(spot::formula&& formula);
    spot::formula aliveUntilGNotAlive();
    spot::formula finiteAlternationSingOpenObservablesOneStep();
    spot::formula finiteAlternationSingOpenObservables();

    spot::formula generateAlternatingFormula(int k, spot::formula p = ap("p"), spot::formula q = ap("q"));
    std::string toFormulaString(const spot::formula& formula);
    std::string toOptimizationLevelString(spot::postprocessor::optimization_level optimizationLevel);
    bool isSing(const spot::formula& formula);
    bool isNotSing(const spot::formula& formula);
    bool isSingOrNotSing(const spot::formula& formula);
    std::tuple<spot::formula, bool> removeSing(spot::formula&& formula);
    bool isXFree(spot::formula& formula);
    bool containsSing(const spot::atomic_prop_set& labels);
    spot::formula generateRtlf(int atomicPropSetSize, int formulaSize, spot::op replaceXWith = spot::op::F);
    spot::atomic_prop_vector collectAtomsNotIn(const spot::atomic_prop_set& forbiddenAtoms, spot::formula& formula);
    spot::atomic_prop_set collectPositiveLiterals(spot::formula&& formula);
    spot::atomic_prop_set extractLabelsFromEdgeGuard(const spot::twa_graph_ptr& twaGraph, const bdd& guard);
    void transpose(const spot::twa_graph_ptr& twaGraph);
    bool isNonRecurrent(spot::formula& formula);
    spot::formula andAtoms(const spot::atomic_prop_set& atoms);
}

#endif //SPOT_UTILS_H
