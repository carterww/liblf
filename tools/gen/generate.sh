#!/bin/sh

set -e

INCLUDE_DIR="$1"

if [ ! -d "$INCLUDE_DIR" ]; then
  echo "Directory '$INCLUDE_DIR' does not exist."
  exit 1
fi

$INCLUDE_DIR/../tools/gen/lf_gen_gcc | clang-format -style=file > $INCLUDE_DIR/lf/gcc/lf_op_gcc.h
