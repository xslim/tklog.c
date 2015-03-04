#!/bin/sh

TPL_NAME="tklog"

if [ -z "${1}" ]; then
  echo "Provide new lib prefix, Ex: ady"
  exit 0;
fi

NEW_NAME="${1}log"

echo "Renaming ${TPL_NAME} -> ${NEW_NAME}"

#mv "${TPL_NAME}.h" "${NEW_NAME}.h"
#mv "${TPL_NAME}.h" "${NEW_NAME}.h"

rm -rf "${NEW_NAME}.h"
rm -rf "${NEW_NAME}.c"

sed "s/${TPL_NAME}/${NEW_NAME}/g" "${TPL_NAME}.h" > "${NEW_NAME}.h" 
sed "s/${TPL_NAME}/${NEW_NAME}/g" "${TPL_NAME}.c" > "${NEW_NAME}.c" 

