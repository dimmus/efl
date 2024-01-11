#!/bin/sh -e
# make sure you run it from toplevel source dir
if test -d ./po && test -f ./scripts/update-potfile.sh; then
  find src/bin src/lib src/benchmarks src/edje_external src/examples src/modules src/generic src/static_libs src/tests -name '*.[ch]' | sort > ./po/POTFILES.in
  echo "File po/POTFILES.in updated succesfully"
else
  echo "Please run this from the toplevel source directory."
  exit 77
fi
