#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TEST 0

typedef struct Date {
    int day;
    int month;
    int year;
} Date;

/////////////////////////////////////////////////////////////////
// 8.1 funkcja bsearch2

#define FOOD_MAX  30   // max. liczba artykułów
#define ART_MAX   15   // max. długość nazwy artykułu +1
#define RECORD_MAX 40  // max. długość rekordu (linii) w pliku

typedef struct Food {
    char art[ART_MAX];
    float price;
    float amount;
    Date valid_date;
} Food;

typedef int (*ComparFp)(const void*, const void*);

void* bsearch2(const void* key, void* base, size_t nitems, size_t size, ComparFp compar, char* result) {
    char* start = base;
    int i = nitems >> 1; // integer division
    int s = 0;
    int e = nitems;
    while (1) {
        void* el = start + (i * size);
        int cmp = compar(key, el);
        if (cmp == 0) {
            *result = 1;
            return el;
        }

        if (s == e) {
            *result = 0;
            return el;
        }

        if (cmp > 0) s = i + 1;
        else e = i;
        i = (s + e) >> 1;
    }
}

void print_art(Food* p, size_t n, char* art) {
    for (int i = 0; i < n; i++) {
        if (strcmp(p[i].art, art) == 0) {
            printf("%f %f %i %i %i\n", p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month, p[i].valid_date.year);
        }
    }
}

Food* add_record(Food* tab, size_t tab_size, int* np, ComparFp compar, Food* new) {
    char is_in_tab;
    Food* res = bsearch2(new, tab, *np, sizeof(Food), compar, &is_in_tab);
    if (is_in_tab) {
        res->amount += new->amount;
    }
    else {
        if (*np >= FOOD_MAX - 1) return NULL;
        for (Food* f = (tab + *np); f >= res; f--) memcpy(f + 1, f, sizeof(Food));
        memcpy(res, new, sizeof(Food));
        (*np)++;
    }
    return res;
}

int compar(const void* a, const void* b) {
    const Food* ca = a;
    const Food* cb = b;

    int art_cmp = strcmp(ca->art, cb->art);
    if (art_cmp) return art_cmp;

    int price_cmp = ((ca->price) - (cb->price)) * 1000;
    if (price_cmp) return price_cmp;

    const Date* da = &ca->valid_date;
    const Date* db = &cb->valid_date;
    int date_cmp = da->year - db->year;
    if (date_cmp) return date_cmp;
    date_cmp = da->month - db->month;
    if (date_cmp) return date_cmp;
    date_cmp = da->day - db->day;
    return date_cmp;
}
int cmp_date(const void* a, const void* b) {
    const Date* da = a;
    const Date* db = b;
    int date_cmp = da->year - db->year;
    if (date_cmp) return date_cmp;
    date_cmp = da->month - db->month;
    if (date_cmp) return date_cmp;
    date_cmp = da->day - db->day;
    return date_cmp;
}
int cmp_food_date(const void* a, const void* b) {
    const Food* ca = a;
    const Food* cb = b;
    return cmp_date(&ca->valid_date, &cb->valid_date);
}

int read_stream(Food* tab, size_t size, int no, FILE* stream) {
    int np = 0;
    for (int i = 0; i < no; i++) {
        Food f1;
        fscanf(stream, "%s %f %f %i %i %i", &f1.art, &f1.price, &f1.amount, &f1.valid_date.day, &f1.valid_date.month, &f1.valid_date.year);
        add_record(tab, size, &np, compar, &f1);
    }
    return np;
}

float value(Food* food_tab, size_t n, Date curr_date, int anticip) {
    qsort(food_tab, n, sizeof(Food), cmp_food_date);
    struct tm t = { .tm_year = curr_date.year, .tm_mon = curr_date.month, .tm_mday = curr_date.day };

    t.tm_mday += anticip;
    mktime(&t);

    Date wanted_date = { .year = t.tm_year, .month = t.tm_mon, .day = t.tm_mday };

    int i = n >> 1; // integer division
    int s = 0;
    int e = n;
    while (1) {
        Food* el = (food_tab + i);
        int cmp = cmp_date(&wanted_date, &el->valid_date);
        if (cmp == 0) {
            break;
        }

        if (s == e) {
            i = -1;
            break;
        }

        if (cmp > 0) s = i + 1;
        else e = i;
        i = (s + e) >> 1;
    }
    if (i == -1) return 0;
    while (i > 0 && cmp_date(&wanted_date, &(food_tab + i - 1)->valid_date) == 0) i--;
    float val = (food_tab + i)->amount * (food_tab + i)->price;
    while (i < n - 1 && cmp_date(&wanted_date, &(food_tab + i + 1)->valid_date) == 0) {
        val += (food_tab + i + 1)->amount * (food_tab + i + 1)->price;
        i++;
    }
    return val;
}

