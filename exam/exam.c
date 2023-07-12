#include <stdio.h>

typedef struct Matrix{
    int rows;
    int cols;
    double **val;
    double *storage;
}Matrix;

int create_matrix(Matrix *pmatrix, int rows, int cols){
    return 0;
}

void free_matrix(Matrix *pmatrix){
    if(pmatrix->val != 0){
        free(pmatrix->val);
    }
    if(pmatrix->storage != 0){
        free(pmatrix->storage);
    }
    pmatrix->rows = 0;
    pmatrix->cols = 0;
    pmatrix->val = 0;
    pmatrix->storage = 0;
}

void create_identity_matrix(Matrix *pmatrix, int size){
    create_matrix(pmatrix, size, size);
    for (int i = 0; i < size; i++){
        pmatrix->val[i][i] = 1;
    }
}

Matrix copy_matrix(const Matrix *psrc){
    Matrix *mat = (Matrix *)malloc(sizeof(Matrix));
    create_matrix(mat, psrc->rows, psrc->cols);
    memcpy(mat->storage, psrc->storage, sizeof(psrc->storage));
    memcpy(mat->val, psrc->val, sizeof(psrc->val));
}

int get(Matrix *pm, int row, int col, double *pvalue){
    if(row <= pm->rows && col <= pm->cols){
        pvalue = pm->val[row][col];
        return 1;
    }else{
        return 0;
    }
}

int set(Matrix *pm, int row, int col, double value){
    if(row <= pm->rows && col <= pm->cols){
        pm->val[row][col] = value;
        return 1;
    }else{
        return 0;
    }
}

int compar(const void *a, const void *b){
    return *(int *)a - *(int *)b;
}
// qsort()


enum Vehicle_type {truck, car, v_ndef};
typedef struct {
    float load;
    short axes;
    int trailer_mass;
} Truck_data;
//



typedef struct {
    char vehicle_ID[15];
    unsigned int places : 3; 
    unsigned int driving_wheels : 3; 
    unsigned int towbar : 1;
} Car_data;

typedef struct {int d; int m; int y;} Date;
typedef struct {
    char owner[30];
    Date valid_date;
    enum Vehicle_type v_type;
    union veh_union;
} Vehicle;

int cmp_date(const void* a, const void* b) {
    const Date* da = a;
    const Date* db = b;
    int date_cmp = da->y - db->y;
    if (date_cmp) return date_cmp;
    date_cmp = da->m - db->m;
    if (date_cmp) return date_cmp;
    date_cmp = da->d - db->d;
    return date_cmp;
}

int delayed(const Vehicle *tv, int size, const Date *base_date, char ***pdelayed_owners){
    int counter = 0;
    for (int i = 0; i < size; i++){
        if(cmp_date(base_date, &tv[i].valid_date)){
            counter++;
        }
    }
    return counter;
}

void print_owners(const char **t_owners, size_t size){
    for (int i = 0; i < size; i++){
        printf("%s", t_owners[i]);
    }   
}

void new_owner(Vehicle *v){
    char str[30];
    for (int i = 0; i < 30; i++){
        scanf("%c", &str[i]);
        if(str[i] == "\n"){
            str[i] = NULL;
        }
    }
    v->owner = str;
}


int main(){

    return 0;
}