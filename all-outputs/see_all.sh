#!/bin/sh

IGNORE1="../grading-scheme.txt"
IGNORE2="../CMakeLists.txt"
OPEN_FLAG="-o"
CLEAR_FLAG="-c"

if [[ "$1" != "" && "$1" != "$OPEN_FLAG" &&  "$1" != "$CLEAR_FLAG" ]]; then
    echo "Invalid flag.\nUsage: ./see_all { -o | -c }"

elif [[ "$1" = "$CLEAR_FLAG" ]]; then
    echo "Removing output images..."
    rm output*

else
    echo "Rendering output images..."

    for i in $( ls ../*.txt ); do
        if [[ "$i" != "$IGNORE1" && "$i" != "$IGNORE2" ]]; then
            .././ray_tracer -i $i
            echo $i
            cp output.png output-${i:3:2}.png
        fi
    done

    if [[ "$1" = "$OPEN_FLAG" ]]; then
        echo "Opening..."
        open output-*
    fi
fi
