#!/usr/bin/env bash

# make.sh
#
# Copyright (C) 2020-2021 Kristofer Berggren
# All rights reserved.
#
# See LICENSE for redistribution information.

# exiterr
exiterr()
{
  >&2 echo "${1}"
  exit 1
}

# process arguments
DEPS="0"
SRC="0"
BUILD="0"
TESTS="0"
DOC="0"
INSTALL="0"
case "${1%/}" in
  deps)
    DEPS="1"
    ;;

  src)
    SRC="1"
    ;;

  build)
    BUILD="1"
    ;;

  test*)
    BUILD="1"
    TESTS="1"
    ;;

  doc)
    BUILD="1"
    DOC="1"
    ;;

  install)
    BUILD="1"
    INSTALL="1"
    ;;

  all)
    DEPS="1"
    SRC="1"
    BUILD="1"
    TESTS="1"
    DOC="1"
    INSTALL="1"
    ;;

  *)
    echo "usage: make.sh <deps|src|build|tests|doc|install|all>"
    echo "  deps      - install project dependencies"
    echo "  src       - reformat source code"
    echo "  build     - perform build"
    echo "  tests     - perform build and run tests"
    echo "  doc       - perform build and generate documentation"
    echo "  install   - perform build and install"
    echo "  all       - perform all actions above"
    exit 1
    ;;
esac

# deps
if [[ "${DEPS}" == "1" ]]; then
  OS="$(uname)"
  if [ "${OS}" == "Linux" ]; then
    DISTRO="$(lsb_release -i | awk -F':\t' '{print $2}')"
    if [[ "${DISTRO}" == "Ubuntu" ]]; then
      true || exiterr "deps failed (linux), exiting."
    else
      exiterr "deps failed (unsupported linux distro ${DISTRO}), exiting."
    fi
  elif [ "${OS}" == "Darwin" ]; then
    true || exiterr "deps failed (mac), exiting."
  else
    exiterr "deps failed (unsupported os ${OS}), exiting."
  fi
fi

# src
if [[ "${SRC}" == "1" ]]; then
  if [[ -x "$(command -v uncrustify)" ]]; then
    TMPDIR=$(mktemp -d)
    for SRC in examples/*.cpp; do
      DST="${TMPDIR}/$(basename ${SRC})"
      printf "\n\n\n\n\n\n"  > ${DST}         # add 6 blank lines
      tail -n +8 ${SRC}     >> ${DST}         # skip header (first 7 lines) 
      head -7 ${SRC}         > ${DST}.header  # store header separately (first 7 lines)
    done
    
    uncrustify -c uncrustify.cfg --replace --no-backup src/rapidcsv.h tests/*.cpp tests/*.h ${TMPDIR}/*.cpp
    if [[ "${?}" != "0" ]]; then
      rm -rf ${TMPDIR}
      echo "src failed, exiting."
      exit 1
    fi

    for DST in examples/*.cpp; do
      SRC="${TMPDIR}/$(basename ${DST})"
      cat ${SRC}.header      > ${DST}.tmp
      cat ${SRC}            >> ${DST}.tmp

      cmp --silent ${DST} ${DST}.tmp
      if [[ "${?}" != "0" ]]; then
        cat ${DST}.tmp > ${DST}
      fi
      rm ${DST}.tmp
    done

    rm -rf ${TMPDIR}
  fi
fi

# build
if [[ "${BUILD}" == "1" ]]; then
  OS="$(uname)"
  MAKEARGS=""
  if [ "${OS}" == "Linux" ]; then
    MAKEARGS="-j$(nproc)"
  elif [ "${OS}" == "Darwin" ]; then
    MAKEARGS="-j$(sysctl -n hw.ncpu)"
  fi
  mkdir -p build-debug && cd build-debug && cmake -DRAPIDCSV_BUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug .. && make ${MAKEARGS} && cd .. && \
  mkdir -p build-release && cd build-release && cmake -DRAPIDCSV_BUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Release .. && make ${MAKEARGS} && cd .. || \
  exiterr "build failed, exiting."
fi

# tests
if [[ "${TESTS}" == "1" ]]; then
  OS="$(uname)"
  CTESTARGS=""
  if [ "${OS}" == "Linux" ]; then
    CTESTARGS="-j$(nproc)"
  elif [ "${OS}" == "Darwin" ]; then
    CTESTARGS="-j$(sysctl -n hw.ncpu)"
  fi
  cd build-debug && ctest --output-on-failure ${CTESTARGS} && cd .. && \
  cd build-release && ctest --verbose && cd .. || \
  exiterr "tests failed, exiting."
fi

# doc
if [[ "${DOC}" == "1" ]]; then
  if [[ -x "$(command -v doxygenmd)" ]]; then
    doxygenmd src doc || exiterr "doc failed, exiting."
  fi
fi

# install
if [[ "${INSTALL}" == "1" ]]; then
  OS="$(uname)"
  if [ "${OS}" == "Linux" ]; then
    cd build-release && sudo make install && cd .. || exiterr "install failed (linux), exiting."
  elif [ "${OS}" == "Darwin" ]; then
    cd build-release && make install && cd .. || exiterr "install failed (mac), exiting."
  else
    exiterr "install failed (unsupported os ${OS}), exiting."
  fi
fi

# exit
exit 0
