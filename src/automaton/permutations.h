#pragma once

#include "BackwardNFA.h"

void permutation(const BackwardNFA& backwardNfa, const std::unordered_map<int, int>& permutation)
{
    BackwardNFA backwardNfaPermutation { backwardNfa };
    spot::twa_graph_ptr twa { backwardNfaPermutation.m_backwardNfa };

}