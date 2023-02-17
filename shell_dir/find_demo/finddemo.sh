#! /bin/bash

# grep -E "regex"               regex like: "^(.)*\.sh$"
# find path -regex "regex"      regex like: "^\(.\)*\.sh$"
# echo "xxx"|xargs cmd          like : cmd "xxx"          
# wc -l    get line num


regex_rule="^\(.\)*\.sh$"
data=$(find ../ -regex ${regex_rule} | xargs cat |wc -l)
echo ${data}

