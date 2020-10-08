
INSERT INTO 
	System_GlobalEvents (Event, PriceModifier, ModifierLength)
VALUES
	('War was declared! Between whom? Who knows!', .1, 10),
	('Alien Invasion has begun! Will the chickens nugget survive?', -.2, 20),
	('Global warming has displaced millions of chickens. They have migrated north in the great, ChickenPocalype.', -.15, 5);

INSERT INTO
	System_CategoryEvents (CategoryId, Event, PriceModifier, ModifierLength)
VALUES
	(1, 'Due to the recent pandemic, video game sales have surged!', .15, 10),
	(2, 'Rocket fuel has skyrocketed in prices, literally!', -.15, 10),
	(3, 'Broken software sales have spiked! Buy your buggy computer crap today!', .05, 5),
	(4, 'Who can afford insurance anymore?', 0.10, 5),
	(5, 'News companys at war? All news companys are hard at work reporting on each other!', 0.10, 5),
	(6, 'hardware conflicts', 0.10, 5);

INSERT INTO
	System_CompanyEvents (CompanyId, Event, PriceModifier, ModifierLength)
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

	(10, 'AnoyScooters has shown a fast and efficient(but a little risky) way of transporting the elderly by themselves!',            .15, 15),
	(10, 'AnoyScooters has been trying to keep its recent electrical fires in the dark',                                             -.15, 15),
	(10, 'AnoyScooters release its new Scooter ''''The Anoyr'''' sparking rage in citizens that live in cities with its loud noise', -.15, 15),
	(10, 'AnoyScooters product used in a spree of robberies. These easy to use scooters zoomed in between law enforcement.',         -.15, 15),
	(10, 'AnoyScooters has updated their software after controversy about electrical failure.',                                       .15, 15),

	(11, 'SunSuc has partnered with WhySpace to further their advancements in space.',            .15, 15),
	(11, 'SunSucs most recent miner has been engulfed by our sun and has been destroyed',        -.15, 15),
	(11, 'SunSucs HQ has been surrounded by Sun conservationists demanding the CEO resign. ',    -.15, 15),
	(11, 'SunSucs most recent solar panel design shows promise and future government contracts.', .15, 15),
	(11, 'SunSucs rocket lauch crashes down into small town causing worry of future launches',   -.15, 15),

	(12, 'The BackUp did a recent 1 month bunker test resulting in 30 healthy humans to become schizophrenic.', -.15, 15),
	(12, 'The BackUp has received government contracts to build bunkers that only take in government employees.', .15, 15);