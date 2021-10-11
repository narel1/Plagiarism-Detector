#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hashmap.h"
#include<dirent.h>


int main(int argc, char *argv[]) {
    char *input;
    FILE *fp;
    long lSize;
    char *buffer;
    DIR *d;
    struct dirent *dir;
    char array[NUMBER_OF_FILES][50];
    strcpy(array[0], argv[1]);
    d = opendir(argv[2]);
    int i = 1;
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            char s[50];
            strcpy(s, argv[2]);
            if (dir->d_name[0] != '.') {
                strcat(s, dir->d_name);
                strcpy(array[i], s);
                // printf("%s -> %s  %d\n",array[i] , s, i);
                i++;
            }
        }
        closedir(d);
    }


    struct hashmap *m = (struct hashmap *) malloc(sizeof(hashmap));
    m->init_hashmap = init_hashmap;
    m->init_hashmap(m);


    int arr[10 * DEFAULT_SIZE];
    int iter = 0;

    double size_measure[NUMBER_OF_FILES];
    int count = 0;
    // for(int i=0; i<NUMBER_OF_FILES; i++){
    //         printf("%s\n", array[i]);
    //     }
    for (int i = 0; i < NUMBER_OF_FILES; i++) {
        // char *string[10000];
        // printf("1\n");
        // printf("%s\n",array[i] );
        // printf("2\n");
        fp = fopen(array[i], "rb");
        if (!fp) perror(array[i]), exit(1);

        fseek(fp, 0L, SEEK_END);
        lSize = ftell(fp);
        rewind(fp);

        buffer = calloc(1, lSize + 1);
        if (!buffer) fclose(fp), fputs("memory alloc fails", stderr), exit(1);

        if (1 != fread(buffer, lSize, 1, fp))
            fclose(fp), free(buffer), fputs("entire read fails", stderr), exit(1);


        char delimit[] = " \t\r\n\v\f,?.:'";
        // int i = 0, j = 0;


        input = strtok(buffer, delimit);
        count++;
        while (input != NULL) {
            // printf("string [%d]=%s\n",i,input);
            // printf("%s\n","DEBUG_STATMENT 1" );
            if (m->containsElement(input, m, i)) {
                // printf("%s\n","DEBUG_STATMENT 2" );
                int p = m->getElement(input, m, i);
                // printf("%s\n","DEBUG_STATMENT 3" );
                p++;
                m->updateElement(input, p, m, i);
                // printf("%s -> %d\n",input, p);
                // printf("%s\n","DEBUG_STATMENT 4" );
            } else {
                int store = m->addElement(input, 1, m, i);
                // printf("%s indexes to %d\n", input, store);
                // printf("%s\n","DEBUG_STATMENT 2 1");
                arr[iter] = store;
                iter++;
            }

            // i++;
            // printf("%s\n", "Result" );
            count++;
            input = strtok(NULL, delimit);
            if (input == NULL) {
                size_measure[i] = count;
                count = 0;
            }

        }


    }
    // printf("%d\n", 1 );
    bool brr[DEFAULT_SIZE];
    for (int i = 0; i < DEFAULT_SIZE; i++)
        brr[i] = false;


    double final_arr[NUMBER_OF_FILES][iter];
    for (int i = 0; i < iter; i++) {
        if (!brr[arr[i]]) {
            struct hashMapElement *e = m->Array[arr[i]];
            while (e != NULL) {
                for (int j = 0; j < NUMBER_OF_FILES; j++) {
                    final_arr[j][i] = e->value[j];
                    // if(j == 24){
                    //     printf("%s  %d\n", e->key, e->value[j]);
                    // }
                }
                e = e->next;
            }
            brr[arr[i]] = true;
        }
    }

    // for (int i = 0; i < NUMBER_OF_FILES; ++i)
    // {
    //     printf("%lf\n",size_measure[i] );
    // }

    double tf[NUMBER_OF_FILES][iter];
    double idf[NUMBER_OF_FILES][iter];

    for (int i = 0; i < NUMBER_OF_FILES; i++) {
        for (int j = 0; j < iter; j++) {
            if (final_arr[i][j] < 100.0) {
                tf[i][j] = final_arr[i][j] / size_measure[i];
            } else {
                tf[i][j] = 0.0;
            }
            // if(i==24){
            //     printf("%lf %lf %lf\n", final_arr[24][j], size_measure[i], tf[24][j]);
            // }
        }
    }

    for (int j = 0; j < iter; j++) {
        double count = 0.0;
        for (int i = 0; i < NUMBER_OF_FILES; i++) {
            // printf("final_arr[%d][%d] = %lf\n",i , j , final_arr[i][j]);
            if (final_arr[i][j] > 0.0) {
                count = count + 1.0;
            }
        }
        // printf("%lf\n", count);
        for (int i = 0; i < NUMBER_OF_FILES; i++) {
            if (final_arr[i][j] > 0 && count > 0 && final_arr[i][j] < 100) {
                idf[i][j] = log(NUMBER_OF_FILES / count);
            } else {
                idf[i][j] = 0.0;
            }
        }
    }

    double final_weight[NUMBER_OF_FILES][iter];
    double square_sum[NUMBER_OF_FILES];

    for (int i = 0; i < NUMBER_OF_FILES; i++) {
        square_sum[i] = 0.0;
    }
    for (int i = 0; i < NUMBER_OF_FILES; i++) {
        for (int j = 0; j < iter; j++) {
            final_weight[i][j] = tf[i][j] * idf[i][j];
            square_sum[i] = square_sum[i] + final_weight[i][j] * final_weight[i][j];
        }
        // if(j == 24){
        //     printf("%s  %d\n", e->key, e->value[j]);
        // }
    }
    for (int i = 0; i < iter; i++) {
        // printf("%lf %lf %lf\n", tf[24][i], idf[24][i], final_weight[24][i]);
    }

    for (int i = 0; i < NUMBER_OF_FILES; i++) {
        square_sum[i] = sqrt(square_sum[i]);
    }
    // printf("%lf\n", square_sum[24]);

    double numerator[NUMBER_OF_FILES - 1];
    for (int i = 0; i < NUMBER_OF_FILES - 1; i++) {
        numerator[i] = 0.0;
    }

    for (int i = 1; i < NUMBER_OF_FILES; i++) {
        for (int j = 0; j < iter; j++) {
            numerator[i - 1] = numerator[i - 1] + final_weight[0][j] * final_weight[i][j];
        }
    }

    double denominator[NUMBER_OF_FILES - 1];
    for (int i = 0; i < NUMBER_OF_FILES - 1; i++) {
        denominator[i] = square_sum[0] * square_sum[i + 1];
    }


    // for(int i=0; i<24; i++){
    //     printf("%lf %lf\n",numerator[i],denominator[i] );
    // }


    for (int i = 0; i < NUMBER_OF_FILES - 1; i++) {
        printf("%s %lf\n", array[i + 1], numerator[i] / denominator[i]);
    }


    free(buffer);
    free(input);
    fclose(fp);
}