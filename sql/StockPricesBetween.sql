
/*
Approximate Method. Relatively Fast

Will filter out records by week, with the hour being either 18 or 19

Good for 3 Month, 6 Month, 1 Year
*/
SELECT 
	SP.Price,
	SP.Time
FROM 
	StockPrices SP 
INNER JOIN 
	Company C ON C.CompanyId = SP.CompanyId AND C.CompanyName = 'WeBeHard'
WHERE 
	    SP.Time BETWEEN '1969-12-31 18:00:00' AND '1995-01-01 00:00:00'
	AND (SP.StockPriceId - 1) % 168 = 0

/*
Exact Method. Slow

Will filter out records by week, with the hour being 0.

NOTE: NOT WORKING

Good for 3 Month, 6 Month, 1 Year
*/
SELECT 
	SP.Price,
	SP.Time
FROM 
	StockPrices SP 
INNER JOIN 
	Company C ON C.CompanyId = SP.CompanyId AND C.CompanyName = 'WeBeHard'
INNER JOIN
	(SELECT Time FROM StockPrices WHERE (StockPriceId - 1) % 168 = 0) SPTime ON DATE(SPTime.Time) = DATE(SP.Time)
WHERE 
	    SP.Time BETWEEN '1969-12-31 18:00:00' AND '1995-01-01 00:00:00' 
	AND SP.Time = (SELECT MIN(Time) FROM StockPrices WHERE CompanyId = C.CompanyId AND DATE(Time) = DATE(SPTime.Time) LIMIT 1)
	
	
/*	
Approximate Method. Relatively Fast

Will filter out records by day, with the hour being 18 or 19.

Good for Week, 2 weeks, 1 month
*/
SELECT 
	SP.Price,
	SP.Time
FROM 
	StockPrices SP 
INNER JOIN 
	Company C ON C.CompanyId = SP.CompanyId AND C.CompanyName = 'WeBeHard'
WHERE 
	    SP.Time BETWEEN '1969-12-31 18:00:00' AND '1995-01-01 00:00:00'
	AND (SP.StockPriceId - 1) % 24 = 0	

/*	
Exact Method. Slow

Will filter out records by day, with the hour being 0 or whatever is the minimum value for that day.

Good for Week, 2 weeks, 1 month
*/
SELECT
	SP.Price,
	MIN(SP.Time)
FROM
	StockPrices SP
INNER JOIN 
	Company C ON C.CompanyId=SP.CompanyId AND C.CompanyName='WeBeHard'
WHERE
	SP.Time BETWEEN '1969-12-31 18:00:00' AND '1995-01-01 00:00:00'
GROUP BY
	SP.CompanyId, DATE(SP.Time)
