#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1000
#define MEMORY_ALLOCATION_ERROR  -1
#define FILE_OPEN_ERROR          -2
#define LIST_ERROR               -3
#define TEST 0               // 1 dla testowania, 0 dla automatycznej oceny

typedef struct tagListElement {
    struct tagListElement *next;
    void                  *data;
} ListElement;

typedef void (*ConstDataFp)(const void*);
typedef void (*DataFp)(void*);
typedef  int (*CompareDataFp)(const void*,const void*);

typedef struct tagList {
    ListElement *head;
    ListElement *tail;
    int         size;
    ConstDataFp dumpData;
    DataFp      freeData;
    CompareDataFp compareData;
    DataFp      modifyData;
} List;

////////////////////////////////////////////////

// Funkcje pomocnicze
void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if (ptr) return ptr;
	printf("malloc error\n");
	exit(MEMORY_ALLOCATION_ERROR);
}
char *safe_strdup(char *str) {
	char *ptr = strdup(str);
	if (ptr) return ptr;
	printf("safe_strdup error\n");
	exit(MEMORY_ALLOCATION_ERROR);
}

////////////////////////////////////////////////////////////////
// Funkcje uniwersalne (ogolnego przeznaczenia) obsługi listy

// Inicjuje listę
void init_List(List *list, ConstDataFp dump, DataFp free, CompareDataFp cmp, DataFp modify) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->dumpData = dump;
    list->freeData = free;
    list->compareData = cmp;
    list->modifyData = modify;
}

// Wypisuje dane elemntów listy
void dumpList(const List* list) {
    if(list->dumpData == NULL) return;
    ListElement *i;
    for(i=list->head; i!=0; i=i->next) list->dumpData(i->data);
}

