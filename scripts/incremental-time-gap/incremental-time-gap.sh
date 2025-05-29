#!/usr/bin/env bash

usage() {
  cat << EOF
Usage: $0 [semantics] [max] [rtl_formula]

Parameters:
  semantics   : (optional) ['fin', 'inf', 'may', 'must'] (default 'fin')
  max_time    : (optional) max time (default 10)
  gap         : (optional) gap thickness (default 1)
  rtl_formula : (optional) RTL formula to use (default "t0 & G(t1) & F(p & F(q))")

Example:
  $0 "fin" 10 "t0 & F(p)"
  $0 "may" 30 "t0 & G(t1) & F(p & F(q))"
EOF
  exit 1
}

if [[ $# -gt 4 ]]; then
  usage
fi

build_polyhedral_system_gap() {
    time=$1
    gap=$2
    polyhedralSystem=$(cat << EOF
Inv ( { a >= 0 & b >= 0 } )
Flow { a + b >= -2 & a + b <= 2 & a >= -1 & a <= 1 & b >= -2 & b <= 2 & t = 1 }
p { a >= b + $gap }
q { b >= a + $gap }
t0 { t = 0 }
t1 { t <= $time }
EOF
    )
    echo "$polyhedralSystem"
}


source ./../geogebra-path.sh

pdfs=()
semantics=${1:-"fin"}
max_time=${2:-11}
gap=${3:-1}
rtl_formula=${4:-"t0 & G(t1) & F(p & F(q))"}
for ((time = 1 ; time <= max_time ; time++)); do
  polyhedralSystem=$(build_polyhedral_system_gap $time $gap)
  result=$(rtl-mc --semantics "${semantics}" -ss "${polyhedralSystem}" -fs "${rtl_formula}" --high || usage)
  polyhedraSpec=$(cat << EOF
p { a >= b + $gap & b >= 0 } 0x800000
q { b >= a + $gap & a >= 0 } 0x0099CC
r $result 0xCCFF66
EOF
  )

  geogebra_file="${0::-3}-gap-$semantics-$time"
  echo "$polyhedraSpec" | poly-ggb -O "$geogebra_file" -x "a" -y "b"
  save_geogebra_file_as_pdf "$geogebra_file"
#  rm -rf "$geogebra_file.ggb"
  pdfs+=("$geogebra_file.pdf")
done

qpdf --empty --pages "${pdfs[@]}" -- "${0::-3}-$semantics-gap=$gap-t=1-$max_time-($rtl_formula).pdf" 2>/dev/null
rm -f "${pdfs[@]}"
