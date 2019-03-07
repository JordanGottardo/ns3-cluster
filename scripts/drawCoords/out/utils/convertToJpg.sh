#!/bin/bash
for filename in ./*.pdf; do
	file=$(basename -- "$filename")
	file="${file%.*}"
	convert -density 300 -trim $filename -quality 80 "$file.jpg"
done