/////////////////////////////////////////////////////////////////
// 8.3 Sukcesja

//Definicję funkcji int read stream(Food *p, size t size, int no, FILE
// *stream), która czyta no rekordów (linii) danych ze strumienia wejściowego.
// Dla każdego rekordu wywołuje funkcję add record.

#define CHILD_MAX  20   // maksymalna liczba przesuwanej w tablicy grupy osób (elementów tablicy)

enum Sex { F, M };
enum BOOL { no, yes };

struct Bit_data {
    enum Sex sex :1;
    enum BOOL pretendent :1;   // =no - nie pretenduje (panuje albo nie żyje) ale jest w zbiorze dla spójności relacji.
};

typedef struct {
    char* name;
    struct Bit_data bits;
    Date born;
    char* parent;
} Person;

typedef struct {
    char* par_name;
    int index;
} Parent;    // strukturę można rozbudować o pole liczby dzieci

const Date primo_date = { 28, 10, 2011 }; // Data zmiany kolejności sukcesji

void print_person(const Person* p) {
    printf("%s\n", p->name);
}

void print_persons(const Person* person_tab, int n) {
    for (int i = 1; i <= n; ++i, ++person_tab) printf("%2d %12s %s\n", i, person_tab->name, person_tab->parent);
    return;
}
void mem_swap(void* a, void* b) {
    Person temp;
    memcpy(&temp, a, sizeof(Person));
    memcpy(a, b, sizeof(Person));
    memcpy(b, &temp, sizeof(Person));
}

int create_list(Person* person_tab, int n, int no) {
    // puts first person to the beginning of the array
    for (int j = 0; j < n; j++) {
        if ((person_tab + j)->parent == NULL) {
            mem_swap((person_tab + j), person_tab);
            break;
        }
    }
    // parents array holds previously used parents in order of use
    int parents[n];
    int par_n = 0;

    // adds first parent
    parents[par_n] = 0;
    par_n++;

    // temp date for comparisons and index of the oldest child
    Date min_date = { .day = 1,.month = 1,.year = 9999 };
    int min_index = n;
    int last_gender = 0;

    int i = 1;
    while (i < n) {
        // checks if any person who should be higher left in the array
        for (int j = i; j < n; j++) {
            Person actual_person = (*(person_tab + j));
            int is_older = cmp_date(&min_date, &actual_person.born) > 0;
            int is_child = actual_person.parent == (*(person_tab + parents[par_n - 1])).name;
            int actual_gender = actual_person.bits.sex;
            Date d1 = { .day = 28, .month = 10, .year = 2011 };
            int is_born_after = cmp_date(&actual_person.born, &d1) > 0;
            if (is_child && ((is_born_after && is_older) || (!is_born_after && ((!last_gender && (actual_gender || is_older)) || (last_gender && actual_gender && is_older))))) {
                last_gender = actual_person.bits.sex;
                min_date = actual_person.born;
                min_index = j;
            }
        }
        // if it found the child of the last parent in parents array it puts it to the next place (i) in person array
        if (min_index < n) {
            mem_swap((person_tab + min_index), (person_tab + i));
            // adds just added child to parents array and that child now becomes the new last parent in parents array
            parents[par_n] = i;
            par_n++;
            i++;
        }
        else {
            // if there are no children of the last parent from parents array left, it removes last parent 
            par_n--;
        }
        // resets variables for comparisons
        min_date.year = 9999;
        min_index = n;
        last_gender = 0;
    }
    int cnt = 0;
    for (int i = 0;i < n;i++) {
        if ((*(person_tab + i)).bits.pretendent) {
            cnt++;
            if (cnt == no) {
                printf("%s\n", (*(person_tab + i)).name);
                return 0;
            }
        }
    }
}

////////////////////////////////////////////////////////////////

