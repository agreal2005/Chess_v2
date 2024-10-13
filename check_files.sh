#!/bin/bash

# Directories
fens_dir="./fens"
evals_dir="./evals"

# Loop through each file in the fens directory
for file in "$fens_dir"/*; do
    # Get the base filename
    filename=$(basename "$file")
    
    # Check if the file exists in the evals directory
    if [[ ! -f "$evals_dir/$filename" ]]; then
        echo "Removing $file (no corresponding file in $evals_dir)"
        rm "$file" # Remove the file if it doesn't exist in evals
    else
        echo "$filename exists in $evals_dir"
    fi
done

