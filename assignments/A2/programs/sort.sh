#! /usr/bin/env bash

function partition {
    local low=$(( $1 - 1 ))
    local high=$(( $2 + 1 ))
    
    # Choose pivot
    local pivot_point=$(( ($low + $high) / 2 ))
    local pivot=${array[$pivot_point]}

    while [ true ]; do
        # Find leftmost element greater than pivot
        ((low++))
        while [[ ${array[$low]} < $pivot ]]; do
            ((low++))
        done

        # Find rightmost element less than pivot
        ((high--))
        while [[ ${array[$high]} > $pivot ]]; do
            ((high--))
        done

        # If low and high indices cross over, no swap necessary
        if (( $low >= $high )); then
            part=$high
            return 0
        fi

        # Swap elements
        swap $low $high
    done
}

function quicksort {
    local low=$1
    local high=$2

    if (( $low < $high )); then
        # Partition array
        partition $low $high
        # Sort left side
        quicksort $low $part
        # Sort right side
        quicksort $(($part + 1)) $high
    fi
}

function swap {
    local tmp=${array[$1]}
    array[$1]=${array[$2]}
    array[$2]=$tmp
}

# Return values from partition() stored here
part=-1

# Read from stdin
read -d '' input
IFS=$'\n'; array=($input); unset IFS;
length=${#array[@]}

# Sort
quicksort 0 $(( $length - 1 ))

# Print to stdout
printf "%s\n" ${array[@]::length - 1}
printf "%s" ${array[$length - 1]}