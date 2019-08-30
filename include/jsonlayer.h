#ifndef JSONLAYER_H
#define JSONLAYER_H

typedef struct Company {

    char *company_name;
    char *category;
    float ipo;
    
    char *description;
    char *start_date;
    char **products;

} Companies;


void *JsonEntry(ALLEGRO_THREAD *thread, void *arg);

#endif