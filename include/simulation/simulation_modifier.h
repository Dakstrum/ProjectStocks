#ifndef SIMULATION_MODIFIER_H
#define SIMULATION_MODIFIER_H

typedef struct SimulationModifier {

    uint32_t company_id;
    time_t played_time;
    float price_modifier;
    uint32_t modifier_length;

} SimulationModifier;


void simulation_modifiers_init(Vector *current_companies);
Vector *simulation_get_modifiers();
time_t simulation_modifier_get_endtime(SimulationModifier *modifier);
void simulation_reset_modifiers();
void simuation_remove_old_modifiers(time_t t);
void simulation_modify_company(uint32_t company_id, time_t play_time, float modifier, uint32_t days, char *event);
void simulation_modify_category(uint32_t category_id, time_t play_time, float modifier, uint32_t days, char *event);
void simulation_modify_global(float modifier, time_t play_time, uint32_t days, char *event);
void simulation_apply_transaction(int transaction_amount, uint32_t company_id, time_t transaction_time);
void simulation_apply_card(uint32_t player_id, uint32_t card_id, uint32_t company_id, time_t play_time);
bool simulation_negative_modifiers_active(uint32_t company_id, time_t t);
bool simulation_positive_modifiers_active(uint32_t company_id, time_t t);

#endif