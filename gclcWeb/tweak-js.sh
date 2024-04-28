#!/usr/bin/env bash

if [ $# -ne 2 ]; then
  echo "usage: $0 <input> <output>" >&2
  exit 1
fi

# fail on error
set -e

echo '/* eslint-disable */' >$2
sed 's/\(if (typeof exports === .object. && typeof module === .object.\)).*/export { createModule };\/*/' $1 >>$2
echo '*/' >>$2
