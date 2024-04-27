#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <Efl_Shared.h>

#include "efl-test-runner.h"

TEST(crc_simple)
{
    unsigned int seed   = 0xffffffff, i, ret;
    const char  *data[] = { "Hello World",
                            "This is to test crc",
                            "4baAsru=A$r&e.-",
                            "=-.^Y@~Lp5e1)b^",
                            "J(uhX4)!&Q#2,jr" };

    unsigned int result[] = { 0x4a17b156,
                              0x738bec38,
                              0xcd56f3c6,
                              0x9732147f,
                              0x12c660a3 };
    const char  *s1       = "Hello ";
    const char  *s2       = "World";

    for (i = 0; i < sizeof(data) / sizeof(data[0]); ++i)
    {
        ret = eina_crc(data[i], strlen(data[i]), seed, EFL_TRUE);
        efl_assert_fail_if(ret != result[i]);
    }

    ret = eina_crc(s1, strlen(s1), 0xffffffff, EFL_TRUE);
    ret = eina_crc(s2, strlen(s2), ret, EFL_FALSE);
    efl_assert_fail_if(ret != result[0]);
}
