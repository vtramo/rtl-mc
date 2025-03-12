#pragma once

enum class Semantics {
    fin,
    inf,
    may,
    must
};

inline std::optional<Semantics> toSemantics(const std::string_view semantics)
{
    if (semantics == "fin") return Semantics::fin;
    if (semantics == "inf") return Semantics::inf;
    if (semantics == "may") return Semantics::may;
    if (semantics == "must") return Semantics::must;
    return {};
}
