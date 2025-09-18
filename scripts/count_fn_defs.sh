#!/bin/bash

def_file=/home/andreilascu/Documents/Repos/InformationFlow/build/src/llvm_snippets.def
grep -Eo "\((.*)\)\[(.*)\]" $def_file | sort | uniq -c
