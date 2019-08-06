import time
import random
from statistics import mean
import matplotlib.pyplot as plt

def main():

    print(time.ctime(time.time()))
    print(time.ctime(time.mktime(time.strptime("30 NOV " + str(2000), "%d %b %Y"))))
    SetUpStocks(1991)


def SetUpStocks(end_date):

    stocks = []
    for i in range(10):
        stocks.append(Stocks( random.randint(1, 200)*random.random(), random.randint(1970, 1990), end_date))
        stocks[i].SimulateStockToElapsedTime()
    plt.plot(stocks[0].prices)
    plt.show()

class Stocks:

    def __init__(self, starting_price, start_date, end_date):

        self.starting_price           = starting_price
        self.start_date               = time.mktime(time.strptime("1 JAN " + str(start_date), "%d %b %Y"))
        self.end_date                 = time.mktime(time.strptime("1 JAN " + str(end_date), "%d %b %Y"))
        self.prices                   = []
        self.timestamp                = []
        self.random_event             = 0
        self.last_confidence_price    = self.starting_price
        self.last_confidence_modifier = 0
        self.last_confidence_time     = self.start_date

    
    def SimulateStockToElapsedTime(self):

        current_time = self.start_date
        while current_time < self.end_date:
            self.prices.append(self.GetStockPrice(current_time))
            self.timestamp.append(current_time)
            current_time += 3600

    def GetStockPrice(self, current_time):

        last_price = self.GetLastPrice()

        return last_price + self.GetRandomFluctuations(last_price) + self.GetRandomEvent(last_price) + self.GetConfidence(current_time, last_price) + self.GetMisc(last_price)

    def GetRandomFluctuations(self, last_price):

        return self.GetRandomSign()*random.random()

    def GetRandomEvent(self, last_price):

        self.random_event += self.GetRandomSign()*random.random() + .1
        if self.random_event < 0:
            self.random_event = 0

        if self.random_event > random.randint(50, 75):
            return last_price*self.GetRandomSign()*random.random()*self.GetRandomEventMagnitude()*.5

        return 0

    def GetConfidence(self, current_time, last_price):

        three_mounhs_approx = 7889400
        six_mounths_approx  = 15778800
        random_time         = random.randint(three_mounhs_approx, six_mounths_approx)

        if current_time - self.last_confidence_time >= random_time:
            self.last_confidence_time     = current_time
            self.last_confidence_modifier = self.GetNewConfidenceModifier()
            self.last_confidence_price    = mean(self.prices)

        return self.last_confidence_modifier

    def GetNewConfidenceModifier(self):

        if mean(self.prices) > self.last_confidence_price:
            return random.random()
        else:
            return -1 * random.random()           


    def GetMisc(self, last_price):

        return 0

    def GetRandomEventMagnitude(self):

        rand = random.randint(1, 3)
        if rand == 1:
            return 0.25
        elif rand == 2:
            return 0.5
        else:
            return 0.75

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