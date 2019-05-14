#!/usr/bin/env bash
# HELP SECTION
if [ "$1" == "-h" ]; then
    echo "Usage: `basename $0` new_version"
    exit 0
fi

if [ "$1" == "--help" ]; then
    echo "Usage: `basename $0` new_version"
    exit 0
fi

# EXIT IF NO NEW VERSION WAS GIVEN
if [ "$1" = "" ]; then
    echo "Usage: `basename $0` new_version"
    exit 0
fi


SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
ROOT_DIR="$SCRIPT_DIR/.."
CURRENT_VERSION=$(git describe --abbrev=0 | sed 's/v//g')
NEW_VERSION="$1"


echo "Current version: $CURRENT_VERSION"
echo "New version: $NEW_VERSION"

# sed -i "s/$CURRENT_VERSION/$NEW_VERSION/" $ROOT_DIR/modEQ.jucer
# sed -i "s/$CURRENT_VERSION/$NEW_VERSION/" $ROOT_DIR/Doxyfile.in
# sed -i "s/$CURRENT_VERSION/$NEW_VERSION/" $ROOT_DIR/appveyor.yml
# sed -i "s/$CURRENT_VERSION/$NEW_VERSION/" $ROOT_DIR/test.txt