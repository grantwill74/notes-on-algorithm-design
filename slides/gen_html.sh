#!/bin/bash

# this script requires that pandoc be installed and findable in PATH
pandoc -s "$1/$1.md" --mathjax -o "$1/$1.html"
