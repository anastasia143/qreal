#!/bin/bash
# Usage: build-qreal-db.sh <path-to-qt> <path-to-qt-installer-framework> [tag]. See build-installer.sh for more info.

set -o nounset
set -o errexit

cd "$(dirname "$0")"

export QREAL_BUILD_TAG=${3:-}

grep -q "darwin" <<< $OSTYPE && export PRODUCT_DISPLAYED_NAME="QRealDB" || :
EXCLUDE=
chmod +x $PWD/build-installer.sh && $PWD/build-installer.sh $1 $2 qreal-db $EXCLUDE