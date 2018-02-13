#!/bin/bash
rm final.txt
rm student.db;rm index.bin;rm avail.bin
gcc assign_2.c -o a -Wall -g
./a  --first-fit student.db < input-01.txt > o.txt
diff o.txt output-01-first.txt >> final.txt
./a  --first-fit student.db < input-02.txt > o.txt
diff o.txt output-02-first.txt >> final.txt

rm student.db;rm index.bin;rm avail.bin
echo "Best test case" >>final.txt
./a  --best-fit student.db < input-01.txt > o.txt
diff o.txt output-01-best.txt >> final.txt
./a  --best-fit student.db < input-02.txt > o.txt
diff o.txt output-02-best.txt >> final.txt

rm student.db;rm index.bin;rm avail.bin
echo "worst test case" >>final.txt
./a  --worst-fit student.db < input-01.txt > o.txt
diff o.txt output-01-worst.txt >> final.txt
./a  --worst-fit student.db < input-02.txt > o.txt
diff o.txt output-02-worst.txt >> final.txt
rm o.txt
cat final.txt
