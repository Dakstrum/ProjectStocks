CREATE TABLE IF NOT EXISTS "System_AIFirstNames" (
	"FirstNameId"	INTEGER,
	"FirstName"	VARCHAR(32) NOT NULL
);
CREATE TABLE IF NOT EXISTS "System_AILastNames" (
	"LastNameId"	INTEGER,
	"LastName"	VARCHAR(32) NOT NULL
);
CREATE TABLE IF NOT EXISTS "System_Cards" (
	"CardId"	INTEGER,
	"CardName"	VARCHAR(128) NOT NULL,
	"CardDesc"	VARCHAR(256) NOT NULL,
	"CardPath"	VARCHAR(256) NOT NULL,
	"PriceModifier"	DOUBLE NOT NULL,
	"ModifierLength"	INT NOT NULL
);
CREATE TABLE IF NOT EXISTS "System_Category" (
	"CategoryId"	INTEGER,
	"CategoryName"	VARCHAR(30)
);
CREATE TABLE IF NOT EXISTS "System_CategoryEvents" (
	"CategoryEventId"	INTEGER,
	"CategoryId"	INT NOT NULL,
	"Event"	VARCHAR(128) NOT NULL,
	"PriceModifier"	DOUBLE NOT NULL,
	"ModifierLength"	INT NOT NULL
);
CREATE TABLE IF NOT EXISTS "System_CompanyEvents" (
	"CompanyEventId"	INTEGER,
	"CompanyId"	INT NOT NULL,
	"Event"	VARCHAR(128) NOT NULL,
	"PriceModifier"	DOUBLE NOT NULL,
	"ModifierLength"	INT NOT NULL
);
CREATE TABLE IF NOT EXISTS "System_CompanyIcons" (
	"CompanyIcon"	INTEGER,
	"CompanyId"	INTEGER NOT NULL,
	"IconPath"	VARCHAR(256) NOT NULL
);
CREATE TABLE IF NOT EXISTS "System_GlobalEvents" (
	"GlobalEventId"	INTEGER,
	"Event"	VARCHAR(128) NOT NULL,
	"PriceModifier"	DOUBLE NOT NULL,
	"ModifierLength"	INT NOT NULL
);
CREATE TABLE IF NOT EXISTS "System_CompanyProducts" (
	"CompanyProductId"	INTEGER,
	"CompanyId"	INT NOT NULL,
	"ProductName"	VARCHAR(40)
);
CREATE TABLE IF NOT EXISTS "System_Company" (
	"CompanyId"	INTEGER,
	"CategoryId"	INTEGER NOT NULL,
	"Ipo"	DOUBLE NOT NULL,
	"CompanyName"	VARCHAR(50) NOT NULL,
	"CompanyDescription"	VARCHAR(128) NOT NULL,
	"CompanyAbbreviation"	VARCHAR(5) NOT NULL,
	"CompanyStartTime"	INT NOT NULL
);
INSERT INTO "System_AIFirstNames" ("FirstNameId","FirstName") VALUES (1,'Julia'),
 (2,'Kyle'),
 (3,'Lea'),
 (4,'John'),
 (5,'Charlie'),
 (6,'Amelia'),
 (7,'Arista'),
 (8,'Hadrian'),
 (9,'Juliette'),
 (10,'Lukas'),
 (11,'Ada'),
 (12,'Tomie'),
 (13,'Ben'),
 (14,'Persephone'),
 (15,'asdasd'),
 (16,'Lee'),
 (17,'sdasd');
INSERT INTO "System_AILastNames" ("LastNameId","LastName") VALUES (1,'Muffin'),
 (2,'Kip'),
 (3,'Smith'),
 (4,'John'),
 (5,'Knightly'),
 (6,'Wong'),
 (7,'Beckett'),
 (8,'Hills'),
 (9,'Johns'),
 (10,'Want'),
 (11,'Dover'),
 (12,'Cummings'),
 (13,'Apple'),
 (14,'Orange');
INSERT INTO "System_Cards" ("CardId","CardName","CardDesc","CardPath","PriceModifier","ModifierLength") VALUES (1,'Deadly Product','Spread false narrative about a deadly product from this company.','assets/images/cards/deadlyproduct.png',-0.5,10),
 (2,'Fake Positive Earnings','Show positive company earnings','assets/images/cards/Temp.png',0.3,5),
 (3,'Fake News','Hire a fake news studio to push out positive articles about this company','assets/images/cards/fakenews.png',0.4,2);
INSERT INTO "System_Category" ("CategoryId","CategoryName") VALUES (1,'Video Games'),
 (2,'Aerospace'),
 (3,'Software'),
 (4,'Insurance'),
 (5,'News'),
 (6,'Hardware');
INSERT INTO "System_CategoryEvents" ("CategoryEventId","CategoryId","Event","PriceModifier","ModifierLength") VALUES (1,1,'Due to the recent pandemic, video game sales have surged!',0.15,10),
 (2,2,'Rocket fuel has skyrocketed in prices, literally!',-0.15,10),
 (3,3,'Broken software sales have spiked! Buy your buggy computer crap today!',0.05,5),
 (4,4,'Who can afford insurance anymore?',0.1,5),
 (5,5,'News companys at war? All news companys are hard at work reporting on each other!',0.1,5),
 (6,6,'hardware conflicts',0.1,5);
