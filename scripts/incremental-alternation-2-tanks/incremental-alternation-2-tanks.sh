#!/usr/bin/env bash

usage() {
  cat << EOF
Usage: $0 [semantics] [max_alternations] [gap] [time]

Parameters:
  semantics        : (optional) ['fin', 'inf', 'may', 'must'] (default 'fin')
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
tanks=2
max_alternations=${2:-10}
gap=${3:-1}
time=${4:-10}

source ./../geogebra-path.sh

current_date=$(date --rfc-3339=seconds | sed 's/ /T/')
csv_file="${0::-3}-$semantics-gap=$gap-time=$time-($current_date).csv"
{
  echo -n "TOTAL_VARIABLES,ALTERNATIONS,EX_TIME_SECONDS,"

  # Denot
  echo -n "DENOT_IS_INCOMPLETE_RESULT,DENOT_TOT_ITERATIONS,DENOT_EX_TIME_SECONDS,"
  echo -n "DENOT_TOT_PATHS,DENOT_TOT_REDUNDANT_PATHS,DENOT_TOT_REACH_CALLS,DENOT_MAX_ITERATIONS,DENOT_RESULT,"

  # Automaton
  echo -n "TOTAL_STATES,TOTAL_INIT_STATES,TOTAL_ACCEPTING_STATES,TOTAL_EDGES,"
  echo -n "TOTAL_PATCHES,MAX_PATCHES,NFA_MAX_REC_DEPTH,"
  echo -n "TOTAL_SCC,AUTOMATON_EX_TIME_SECONDS,"

  # Translation
  echo -n "TRANSLATION_OPT_LEVEL,TRANSLATION_EX_TIME_SECONDS,TRANSLATION_TOTAL_STATES,TRANSLATION_TOTAL_INIT_STATES,"
  echo "TRANSLATION_TOTAL_EDGES,TRANSLATION_TOTAL_SCC"
} > "$csv_file"

stats_format=""
if [[ $semantics == "inf" || $semantics == "fin" ]]; then
  stats_format="%Dd[0],%Di[0],%Dx[0],%Dp[0],%Dpr[0],%Drc[0],%Dmi[0],%Dr[0],%As[0],%Ais[0],%Aas[0],%Ae[0],%Atp[0],%Amp[0],%ANr[0],%Ascc[0],%Ax[0],%ATo[0],%ATx[0],%ATs[0],%ATis[0],%ATe[0],%ATscc[0]"
else
  stats_format="%Dd[1],%Di[1],%Dx[1],%Dp[1],%Dpr[1],%Drc[1],%Dmi[1],%Dr[1],%As[1],%Ais[1],%Aas[1],%Ae[1],%Atp[1],%Amp[1],%ANr[1],%Ascc[1],%Ax[1],%ATo[1],%ATx[1],%ATs[1],%ATis[1],%ATe[1],%ATscc[1]"
fi

export DENOT_MAX=10000

pdfs=()

step=1
max_time=251
for ((alternations = 251 ; alternations <= 260 ; alternations=alternations+step)); do
  unique_filename="${semantics}_${tanks}_${alternations}_${gap}_${time}"

  if [[ $alternations -gt 1 ]]; then
    max_time=$((max_time+step))
  fi

  start=$(microtime)
  polyhedral_system=$(sys-gen gap -t "$tanks" -m "$max_time" -g "$gap")
  result="$((tanks + 1)),$alternations,"
  rtl_mc_result=$(rtl-mc \
    -ss "$polyhedral_system" \
    -fs "t0 & G(t1) & $(rtl-gen alternation -k $alternations)" \
    --semantics "$semantics" \
    --high \
    --stats "$stats_format" \
    || usage)
  end=$(microtime)
  execution_time=$(echo "$end - $start" | bc)
  result+="$execution_time,$rtl_mc_result"
  echo "$result" >> "$csv_file"

  if [[ $tanks == 2 ]]; then
      poly_result=$(echo "$rtl_mc_result" | cut -d',' -f"8")
      polyhedra_spec=$(cat << EOF
p { a >= b + $gap & b >= 0 } 0x800000
q { b >= a + $gap & a >= 0 } 0x0099CC
r $poly_result 0xCCFF66
EOF
      )
      echo "$polyhedra_spec" | poly-ggb -O "$unique_filename" -x "a" -y "b"
      save_geogebra_file_as_pdf "$unique_filename"
      rm -rf "$unique_filename.ggb"
      pdfs+=("$unique_filename.pdf")
  fi
done

pdf_result="$semantics-tanks=$tanks-alternations=1-$max_alternations-gap=$gap-t=$time"
qpdf --empty --pages "${pdfs[@]}" -- "$pdf_result.pdf" 2>/dev/null
rm -f "${pdfs[@]}"