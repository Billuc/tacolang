mkdir -p ./build
cp -R -u src/* build
bison -vd -o ./build/taco.tab.c ./build/taco.y
flex -v -o ./build/lex.yy.c ./build/taco.lex 2> ./build/lex.output
find ./build -name '*.c' | xargs gcc -o ./build/taco $*