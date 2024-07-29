#include <stdio.h>
#include <math.h>
#include <stdlib.h>

struct cord {
    double value;
    struct cord *next;
};

struct vector
{
    struct vector *next;
    struct cord *cords;
    struct vector *closest;
};


double max(double a, double b)
{
    if(a>b)
    {
        return a;
    }
    return b;
}

void printVector(struct vector *vec) {
    struct cord *curr_cord = vec->cords;
    while (curr_cord != NULL) {
        printf("%.4f", curr_cord->value);
        curr_cord = curr_cord->next;
    }
    printf("\n");
}

void printCord(struct cord *cord) {

    struct cord *curr_cord = cord;

    while (curr_cord != NULL) {
        printf("%f ", curr_cord->value);
        curr_cord = curr_cord->next;
    }
    printf("\n");
}

void free_malloc(struct vector *vec)
{
    struct vector *temp_vec;
    struct cord *temp_cord;
    struct vector *curr_vec;
    struct cord *curr_cord;

    int i;
    i = 0;

    curr_vec = vec;
    while (curr_vec != NULL) {

        i++; 

        temp_vec = curr_vec;

        printCord(temp_vec->cords);

        curr_vec = curr_vec->next;

        curr_cord = temp_vec->cords;
        while (curr_cord != NULL)
        {
            temp_cord = curr_cord;
            curr_cord = curr_cord->next;
            free(temp_cord);
        }

        printCord(temp_vec->cords);
        printf("\n");
        free(temp_vec);


    }
}


void free_malloc_cord(struct cord *cord)
{
    struct cord *temp_cord;
    struct cord *curr_cord;


    curr_cord = cord;
    while (curr_cord != NULL)
    {
        temp_cord = curr_cord;
        curr_cord = curr_cord->next;
        free(temp_cord);
    }
}


double euclideanDistance(struct cord *cord1, struct cord *cord2, int d)
{
    struct cord *curr_cord1 = cord1;
    struct cord *curr_cord2 = cord2;

    double sum;
    int i;
    sum=0;


    for (i = 1; i <= d; i++)
    {
        sum += (curr_cord1->value-curr_cord2->value)*(curr_cord1->value-curr_cord2->value);
        curr_cord1= curr_cord1->next;
        curr_cord2= curr_cord2->next;
    }
    return sqrt(sum);
}




struct cord* initialize_vector(int d)
{
    struct cord *head_cord;
    struct cord *curr_cord;
    int iter_cord;

    head_cord = malloc(sizeof(struct cord));
    if (head_cord == NULL) {
        printf("An Error Has Occurred");
        exit(1);
    }
    curr_cord = head_cord;

    for (iter_cord = 1; iter_cord < d; iter_cord++) {
        curr_cord->next = malloc(sizeof(struct cord));
        if (curr_cord->next == NULL)
        {
            free_malloc_cord(head_cord);
            printf("An Error Has Occurred");
            exit(1);
        }
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;
    }
    return head_cord;
}




struct vector* initialize_centroids(int k, int d) {


    struct vector *head_vec = malloc(sizeof(struct vector));
    struct vector *curr_vec = head_vec;
    int centroid;
    
    if (head_vec == NULL)
    {
        return NULL;
        printf("An Error Has Occurred");
    }


    for (centroid = 1; centroid <= k; centroid++) {

        curr_vec->cords = initialize_vector(d);


        if (centroid < k) {
            curr_vec->next = malloc(sizeof(struct vector));
            if (curr_vec->next == NULL)
            {
                free_malloc(head_vec);
                return NULL;
                printf("An Error Has Occurred");
            }
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;
        } else {
            curr_vec->next = NULL;
        }
    }

    return head_vec;
}

void copy_C_to_C(struct cord* cord1, struct cord* cord2, int len) {

    struct cord *curr_cord1 = cord1;
    struct cord *curr_cord2 = cord2;
    int i;
    for (i = 1; i <= len; i++) {
        curr_cord1->value = curr_cord2->value;
        curr_cord1 = curr_cord1->next;
        curr_cord2 = curr_cord2->next;
    }
}

void add_C_to_C(struct cord* cord1, struct cord* cord2, int len) {

    struct cord *curr_cord1 = cord1;
    struct cord *curr_cord2 = cord2;
    int i;
    for (i = 1; i <= len; i++) {
        curr_cord1->value += curr_cord2->value;
        curr_cord1 = curr_cord1->next;
        curr_cord2 = curr_cord2->next;
    }
}
void divide_C(struct cord* cord, int div, int len) {

    struct cord *curr = cord;
    int i;
    for (i = 1; i <= len; i++) {
        curr->value /= div;
        curr = curr->next;
    }

}


void set_centroids(struct vector* centroids, struct vector* vec, int k, int len) {
    struct vector *curr_centroid = centroids;
    struct vector *curr_vec = vec;
    int i;

    for (i = 1; i <= k; i++) {

        copy_C_to_C(curr_centroid->cords, curr_vec->cords, len);
        curr_centroid = curr_centroid->next;
        curr_vec = curr_vec->next;
    }
}

