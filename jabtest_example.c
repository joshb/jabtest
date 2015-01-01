#include "jabtest.h"

static TestResult test1() { ASSERT(1 + 1 == 2); return SUCCESS; } /* will succeed */
static TestResult test2() { ASSERT(1 + 1 == 3); return SUCCESS; } /* will fail */
static TestResult test3() { ASSERT(1 + 2 == 3); return SUCCESS; } /* will succeed */

int main(int argc, const char *argv[])
{
    run_test(test1); run_test(test2); run_test(test3);
    print_test_summary();
    return 0;
}
