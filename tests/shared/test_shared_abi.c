#include <Efl_Shared.h>

#include "efl-test-runner.h"

TEST(alignof)
{
    assert(eina_mempool_alignof(6) == 8);
    assert((eina_mempool_alignof(10) & 0x7) == 0);
}
