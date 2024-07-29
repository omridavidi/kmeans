#include <stdio.h>
#include <math.h>
#include <stdlib.h>

struct cord {
    double value;
    struct cord *next;
};

struct vector
{
    int serial;
    struct vector *next;
    struct cord *cords;
    int closest;
};


double max(double a, double b)
{
    if(a>b)
    {
        return a;
    }
    return b;
}


void freeVec(struct vector *vec)
{
    /*runs through each cord in each vector and frees the allocated memory*/
    struct vector *temp_vec;
    struct cord *temp_cord;
    struct vector *curr_vec;
    struct cord *curr_cord;

    int i = 0;

    curr_vec = vec;
    while (curr_vec != NULL) {
        temp_vec = curr_vec;
        curr_vec = curr_vec->next;
        curr_cord = temp_vec->cords;
        while (curr_cord != NULL)
        {
            temp_cord = curr_cord;
            curr_cord = curr_cord->next;
            free(temp_cord);
            printf("%d, ",i);
        }
        free(temp_vec);
    }
}


void freeCord(struct cord *cord)
{
    /*runs through each cord in certain vector and frees the allocated memory*/

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





void printVector(struct vector *vec) {
    /*prints a single vector*/

    struct cord *curr_cord = vec->cords;
    while (curr_cord != NULL) {
        if(curr_cord->next==NULL)
        {
            printf("%.4f", curr_cord->value);
        }
        else
        {
            printf("%.4f,", curr_cord->value);
        }
        curr_cord = curr_cord->next;
    }
    printf("\n");
}

void printcord(struct cord *curr_cord) {
    /*prints a single vector*/

    while (curr_cord != NULL) {
        if(curr_cord->next==NULL)
        {
            printf("%.4f", curr_cord->value);
        }
        else
        {
            printf("%.4f,", curr_cord->value);
        }
        curr_cord = curr_cord->next;
    }
    printf("\n");
}


void freeVec2(struct vector *vec)
{
    /*runs through each cord in each vector and frees the allocated memory*/
    struct vector *temp_vec;
    struct vector *curr_vec;
    struct cord *curr_cord;

    curr_vec = vec;
    while (curr_vec != NULL) {
        temp_vec = curr_vec;
        curr_cord = temp_vec->cords;
        freeCord(curr_cord);
        curr_vec = curr_vec->next;
        free(temp_vec);
    }

    printf("\n\n");

}




double euclideanDistance(struct cord *cord1, struct cord *cord2, int d)
{
    /*given two first cord of two vectors - calculates the euclidian distance between them*/

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
    /*initialize a d-dimensional vector*/

    struct cord *head_cord;
    struct cord *curr_cord;
    int iter_cord;

    head_cord = malloc(sizeof(struct cord));
    if (head_cord == NULL) {
        return NULL; /* when we return null the calling function will know we have failed allocating memory*/
    }
    curr_cord = head_cord;

    for (iter_cord = 1; iter_cord < d; iter_cord++) {
        curr_cord->next = malloc(sizeof(struct cord));
        if (curr_cord->next == NULL)
        {
            freeCord(head_cord);
            return NULL;
        }
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;
    }
    return head_cord;
}




struct vector* initialize_centroids(int k, int d) {
    /*initialize k d-dimensional centroids vector*/

    struct vector *head_vec = malloc(sizeof(struct vector));
    struct vector *curr_vec = head_vec;
    int i;
    
    if (head_vec == NULL)
    {
        return NULL; /* when we return null the calling function will know we have failed allocating memory*/
    }


    for (i = 1; i <= k; i++) {

        curr_vec->cords = initialize_vector(d);
        curr_vec->serial = i;
        if (curr_vec->cords == NULL)
        {
            freeVec2(head_vec);
            return NULL;
        }

        if (i < k) {
            curr_vec->next = malloc(sizeof(struct vector));
            if (curr_vec->next == NULL)
            {
                freeVec2(head_vec);
                return NULL;/* when we return null the calling function will know we have failed allocating memory*/
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
    /*given two len lengthed vector, copies the second's values to the first*/

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
    /*given two len lengthed vector, adds the second's values to the first*/

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
    /*given a len lengthed vector, divides each cord value by div*/

    struct cord *curr = cord;
    int i;
    for (i = 1; i <= len; i++) {
        curr->value /= div;
        curr = curr->next;
    }

}


void set_centroids(struct vector* centroids, struct vector* vec, int k, int len) {
    /*sets the centroids to be equal to the first k datapoints*/

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
    /*assigns each datapoint to the closest centroid (by the calculated euc distance)*/

    struct vector *curr_centroid = centroids;
    int closest_centroid = 1;
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
            closest_centroid = i;
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
    double max_change;
    double EPSILON= 0.001;



    k = atoi(argv[1]);

    if (atof(argv[1]) != floor(atof(argv[1]))) { /* checks if number is whole*/
        printf("Invalid number of clusters!\n");
        exit(1);
    }

    iter = 200;
    if(argc == 3)
    {
        iter = atoi(argv[2]);
        if (atof(argv[2]) != floor(atof(argv[2]))) /* checks if number is whole*/
        {
            printf("Invalid maximum iteration!\n");
            exit(1);    
        }
    }

    head_vec = malloc(sizeof(struct vector));
    if (head_vec == NULL)
    {
        printf("An Error Has Occurred");
        exit(1);
    }  
    curr_vec = head_vec;


    head_cord = malloc(sizeof(struct cord));
    if (head_cord == NULL) {
        printf("An Error Has Occurred");
        exit(1);
        }
    curr_cord = head_cord;
    curr_cord->next = NULL;
    curr_vec->next = NULL;

    N = 0;
    d = 0;

    while (scanf("%lf%c", &n, &c) == 2) { /*read datapoint input*/
        curr_cord->value = n;
        if (c == '\n') {
            d++;
            N++;
            curr_vec->cords = head_cord;
            curr_vec->next = malloc(sizeof(struct vector));
            if (curr_vec->next == NULL)
            {
                freeVec(head_vec);
                exit(1);
            }
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;
            head_cord = malloc(sizeof(struct cord));
            if (head_cord == NULL)
            {
                freeVec(head_vec);
                exit(1);
            }
            curr_cord = head_cord;
            curr_cord->next = NULL;
        } else {
            d++;
            curr_cord->next = malloc(sizeof(struct cord));
            if (curr_cord->next == NULL)
            {
                freeVec(head_vec);
                printf("An Error Has Occurred");
                exit(1);
            }
            curr_cord = curr_cord->next;
            curr_cord->next = NULL;
        }
    }
    d/=N;


    if (k <= 1 || k >= N) { /*checks k orderd limitations*/
        printf("Invalid number of clusters!\n");
        exit(1);
    }

    if (iter <= 1 || iter >= 1000) {/*checks iter orderd limitations*/
        printf("Invalid maximum iteration!\n");
        exit(1);
    }


    centroids = initialize_centroids(k, d);
    if (centroids == NULL)
    {
        freeVec(head_vec);
        printf("An Error Has Occurred");
        exit(1);
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
                freeVec(head_vec); /*frees memory before code termination*/
                freeVec(centroids);
                printf("An Error Has Occurred");
                exit(1);
            }
            count = 0;
            curr_vec = head_vec;


            for (checked_points = 1; checked_points <= N; checked_points++)
            {
                if(curr_vec->closest == centroids_count)
                {
                    add_C_to_C(avg_cord, curr_vec->cords, d);
                    count++;
                }
                curr_vec=curr_vec->next;
            }
            if (count>0)
            {
                divide_C(avg_cord, count, d); /*calculate avg point*/
                max_change = max(max_change, euclideanDistance(curr_centroid->cords,avg_cord, d));
                copy_C_to_C(curr_centroid->cords, avg_cord, d); 
            }
            curr_vec = curr_vec->next;
            curr_centroid = curr_centroid->next;
            freeCord(avg_cord);
        }
    }
    
    curr_centroid = centroids;

    for (centroids_count = 1; centroids_count <= k; centroids_count++) /*prints the final centroid value*/
    {
        printVector(curr_centroid);
        curr_centroid = curr_centroid->next;
    }
    
    curr_vec = head_vec;



    printf("\nhereeee1\n");
    printVector(head_vec);
    freeVec2(head_vec);
    printVector(head_vec);
    printf("hereeee1\n");


    printf("\nhereeee\n");
    printVector(centroids);
    freeVec2(centroids);
    printVector(centroids);
    printf("hereeee\n");




    return 0;
}