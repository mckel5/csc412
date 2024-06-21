#! /usr/bin/env bash

# Declare associative array
unset dictionary
declare -A dictionary

# Read dictionary
input=$(cat unix_dict.text)
IFS=$'\n'; dict_array=($input); unset IFS;

# Fill associative array
for word in ${dict_array[@]}; do
    dictionary[$word]=1
done

while IFS= read -r word; do
    # If word not in dictionary, print
    if [[ -z ${dictionary[$word]} ]]; then
        echo "$word"
    fi
done

# Include final word
if [[ -z ${dictionary[$word]} ]]; then
        echo -n "$word"
fi