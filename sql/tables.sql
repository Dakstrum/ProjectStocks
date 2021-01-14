
CREATE TABLE IF NOT EXISTS Game_CompanyMetadata
(
    CompanyMetaId INTEGER PRIMARY KEY, 
    CompanyId INT NOT NULL, 
    SaveId INT NOT NULL, 
    TotalEmployees INT NOT NULL, 
    StocksInMarket UNSIGNED BIG INT, 
    PriceModifier DOUBLE DEFAULT(0.0), 
    EntryTime DATETIME NOT NULL
);

CREATE TABLE IF NOT EXISTS Game_Saves
(
    SaveId INTEGER PRIMARY KEY, 
    SaveName VARCHAR(20) NOT NULL, 
    TimeSpentInGame UNSIGNED BIG INT DEFAULT(0), 
    RandomSeed UNSIGNED BIG INT
);

CREATE TABLE IF NOT EXISTS Game_Players
(
    PlayerId INTEGER PRIMARY KEY, 
    SaveId INT NOT NULL, 
    PlayerName VARCHAR(64) NOT NULL, 
    Money DOUBLE NOT NULL, 
    SaveOwner INT NOT NULL DEFAULT(0)
);

CREATE TABLE IF NOT EXISTS Game_Settings
(
    SettingsId INTEGER PRIMARY KEY, 
    WindowWidth UNSIGNED INT NOT NULL, 
    WindowHeight UNSIGNED INT NOT NULL, 
    WindowStyle UNSIGNED INT NOT NULL,
    FPS DOUBLE NOT NULL,
    FullScreen UNSIGNED INT NOT NULL
);

CREATE TABLE IF NOT EXISTS Player_Transactions
(
    TransactionId INTEGER PRIMARY KEY, 
    PlayerId INT NOT NULL, 
    CompanyId INT NOT NULL, 
    TransactionAmount DOUBLE NOT NULL, 
    StocksExchanged INT NOT NULL, 
    TransactionTime DATETIME NOT NULL
);

CREATE TABLE IF NOT EXISTS Player_Cards
(
    PlayerCardId INTEGER PRIMARY KEY,
    PlayerId INTEGER NOT NULL,
    SaveId INTEGER NOT NULL,
    CardId INTEGER NOT NULL,
    Played BIT NOT NULL DEFAULT(0)
);

CREATE TABLE IF NOT EXISTS Player_CardsPlayed
(
    PlayerCardsPlayedId INTEGER PRIMARY KEY,
    CardId INTEGER NOT NULL,
    SaveId INTEGER NOT NULL,
    CompanyId INTEGER NOT NULL,
    PlayedTime DATETIME NOT NULL
);

CREATE TABLE IF NOT EXISTS DB_Events
(
    DBEventId INTEGER PRIMARY KEY,
    Event VARCHAR(128) NOT NULL
);

CREATE TABLE IF NOT EXISTS System_Company
(
    CompanyId INTEGER PRIMARY KEY,
    CategoryId INTEGER NOT NULL, 
    Ipo DOUBLE NOT NULL, 
    CompanyName VARCHAR(50) NOT NULL,
    CompanyDescription VARCHAR(128) NOT NULL,
    CompanyAbbreviation VARCHAR(5) NOT NULL
);

CREATE TABLE IF NOT EXISTS System_CompanyIcons
(
    CompanyIcon INTEGER PRIMARY KEY,
    CompanyId INTEGER NOT NULL,
    IconPath VARCHAR(256) NOT NULL
);

CREATE TABLE IF NOT EXISTS System_CompanyProducts
(
    CompanyProductId INTEGER PRIMARY KEY,
    CompanyId INT NOT NULL,
    ProductName VARCHAR(40)
);

CREATE TABLE IF NOT EXISTS System_GlobalEvents
(
    GlobalEventId INTEGER PRIMARY KEY,
    Event VARCHAR(128) NOT NULL,
    PriceModifier DOUBLE NOT NULL,
    ModifierLength INT NOT NULL
);

CREATE TABLE IF NOT EXISTS System_Category
(
    CategoryId INTEGER PRIMARY KEY,
    CategoryName VARCHAR(30)
);

CREATE TABLE IF NOT EXISTS System_CategoryEvents
(
    CategoryEventId INTEGER PRIMARY KEY,
    CategoryId INT NOT NULL,
    Event VARCHAR(128) NOT NULL,
    PriceModifier DOUBLE NOT NULL,
    ModifierLength INT NOT NULL
);

CREATE TABLE IF NOT EXISTS System_CompanyEvents
(
    CompanyEventId INTEGER PRIMARY KEY,
    CompanyId INT NOT NULL,
    Event VARCHAR(128) NOT NULL,
    PriceModifier DOUBLE NOT NULL,
    ModifierLength INT NOT NULL
);

CREATE TABLE IF NOT EXISTS System_AIFirstNames
(
    FirstNameId INTEGER PRIMARY KEY,
    FirstName VARCHAR(32) NOT NULL
);

CREATE TABLE IF NOT EXISTS System_AILastNames
(
    LastNameId INTEGER PRIMARY KEY,
    LastName VARCHAR(32) NOT NULL
);

CREATE TABLE IF NOT EXISTS System_Cards
(
    CardId INTEGER PRIMARY KEY,
    CardName VARCHAR(128) NOT NULL,
    CardDesc VARCHAR(256) NOT NULL,
    CardPath VARCHAR(256) NOT NULL,
    PriceModifier DOUBLE NOT NULL,
    ModifierLength INT NOT NULL
);
