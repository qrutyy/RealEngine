#include "unity.h"
#include "log.h"
#include "entity.h"
#include <string.h>

void test_RE_add_entity_should_add_basic_entity(void) {
    // When
    int id = RE_add_entity(100, 200, NPC);
    
    // Then
    TEST_ASSERT_EQUAL(0, id);
    TEST_ASSERT_EQUAL(1, curr_entities_num);
    TEST_ASSERT_EQUAL(100, entities[0].x);
    TEST_ASSERT_EQUAL(200, entities[0].y);
    TEST_ASSERT_EQUAL(NPC, entities[0].beh);
    TEST_ASSERT_EQUAL(1, entities[0].depth);
    TEST_ASSERT_EQUAL(0, entities[0].asset_id);
    TEST_ASSERT_EQUAL(0, entities[0].animations_num);
    TEST_ASSERT_EQUAL(0, entities[0].curr_animation);
    TEST_ASSERT_NULL(entities[0].asset);
}

void test_RE_add_entity_should_return_sequential_ids(void) {
    // When
    int id1 = RE_add_entity(10, 20, NPC);
    int id2 = RE_add_entity(30, 40, FOLLOW);
    int id3 = RE_add_entity(50, 60, PLAYER);
    
    // Then
    TEST_ASSERT_EQUAL(0, id1);
    TEST_ASSERT_EQUAL(1, id2);
    TEST_ASSERT_EQUAL(2, id3);
    TEST_ASSERT_EQUAL(3, curr_entities_num);
}

void test_RE_add_entity_should_enforce_max_entities_limit(void) {
    // Given - Fill up to maximum
    for (int i = 0; i < MAX_ENTITIES_NUM; i++) {
        int id = RE_add_entity(i * 10, i * 20, NPC);
        TEST_ASSERT_EQUAL(i, id);
    }
    
    TEST_ASSERT_EQUAL(MAX_ENTITIES_NUM, curr_entities_num);
    
    // When - Try to add one more entity
    int result = RE_add_entity(999, 999, NPC);
    
    // Then - Should fail gracefully
    TEST_ASSERT_EQUAL(-1, result);
    TEST_ASSERT_EQUAL(MAX_ENTITIES_NUM, curr_entities_num);
}

void test_RE_add_entity_should_allow_only_one_player(void) {
    // When - Add first player
    int player_id = RE_add_entity(50, 60, PLAYER);
    
    // Then - Should succeed
    TEST_ASSERT_EQUAL(0, player_id);
    TEST_ASSERT_EQUAL(PLAYER, entities[0].beh);
    
    // When - Try to add second player
    int result = RE_add_entity(70, 80, PLAYER);
    
    // Then - Should fail
    TEST_ASSERT_EQUAL(-1, result);
    TEST_ASSERT_EQUAL(1, curr_entities_num); // Count should not increase
}

void test_RE_add_entity_should_allow_other_entities_after_player(void) {
    // Given - Add a player first
    RE_add_entity(50, 60, PLAYER);
    
    // When - Add other entity types
    int npc_id = RE_add_entity(70, 80, NPC);
    int follow_id = RE_add_entity(90, 100, FOLLOW);
    
    // Then - Should succeed
    TEST_ASSERT_EQUAL(1, npc_id);
    TEST_ASSERT_EQUAL(2, follow_id);
    TEST_ASSERT_EQUAL(3, curr_entities_num);
    TEST_ASSERT_EQUAL(NPC, entities[1].beh);
    TEST_ASSERT_EQUAL(FOLLOW, entities[2].beh);
}

void test_get_entities_should_return_correct_array(void) {
    // Given - Add some entities
    RE_add_entity(10, 20, NPC);
    RE_add_entity(30, 40, FOLLOW);
    
    // When
    entity_t* entities_ptr = get_entities();
    
    // Then
    TEST_ASSERT_NOT_NULL(entities_ptr);
    TEST_ASSERT_EQUAL_PTR(entities, entities_ptr);
    TEST_ASSERT_EQUAL(10, entities_ptr[0].x);
    TEST_ASSERT_EQUAL(20, entities_ptr[0].y);
    TEST_ASSERT_EQUAL(NPC, entities_ptr[0].beh);
    TEST_ASSERT_EQUAL(30, entities_ptr[1].x);
    TEST_ASSERT_EQUAL(40, entities_ptr[1].y);
    TEST_ASSERT_EQUAL(FOLLOW, entities_ptr[1].beh);
}

