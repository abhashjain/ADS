#!/bin/bash
rm *.bin assn_4 o1.txt o2.txt final.txt
gcc ajain28_assn_4.c -Wall -o assn_4 -g
./assn_4 index.bin 4 < input-01.txt > o1.txt
./assn_4 index.bin 4 < input-02.txt > o2.txt

echo "diff from output"
diff o1.txt output-01.txt > final.txt
diff o2.txt output-02.txt >> final.txt
cat final.txt
