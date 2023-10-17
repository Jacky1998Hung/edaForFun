#!/bin/bash

# Compile the project
make

# Check if compilation was successful
if [[ $? -ne 0 ]]; then
    echo "Compilation failed. Exiting."
    exit 1
fi

echo "Compilation successful. The executable 'mlrcs' has been generated in the current directory."
echo "You can run it using ./mlrcs or add the directory to your PATH for global access."
