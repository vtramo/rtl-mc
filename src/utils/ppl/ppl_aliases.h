#pragma once

#ifndef PPL_ppl_hh
#include <ppl.hh>
#endif

using Poly = Parma_Polyhedra_Library::NNC_Polyhedron;
using PolyUniquePtr = std::unique_ptr<Poly>;
using PolyConstUniquePtr = std::unique_ptr<const Poly>;
using Powerset = Parma_Polyhedra_Library::Pointset_Powerset<Poly>;
using PowersetSharedPtr = std::shared_ptr<Powerset>;
using PowersetConstSharedPtr = std::shared_ptr<const Powerset>;
using PowersetUniquePtr = std::unique_ptr<Powerset>;
using PowersetConstUniquePtr = std::unique_ptr<const Powerset>;
namespace PPL = Parma_Polyhedra_Library;