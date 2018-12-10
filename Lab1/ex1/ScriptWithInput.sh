#!/bin/bash
echo write in word you want to search for:
read varSearch
echo write in what file you want to search in:
read varSearchFile
echo write in which file you want to store the lines containing your word:
read varOutputFile
grep "\<$varSearch\>" $varSearchFile | tee $varOutputFile.txt | wc -l
