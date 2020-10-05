

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
    (25.0,  'WeBeHard',                     'WeBeHard is a video game company. Their rise to fame was from publishing extremely difficult games causing controversial rage among players',  'WBH',  1),
    (35.0,  'Unimpressive Games',           'Unimpressive Games Inc is a video game company. Being founded in Tom Swetner''s mother''s basement has since grown.',                          'UG',   1),
    (20.0,  'WhySpace',                     'Who wants to go into space? WhySpace''s objective is to develop low costing rockets to further the expansion into space. ',                    'YSP',  2),
    (162.0, 'Interstellar Business Robots', 'Research company devoted to interstellar automation and robots.',                                                                              'IBR',  3),
    (30.0,  'Pear',                         'Designs and develops consumer electronics. Known for the Pear phones and computers that are sold at a premium.',                               'PEAR', 3),
    (15.0,  'Left Back Tire Insurance',     'Startup founded by Jim Roger a formerly homeless man, This business boasts a 75% reduction in traditional car insurance.',                     'LBTI', 4),
    (3.0,   'Sword Violence Insurance',     'With the rise of sword collecting Justin Yimber has sought out to protect those and their bodies from damage.',                                'SWI',  4),
    (12.0,  'K3 News',                      'News company reporting on drama, alliances, failures, and flourishing of companies traded on the stock market.',                               'KTN',  5),
    (45.0,  'Ferrit News',                  'News company is known for its aggression toward other news providers and aggressive techniques in reporting news.',                            'FTN',  5);

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
    (5, 'Pear Phone'),
	(6, 'Premium Back Left Tire Package'),
    (7, 'Katana Package'),
	(8, 'K3 Premium Subscription');
    (8, 'Ferrit News Premium Subscription');
