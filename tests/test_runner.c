#include "unity.h"
#include "unity_internals.h"

extern void run_app_tests(void);

int main(void) {
    UNITY_BEGIN();
    run_app_tests();
    return UNITY_END();
}