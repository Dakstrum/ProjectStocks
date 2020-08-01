

INSERT INTO
    Category (CategoryName)
VALUES
    ('Video Games'),
    ('Aerospace'),
    ('Computer Software'),
    ('Insurance'),
    ('News');

INSERT INTO 
    Company (IPO, CompanyName, CompanyDescription, CompanyAbbreviation, CategoryId) 
VALUES
    (25.0,  'WeBeHard', 'A video game company that produces impossible to beat video games.', 'WBH', 1),
    (35.0,  'Unimpressive Games', 'A video game company that often times sells medicore rated games.', 'UG', 1),
    (20.0,  'WhySpace', 'Who wants to go into space? Here at YSpace, we focus on delivering low cost rockets.' , 'YSP', 2),
    (162.0, 'Interstellar Business Robots', 'Delivering all your intergalactic automation needs.', 'IBR', 3),
    (15.0,  'Left Back Tire Insurance', 'We insure your back left tire.', 'LBTI', 4),
    (3.0,   'Sword Violence Insurance', 'Insurance for when you get impaled by a sword.', 'SWI', 4),
    (12.0,  'K3 News', 'A competition driven news provider ran by a lethargic CEO', 'KTN', 5);

INSERT INTO
	CompanyProducts (CompanyId, ProductName)
VALUES
	(1, 'Naon'),
	(1, 'Backdoor Belief'),
	(2, 'One Day Morning'),
	(2, 'Screws of Peace'),
	(2, 'Very Real Tournament'),
	(3, 'Rat 1 Rocket'),
	(3, 'Bird Space Shuttle'),
	(4, 'Demonic A.I'),
	(4, 'Space A.I (Whatever that means)'),
	(5, 'Insurance'),
	(6, 'Insurnace'),
	(7, 'K3 Premium Subscription');
