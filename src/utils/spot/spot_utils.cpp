#include "spot_constants.h"
#include "spot_utils.h"

#include <spot/tl/apcollect.hh>
#include <spot/tl/randomltl.hh>
#include <spot/twa/formula2bdd.hh>
#include <spot/twa/twagraph.hh>

namespace SpotUtils
{
    spot::formula top()
    {
        return spot::formula::tt();
    }

    spot::formula bottom()
    {
        return spot::formula::ff();
    }

    spot::formula ap(const std::string_view ap)
    {
        return spot::formula::ap(std::string { ap });
    }

    spot::formula F(const spot::formula& formula)
    {
        return spot::formula::F(formula);
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

    spot::formula strongX(const spot::formula& formula)
    {
        return spot::formula::unop(spot::op::strong_X, formula);
    }

    spot::formula strongX(spot::formula&& formula)
    {
        return spot::formula::unop(spot::op::strong_X, std::move(formula));
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

    spot::formula Equiv(const spot::formula& formula1, const spot::formula& formula2)
    {
        return spot::formula::Equiv(formula1, formula2);
    }

    spot::formula And(std::vector<spot::formula>&& formulas)
    {
        return spot::formula::And(std::move(formulas));
    }

    spot::formula andAtoms(const spot::atomic_prop_set& atoms)
    {
        spot::formula result {};
        for (spot::formula atom: atoms)
            result = And(std::vector { std::move(atom), std::move(result) });
        return result;
    }

    spot::formula generateAlternatingFormula(int k, spot::formula p, spot::formula q)
    {
        assert(k > 0);

        spot::formula result { F(And({ p, F(q) })) };
        if (k == 1)
            return result;

        bool pTurn = false;
        while (--k > 0)
        {
            result = pTurn ? F(And({ p, result })) : F(And({ q  , result }));
            pTurn = !pTurn;
        }

        return result;
    }

    std::string toOptimizationLevelString(const spot::postprocessor::optimization_level optimizationLevel)
    {
        switch (optimizationLevel)
        {
        case spot::postprocessor::Low:
            return "Low";
        case spot::postprocessor::Medium:
            return "Medium";
        case spot::postprocessor::High:
            return "High";
        }

        return "Unknown";
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

    spot::formula notAlive()
    {
        return Not(spot::constants::g_alive);
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

    spot::formula finiteAlternationSingOpenObservablesOneStep()
    {
        return spot::constants::g_finiteAlternationSingOpenObservablesOneStep;
    }

    spot::formula finiteAlternationSingOpenObservables()
    {
        return spot::constants::g_finiteAlternationSingOpenObservables;
    }

    spot::formula aliveUntilGNotAlive()
    {
        return spot::constants::g_aliveUntilGNotAlive;
    }

    spot::formula generateRtlf(const int atomicPropSetSize, const int formulaSize, spot::op replaceXWith)
    {
        const spot::atomic_prop_set AP {  spot::create_atomic_prop_set(atomicPropSetSize) };
        const spot::random_ltl randomLtl { &AP };
        spot::formula generatedFormula { randomLtl.generate(formulaSize) };

        auto replaceX = [&] (spot::formula formula, auto&& self)
        {
            if (formula.is(spot::op::X))
            {
                switch (replaceXWith)
                {
                case spot::op::F:
                    formula = { F(formula[0]) };
                    break;

                case spot::op::G:
                    formula = { G(formula[0]) };
                    break;

                default:
                    formula = { F(formula[0]) };
                    break;
                }
            }

            return formula.map(self, self);
        };

        return replaceX(generatedFormula, replaceX);
    }

    bool isXFree(spot::formula& formula)
    {
        bool hasX { false };

        formula.traverse([&hasX] (const spot::formula& child)
                        {
                            if (child.is(spot::op::X))
                            {
                                hasX = true;
                            }

                            return hasX;
                        });

        return !hasX;
    }

    spot::atomic_prop_set AP(std::set<std::string>&& atoms)
    {
        spot::atomic_prop_set atomicPropVector {};
        for (const auto& atom : atoms)
            atomicPropVector.insert(ap(atom));
        return atomicPropVector;
    }

    spot::atomic_prop_vector collectAtomsNotIn(const spot::atomic_prop_set& forbiddenAtoms, spot::formula& formula)
    {
        spot::atomic_prop_vector result {};
        result.reserve(forbiddenAtoms.size());

        formula.traverse([&] (const spot::formula& child)
        {
            if (child.is(spot::op::ap))
            {
                if (!forbiddenAtoms.count(child))
                {
                    result.push_back(child);
                }
            }

            return false;
        });

        return result;
    }

    std::string toFormulaString(const spot::formula& formula)
    {
        std::ostringstream oss {};
        oss << formula;
        return { oss.str() };
    }

    std::tuple<spot::formula, bool> removeSing(spot::formula&& formula)
    {
        assert(formula.is_sugar_free_boolean());
        assert(formula.is_in_nenoform());

        if (isSingOrNotSing(formula))
            return std::make_tuple(
                spot::formula::ff(),
                true
            );

        if (formula.is_literal() || formula.is_tt() || formula.is_ff())
            return std::make_tuple(
                formula,
                false
            );

        bool removedAtLeastOneSing {};
        auto removeAllSingAtoms = [&](spot::formula&& child, auto&& self) -> spot::formula
        {
            const bool isAnd { child.is(spot::op::And) };
            std::vector<spot::formula> children {};
            children.resize(child.size());

            for (auto&& childChild: child)
            {
                if (isSing(childChild) || isNotSing(childChild))
                {
                    if (isSing(childChild)) removedAtLeastOneSing = true;
                    children.push_back(isAnd ? top() : bottom());
                } else if (childChild.is_literal())
                {
                    children.push_back(childChild);
                } else
                {
                    children.push_back(self(std::move(childChild), self));
                }
            }

            return isAnd ? spot::formula::And(std::move(children)) : spot::formula::Or(std::move(children));
        };

        return std::make_tuple(
            removeAllSingAtoms(std::move(formula), removeAllSingAtoms),
            removedAtLeastOneSing
        );
    }

    bool isSingOrNotSing(const spot::formula& formula)
    {
        return isSing(formula) || (formula.is(spot::op::Not) && isSing(formula[0]));
    }

    bool isNotSing(const spot::formula& formula)
    {
        return formula.is(spot::op::Not) && isSing(formula[0]);
    }

    spot::atomic_prop_set collectPositiveLiterals(spot::formula&& formula)
    {
        spot::atomic_prop_set result {};

        formula.traverse([&] (const spot::formula& child)
        {
            if (child.is(spot::op::Not) && child[0].is(spot::op::ap))
            {
                return true;
            }

            if (child.is(spot::op::ap))
            {
                result.insert(child);
            }

            return false;
        });

        return result;
    }

    spot::atomic_prop_set extractLabelsFromEdgeGuard(const spot::twa_graph_ptr& twaGraph, const bdd& guard)
    {
        spot::formula formula = spot::bdd_to_formula(guard, twaGraph->get_dict());
        minterms_of minterms { guard, twaGraph->ap_vars() };
        spot::atomic_prop_set labels { };

        for (const bdd& minterm: minterms)
        {
            spot::formula mintermFormula { bdd_to_formula(minterm, twaGraph->get_dict()) };
            for (const spot::formula& label: collectPositiveLiterals(std::move(mintermFormula)))
            {
                labels.insert(std::move(label));
            }
        }

        return labels;
    }

    bool containsSing(const spot::atomic_prop_set& labels)
    {
        return labels.find(sing()) != labels.end();
    }

    bool isSing(const spot::formula& formula)
    {
        return spot::constants::g_sing == formula;
    }

    void transpose(const spot::twa_graph_ptr& twaGraph)
    {
        const unsigned totalStates { twaGraph->num_states() };

        for (unsigned srcStateId { 0 }; srcStateId < totalStates; ++srcStateId)
        {
            for (auto& nfaEdge: twaGraph->out(srcStateId))
            {
                unsigned nfaEdgeDst = nfaEdge.dst;
                nfaEdge.dst = srcStateId;
                nfaEdge.src = nfaEdgeDst;
            }
        }
    }

    bool isNonRecurrent(spot::formula& formula)
    {
        if (!formula.is_ltl_formula())
            throw std::invalid_argument("Spot formula is not a LTL formula");

        bool isNonRecurrent { true };

        formula.traverse([&isNonRecurrent] (const spot::formula& child)
                        {
                            if (!isNonRecurrent) return true;

                            if (child.is(spot::op::G))
                            {
                                isNonRecurrent = child[0].is_boolean();
                            } else if (child.is(spot::op::R))
                            {
                                isNonRecurrent = child[1].is_boolean();
                            }

                            return !isNonRecurrent;
                        });

        return isNonRecurrent;
    }
}
