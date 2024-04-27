#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Efl_Shared.h>
#include <check.h>
#include "efl-test-runner.h"
#ifndef _WIN32
#  include <pwd.h>
#endif

TEST(test_vpath_valid)
{
    char test[PATH_MAX];

    efl_assert_str_eq(eina_vpath_resolve("/"), "/");
    efl_assert_str_eq(eina_vpath_resolve("./"), "./");
    efl_assert_str_eq(eina_vpath_resolve("..bla"), "..bla");
    efl_assert_str_eq(eina_vpath_resolve(".bla"), ".bla");

    snprintf(test, sizeof(test), "%s/", eina_environment_home_get());
    efl_assert_str_eq(eina_vpath_resolve("~/"), test);

    snprintf(test, sizeof(test), "%s/bla", eina_environment_home_get());
    efl_assert_str_eq(eina_vpath_resolve("(:home:)/bla"), test);
    efl_assert_str_eq(eina_vpath_resolve("${home}/bla"), test);
    efl_assert_str_eq(eina_vpath_resolve("/test/for/the/last/case"),
                      "/test/for/the/last/case");
}

TEST(test_vpath_invalid)
{
    EXPECT_ERROR_START;
    efl_assert_ptr_eq(eina_vpath_resolve("(:asdfasdfafasdf"), NULL);
    EXPECT_ERROR_END;
    EXPECT_ERROR_START;
    efl_assert_ptr_eq(eina_vpath_resolve("(:missing_slash:)"), NULL);
    EXPECT_ERROR_END;
    EXPECT_ERROR_START;
    efl_assert_ptr_eq(eina_vpath_resolve("(:"), NULL);
    EXPECT_ERROR_END;
    EXPECT_ERROR_START;
    efl_assert_ptr_eq(eina_vpath_resolve("(:home:)"), NULL);
    EXPECT_ERROR_END;
    EXPECT_ERROR_START;
    efl_assert_ptr_eq(eina_vpath_resolve("(:wrong_meta_key:)/"), NULL);
    EXPECT_ERROR_END;
    EXPECT_ERROR_START;
    efl_assert_ptr_eq(eina_vpath_resolve("${asdfasdfafasdf"), NULL);
    EXPECT_ERROR_END;
    EXPECT_ERROR_START;
    efl_assert_ptr_eq(eina_vpath_resolve("${missing_slash}"), NULL);
    EXPECT_ERROR_END;
    EXPECT_ERROR_START;
    efl_assert_ptr_eq(eina_vpath_resolve("${"), NULL);
    EXPECT_ERROR_END;
    EXPECT_ERROR_START;
    efl_assert_ptr_eq(eina_vpath_resolve("${home}"), NULL);
    EXPECT_ERROR_END;
    EXPECT_ERROR_START;
    efl_assert_ptr_eq(eina_vpath_resolve("${wrong_meta_key}/"), NULL);
    EXPECT_ERROR_END;
    EXPECT_ERROR_START;
    efl_assert_ptr_eq(eina_vpath_resolve("${home:)"), NULL);
    EXPECT_ERROR_END;
    EXPECT_ERROR_START;
    efl_assert_ptr_eq(eina_vpath_resolve("${wrong_meta_key:)/"), NULL);
    EXPECT_ERROR_END;
}

TEST(test_vpath_snprintf)
{
    char *string = "blablabla";
    int   x      = 1337;
    char  buf[PATH_MAX];
    char  cmp[PATH_MAX];

    eina_vpath_resolve_snprintf(buf, sizeof(buf), "(:home:)/%s/%d/", string, x);
    snprintf(cmp,
             sizeof(cmp),
             "%s/%s/%d/",
             eina_environment_home_get(),
             string,
             x);
    efl_assert_str_eq(buf, cmp);

    eina_vpath_resolve_snprintf(buf, sizeof(buf), "${home}/%s/%d/", string, x);
    snprintf(cmp,
             sizeof(cmp),
             "%s/%s/%d/",
             eina_environment_home_get(),
             string,
             x);
    efl_assert_str_eq(buf, cmp);
}

TEST(test_vpath_user)
{
#ifdef HAVE_GETPWENT
    char           buf[PATH_MAX];
    char           cmp[PATH_MAX];
    struct passwd *pwent;

    pwent = getpwuid(getuid());

    eina_vpath_resolve_snprintf(buf,
                                sizeof(buf),
                                "~%s/foo/bar/king/kong/",
                                pwent->pw_name);
    snprintf(cmp, sizeof(cmp), "%s/foo/bar/king/kong/", pwent->pw_dir);
    efl_assert_str_eq(buf, cmp);
#endif
}
