#!/usr/bin/env bash

LOG_FILE="./diff-log"

compare_outputs() {
    echo $3

    directory=${3%/*}
    filename=$(basename "$3")
    filename="${filename%.*}"
    
    for flag in "pic" "tikz" "pst" "eps" "svg" "xml"; do
        if [ "$flag" = "tikz" ]; then
            ext="tkz"
        else
            ext="$flag"
        fi

        FILE1="./1$filename.$ext"
        FILE2="./2$filename.$ext"

        gclcOutput=$("./$1" "$3" "-$flag")
        if [ -f "$directory/$filename.$ext" ] ; then
            mv "$directory/$filename.$ext" "$FILE1"
        else
            {
                echo "\`$3\`: $flag"
                echo "\`\`\`"
                echo "$gclcOutput"
                echo "\`\`\`"
                echo ""
            } >> "$LOG_FILE"
            continue
        fi

        gclcOutput=$("./$2" "$3" "-$flag")
        if [ -f "$directory/$filename.$ext" ] ; then
            mv "$directory/$filename.$ext" "$FILE2"
        else
            {
                echo "\`$3\`: $flag"
                echo "\`\`\`"
                echo "$gclcOutput"
                echo "\`\`\`"
                printf ""
            } >> "$LOG_FILE"
            continue
        fi

        difference=$(diff --suppress-common-lines "$FILE1" "$FILE2")

        if [ -n "$difference" ] ; then
            {
                echo "\`$3\`: $flag"
                echo "\`\`\` diff"
                echo "$difference"
                echo "\`\`\`"
                echo ""
            } >> "$LOG_FILE"
        fi

        rm "$FILE1" "$FILE2"
    done
}

if [ "$#" -ne 3 ]; then
    echo "Illegal number of parameters!"
    echo "Usage: ./compare_output.sh program1 program2 directory|file"
    exit 1
fi

if ! command -v diff &> /dev/null
then
    echo "Program diff is not available"
    exit 1
fi

if ! [[ -x "$1" ]] ; then
    echo "File '$1' is not executable or found"
    exit 1
fi

if ! [[ -x "$2" ]] ; then
    echo "File '$2' is not executable or found"
    exit 1
fi

touch "$LOG_FILE"

if [ -d "$3" ]; then
    find "$3" -maxdepth 1 -type f -name "*.gcl" | while read -r file; do
        compare_outputs "$1" "$2" "$file"
    done
else
    compare_outputs "$1" "$2" "$3"
fi
