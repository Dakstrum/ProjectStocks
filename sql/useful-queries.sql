
-- Inserting a player entry for AI's
INSERT INTO Game_Players (SaveId, PlayerName, Money, SaveOwner)
SELECT 
	1,
	(SELECT FirstName FROM System_AIFirstnames ORDER BY RANDOM() LIMIT 1) || ' ' || (SELECT LastName FROM System_AILastNames ORDER BY RANDOM() LIMIT 1),
	15000,
	0


-- Inserting a player entry for AI's, using WITH clauses (Read only tables that are used for that transaction).
INSERT INTO Game_Players (SaveId, PlayerName, Money, SaveOwner)
WITH AINames as 
(
	SELECT * FROM (SELECT 'First' as Type, FirstName as Name FROM System_AIFirstnames ORDER BY RANDOM() LIMIT 1)
	UNION
	SELECT * FROM (SELECT 'Last' as Type, LastName as Name FROM System_AILastNames ORDER BY RANDOM() LIMIT 1)
)
SELECT 
	1,
	FirstName.Name || ' ' || LastName.Name,
	15000,
	0
FROM 
	AINames FirstName
INNER JOIN 
	AINames Lastname ON LastName.Type <> FirstName.Type
WHERE 
	FirstName.Type = 'First'
LIMIT 1;