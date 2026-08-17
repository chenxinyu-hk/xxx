#include "test.h"

TestContext g_ctx;

int main() {
    int passed = 0;
    int failed = 0;

    for (auto& it : TestRegister::TestManager()) {
        printf("========== %s ==========\n", it.first.c_str());

        for (TestFactory factory : it.second) {
            TestBase* test = factory();

            test->run();

            if (g_ctx.failed) {
                ++failed;
                printf("[ NOT OK ] %-*s %s:%d: %s\n", 25,
                        test->name(), g_ctx.file, g_ctx.line, g_ctx.msg);
                g_ctx.reset();
            } else {
                ++passed;
                printf("[     OK ] %s\n", test->name());
            }

            delete test;
        }

        printf("\n");
    }

    printf("Summary: %d passed, %d failed\n", passed, failed);
}
