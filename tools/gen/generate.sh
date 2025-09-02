#!/bin/sh

set -e

INCLUDE_DIR="$1"

if [ ! -d "$INCLUDE_DIR" ]; then
  echo "Directory '$INCLUDE_DIR' does not exist."
  exit 1
fi

GEN_DIR="$INCLUDE_DIR/../tools/gen"

$GEN_DIR/lf_gen_gcc op | clang-format -style=file > $INCLUDE_DIR/lf/gcc/lf_op_gcc.h
$GEN_DIR/lf_gen_x64 op | clang-format -style=file > $INCLUDE_DIR/lf/gcc/lf_op_x64.h
