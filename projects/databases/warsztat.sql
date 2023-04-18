CREATE TABLE samochod (
  id_sam INT(4) PRIMARY KEY,
  marka VARCHAR(12) NOT NULL,
  wartosc INT(8) NOT NULL
);

CREATE TABLE usterka (
  id_ust INT(4) PRIMARY KEY,
  czesc VARCHAR(12) NOT NULL,
  procent_zniszczenia INT(4) NOT NULL,
  koszt_naprawy INT(7) NOT NULL,
  sam INT(4) REFERENCES samochod 
);

CREATE TABLE mechanik (
  id_mech INT(4) PRIMARY KEY,
  imie VARCHAR(12) NOT NULL,
  nazwisko VARCHAR(12) NOT NULL,
  kompetencje INT(4) NOT NULL
);

CREATE TABLE umiejetnosc (
  id_um INT(4) PRIMARY KEY,
  czesc VARCHAR(12) NOT NULL,
  mech INT(4) REFERENCES mechanik
);

INSERT INTO samochod VALUES (1, 'BMW', 50000);
INSERT INTO samochod VALUES (2, 'Skoda', 25000);
INSERT INTO samochod VALUES (3, 'Mazda', 13400);
INSERT INTO samochod VALUES (4, 'Mercedes', 420000);
INSERT INTO samochod VALUES (5, 'BMW', 134000);
INSERT INTO samochod VALUES (6, 'Citroen', 74000);
INSERT INTO samochod VALUES (7, 'Opel', 45300);
INSERT INTO samochod VALUES (8, 'Fiat', 111000);
INSERT INTO samochod VALUES (9, 'Mercedes', 230000);
INSERT INTO samochod VALUES (10, 'BMW', 178999);

INSERT INTO usterka VALUES(1, 'silnik', 40, 3500, 1);
INSERT INTO usterka VALUES(2, 'hamulce', 50, 1800, 1);
INSERT INTO usterka VALUES(3, 'swiatla', 60, 2200, 2);
INSERT INTO usterka VALUES(4, 'klimatyzacja', 20, 600, 3);
INSERT INTO usterka VALUES(5, 'sprzeglo', 30, 1600, 3);
INSERT INTO usterka VALUES(6, 'silnik', 50, 24000, 4);
INSERT INTO usterka VALUES(7, 'swiatla', 30, 4400, 4);
INSERT INTO usterka VALUES(8, 'klimatyzacja', 60, 9600, 4);
INSERT INTO usterka VALUES(9, 'klimatyzacja', 50, 1600, 5);
INSERT INTO usterka VALUES(10, 'silnik', 30, 2600, 6);
INSERT INTO usterka VALUES(11, 'hamulce', 70, 1100, 6);
INSERT INTO usterka VALUES(12, 'sprzeglo', 60, 5600, 7);
INSERT INTO usterka VALUES(13, 'silnik', 50, 13000, 8);
INSERT INTO usterka VALUES(14, 'swiatla', 60, 9700, 9);
INSERT INTO usterka VALUES(15, 'silnik', 60, 17700, 9);
INSERT INTO usterka VALUES(16, 'hamulce', 20, 3800, 10);

INSERT INTO mechanik VALUES(1, 'Jan', 'Kowalski', 60);
INSERT INTO mechanik VALUES(2, 'Andrzej', 'Nowak', 40);
INSERT INTO mechanik VALUES(3, 'Marcin', 'Jablonski', 70);
INSERT INTO mechanik VALUES(4, 'Anna', 'Makota', 80);
INSERT INTO mechanik VALUES(5, 'Jakub', 'Bukaj', 50);

INSERT INTO umiejetnosc VALUES(1, 'silnik', 1);
INSERT INTO umiejetnosc VALUES(2, 'silnik', 3);
INSERT INTO umiejetnosc VALUES(3, 'silnik', 4);
INSERT INTO umiejetnosc VALUES(4, 'hamulce', 4);
INSERT INTO umiejetnosc VALUES(5, 'swiatla', 2);
INSERT INTO umiejetnosc VALUES(6, 'swiatla', 3);
INSERT INTO umiejetnosc VALUES(7, 'swiatla', 5);
INSERT INTO umiejetnosc VALUES(8, 'klimatyzacja', 1);
INSERT INTO umiejetnosc VALUES(9, 'klimatyzacja', 5);
INSERT INTO umiejetnosc VALUES(10, 'sprzeglo', 2);
INSERT INTO umiejetnosc VALUES(11, 'sprzeglo', 5);

COMMIT;
