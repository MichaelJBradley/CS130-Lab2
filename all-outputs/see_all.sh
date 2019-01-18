#!/bin/sh

IGNORE1="../grading-scheme.txt"
IGNORE2="../CMakeLists.txt"

for i in $( ls ../*.txt ); do
    if [[ "$i" != "$IGNORE1" && "$i" != "$IGNORE2" ]]; then
        .././ray_tracer -i $i
        cp output.png output-${i:3:2}.png
    fi
done


