#!/bin/sh

set -e

git config --global --add safe.directory /src

mkdir build_emscripten
cd build_emscripten
emcmake cmake .. -DCMAKE_BUILD_PLATFORM=emscripten -DCMAKE_BUILD_TYPE=Release -DOPTION_BUILD_WEBSITE_TOOLS="OFF" -DOPTION_ASAN="OFF" -DOPTION_BUILD_CODECHECK="OFF" -DCMAKE_EXE_LINKER_FLAGS="-sMODULARIZE -sEXPORTED_RUNTIME_METHODS=callMain -sEXPORT_ALL -sEXPORT_NAME=widelands" -DOPTION_BUILD_TESTS=OFF -DOPTION_BUILD_WEBSITE_TOOLS=OFF
grep -v REVDETECT-BROKEN VERSION
emmake make -k -j$(nproc) VERBOSE=1
