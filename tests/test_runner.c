#include "unity.h"
#include "unity_internals.h"

extern void run_app_tests(void);
extern void run_asset_tests(void);
extern void run_entity_tests(void);

int main(void) {
    UNITY_BEGIN();
    run_app_tests();
    run_asset_tests();
    run_entity_tests();
    return UNITY_END();
}