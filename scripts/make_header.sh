#!/bin/bash

set -euo pipefail

set +e
if [[ "${MESON_EXE_WRAPPER:-}" != "" ]]; then
  json="$($MESON_EXE_WRAPPER ./$1 -j)"
  status=$?
else
  json="$(./$1 -j)"
  status=$?
fi
set -e
if [[ -z "${json// }" ]]; then
  echo 'empty json'
  exit 1
fi
echo "$json" > $2
if [[ "$status" == "255" ]]; then
  exit 0
fi
exit $status
