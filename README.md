# mlrcs

## Prerequisites

C++ compiler with C++17 support (e.g., g++)

## Installation
simply run :
$>chmod +x install.sh
$>./install.sh

After compilation, the mlrcs binary will be generated in the current directory. You can run it using:
./mlrcs 
Alternatively, for global access, add the directory to your PATH or move the mlrcs binary to a directory in your PATH.

## Usage
mlrcs -h "path/to/input.blif" AND_CONSTRAINT OR_CONSTRAINT NOT_CONSTRAINT

## Example
mlrcs -h "aoi_benchmark/aoi_sample02.blif" 2 1 1

## NOTE
Here's a revised version of your text:

There will be outputs for the gate name, gate type, and various checks to determine whether the gate satisfies the conditions for the current cycle, among other details. If the constraints related to the input file are too restrictive, it may take a while to run. (An infinite loop is not possible.)