#! /usr/bin/env bash

### compile all c code in the current directory

c_files=($(find . -maxdepth 1 -name "*.c"))

# Check if any files of a type exist: https://stackoverflow.com/a/3856909
if [ ${#c_files[@]} -gt 0 ]; then
    # Loop through C files in current directory
    for file in ${c_files[@]}; do
        # Get source and header files necessary for compilation
        IFS=' '; input_files=($(gcc -MM $file)); unset IFS;
        # Drop ".o" file from command output
        input_files=(${input_files[@]:1})
        # Replace header files (".h") with source files (".c") -- hacky but functional
        input_files=(${input_files[@]//.h/.c})
        output_file=$(basename $file .c)

        gcc -Werror -Wall -o $output_file ${input_files[@]}
    done
else
    echo "Oops, we found no C code in the directory: $(pwd)"
    exit 1
fi