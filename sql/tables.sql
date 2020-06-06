CREATE TABLE IF NOT EXISTS Company
(
    CompanyId INTEGER PRIMARY KEY,
    CategoryId INTEGER NOT NULL, 
    Ipo DOUBLE NOT NULL, 
    CompanyName VARCHAR(50) NOT NULL
);

CREATE TABLE IF NOT EXISTS CompanyMetadata
(
    CompanyMetaId INTEGER PRIMARY KEY, 
    CompanyId INT NOT NULL, 
    SaveId INT NOT NULL, 
    TotalEmployees INT NOT NULL, 
    StocksInMarket UNSIGNED BIG INT, 
    PriceModifier DOUBLE DEFAULT(0.0), 
    EntryTime DATETIME NOT NULL
);

CREATE TABLE IF NOT EXISTS Players
(
    PlayerId INTEGER PRIMARY KEY, 
    SaveId INT NOT NULL, 
    PlayerName VARCHAR(20) NOT NULL, 
    Money DOUBLE NOT NULL, 
    SaveOwner INT NOT NULL DEFAULT(0)
);

CREATE TABLE IF NOT EXISTS Saves
(
    SaveId INTEGER PRIMARY KEY, 
    SaveName VARCHAR(20) NOT NULL, 
    TimeSpentInGame UNSIGNED BIG INT DEFAULT(0), 
    RandomSeed UNSIGNED BIG INT
);

CREATE TABLE IF NOT EXISTS OwnedStocks
(
    OwnedStockId INTEGER PRIMARY KEY, 
    SaveId INT NOT NULL, PlayerId INT NOT NULL, 
    CompanyId INT NOT NULL, 
    HowManyOwned UNSIGNED BIG INT NOT NULL
);

CREATE TABLE IF NOT EXISTS Transactions
(
    TransactionId INTEGER PRIMARY KEY, 
    SaveId INT NOT NULL, 
    PlayerId INT NOT NULL, 
    CompanyId INT NOT NULL, 
    TransactionAmount DOUBLE NOT NULL, 
    StocksExchanged INT NOT NULL, 
    TransactionTime DATETIME NOT NULL
);

CREATE TABLE IF NOT EXISTS Settings
(
    SettingsId INTEGER PRIMARY KEY, 
    WindowWidth UNSIGNED INT NOT NULL, 
    WindowHeight UNSIGNED INT NOT NULL, 
    WindowStyle UNSIGNED INT NOT NULL
);

CREATE TABLE IF NOT EXISTS CompanyProducts
(
    CompanyProductId INTEGER PRIMARY KEY,
    CompanyId INT NOT NULL,
    Name VARCHAR(40)
);

CREATE TABLE IF NOT EXISTS GlobalEvents
(
    GlobalEventId INTEGER PRIMARY KEY,
    Event VARCHAR(128) NOT NULL,
    PriceModifier DOUBLE NOT NULL
);

CREATE TABLE IF NOT EXISTS CategoryEvents
(
    CategoryEventId INTEGER PRIMARY KEY,
    CategoryId INT NOT NULL,
    Event VARCHAR(128) NOT NULL,
    PriceModifier DOUBLE NOT NULL
);

CREATE TABLE IF NOT EXISTS Category
(
    CategoryId INTEGER PRIMARY KEY,
    CategoryName VARCHAR(30)
);

CREATE TABLE IF NOT EXISTS CompanyEvents
(
    CompanyEventId INTEGER PRIMARY KEY,
    CompanyId INT NOT NULL,
    Event VARCHAR(128) NOT NULL,
    PriceModifier DOUBLE NOT NULL
);

CREATE TABLE IF NOT EXISTS DBEvents
(
    DBEventId INTEGER PRIMARY KEY,
    Event VARCHAR(128) NOT NULL
);
