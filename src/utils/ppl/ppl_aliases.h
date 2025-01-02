//
// Created by vincenzo on 19/12/24.
//

#ifndef PPL_ALIASES_H
#define PPL_ALIASES_H

#ifndef PPL_ppl_hh
#include <ppl.hh>
#endif

using Poly = Parma_Polyhedra_Library::NNC_Polyhedron;
using Powerset = Parma_Polyhedra_Library::Pointset_Powerset<Poly>;
namespace PPL = Parma_Polyhedra_Library;

#endif //PPL_ALIASES_H
