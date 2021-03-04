#ifndef COMPANY_UTILS_H
#define COMPANY_UTILS_H

#include <stdint.h>

#include "vector.h"

Vector *company_utils_get_random(uint32_t amount);
Vector *company_utils_get_lowest(uint32_t amount);
Vector *company_utils_get_mean(uint32_t amount);
Vector *company_utils_get_all_active();
Vector *company_utils_remove_duplicates(Vector *companies);

#endif