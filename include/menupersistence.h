#ifndef MENUPERSISTENCE_H
#define MENUPERSISTENCE_H

typedef enum MENUS {

	ACCOUNT,
	CARDS,
	MAIN,
	NEWS,
	OPTIONS,
	PAUSE,
	SAVE,
	STOCKS

} MENUS;

void SetCompanyIdViewing(int id);
int GetCompanyIdViewing();
char *GetCompanyNameViewing();

void SetAccountHistoryDisplayNum(int num);
int GetAccountHistoryDisplayNum();

void SetStockMenuTimeSpanNum(TimeSpan time_span);
TimeSpan GetStockMenuTimeSpanNum();

void persistence_set_current_menu(MENUS menu);
MENUS persistence_get_current_menu();

#endif