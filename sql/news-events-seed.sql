
INSERT INTO 
	GlobalEvents (Event, PriceModifier, ModifierLength)
VALUES
	('War was declared! Between whom? Who knows!', .1, 10),
	('Alien Invasion has begun! Will the chickens nugget survive?', -.2, 20),
	('Global warming has displaced millions of chickens. They have migrated north in the great, ChickenPocalype.', -.15, 5);

INSERT INTO
	CategoryEvents (CategoryId, Event, PriceModifier, ModifierLength)
VALUES
	(1, 'Due to the recent pandemic, video game sales have surged!', .15, 10),
	(2, 'Rocket fuel has skyrocketed in prices, literally!', -.15, 10),
	(3, 'Broken software sales have spiked! Buy your buggy computer crap today!', .05, 5),
	(4, 'Who can afford insurance anymore?', 0.10, 5),
	(5, 'News companys at war? All news companys are hard at work reporting on each other!', 0.10, 5),
	(6, 'hardware conflicts', 0.10, 5);

INSERT INTO
	CompanyEvents (CompanyId, Event, PriceModifier, ModifierLength)
VALUES
	(1, 'WeBeHard: CEO has gotten soft on running his company.', -.20, 10),
	(2, 'Unimpressive Games: Has released an epic game? What?', -.10, 10),
	(3, 'WhySpace: Has affirmed that we need not good to space and that Earth has all the pollution we need!', .05, 5),
	(4, 'Interstellar Business Robots: Wait, we still exist?', -.10, 5),
	(5, 'Pear Event', -.20, 5),
	(6, 'A recent study finds back right tires often hit nails', -.20, 5),
	(7, 'There has been a recent surge in sword related violence', .15, 15),
	(8, 'K3 News Event', .15, 15),
	(9, 'Ferrit News Event', .15, 15),
	(10, 'Annoy Scooters Event', .15, 15),
	(11, 'Sun Suc Event', .15, 15),
	(12, 'The BackUp Event', .15, 15);