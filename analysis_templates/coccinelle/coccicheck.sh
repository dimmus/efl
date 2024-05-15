#!/bin/sh
set -eu

read -p "Type the path to the testing folder [src/bin]: " path
if [ -z $path ]; then
   path='src/lib'
else
   path=$path
fi

COCCI_FILES="
andconst.cocci
badzero.cocci
continue.cocci
free_stack.cocci
mutex.cocci
notand.cocci
notnull.cocci
null_ref.cocci
unused.cocci
use_after_iter.cocci
macros.cocci

cpu-reset.cocci
cpu_restore_state.cocci
err-bad-newline.cocci
error_propagate_null.cocci
error-use-after-free.cocci
errp-guard.cocci
exec_rw_const.cocci
inplace-byteswaps.cocci
memory-region-housekeeping.cocci
overflow_muldiv64.cocci
remove_muldiv64.cocci
return_directly.cocci
round.cocci
simplify_muldiv64.cocci
swap_muldiv64.cocci
tcg_gen_extract.cocci
timer-del-timer-free.cocci
typecast.cocci
"

HAS_ERROR=0
for f in $COCCI_FILES; do
   OPTIONS=""
   if [ "$f" = "macros.cocci" ]; then
      OPTIONS="--defined DIV_ROUND_UP --defined ROUND_UP --defined MIN --defined MAX"
   fi
   CMD="spatch --timeout 200 --max-width 80 --very-quiet --cocci-file scripts/coccinelle/$f --include-headers --dir $path $OPTIONS"
   OUT=$($CMD)
   echo "$CMD"
   if [ -n "$OUT" ]; then
      echo "$OUT"
      HAS_ERROR=1
   fi
done
exit $HAS_ERROR
