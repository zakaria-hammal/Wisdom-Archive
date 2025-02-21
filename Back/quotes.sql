DROP TABLE IF EXISTS "Quotes";

CREATE TABLE "Quotes" (
    "QuoteId" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    "Name" nvarchar (100) NULL,
    "Country" nvarchar (100) NULL,
    "QuoteBody" TEXT NOT NULL
);


INSERT INTO Quotes (Name, Country, QuoteBody) VALUES ("Confucius(Kong Qiu)", "China", "Our greatest glory is not in never falling, but in rising every time we fall");
INSERT INTO Quotes (Name, Country, QuoteBody) VALUES ("Oscar Wilde", "Irland", "Be yourself; everyone else is already taken");
INSERT INTO Quotes (Name, Country, QuoteBody) VALUES ("Mahatma Gandhi", "India", "Be the change that you wish to see in the world");
INSERT INTO Quotes (Name, Country, QuoteBody) VALUES ("Marcus Aurelius", "Rome", "The soul becomes dyed with the color of its thoughts");
INSERT INTO Quotes (Name, Country, QuoteBody) VALUES ("Dr. Seuss", "USA", "You know you're in love when you can't fall asleep because reality is finally better than your dreams");