#!/bin/sh

REPO="xslim/tklog.c"
RAW_URL="https://github.com/${REPO}/raw/master"
TPL_NAME="tklog"


if [ -z "${1}" ]; then
  echo "No prefix specified"
  exit 0;
fi

NEW_NAME="${1}log"
CURR_DIR=`pwd`
OUT_DIR=${2}

if [ -z "${CURR_DIR}" ]; then
  echo "Can't do pwd"
  exit 0;
fi

if [ -z "${OUT_DIR}" ]; then
  echo "No path specified, using current: ${CURR_DIR}"
  OUT_DIR="${CURR_DIR}"
fi

cd "${OUT_DIR}"

echo "Deleating previous version"
rm -rf "${NEW_NAME}.h"
rm -rf "${NEW_NAME}.c"

curl -sLO "${RAW_URL}/${TPL_NAME}.h"
curl -sLO "${RAW_URL}/${TPL_NAME}.c"

sed "s/${TPL_NAME}/${NEW_NAME}/g" "${TPL_NAME}.h" > "${NEW_NAME}.h" 
sed "s/${TPL_NAME}/${NEW_NAME}/g" "${TPL_NAME}.c" > "${NEW_NAME}.c"

rm -rf "${TPL_NAME}.h"
rm -rf "${TPL_NAME}.c"

cd "${CURR_DIR}"
