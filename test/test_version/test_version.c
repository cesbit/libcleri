#include "../test.h"
#include <cleri/version.h>

static int test_version(void)
{
    test_start("version");

    _assert ( strcmp(cleri_version(), "1.0.0") == 0 );

    return test_end();
}

int main()
{
    return (
        test_version() ||
        0
    );
}
