#!/usr/bin/env bash

usage() {
  cat << EOF
Usage: $0 [semantics] [gap] [max_distance] [rtl_formula]

Parameters:
  semantics   : (optional) ['fin', 'inf', 'may', 'must'] (default 'fin')
  gap         : (optional) gap thickness (default 1)
  max_distance: (optional) how far the flow must move away from the origin (default 10)
  rtl_formula : (optional) RTL formula to use (default "t0 & G(t1) & F(p & F(q))")

Example:
  $0 "fin" 2 20 "t0 & F(p)"
  $0 "may" 5 15 "t0 & G(t1) & F(p & F(q))"
EOF
  exit 1
}

if [[ $# -gt 4 ]]; then
  usage
fi

build_polyhedral_system() {
    gap=$1
    distance=$2
    polyhedralSystem=$(cat << EOF
Inv ( { a >= 0 & b >= 0 } )
Flow { a + b >= -1 & a + b <= 3 & a >= -1 & a <= 1 & 10*b >= $distance & b <= 2 & t = 1 }
p { a >= b + $gap }
q { b >= a + $gap }
t0 { t = 0 }
t1 { t <= 10 }
EOF
    )
    echo "$polyhedralSystem"
}

semantics=${1:-"fin"}
gap=${2:-1}
max=${3:-10}
rtl_formula=${4:-"t0 & G(t1) & F(p & F(q))"}
distance=0
for ((distance = 0 ; distance <= max ; distance++)); do
  polyhedralSystem=$(build_polyhedral_system "$gap" "$distance")
  rtl-mc --semantics "${semantics}" -ss "${polyhedralSystem}" -fs "${rtl_formula}" --high || usage
done