void assign_closest_centroid(struct vector* vec, struct vector* centroids, int k, int d)
{
    struct vector *curr_centroid = centroids;
    struct vector *closest_centroid = centroids;
    int i;
    double curr_distance;
    double min_distance;



    min_distance = euclideanDistance(vec->cords, centroids->cords,d);

    for (i = 1; i <= k; i++)
    {

        curr_distance = euclideanDistance(vec->cords, curr_centroid->cords, d);

        if(curr_distance<min_distance)
        {
            min_distance = curr_distance;
            closest_centroid = curr_centroid;
        }

        curr_centroid = curr_centroid->next;

    }
    vec->closest = closest_centroid;
}


int main(int argc, char **argv) {
    int k;
    int iter;
    int i;
    struct vector *head_vec;
    struct vector *curr_vec;
    struct cord *head_cord;
    struct cord *curr_cord;
    struct vector *curr_centroid;
    struct cord *avg_cord;

    int N;
    int d;
    double n;
    char c;
    struct vector *centroids;

    int points_count;
    int checked_points;
    int centroids_count;
    int count;
    float max_change;
    float EPSILON= 0.001;

    k = atoi(argv[1]);

    iter = 200;
    if(argc == 3)
    {
        iter = atoi(argv[2]);
    }

    head_vec = malloc(sizeof(struct vector));
    if (head_vec == NULL)
    {
        printf("An Error Has Occurred");
        return 1;
    }  
    curr_vec = head_vec;


    head_cord = malloc(sizeof(struct cord));
    if (head_cord == NULL) {
        printf("An Error Has Occurred");
        return 1;
        }
    curr_cord = head_cord;
    curr_cord->next = NULL;
    curr_vec->next = NULL;

    N = 0;
    d = 0;

    while (scanf("%lf%c", &n, &c) == 2) {
        curr_cord->value = n;
        if (c == '\n') {
            d++;
            N++;
            curr_vec->cords = head_cord;
            curr_vec->next = malloc(sizeof(struct vector));
            if (curr_vec->next == NULL)
            {
                free_malloc(head_vec);
                return 1;
            }
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;
            head_cord = malloc(sizeof(struct cord));
            if (head_cord == NULL)
            {
                free_malloc(head_vec);
                return 1;
            }
            curr_cord = head_cord;
            curr_cord->next = NULL;
        } else {
            d++;
            curr_cord->next = malloc(sizeof(struct cord));
            if (curr_cord->next == NULL)
            {
                free_malloc(head_vec);
                printf("An Error Has Occurred");
                return 1;
            }
            curr_cord = curr_cord->next;
            curr_cord->next = NULL;
        }
    }
    d/=N;


    if (k <= 1 || k >= N || k != floor(k)) {
        printf("Invalid number of clusters!\n");
        exit(1);
    }

    if (iter <= 1 || iter >= 1000 || iter != floor(iter)) {
        printf("Invalid maximum iteration!\n");
        exit(1);
    }


    centroids = initialize_centroids(k, d);
    if (centroids == NULL)
    {
        free_malloc(head_vec);
        printf("An Error Has Occurred");
        return 1;
    }

    set_centroids(centroids, head_vec, k, d);
    curr_vec = head_vec;
    curr_centroid = centroids;

    max_change = EPSILON+1;

    for (i = 0; i<=iter; i++)
    {
        if(max_change <= EPSILON)
        {
            break;
        }
        max_change = 0;

        curr_vec = head_vec;

        for(points_count = 1; points_count<=N;points_count++)
        {            
            assign_closest_centroid(curr_vec, centroids, k, d);
            curr_vec = curr_vec->next;
        }

        curr_centroid = centroids;
        curr_vec = head_vec;

        for(centroids_count = 1; centroids_count<=k; centroids_count++)
        {
            avg_cord = initialize_vector(d);
            if (avg_cord == NULL)
            {
                free_malloc(head_vec);
                free_malloc(centroids);
                printf("An Error Has Occurred");
                return 1;
            }
            count = 0;
            curr_vec = head_vec;

            for (checked_points = 1; checked_points <= N; checked_points++)
            {
                if(curr_vec->closest == curr_centroid)
                {
                    add_C_to_C(avg_cord, curr_vec->cords, d);
                    count++;
                }
                curr_vec=curr_vec->next;
            }
            if (count>0)
            {
                divide_C(avg_cord, count, d);
                max_change = max(max_change, euclideanDistance(curr_centroid->cords,avg_cord, d));
                copy_C_to_C(curr_centroid->cords, avg_cord, d);
            }
            curr_vec = curr_vec->next;
            curr_centroid = curr_centroid->next;
        }
    }

    curr_centroid = centroids;

    for (centroids_count = 1; centroids_count <= k; centroids_count++)
    {
        printVector(curr_centroid);
        curr_centroid = curr_centroid->next;
    }
    

    printf(";%f\n",head_vec->cords->value);

    curr_vec = head_vec;
    free_malloc(head_vec);


    free_malloc(centroids);

    printVector(centroids);
    printf("------b\n");

    printVector(head_vec->next);
    printf("***\n");
    printCord(centroids->cords);



    printf("\n\n\n");

    return 0;
}

