#!/usr/bin/env bash

usage() {
  cat << EOF
Usage: $0 [semantics] [max_tanks] [max_alternations]

Parameters:
  semantics        : (optional) ['fin', 'inf', 'may', 'must'] (default 'fin')
  max_tanks        : (optional) max tanks (default 10)
  max_alternations : (optional) max alternations between p & q (default 10)
  gap              : (optional) gap thickness (default 1)
  time             : (optional) max time (default 10)
EOF
  exit 1
}

if [[ $# -gt 4 ]]; then
  usage
fi

microtime()
{
    python3 -c 'import time; print(time.time())'
}

semantics=${1:-"fin"}
max_tanks=${2:-10}
max_alternations=${3:-10}
gap=${4:-1}
time=${5:-10}

source ./../geogebra-path.sh

csv_file="${0::-3}-$semantics-gap=$gap-time=$time.csv"
echo "TANKS,ALTERNATIONS,EXECUTION_TIME_SECONDS" > "$csv_file"
for ((tanks = 2 ; tanks <= max_tanks ; tanks++)); do

  pdfs=()

  for ((alternations = 1 ; alternations <= max_alternations ; alternations++)); do
    unique_filename="${semantics}_${tanks}_${alternations}_${gap}_${time}"

    start=$(microtime)
    result=$(sys-gen gap -t "$tanks" -m "$time" -g "$gap" | rtl-mc -fs "t0 & G(t1) & $(rtl-gen alternation -k $alternations)" --semantics "$semantics" --high || usage)
    end=$(microtime)
    execution_time=$(echo "$end - $start" | bc)
    echo "$tanks,$alternations,$execution_time" >> "$csv_file"

    if [[ $tanks == 2 ]]; then
        polyhedra_spec=$(cat << EOF
p { a >= b + $gap & b >= 0 } 0x800000
q { b >= a + $gap & a >= 0 } 0x0099CC
r $result 0xCCFF66
EOF
        )
        echo "$polyhedra_spec" | poly-ggb -O "$unique_filename" -x "a" -y "b"
        save_geogebra_file_as_pdf "$unique_filename"
        rm -rf "$unique_filename.ggb"
        pdfs+=("$unique_filename.pdf")
    fi
  done

  if [[ $tanks == 2 ]]; then
    pdf_result="$semantics-tanks=$tanks-alternations=1-$max_alternations-gap=$gap-t=$time"
    qpdf --empty --pages "${pdfs[@]}" -- "$pdf_result.pdf" 2>/dev/null
    rm -f "${pdfs[@]}"
  fi
done