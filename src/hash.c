// Hash function invented by Daniel J. Bernstein
static unsigned long hash_djb2(char *str) {
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

unsigned long hashf(char *str) {
    return hash_djb2(str);
}