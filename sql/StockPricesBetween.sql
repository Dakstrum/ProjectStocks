

/*

    Good for all time.

    Shows values in increment of 6 months.

*/

DROP TABLE IF EXISTS TEMP;
CREATE TABLE TEMP (Name VARCHAR(10),MonthVal INTEGER, HourVal INTEGER, DayVal INTEGER);
INSERT INTO TEMP (Name, MonthVal, HourVal, DayVal) VALUES
('After',strftime('%m', '1995-01-01 00:00:00'), strftime('%H', '1995-01-01 00:00:00'), strftime('%d', '1995-01-01 00:00:00'));
SELECT 
    SP.Price,
    SP.Time
FROM 
    StockPrices SP 
INNER JOIN 
    Company C ON C.CompanyId = SP.CompanyId AND C.CompanyName = 'WeBeHard'
WHERE 
        SP.Time BETWEEN '1969-12-31 18:00:00' AND '1995-01-01 00:00:00'
    AND strftime('%H', SP.Time) = (SELECT HourVal FROM TEMP WHERE Name='After' LIMIT 1)
    AND strftime('%d', SP.Time) = (SELECT DayVal FROM TEMP WHERE Name='After' LIMIT 1)
    AND ABS(strftime('%m', SP.Time) - (SELECT MonthVal FROM TEMP WHERE Name='After' LIMIT 1)) IN (0, 6);

/*

    Good for 2 years and 5 years.

*/
DROP TABLE IF EXISTS TEMP;
CREATE TABLE TEMP (Name VARCHAR(10), HourVal INTEGER, DayVal INTEGER);
INSERT INTO TEMP (Name, HourVal, DayVal) VALUES ('After', strftime('%H', '1995-01-01 00:00:00'), strftime('%d', '1995-01-01 00:00:00'));
SELECT 
    SP.Price,
    SP.Time
FROM 
    StockPrices SP 
INNER JOIN 
    Company C ON C.CompanyId = SP.CompanyId AND C.CompanyName = 'WeBeHard'
WHERE 
        SP.Time BETWEEN '1969-12-31 18:00:00' AND '1995-01-01 00:00:00'
    AND strftime('%H', SP.Time) = (SELECT HourVal FROM TEMP WHERE Name='After' LIMIT 1)
    AND strftime('%d', SP.Time) = (SELECT DayVal FROM TEMP WHERE Name='After' LIMIT 1);


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