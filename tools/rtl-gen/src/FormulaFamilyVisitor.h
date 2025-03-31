#pragma once

#include <spot/tl/apcollect.hh>
#include "AlternationFormulaFamilyParams.h"
#include "formula.h"

/*!
 *  \brief Visitor for generating formulae based on different formula families.
 *
 *  The \c FormulaFamilyVisitor struct implements the visitor design pattern to generate formulae
 *  based on the \c FormulaFamilyParams variant type.
 *  It can be easily extended to support additional formula families by adding new visitor functions for each.
 */
struct FormulaFamilyVisitor
{
    /*!
     *  \brief Generates an alternating formula based on the given parameters.
     *
     *  It generates an alternating formula that alternates between two atomic propositions \f$p\f$ and \f$q\f$ a specified
     *  number of times (`alternations`).
     *
     *  \param params The parameters containing the alternation count and proposition names.
     *  \return A formula representing the alternating formula.
     */
    spot::formula operator() (const AlternationFormulaFamilyParams& params) const
    {
        return generateAlternatingFormula(
            params.alternations,
            ap(params.pAtomName),
            ap(params.qAtomName)
        );
    }
};