int main(void)
{
    // Wszyscy potomkowie Jerzego VI (w kolejności przypadkowej):
    Person person_tab[33] = {
    {"Charles III",M,no,14,11,1948,"Elizabeth II"},
    {"Anne",F,yes,15,8,1950,"Elizabeth II"},
    {"Andrew",M,yes,19,2,1960,"Elizabeth II"},
    {"Edward",M,yes,10,3,1964,"Elizabeth II"},
    {"David",M,yes,3,11,1961,"Margaret"},
    {"Sarah",F,yes,1,5,1964,"Margaret"},
    {"William",M,yes,21,6,1982,"Charles III"},
    {"Henry",M,yes,15,9,1984,"Charles III"},
    {"Peter",M,yes,15,11,1977,"Anne"},
    {"Zara",F,yes,15,5,1981,"Anne"},
    {"Beatrice",F,yes,8,8,1988,"Andrew"},
    {"Eugenie",F,yes,23,3,1990,"Andrew"},
    {"James",M,yes,17,12,2007,"Edward"},
    {"Louise",F,yes,8,11,2003,"Edward"},
    {"Charles",M,yes,1,7,1999,"David"},
    {"Margarita",F,yes,14,5,2002,"David"},
    {"Samuel",M,yes,28,7,1996,"Sarah"},
    {"Arthur",M,yes,6,5,2019,"Sarah"},
    {"Georg",M,yes,22,7,2013,"William"},
    {"George VI",M,no,14,12,1895,NULL},
    {"Charlotte",F,yes,22,5,2015,"William"},
    {"Louis",M,yes,23,4,2018,"William"},
    {"Archie",M,yes,6,5,2019,"Henry"},
    {"Lilibet",F,yes,4,6,2021,"Henry"},
    {"Savannah",F,yes,29,12,2010,"Peter"},
    {"Isla",F,yes,29,3,2012,"Peter"},
    {"Mia",F,yes,17,1,2014,"Zara"},
    {"Lena",F,yes,18,6,2018,"Zara"},
    {"Elizabeth II",F,no,21,4,1925,"George VI"},
    {"Margaret",F,no,21,8,1930,"George VI"},
    {"Lucas",M,yes,21,3,2021,"Zara"},
    {"Sienna",F,yes,18,9,2021,"Beatrice"},
    {"August",M,yes,9,2,2021,"Eugenie"}
    };

    int to_do, no;
    size_t size, n;
    Food food_tab[FOOD_MAX];
    char buff[ART_MAX];
    FILE* file;
    if (TEST) printf("Wpisz nr zadania (1 - 3) ");
    scanf("%d", &to_do);

    switch (to_do) {
    case 1:  // bsearch2
        if (TEST) printf("Wpisz liczbe linii danych: ");
        scanf("%d", &no); getchar();
        if (TEST) file = stdin;
        else {
            scanf("%s", buff);
            file = fopen(buff, "r");
            // file = fopen("foods0.txt", "r");
            if (file == NULL) { printf("Error 1\n"); break; }
        }
        if (TEST) printf("W %d liniach wpisuj dane: nazwa cena ilosc dd mm yyyy: \n", no);
        n = read_stream(food_tab, FOOD_MAX, no, file);
        if (TEST) printf("Wpisz nazwe artykulu: ");
        scanf("%s", buff);
        print_art(food_tab, n, buff);
        break;
    case 2: // proste sortowanie struktur
        if (TEST) printf("Wpisz liczbe linii danych: ");
        scanf("%d", &no); getchar();
        if (TEST) file = stdin;
        else {
            scanf("%s", buff);
            file = fopen(buff, "r");
            //            file = fopen("foods0.txt","r");
            if (file == NULL) { printf("Error 1\n"); break; }
        }
        if (TEST) printf("W %d liniach wpisuj dane: nazwa cena ilosc dd mm yyyy: \n", no);
        n = read_stream(food_tab, FOOD_MAX, no, file);
        Date curr_date;
        int anticip;
        if (TEST) printf("Wpisz date odniesienia dd mm yyyy: ");
        scanf("%d %d %d", &curr_date.day, &curr_date.month, &curr_date.year);
        if (TEST) printf("Wpisz roznice dni: ");
        scanf("%d", &anticip);
        printf("%.2f\n", value(food_tab, n, curr_date, anticip));
        break;
    case 3: // sukcesja
        if (TEST == 1) printf("Wpisz pozycję w kolejce (liczona od 1): ");
        scanf("%d", &no);
        int no_persons = sizeof(person_tab) / sizeof(Person);
        no_persons = create_list(person_tab, no_persons, no);
        // if (TEST) print_persons(person_tab, no_persons);
        // else print_person(person_tab + no - 1);
        break;
    default:
        printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}

