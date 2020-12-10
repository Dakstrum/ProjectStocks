#ifndef STOCKSMENU_H
#define STOCKSMENU_H

void StocksMenu_Init();

char *StocksMenu_CurrentCompanyFromGraph();
void StocksMenu_RenderLogic();
void StocksMenu_InitCompanyScrollbox();

void StocksMenu_SellMenu_BCB();
void StocksMenu_BuyMenu_BCB();

void StocksMenu_SellMenu_Sell_BCB();
void StocksMenu_BuyMenu_Buy_BCB();

void OneD_BCB();
void OneW_BCB();
void OneM_BCB();
void OneY_BCB();
void All_BCB();

void StocksMenu_Clean();

#endif