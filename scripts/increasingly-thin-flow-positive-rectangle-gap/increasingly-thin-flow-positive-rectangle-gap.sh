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

source ./../geogebra-path.sh

pdfs=()
semantics=${1:-"fin"}
gap=${2:-1}
max=${3:-10}
rtl_formula=${4:-"t0 & G(t1) & F(p & F(q))"}

for ((distance = 0 ; distance <= max ; distance++)); do
  polyhedralSystem=$(build_polyhedral_system "$gap" "$distance")
  result=$(rtl-mc --semantics "${semantics}" -ss "${polyhedralSystem}" -fs "${rtl_formula}" --high && usage)
  polyhedraSpec=$(cat << EOF
p { a >= b + $gap & b >= 0 } 0x800000
q { b >= a + $gap & a >= 0 } 0x0099CC
r $result 0xCCFF66
EOF
  )

  flow="f { a + b >= -1 & a + b <= 3 & a >= -1 & a <= 1 & 10*b >= $distance & b <= 2 } 0x800000"
  flow_geogebra_file="flow-$distance"
  echo "$flow" | poly-ggb -O "$flow_geogebra_file" -x "a" -y "b" --euclidean-view 962 461 60 60
  save_geogebra_file_as_pdf "$flow_geogebra_file"
  pdfs+=("$flow_geogebra_file.pdf")

  geogebra_file="${0::-3}-distance-$semantics-$distance"
  echo "$polyhedraSpec" | poly-ggb -O "$geogebra_file" -x "a" -y "b"  --euclidean-view 77 828 30 30
  save_geogebra_file_as_pdf "$geogebra_file"
  pdfs+=("$geogebra_file.pdf")
done

qpdf --empty --pages "${pdfs[@]}" -- "${0::-3}-$semantics-$gap-$max-($rtl_formula).pdf" 2>/dev/null
rm -f "${pdfs[@]}"