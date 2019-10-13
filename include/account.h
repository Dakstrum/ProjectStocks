#ifndef ACCOUNT_H
#define ACCOUNT_H

void InitAccount();
// UpdateSaveRecord();
// GetAllSaves();
// SetSaveToLoad(int save_id);
unsigned int GetSaveSeed();
int GetSaveId();
// SaveTransaction(int company_id, int stocks_exchanged, double transaction_amount);
// GetAllTransactionsBetween(time_t start_date_time, time_t end_date_time);
// GetAllTransactions();
// GetAllTransactionsForCompany(int company_id);
// 

void TogglePauseGameTime();
void SetGameTime(time_t time_to_set);

time_t GetGameTime();

#endif