// Zwalnia pamięć elementów listy i danych
void freeList(List *list) {
    ListElement *current = 0;
    current = list->head;
    while(current) {
        ListElement *todelete = current;
        current = current->next;
        if(list->freeData) list->freeData(todelete->data); // zwolnienie pamięci struktury data
        free(todelete);              // zwolnienie pamięci elementu
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

// Uniwersalne funkcje obsługi elementów listy

// Dodaje element na początku listy
void pushFront(List *list, void *data) {
    ListElement *el = safe_malloc(sizeof(ListElement));
    el->data = data;
    el->next = list->head;
    if(list->head == NULL){
        list->tail = el;
    }
    list->head = el;
}

// Dodaje element na końcu listy
void pushBack(List *list, void *data) {
    ListElement *el = safe_malloc(sizeof(ListElement));
    el->data = data;
    el->next = NULL;
    if(list->tail == NULL){
        list->head = el;
    }else{
        list->tail->next = el;
    }
    list->tail = el;
}

// Usuwa pierwszy element listy
void popFront(List *list) {
    if(list->head != NULL){
        ListElement *el = list->head;
        if(list->head == list->tail){
            list->head = NULL;
            list->tail = NULL;
        }else{
            list->head = list->head->next;
        }
        list->freeData(el->data);
        free(el);
    }
}

// Odwraca kolejność elementów listy
void reverse(List *list) {
    ListElement *prev = list->head;

    if (prev != NULL){
        ListElement *curr = prev->next;
        prev->next = NULL;
        ListElement *temp;

        while (curr != NULL){
            temp = curr->next;
            curr->next = prev;
            prev = curr;
            curr = temp;
        }

        temp = list->head;
        list->head = list->tail;
        list->tail = temp;
    }
}

// Funkcje pomocnicze dla list uporzadkowanych 
// (porządek wyznacza funkcja wskazana polem compareData)

// Zwraca element w posortowanej liście, a jeżeli nie ma, to element poprzedzający (nie ma, to NULL)
ListElement* findInsertionPoint(const List *list, ListElement *element) {
    ListElement *curr = list->head;
    ListElement *prev = NULL;
    while(curr != NULL){
        if(list->compareData(curr->data, element->data) > 0){
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    return prev;
}


// Drugi parametr przekazuje do funkcji adres przydzielonej dla "nowych" danych pamięci.
// Jeżeli takie dane są już w elemencie listy, to dane tego elementu są modyfikowane
// funkcją wskazaną w polu modifyData, a pamięć "nowych" danych jest zwalniana.
// Jeżeli w liście takiego elementu nie ma, to nowy element jest tworzony
// i dopisany do listy zachowując uporządkowanie listy.
void insertInOrder(List *list, void *a) {
    ListElement* el = safe_malloc(sizeof(ListElement));
    el->data = a;
    el->next = NULL;
    ListElement* prev = findInsertionPoint(list, el);
    if(prev == NULL){
        pushFront(list, a);
        free(el);
    }else if(list->compareData(el->data, prev->data) != 0){
        el->next = prev->next;
        prev->next = el;
        if(prev == list->tail){
            list->tail = el;
        }
    }else if(list->modifyData != NULL){
        list->modifyData(prev->data);
    }

}


/////////////////// Funkcje do zadania 11.1.2

// Dodaje element na końcu listy bez korzystania z pola tail
void pushBack_v0(List *list, void *data) {
    ListElement* new = safe_malloc(sizeof(ListElement));
    new->data = data;
    new->next = NULL;
    if(list->head != NULL){
        ListElement* el = list->head;
        while(el->next != NULL){
            el = el->next;
        }
        el->next = new;
        list->tail = new;
    }else{
        list->head = new;
        list->tail = new;
    }
}


// Funkcje warstwy obsługi danych wskazywanych w polach struktury List


//////  Dla zadania 11.1.1 i 11.1.2
// 1. Dana jest liczbą typu int

typedef int DataInt;

void dump_int(const void *d) {
    const DataInt* data = (const DataInt*)d;
    printf("%d ", *data);
}

void free_int(void *d) {
    if (d == NULL) return;
    DataInt* data = (DataInt*)d;
    free(data);
}

int cmp_int(const void *a, const void *b) {
    const DataInt* d1 = (const DataInt*)a;
    const DataInt* d2 = (const DataInt*)b;

    return *d1 - *d2;
}

// Przydziela pamięć i zapisuje w niej daną o wartości v
void *create_data_int(int v) {
    DataInt* data = safe_malloc(sizeof(DataInt));
    *data = v;
    return data;
}

//////  Dla zadania 11.1.3 i 11.1.4
// 2. Dana jest zapisana w  strukturze typu DataWord

typedef struct DataWord {
    char *word;
    int counter;
} DataWord;

void dump_word (const void *d) {
    const DataWord* data = (const DataWord*)d;
    printf("%s ", data->word);
}

void free_word(void *d) {
    DataWord* data = (DataWord*)d;
    free(data);
}

int cmp_word_alphabet(const void *a, const void *b) {
    const DataWord* d1 = (const DataWord*)a;
    const DataWord* d2 = (const DataWord*)b;
    return strcmp(d1->word, d2->word);
}

int cmp_word_counter(const void *a, const void *b) {
    const DataWord* d1 = (const DataWord*)a;
    const DataWord* d2 = (const DataWord*)b;
    return d1->counter - d2->counter;
}

void modify_word(void *a) {
    DataWord *data = (DataWord *)a;
    data->counter++;
}

// Wypisz dane elementów spełniających warunek równości sprawdzany funkcją 
// wskazywaną w polu compareData nagłówka listy
void dumpList_word_if(List *plist, int n) {
    ListElement* curr = plist->head;
    DataWord data = {.counter = n, .word = NULL};
    while(curr!=NULL){
        if(plist->compareData(curr->data, &data)==0){
            plist->dumpData(curr->data);
        }
        curr = curr->next;
    }
}

// Przydziela pamięć dla łańcucha string i struktury typu DataWord.
// Do przydzielonej pamięci wpisuje odpowiednie dane.
// Zwraca adres struktury.
void *create_data_word(char *string, int counter) {
    DataWord* data = safe_malloc(sizeof(DataWord));
    data->word = safe_strdup(string);
    data->counter = counter;
}

//////////////////////////////////////////////////

/////////////////// Funkcje pomocnicze dla zadan 11.1.3 i 11.1.4

// Wyrazy ze strumienia wejściowego zapisuje w elementach listy
// w kolejności określonej ostatnim parametrem funkcji (cmp)
// - adresem funkcji porównującej typu CompareDataFp. Adres zerowy oznacza, 
// że kolejność elementów listy jest zgodna z kolejnością 
void stream_to_list(List *list, FILE *stream, CompareDataFp cmp) {
    DataWord *data;
    char *p, buf[BUFFER_SIZE];
    char delimits[] = " \r\t\n.,?!-;";
    if(cmp) list->compareData = cmp;
    while(fgets(buf,BUFFER_SIZE,stream)) {
        p = strtok(buf, delimits);
        while(p) {
            data = create_data_word(p, 1); 
            if(cmp) {
                for (int i = 0; i < strlen(data->word); i++){
                    data->word[i] = tolower(data->word[i]);
                }
                insertInOrder(list,data);
            }
            else    pushBack(list, data);
            p = strtok(NULL,delimits);
        }
    }
}

void list_test(List *plist, int n) {
	char op[2];
	int v;
	ListElement element, *pelement;
	DataInt data;
	for (int i = 0; i < n; ++i) {
        if(TEST) printf("Wpisz kod polecenia ");
		scanf("%s", op);
		switch (op[0]) {
			case 'f': // dodaj na początku listy
                if(TEST) printf("Wpisz wartość ");
				scanf("%d", &v);
				pushFront(plist,create_data_int(v));
				break;
			case 'b': // dodaj na końcu listy
                if(TEST) printf("Wpisz wartość ");
				scanf("%d", &v);
				pushBack(plist,create_data_int(v));
				break;
			case 'd': // usuń pierwszy element
			    popFront(plist);
				break;
            case 'r': // odwróć kolejność
                reverse(plist);
                break;
			case 'i': // dodaj wg porządku rosnącego (int) lub alfabetycznego (char),
			          // jeżeli element już istnieje, to akcja zależna od typu danych
                if(TEST) printf("Wpisz wartość ");
				scanf("%d",&v);
				insertInOrder(plist,create_data_int(v));
                break;
			default:
				printf("No such operation: %s\n", op);
				break;
		}
	}
}

int main(void) {
    int to_do, n, size;
    char buf[257];
    clock_t t_start, t_end;
    double t_total;
    FILE *stream;
    List list;
    ListElement *head;

	if(TEST) printf("Wpisz nr zadania ");
	scanf ("%d", &to_do);
	switch (to_do) {
case 1:
	    if(TEST) printf("Wpisz liczbę poleceń ");
        scanf("%d",&n);
        head = NULL;
        init_List(&list, dump_int, free_int, cmp_int, NULL);
//        list_test(&list, n, int (cmp)(const void*, const void*))
        list_test(&list, n);
        dumpList(&list);
        freeList(&list);
        break;
case 2:
        if(TEST) printf("Wpisz liczbę elementów ");
        scanf("%d",&size);
        init_List(&list, dump_int, free_int, cmp_int, NULL);
        t_start = clock();
        for(int i=0; i<size; ++i) pushBack_v0(&list,create_data_int(i));
        t_end = clock();
        t_total = (double)(t_end - t_start) / CLOCKS_PER_SEC;
        printf("n = %d. Back bez tail.  Czas = %g s.\n",size,t_total);
        freeList(&list);

        init_List(&list, dump_int, free_int, cmp_int, NULL);
        t_start = clock();
        for(int i=0; i<size; ++i) pushFront(&list,create_data_int(i));
        reverse(&list);
        t_end = clock();
        t_total = (double)(t_end - t_start) / CLOCKS_PER_SEC;
        printf("n = %d. Front + revers. Czas = %g s.\n",size,t_total);
        freeList(&list);

        init_List(&list, dump_int, free_int, cmp_int, NULL);
        t_start = clock();
        for(int i=0; i<size; ++i) pushBack(&list,create_data_int(i));
        t_end = clock();
        t_total = (double)(t_end - t_start) / CLOCKS_PER_SEC;
        printf("n = %d. Back z tail.    Czas = %g s.\n",size,t_total);
        freeList(&list);
        break;
case 3:      // wypisz wszystkie słowa wczytane do elementów listy
        init_List(&list,dump_word,free_word,NULL,NULL);
        if(TEST) {
            stream = stdin;
            printf("Wpisz linie tekstu\n");
            stream_to_list(&list, stream, NULL);
            rewind(stream);
        }
        else {
            stream = fopen("Tekst_1.txt","r");
            if(stream == NULL) exit(FILE_OPEN_ERROR);
            stream_to_list(&list, stream, NULL);
            fclose(stream);
        }
        dumpList(&list);
        freeList(&list);
        break;
case 4:      // wypisz w porządku alfabetycznym słowa o zadanej krotności
        if(TEST) printf("Wpisz krotność słowa ");
        scanf("%d",&n);
        init_List(&list,dump_word,free_word,NULL,modify_word);
        if(TEST) {
            stream = stdin;
            printf("Wpisz linie tekstu\n");
            stream_to_list(&list, stream, cmp_word_alphabet);
            rewind(stream);
        }
        else {
            stream = fopen("Tekst_C.txt","r");
            if(stream == NULL) exit(FILE_OPEN_ERROR);
            stream_to_list(&list, stream, cmp_word_alphabet);
            fclose(stream);
        }
        list.compareData = cmp_word_counter;
        dumpList_word_if(&list,n); printf("\n");
        freeList(&list);
        break;
default:
		printf("NOTHING TO DO FOR %d\n", to_do);
	}
    return 0;
}