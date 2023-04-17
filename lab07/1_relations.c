#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct {
    int first;
    int second;
} pair;

// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair* set, int n) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            if (set[i].first == set[j].first && set[i].first == set[j].second) goto breakk;
        }
        return 0;
    breakk:;
    }
    return 1;
}

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair* set, int n) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            if (set[i].first == set[j].first && set[i].first == set[j].second) return 0;
        }
    }
    return 1;
}

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair* set, int n) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            if (set[i].first == set[j].second && set[i].second == set[j].first) goto breakk;
        }
        return 0;
    breakk:;
    }
    return 1;
}

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair* set, int n) {
    for (size_t i = 0; i < n; i++) {
        int boolean = 1;
        for (size_t j = 0; j < n; j++) {
            if (set[i].second == set[j].first) {
                if (set[i].first != set[j].second) {
                    boolean = 1;
                    break;
                }
                else boolean = 0;
            }
            if (boolean) break;
        }
        if (!boolean) return 0;
    }
    return 1;
}

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair* set, int n) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            if (set[i].first == set[j].second && set[i].second == set[j].first) return 0;
        }
    }
    return 1;
}

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair* set, int n) {
    for (size_t i = 0; i < n; i++) {
        int boolean = 1;
        for (size_t j = 0; j < n; j++) {
            if (set[i].second == set[j].first) {
                for (size_t k = 0; k < n; k++) {
                    if (set[k].first == set[i].first && set[k].second == set[j].second) {
                        boolean = 1;
                        break;
                    }
                    else boolean = 0;
                }
                if (boolean) break;
            }
        }
        if (!boolean) return 0;
    }
    return 1;
}

// Case 2:

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair* set, int n) {
    return is_reflexive(set, n) && is_transitive(set, n) && is_antisymmetric(set, n);
}

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair* set, int n) {
    return 0;
}

// A total order relation is a partial order relation that is connected
int is_total_order(pair* set, int n) {
    return is_partial_order(set, n) && is_connected(set, n);
}

int find_max_elements(pair* set, int n, int* max_elements) {
    int n_max = 0;
    for (size_t i = 0; i < n; i++) {
        int boolean = 1;
        for (size_t j = 0; j < n; j++) {
            if (set[i].second == set[j].first && set[j].second != set[i].second) {
                boolean = 0;
                break;
            }
        }

        int isUnique = 1;
        for (size_t j = 0; j < n_max; j++) {
            if (set[i].second == max_elements[j]) {
                isUnique = 0;
                break;
            }
        }


        if (boolean && isUnique) {
            max_elements[n_max] = set[i].second;
            n_max++;
        }
    }
    return n_max;
}

int find_min_elements(pair* set, int n, int* min_elements) {
    int n_min = 0;
    for (size_t i = 0; i < n; i++) {
        int boolean = 1;
        for (size_t j = 0; j < n; j++) {
            if (set[i].first == set[j].second && set[j].first != set[i].first) {
                boolean = 0;
                break;
            }
        }

        int isUnique = 1;
        for (size_t j = 0; j < n_min; j++) {
            if (set[i].first == min_elements[j]) {
                isUnique = 0;
                break;
            }
        }

        if (boolean && isUnique) {
            min_elements[n_min] = set[i].first;
            n_min++;
        }
    }
    return n_min;
}

int get_domain(pair* set, int n, int* domain) {
    int n_dom = 0;
    for (size_t i = 0; i < n; i++) {
        int isUniqueF = 1;
        int isUniqueS = 1;
        for (size_t j = 0; j < n_dom; j++) {
            if (set[i].first == domain[j]) isUniqueF = 0;
        }
        if (isUniqueF) {
            domain[n_dom] = set[i].first;
            n_dom++;
        }
        for (size_t j = 0; j < n_dom; j++) {
            if (set[i].second == domain[j]) isUniqueS = 0;
        }
        if (isUniqueS) {
            domain[n_dom] = set[i].second;
            n_dom++;
        }
    }
    //selection sort
    for (int i = 0; i < n_dom - 1; i++) {
        int max_i = i;
        for (int j = i + 1; j < n_dom; j++) {
            if (domain[j] < domain[max_i]) {
                max_i = j;
            }
        }
        //swap
        int helper = domain[max_i];
        domain[max_i] = domain[i];
        domain[i] = helper;
    }
    return n_dom;
}

// Case 3:

int composition(pair* set, int n, pair* set_2, int n_2, pair* comp_relation) {
    int n_comp = 0;
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n_2; j++) {
            if (set[i].second == set_2[j].first) {
                int isUnique = 1;
                for (size_t k = 0; k < n_comp; k++) {
                    if (set[i].first == comp_relation[k].first && set[i].second == comp_relation[k].second) {
                        isUnique = 0;
                        break;
                    }
                }
                if (isUnique) {
                    comp_relation[n_comp] = set[i];
                    n_comp++;
                }
            }
        }

    }
    return n_comp;
}

// Add pair to existing relation if not already there
int add_relation(pair* tab, int n, pair new_pair) {
    int isUnique = 1;
    for (size_t i = 0; i < n; i++) {
        if (tab[i].first == new_pair.first && tab[i].second == new_pair.second) isUnique = 0;
    }
    if (isUnique) tab[n] = new_pair;
    return isUnique;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair* relation) {
    int n;
    scanf("%i", &n);
    for (size_t i = 0; i < n; i++) {
        int first, second;
        scanf("%i", &first);
        scanf("%i", &second);
        relation[i].first = first;
        relation[i].second = second;
    }
    return n;
}

void print_int_array(int* array, int n) {
    printf("%i\n", n);
    for (size_t i = 0; i < n; i++) {
        printf("%i ", array[i]);
    }
    printf("\n");
}

int main(void) {
    int to_do;
    pair relation[MAX_REL_SIZE];
    pair relation_2[MAX_REL_SIZE];
    pair comp_relation[MAX_REL_SIZE];
    int domain[MAX_REL_SIZE];
    int max_elements[MAX_REL_SIZE];
    int min_elements[MAX_REL_SIZE];

    scanf("%d", &to_do);
    int size = read_relation(relation);
    int ordered, size_2, n_domain;

    switch (to_do) {
    case 1:
        printf("%d ", is_reflexive(relation, size));
        printf("%d ", is_irreflexive(relation, size));
        printf("%d ", is_symmetric(relation, size));
        printf("%d ", is_antisymmetric(relation, size));
        printf("%d ", is_asymmetric(relation, size));
        printf("%d\n", is_transitive(relation, size));
        break;
    case 2:
        ordered = is_partial_order(relation, size);
        n_domain = get_domain(relation, size, domain);
        printf("%d %d\n", ordered, is_total_order(relation, size));
        print_int_array(domain, n_domain);
        if (!ordered) break;
        int no_max_elements = find_max_elements(relation, size, max_elements);
        int no_min_elements = find_min_elements(relation, size, min_elements);
        print_int_array(max_elements, no_max_elements);
        print_int_array(min_elements, no_min_elements);
        break;
    case 3:
        size_2 = read_relation(relation_2);
        printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
        break;
    default:
        printf("NOTHING TO DO FOR %d\n", to_do);
        break;
    }
    return 0;
}
