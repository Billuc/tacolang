#!/bin/sh

# Create the build directory if it doesn't exist
mkdir -p ./build

# Generate the parser C files from the source files
bison -vd -o ./build/taco.tab.c ./src/taco.y
echo "Generated ./build/taco.tab.c from ./src/taco.y"
 
# Generate the lexer C files from the source files
flex -v -o ./build/lex.yy.c ./src/taco.lex 2> ./build/lex.output
echo "Generated ./build/lex.yy.c from ./src/taco.lex"

# Compile each C file from the build folder into .o files
find ./build -name '*.c' | while read -r file; do
    gcc -g -c "$file" -o "${file}.o" -Isrc
    echo "Compiled $file to ${file}.o"
done

# Compile each C file from the src folder into .o files in the build folder
find ./src -name '*.c' | while read -r file; do
    output_file="./build/$(basename "$file").o"
    gcc -g -c "$file" -o "$output_file" -Isrc
    echo "Compiled $file to $output_file"
done

# Link all .o files in the build folder to create the final executable
gcc -o ./build/taco $(find ./build -name '*.o')
echo "Linked all object files to create ./build/taco"