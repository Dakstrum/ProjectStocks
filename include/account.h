#ifndef ACCOUNT_H
#define ACCOUNT_H

void InitAccount();
// UpdateSaveRecord();
// GetAllSaves();
// SetSaveToLoad(int save_id);
unsigned int GetSaveSeed();
int GetSaveId();
void CreateNewSave(char *save_name, char *player_name);
void Save();
void LoadSave(int load_save_id, int save_player_id);
int GetCurrentPlayerId();
// SaveTransaction(int company_id, int stocks_exchanged, double transaction_amount);
// GetAllTransactionsBetween(time_t start_date_time, time_t end_date_time);
// GetAllTransactions();
// GetAllTransactionsForCompany(int company_id);
// SaveCurrentSaveSeed();

void TogglePauseGameTime();
void SetGameTime(time_t time_to_set);

time_t GetGameTime();
char *GetDate();

void SetInGameStatus(int status);
int GetInGameStatus();

void SetGameSpeed(const int speed);
void SetCompanyIdViewing(int id);
int GetCompanyIdViewing();
void SetAccountMoney(float amount);
float GetAccountMoney();
char *GetCompanyNameViewing();

char *GetCurrentSaveName();
char *GetCurrentPlayerName();

#endif