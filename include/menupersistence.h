#ifndef MENUPERSISTENCE_H
#define MENUPERSISTENCE_H

void SetCompanyIdViewing(int id);
int GetCompanyIdViewing();
char *GetCompanyNameViewing();

void SetAccountHistoryDisplayNum(int num);
int GetAccountHistoryDisplayNum();

void SetStockMenuTimeSpanNum(TimeSpan time_span);
TimeSpan GetStockMenuTimeSpanNum();

#endif