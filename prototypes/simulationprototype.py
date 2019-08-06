import time
import random
import matplotlib.pyplot as plt

def main():

    print(time.ctime(time.time()))
    print(time.ctime(time.mktime(time.strptime("30 NOV " + str(2000), "%d %b %Y"))))
    SetUpStocks(2015)


def SetUpStocks(end_date):

    stocks = []
    for i in range(10):
        stocks.append(Stocks( 100.0*random.random(), random.randint(1970, 1990), end_date))
        stocks[i].SimulateStockToElapsedTime()

class Stocks:

    def __init__(self, starting_price, start_date, end_date):

        self.starting_price = starting_price
        self.start_date     = time.mktime(time.strptime("1 JAN " + str(start_date), "%d %b %Y"))
        self.end_date       = time.mktime(time.strptime("1 JAN " + str(end_date), "%d %b %Y"))
        self.prices         = []
        self.timestamp      = []

    
    def SimulateStockToElapsedTime(self):

        current_time = self.start_date
        while current_time < self.end_date:
            self.prices.append(self.GetStockPrice(current_time))
            self.timestamp.append(current_time)
            current_time += 3600

    def GetStockPrice(self, current_time):

        last_price = self.GetLastPrice()

        return last_price + self.GetRandomFluctuations(last_price) + self.GetRandomEvent(last_price) + self.GetConfidence(last_price) + self.GetMisc(last_price)

    def GetRandomFluctuations(self, last_price):

        return self.GetRandomSign()*random.random()

    def GetRandomEvent(self, last_price):

        return 0

    def GetConfidence(self, last_price):

        return 0

    def GetMisc(self, last_price):

        return 0

    def GetRandomSign(self):

        if random.randint(1, 2) == 1:
            return -1
        return 1

    def GetLastPrice(self):

        if len(self.prices) == 0:
            return self.starting_price
        else:
            return self.prices[-1]

if __name__ == '__main__':
    import sys
    sys.exit(main())