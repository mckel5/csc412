#! /usr/bin/env bash

# Read from stdin
read -d '' input

# Split string on both spaces and newlines
input=${input//$'\n'/;}
input=${input// /;}
IFS=';'; input_array=($input); unset IFS;

length=${#input_array[@]}

# Print each word
for word in ${input_array[@]::length - 1}; do
    if [ $word == "" ]; then
        continue
    fi

    echo $word
done

# Print final word with no trailing newline
printf "%s" ${input_array[length - 1]}