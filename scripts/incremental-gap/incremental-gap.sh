#!/usr/bin/env bash

usage() {
  cat << EOF
Usage: $0 [semantics] [max] [rtl_formula]

Parameters:
  semantics   : (optional) ['fin', 'inf', 'may', 'must'] (default 'fin')
  max         : (optional) max gap thickness (default 10)
  rtl_formula : (optional) RTL formula to use (default "t0 & G(t1) & F(p & F(q))")

Example:
  $0 "fin" 10 "t0 & F(p)"
  $0 "may" 5  "t0 & G(t1) & F(p & F(q))"
EOF
  exit 1
}

if [[ $# -gt 4 ]]; then
  usage
fi

build_polyhedral_system_gap() {
    gap=$1
    polyhedralSystem=$(cat << EOF
Inv ( { a >= 0 & b >= 0 } )
Flow { a + b >= -2 & a + b <= 2 & a >= -1 & a <= 1 & b >= -2 & b <= 2 & t = 1 }
p { a >= b + $gap }
q { b >= a + $gap }
t0 { t = 0 }
t1 { t <= 10 }
EOF
    )
    echo "$polyhedralSystem"
}


source ./../geogebra-path.sh

flow="f { a + b >= -2 & a + b <= 2 & a >= -1 & a <= 1 & b >= -2 & b <= 2 } 0x800000"
echo "$flow" | poly-ggb -O "flow" -x "a" -y "b" --euclidean-view 962 461 132 132
save_geogebra_file_as_pdf "flow"

pdfs=()
semantics=${1:-"fin"}
max=${2:-11}
rtl_formula=${3:-"t0 & G(t1) & F(p & F(q))"}
for ((gap = 1 ; gap <= max ; gap++)); do
  polyhedralSystem=$(build_polyhedral_system_gap $gap)
  result=$(rtl-mc --semantics "${semantics}" -ss "${polyhedralSystem}" -fs "${rtl_formula}" --high && usage)
  polyhedraSpec=$(cat << EOF
p { a >= b + $gap & b >= 0 } 0x800000
q { b >= a + $gap & a >= 0 } 0x0099CC
r $result 0xCCFF66
EOF
  )

  geogebra_file="${0::-3}-gap-$semantics-$gap"
  echo "$polyhedraSpec" | poly-ggb -O "$geogebra_file" -x "a" -y "b" --euclidean-view 77 828 30 30
  save_geogebra_file_as_pdf "$geogebra_file"
  pdfs+=("$geogebra_file.pdf")
done

qpdf --empty --pages "${pdfs[@]}" -- "${0::-3}-$semantics-1-$max-($rtl_formula).pdf" 2>/dev/null
rm -f "${pdfs[@]}"