INSERT INTO "System_CompanyEvents" ("CompanyEventId","CompanyId","Event","PriceModifier","ModifierLength") VALUES (1,1,'WeBeHard: CEO has gotten soft on running his company.',-0.2,10),
 (2,2,'Unimpressive Games: Has released an epic game? What?',-0.1,10),
 (3,3,'WhySpace: Has affirmed that we need not good to space and that Earth has all the pollution we need!',0.05,5),
 (4,4,'Interstellar Business Robots: Wait, we still exist?',-0.1,5),
 (5,5,'Pear Event',-0.2,5),
 (6,6,'There has been a recent surge in sword related violence',0.15,15),
 (7,7,'K3 News Event',0.15,15),
 (8,8,'Ferrit News Event',0.15,15),
 (9,9,'AnoyScooters has shown a fast and efficient(but a little risky) way of transporting the elderly by themselves!',0.15,15),
 (10,9,'AnoyScooters has been trying to keep its recent electrical fires in the dark',-0.15,15),
 (11,9,'AnoyScooters release its new Scooter ''''The Anoyr'''' sparking rage in citizens that live in cities with its loud noise',-0.15,15),
 (12,9,'AnoyScooters has updated their software after controversy about electrical failure.',0.15,15),
 (13,10,'The BackUp did a recent 1 month bunker test resulting in 30 healthy humans to become schizophrenic.',-0.15,15),
 (14,10,'The BackUp has received government contracts to build bunkers that only take in government employees.',0.15,15);
INSERT INTO "System_CompanyIcons" ("CompanyIcon","CompanyId","IconPath") VALUES (1,1,'assets/images/companyicons/webehard.png'),
 (2,2,'assets/images/companyicons/unimpressivegames.png'),
 (3,3,'assets/images/companyicons/whyspace.png'),
 (4,4,'assets/images/companyicons/webehard.png'),
 (5,5,'assets/images/companyicons/pear.png'),
 (6,6,'assets/images/companyicons/webehard.png'),
 (7,7,'assets/images/companyicons/webehard.png'),
 (8,8,'assets/images/companyicons/webehard.png'),
 (9,9,'assets/images/companyicons/webehard.png'),
 (10,10,'assets/images/companyicons/webehard.png');
INSERT INTO "System_GlobalEvents" ("GlobalEventId","Event","PriceModifier","ModifierLength") VALUES (1,'War was declared! Between whom? Who knows!',0.1,10),
 (2,'Alien Invasion has begun! Will the chickens nugget survive?',-0.2,20);
INSERT INTO "System_CompanyProducts" ("CompanyProductId","CompanyId","ProductName") VALUES (1,1,'Naon'),
 (2,1,'Backdoor Belief'),
 (3,2,'One Day Morning'),
 (4,2,'Screws of Peace'),
 (5,3,'Rat 1 Rocket'),
 (6,3,'Bird Space Shuttle'),
 (7,4,'Demonic A.I'),
 (8,5,'Pear Phone'),
 (9,6,'Katana Package'),
 (10,7,'EVRYNBC Premium Subscription'),
 (11,8,'Baked News Premium Subscription'),
 (12,9,'Anoyance 5000 Electic Scooter'),
 (13,10,'Peasant Package'),
 (14,10,'Middle Class Package'),
 (15,10,'Elite Package');
INSERT INTO "System_Company" ("CompanyId","CategoryId","Ipo","CompanyName","CompanyDescription","CompanyAbbreviation","CompanyStartTime") VALUES (1,1,25.0,'WeBeHard','WeBeHard is a video game company. Their rise to fame was from publishing extremely difficult games causing controversial rage among players','WBH',0),
 (2,1,35.0,'Unimpressive Games','Unimpressive Games Inc is a video game company. Being founded in Tom Swetner''s mother''s basement has since grown.','UG',0),
 (3,2,20.0,'WhySpace','Who wants to go into space? WhySpace''s objective is to develop low costing rockets to further the expansion into space. ','YSP',0),
 (4,3,162.0,'Interstellar Business Robots','Research company devoted to interstellar automation and robots.','IBR',0),
 (5,3,30.0,'Pear','Designs and develops consumer electronics. Known for the Pear phones and computers that are sold at a premium.','PEAR',0),
 (6,4,3.0,'Sword Violence Insurance','With the rise of sword collecting Justin Yimber has sought out to protect those and their bodies from damage.','SWI',0),
 (7,5,12.0,'EVRYNBC','EVRYNBC DESC','NBC',0),
 (8,5,45.0,'Baked News','Baked News','FTN',0),
 (9,6,45.0,'AnoyScooters','Electric Scooters that are rented in cities across the world','ANOY',0),
 (10,4,45.0,'TheBackUp',' ''''Its not if you fuck up, its when'''' says TheBackUps CEO. Specializing in underground bunkers for when disaster strikes','TBU',0);