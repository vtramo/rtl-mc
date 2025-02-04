#!/bin/bash

export PATH=/home/vincenzo/project/rtl-mc/buildDir/src:$PATH

# Default values for k and t
default_k=1
default_t=10

# Parse command line arguments
if [[ $# -lt 2 ]]; then
    echo "Usage: $0 <totIterations> <--gap|--nogap> [k] [t] [output_file]"
    exit 1
fi

totIterations=$1  # Total iterations
mode=$2           # Mode: --gap or --nogap
k=${3:-$default_k}  # Default k value
t=${4:-$default_t}  # Default t value
output_file=${5:-"output.csv"}  # Output file for the data

# Validate mode
if [[ "$mode" != "--gap" && "$mode" != "--nogap" ]]; then
    echo "Error: Invalid mode. Use --gap or --nogap."
    exit 1
fi

# Write the header to the CSV file
echo "Iteration,k,t,%Pta,%Psp,%rta,%rl,%dta,%dl,%dx,%Ata,%Ats,%Ate,%Ato,%Atx,%Afx,%Afs,%Afe,%Aff,%Afox,%Afos,%Afoe,%Afof,%Abx,%Abs,%Abe,%Abf,%Abi,%Abc,%Abm,%Dt,%Dx" > $output_file

# Loop for the total number of iterations
for ((i=1; i<=totIterations; i++))
do
    # Run the rtl-mc command with a timeout of 30 minutes (18000 seconds) and capture the output
    output=$(timeout 18000 rtl-mc --high "$mode" "$k" "$t" -s "%Pta %Psp %rta %rl %dta %dl %dx %Ata %Ats %Ate %Ato %Atx %Afx %Afs %Afe %Aff %Afox %Afos %Afoe %Afof %Abx %Abs %Abe %Abf %Abi %Abc %Abm %Dt %Dx")

    # Check if the command timed out
    if [[ $? -eq 124 ]]; then
        echo "Iteration $i timed out after 30 minutes."
        exit 124
    fi

    # Extract the values for each placeholder from the output
    Pta=$(echo "$output" | awk '{print $1}')
    Psp=$(echo "$output" | awk '{print $2}')
    rta=$(echo "$output" | awk '{print $3}')
    rl=$(echo "$output" | awk '{print $4}')
    dta=$(echo "$output" | awk '{print $5}')
    dl=$(echo "$output" | awk '{print $6}')
    dx=$(echo "$output" | awk '{print $7}')
    Ata=$(echo "$output" | awk '{print $8}')
    Ats=$(echo "$output" | awk '{print $9}')
    Ate=$(echo "$output" | awk '{print $10}')
    Ato=$(echo "$output" | awk '{print $11}')
    Atx=$(echo "$output" | awk '{print $12}')
    Afx=$(echo "$output" | awk '{print $13}')
    Afs=$(echo "$output" | awk '{print $14}')
    Afe=$(echo "$output" | awk '{print $15}')
    Aff=$(echo "$output" | awk '{print $16}')
    Afox=$(echo "$output" | awk '{print $17}')
    Afos=$(echo "$output" | awk '{print $18}')
    Afoe=$(echo "$output" | awk '{print $19}')
    Afof=$(echo "$output" | awk '{print $20}')
    Abx=$(echo "$output" | awk '{print $21}')
    Abs=$(echo "$output" | awk '{print $22}')
    Abe=$(echo "$output" | awk '{print $23}')
    Abf=$(echo "$output" | awk '{print $24}')
    Abi=$(echo "$output" | awk '{print $25}')
    Abc=$(echo "$output" | awk '{print $26}')
    Abm=$(echo "$output" | awk '{print $27}')
    Dt=$(echo "$output" | awk '{print $28}')
    Dx=$(echo "$output" | awk '{print $29}')

    # Write the data in CSV format
    echo "$i,$k,$t,$Pta,$Psp,$rta,$rl,$dta,$dl,$dx,$Ata,$Ats,$Ate,$Ato,$Atx,$Afx,$Afs,$Afe,$Aff,$Afox,$Afos,$Afoe,$Afof,$Abx,$Abs,$Abe,$Abf,$Abi,$Abc,$Abm,$Dt,$Dx" >> $output_file

    echo "[$mode] k=$k t=$t completed!"

    # Increment the parameters k and t
    k=$((k + 1))

    # Every 10 iterations, also increment t by 10
    if ((i % 10 == 0)); then
        t=$((t + 10))
    fi
done

echo "Script completed! The results have been written to $output_file."
