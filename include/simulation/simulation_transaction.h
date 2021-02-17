#ifndef SIMULATION_TRANSACTION_H
#define SIMULATION_TRANSACTION_H

#include <stdint.h>

float simulation_transaction_step(time_t t, float price, uint32_t company_id);

#endif