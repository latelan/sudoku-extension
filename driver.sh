#!/bin/sh

# 求解sample文件下所用输入
# 结果分别对应tmp下output_*.txt

for i in `seq 6`
do
	./driver ../sample/$i/input.txt
done 
