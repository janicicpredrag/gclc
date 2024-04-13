#!/bin/bash

LOG_FILE="./proof-log"

compare_proofs() {
    echo $3

    directory=${3%/*}
    filename=$(basename "$3")
    filename="${filename%.*}"
    
    for flag in "a" "w" "g"; do
        gclcOutput=$("./$1" "$3" "-$flag")
        if [ -f "$directory/${filename}_proof.tex" ] && [ -f "$directory/$filename.pic" ]; then
            mv "$directory/${filename}_proof.tex" "1.tex"
            mv "$directory/$filename.pic" "1.pic"
        else
            echo "  $flag: ERROR"
            { 
                echo "$3: $flag"
                echo "\`\`\`"
                echo "$gclcOutput"
                echo "\`\`\`"
                echo ""
            } >> "$LOG_FILE"
            continue
        fi

        gclcOutput=$("./$2" "$3" "-$flag")
        if [ -f "$directory/${filename}_proof.tex" ] && [ -f "$directory/$filename.pic" ]; then
            mv "$directory/${filename}_proof.tex" "2.tex"
            mv "$directory/$filename.pic" "2.pic"
        else
            echo "  $flag: ERROR"
            { 
                echo "$3: $flag"
                echo "\`\`\`"
                echo "$gclcOutput"
                echo "\`\`\`"
                 echo ""
            } >> "$LOG_FILE"
            continue
        fi

        difference=$(diff --suppress-common-lines "1.tex" "2.tex")
        if [ -n "$difference" ] ; then
            { 
                echo "$3: $flag tex"
                echo "\`\`\`"
                echo "$difference"
                echo "\`\`\`"
                echo ""
            } >> "$LOG_FILE"
        fi

        difference=$(diff --suppress-common-lines "1.pic" "2.pic")
        if [ -n "$difference" ] ; then
            { 
                echo "$3: $flag pic"
                echo "\`\`\`"
                echo "$difference"
                echo "\`\`\`"
                echo ""
            } >> "$LOG_FILE"
        fi

        rm "1.tex" "1.pic" "2.tex" "2.pic"
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

echo "" > "$LOG_FILE"

if [ -d "$3" ]; then
    find "$3" -maxdepth 1 -type f -name "*.gcl" | while read -r file; do
        compare_proofs "$1" "$2" "$file"
    done
else
    compare_proofs "$1" "$2" "$3"
fi
