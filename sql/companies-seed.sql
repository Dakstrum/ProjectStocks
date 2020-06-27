

INSERT INTO
    Category (CategoryName)
VALUES
    ('Video Games'),
    ('Aerospace'),
    ('Computer Software');

INSERT INTO 
    Company (IPO, CompanyName, CompanyAbbreviation, CategoryId) 
VALUES
    (25.0, 'WeBeHard', 'WBH', 1),
    (35.0, 'Unimpressive Games', 'UG', 1),
    (20.0, 'WhySpace', 'YSP', 2),
    (162.0, 'Interstellar Business Robots', 'IBR', 3);

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
	(4, 'Space A.I (Whatever that means)');
