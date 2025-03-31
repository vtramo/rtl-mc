#pragma once

/*!
 *  \brief Parameters for the `alternation` formula family.
 *  \note The default values for the proposition names are \f$p\f$ for `pAtomName` and \f$q\f$ for `qAtomName`.
 *  \see RtlGenProgram
 */
struct AlternationFormulaFamilyParams
{
    /*!
     *  \brief The name of the first atomic proposition (`p`).
     *  \note Default value is "p".
     */
    std::string pAtomName {};

    /*!
     *  \brief The name of the second atomic proposition (`q`).
     *  \note Default value is "q".
     */
    std::string qAtomName {};

    /*!
     *  \brief The number of alternations between the propositions `p` and `q`.
     *  This value specifies how many times the trajectory should alternate between `p` and `q`.
     *  \note Default value is 1.
     */
    unsigned alternations {};
};