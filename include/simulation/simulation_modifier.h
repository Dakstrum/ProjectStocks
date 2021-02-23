#ifndef SIMULATION_MODIFIER_H
#define SIMULATION_MODIFIER_H


void simulation_modifiers_init(Vector *current_companies);
Vector *simulation_get_modifiers();
void simuation_remove_old_modifiers(time_t t);
void simulation_modify_company(uint32_t company_id, time_t play_time, float modifier, uint32_t days, char *event);
void simulation_modify_category(uint32_t category_id, time_t play_time, float modifier, uint32_t days, char *event);
void simulation_modify_global(float modifier, time_t play_time, uint32_t days, char *event);
void simulation_apply_transaction(int transaction_amount, uint32_t company_id, time_t transaction_time);

#endif