#include "unity.h"
#include "entity.h"
#include "asset.h"
#include "errors.h"
#include "log.h"
#include <SDL3/SDL.h>

void test_RE_add_entity_basic(void) {
    reset_entities();
    int id = RE_add_entity(5, 10, NPC);
    TEST_ASSERT_EQUAL(0, id);
    
    entity_t* entities = get_entities();
    TEST_ASSERT_EQUAL(5, entities[0].x);
    TEST_ASSERT_EQUAL(10, entities[0].y);
    TEST_ASSERT_EQUAL(NPC, entities[0].beh);
    TEST_ASSERT_EQUAL(1, entities[0].depth);
}

void test_RE_add_entity_max_limit(void) {
    reset_entities();
    for (int i = 0; i < MAX_ENTITIES_NUM; i++) {
        int id = RE_add_entity(0, 0, NPC);
        TEST_ASSERT_EQUAL(i, id);
    }
    
    int id = RE_add_entity(0, 0, NPC);
    TEST_ASSERT_EQUAL(-1, id);
}

void test_RE_add_entity_player_unique(void) {
    reset_entities();
    int player_id = RE_add_entity(0, 0, PLAYER);
    TEST_ASSERT_EQUAL(0, player_id);
    
    int second_player = RE_add_entity(1, 1, PLAYER);
    TEST_ASSERT_EQUAL(-1, second_player);
}

void test_RE_add_entity_multiple_npc(void) {
    reset_entities();
    int npc1 = RE_add_entity(1, 1, NPC);
    int npc2 = RE_add_entity(2, 2, NPC);
    int npc3 = RE_add_entity(3, 3, FOLLOW);
    
    TEST_ASSERT_EQUAL(0, npc1);
    TEST_ASSERT_EQUAL(1, npc2);
    TEST_ASSERT_EQUAL(2, npc3);
    TEST_ASSERT_EQUAL(3, get_entities_num());
}

void test_RE_add_entity_negative_coords(void) {
    reset_entities();
    int id = RE_add_entity(-5, -10, NPC);
    TEST_ASSERT_EQUAL(0, id);
    
    entity_t* entities = get_entities();
    TEST_ASSERT_EQUAL(-5, entities[0].x);
    TEST_ASSERT_EQUAL(-10, entities[0].y);
}

void test_RE_add_entity_large_coords(void) {
    reset_entities();
    int id = RE_add_entity(1000, 2000, FOLLOW);
    TEST_ASSERT_EQUAL(0, id);
    
    entity_t* entities = get_entities();
    TEST_ASSERT_EQUAL(1000, entities[0].x);
    TEST_ASSERT_EQUAL(2000, entities[0].y);
}

void test_RE_add_entity_mixed_behaviours(void) {
    reset_entities();
    int player = RE_add_entity(0, 0, PLAYER);
    int npc = RE_add_entity(1, 1, NPC);
    int follow = RE_add_entity(2, 2, FOLLOW);
    
    TEST_ASSERT_EQUAL(0, player);
    TEST_ASSERT_EQUAL(1, npc);
    TEST_ASSERT_EQUAL(2, follow);
    
    entity_t* entities = get_entities();
    TEST_ASSERT_EQUAL(PLAYER, entities[0].beh);
    TEST_ASSERT_EQUAL(NPC, entities[1].beh);
    TEST_ASSERT_EQUAL(FOLLOW, entities[2].beh);
}

void test_get_entities(void) {
    reset_entities();
    RE_add_entity(1, 2, NPC);
    RE_add_entity(3, 4, FOLLOW);
    
    entity_t* entities_ptr = get_entities();
    TEST_ASSERT_NOT_NULL(entities_ptr);
    TEST_ASSERT_EQUAL(1, entities_ptr[0].x);
    TEST_ASSERT_EQUAL(2, entities_ptr[0].y);
    TEST_ASSERT_EQUAL(3, entities_ptr[1].x);
    TEST_ASSERT_EQUAL(4, entities_ptr[1].y);
}

void test_get_entities_num(void) {
    reset_entities();
    TEST_ASSERT_EQUAL(0, get_entities_num());
    
    RE_add_entity(1, 1, NPC);
    TEST_ASSERT_EQUAL(1, get_entities_num());
    
    RE_add_entity(2, 2, FOLLOW);
    TEST_ASSERT_EQUAL(2, get_entities_num());
}

void test_get_entities_num_after_max(void) {
    reset_entities();
    for (int i = 0; i < MAX_ENTITIES_NUM; i++) {
        RE_add_entity(i, i, NPC);
    }
    
    TEST_ASSERT_EQUAL(MAX_ENTITIES_NUM, get_entities_num());
    
    RE_add_entity(0, 0, NPC);
    TEST_ASSERT_EQUAL(MAX_ENTITIES_NUM, get_entities_num());
}

void test_entity_asset_id_default(void) {
    reset_entities();
    int id = RE_add_entity(5, 5, NPC);
    
    entity_t* entities = get_entities();
    TEST_ASSERT_EQUAL(0, entities[id].asset_id);
}

void run_entity_tests(void) {
    RUN_TEST(test_RE_add_entity_basic);
    RUN_TEST(test_RE_add_entity_max_limit);
    RUN_TEST(test_RE_add_entity_player_unique);
    RUN_TEST(test_RE_add_entity_multiple_npc);
    RUN_TEST(test_RE_add_entity_negative_coords);
    RUN_TEST(test_RE_add_entity_large_coords);
    RUN_TEST(test_RE_add_entity_mixed_behaviours);
    RUN_TEST(test_get_entities);
    RUN_TEST(test_get_entities_num);
    RUN_TEST(test_get_entities_num_after_max);
    RUN_TEST(test_entity_asset_id_default);
}