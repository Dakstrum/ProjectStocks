#ifndef JSONLAYER_H
#define JSONLAYER_H

typedef struct Company {

    char *company_name;
    char *category;
    double ipo;
    
    char *description;
    char *start_date;
    char **products;

} Company;


void *JsonEntry(ALLEGRO_THREAD *thread, void *arg);
void CleanUpJson();

#endif