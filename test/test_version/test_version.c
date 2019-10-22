#include "../test.h"
#include <cleri/version.h>

static int test_version(void)
{
    test_start("version");

    _assert ( strlen(cleri_version()) > 5 );

    return test_end();
}

int main()
{
    return (
        test_version() ||
        0
    );
}
