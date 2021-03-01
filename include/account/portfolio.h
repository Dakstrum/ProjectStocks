#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <stdint.h>

float portfolio_get_networth(uint32_t player_id);
float portfolio_get_percentage(uint32_t player_id, uint32_t company_id);

#endif