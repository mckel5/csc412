#! /usr/bin/env bash

word=""
previous=""

while IFS= read -r word; do
    # Print word iff not equal to previous word
    if [[ $word != $previous ]]; then
        echo "$word"
    fi

    # Save word for next interation
    previous=$word
done

# "read" skips the final word, so we check it here
if [[ $word != $previous ]]; then
        echo -n "$word"
fi