void test_get_entities_num_should_return_correct_count(void) {
    // Given - Initial state
    TEST_ASSERT_EQUAL(0, get_entities_num());
    
    // When - Add entities
    RE_add_entity(1, 2, NPC);
    
    // Then
    TEST_ASSERT_EQUAL(1, get_entities_num());
    
    // When - Add more entities
    RE_add_entity(3, 4, FOLLOW);
    RE_add_entity(5, 6, PLAYER);
    
    // Then
    TEST_ASSERT_EQUAL(3, get_entities_num());
}

void test_RE_add_entity_should_handle_negative_coordinates(void) {
    // When
    int id = RE_add_entity(-100, -200, NPC);
    
    // Then
    TEST_ASSERT_EQUAL(0, id);
    TEST_ASSERT_EQUAL(-100, entities[0].x);
    TEST_ASSERT_EQUAL(-200, entities[0].y);
    TEST_ASSERT_EQUAL(NPC, entities[0].beh);
}

void test_RE_add_entity_should_support_all_behaviour_types(void) {
    // When
    int player_id = RE_add_entity(10, 10, PLAYER);
    int npc_id = RE_add_entity(20, 20, NPC);
    int follow_id = RE_add_entity(30, 30, FOLLOW);
    
    // Then
    TEST_ASSERT_EQUAL(PLAYER, entities[player_id].beh);
    TEST_ASSERT_EQUAL(NPC, entities[npc_id].beh);
    TEST_ASSERT_EQUAL(FOLLOW, entities[follow_id].beh);
    TEST_ASSERT_EQUAL(3, curr_entities_num);
}

void test_entity_fields_should_have_correct_default_values(void) {
    // When
    int id = RE_add_entity(150, 250, FOLLOW);
    
    // Then
    TEST_ASSERT_EQUAL(150, entities[id].x);
    TEST_ASSERT_EQUAL(250, entities[id].y);
    TEST_ASSERT_EQUAL(FOLLOW, entities[id].beh);
    TEST_ASSERT_EQUAL(1, entities[id].depth); // Default depth
    TEST_ASSERT_EQUAL(0, entities[id].asset_id); // Default asset_id
    TEST_ASSERT_EQUAL(0, entities[id].animations_num); // Default animations_num
    TEST_ASSERT_EQUAL(0, entities[id].curr_animation); // Default curr_animation
    TEST_ASSERT_NULL(entities[id].asset); // Default asset pointer
}

void test_entity_structure_should_have_animation_arrays(void) {
    // When
    int id = RE_add_entity(100, 100, NPC);
    
    // Then - Verify animation array exists and is accessible
    TEST_ASSERT_NOT_NULL(entities[id].animations);
    TEST_ASSERT_EQUAL(MAX_ANIMATIONS_NUM, sizeof(entities[id].animations) / sizeof(animation_t));
}

void test_RE_add_entity_should_initialize_all_entity_fields_to_zero(void) {
    // When
    int id = RE_add_entity(0, 0, NPC);
    
    // Then - Verify all fields are properly initialized
    entity_t* ent = &entities[id];
    
    // Check that animation array is zero-initialized
    for (int i = 0; i < MAX_ANIMATIONS_NUM; i++) {
        // Assuming animation_t has fields we can check - adjust based on your animation_t definition
        // This ensures the entire animations array is properly initialized
        TEST_ASSERT_EQUAL_MEMORY(&(animation_t){0}, &ent->animations[i], sizeof(animation_t));
    }
}

TEST_GROUP_RUNNER(Entity) {
    RUN_TEST_CASE(Entity, test_RE_add_entity_should_add_basic_entity);
    RUN_TEST_CASE(Entity, test_RE_add_entity_should_return_sequential_ids);
    RUN_TEST_CASE(Entity, test_RE_add_entity_should_enforce_max_entities_limit);
    RUN_TEST_CASE(Entity, test_RE_add_entity_should_allow_only_one_player);
    RUN_TEST_CASE(Entity, test_RE_add_entity_should_allow_other_entities_after_player);
    RUN_TEST_CASE(Entity, test_get_entities_should_return_correct_array);
    RUN_TEST_CASE(Entity, test_get_entities_num_should_return_correct_count);
    RUN_TEST_CASE(Entity, test_RE_add_entity_should_handle_negative_coordinates);
    RUN_TEST_CASE(Entity, test_RE_add_entity_should_support_all_behaviour_types);
    RUN_TEST_CASE(Entity, test_entity_fields_should_have_correct_default_values);
    RUN_TEST_CASE(Entity, test_entity_structure_should_have_animation_arrays);
    RUN_TEST_CASE(Entity, test_RE_add_entity_should_initialize_all_entity_fields_to_zero);
}

static void RunAllTests(void) {
    RUN_TEST_GROUP(Entity);
}

int main(int argc, const char * argv[]) {
    return UnityMain(argc, argv, RunAllTests);
}
