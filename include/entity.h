/*
    A number specifying entity's behaviour.
*/
enum e_behaviour {
    PLAYER = 0,
    NPC = 1
    // these should be more precise, to be added
};

int RE_add_entity(int x, int y, enum e_behaviour beh);