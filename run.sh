#!/usr/bin/env bash

BUILD="0"
TESTS="0"
EXAMPLES="0"
DOC="0"
SRC="0"
ALL="0"
case "${1%/}" in
  build)
    BUILD="1"
    ;;

  tests)
    TESTS="1"
    ;;

  examples)
    EXAMPLES="1"
    ;;

  doc)
    DOC="1"
    ;;

  src)
    SRC="1"
    ;;

  all)
    BUILD="1"
    TESTS="1"
    EXAMPLES="1"
    DOC="1"
    SRC="1"
    TEST="1"
    ;;

  *)
    echo "usage: run.sh <build|test|examples|gendoc|format|all>"
    echo "  build     - build tests and examples"
    echo "  tests     - perform build and run tests"
    echo "  examples  - run examples"
    echo "  doc       - generate/update API documentation in doc/"
    echo "  src       - reformat source code files using uncrustify"
    echo "  all       - performs all actions above"
    exit 1
    ;;
esac

if [[ "${BUILD}" == "1" ]] || [[ "${TESTS}" == "1" ]]; then
    mkdir -p build && cd build && cmake .. && make
    if [[ "${?}" != "0" ]]; then
        echo "build failed, exiting."
        exit 1
    fi
    cd - > /dev/null
fi

if [[ "${TESTS}" == "1" ]]; then
    cd build && ctest -C unit --output-on-failure && ctest -C perf --verbose
    if [[ "${?}" != "0" ]]; then
        echo "tests failed, exiting."
        exit 1
    fi
    cd - > /dev/null
fi

if [[ "${EXAMPLES}" == "1" ]]; then
    for EXAMPLE in examples/*.cpp; do
      ./${EXAMPLE}
      if [[ "${?}" != "0" ]]; then
          echo "${EXAMPLE} failed, exiting."
          exit 1
      fi
    done
fi

if [[ "${DOC}" == "1" ]]; then
    doxyman2md src doc
    if [[ "${?}" != "0" ]]; then
        echo "doc failed, exiting."
        exit 1
    fi
fi

if [[ "${SRC}" == "1" ]]; then
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

exit 0
