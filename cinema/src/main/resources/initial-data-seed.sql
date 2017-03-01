-- noinspection SqlResolveForFile

INSERT INTO users
(id, email,                          username, password, roles, enabled, name,            tickets,     birthday, systemMessage)
VALUES
  (1,  'green@org',                  'green',  '7110eda4d09e062aa5e4a390b0a572ac0d2c0220', 'ROLE_ADMIN',  TRUE, 'Alex Green',          0, '1986-04-06', ''),
  (2,  'test@user',                  'test',   '7110eda4d09e062aa5e4a390b0a572ac0d2c0220', 'ROLE_USER',   TRUE, 'Test User',           0, '1986-04-07', ''),
  (3,  'Kuzya@TheCat.org',           'cat',    '7110eda4d09e062aa5e4a390b0a572ac0d2c0220', 'ROLE_USER,ROLE_ADMIN',   TRUE, 'Moon Light Cat',      0, '2011-09-15', ''),
  (4,  'Homer@simpsons.org',         'homer',  '7110eda4d09e062aa5e4a390b0a572ac0d2c0220', 'ROLE_USER',   TRUE, 'Homer Simpson',       0, '1986-04-08', ''),
  (5,  'BlueBeard@pirates.org',      'bbeard', '7110eda4d09e062aa5e4a390b0a572ac0d2c0220', 'ROLE_USER',   TRUE, 'Blue Beard',          0, '1985-01-02', ''),
  (6,  'DedMoroz@newyear.party.ice', 'dm',     '7110eda4d09e062aa5e4a390b0a572ac0d2c0220', 'ROLE_USER',   TRUE, 'Ded Moroz',           0, '1984-07-03', ''),
  (7,  'YellouPukki@zloy.com',       'ypp',    '7110eda4d09e062aa5e4a390b0a572ac0d2c0220', 'ROLE_USER',   TRUE, 'YellouPukki',         0, '1984-03-02', ''),
  (8,  'mr.president@gov',           'mnp',    '7110eda4d09e062aa5e4a390b0a572ac0d2c0220', 'ROLE_USER',   TRUE, 'Mr. President',       0, '1988-12-22', ''),
  (9,  'sunflower@gmail.org',        'sun',    '7110eda4d09e062aa5e4a390b0a572ac0d2c0220', 'ROLE_USER',   TRUE, 'Jessica Smith',       0, '1986-11-15', ''),
  (10, 'John_Doe@unknown.person',    'john',   '7110eda4d09e062aa5e4a390b0a572ac0d2c0220', 'ROLE_USER',   TRUE, 'John Doe',            0, '1975-07-18', ''),
  (11, 'ufo@gmail.com',              'dog',    '7110eda4d09e062aa5e4a390b0a572ac0d2c0220', 'ROLE_USER',   TRUE, 'UnknownFlyingObject', 0, '1989-03-25', ''),
  (12, 'say_me_hello@universe',      'tom',    '7110eda4d09e062aa5e4a390b0a572ac0d2c0220', 'ROLE_USER',   TRUE, 'World',               0, '1984-02-05', '');

INSERT INTO events
(id, date,         name,                     price, auditorium_id, image_url)
VALUES
  (1,  '2016-02-22', 'The Dark Knight',           100,      1, '1.jpg'),
  (2,  '2016-02-23', 'It`s a Wonderful Life',           200,      2, '2.jpg'),
  (3,  '2016-02-24', 'A Beautiful Mind',     50,       3, '3.jpg'),
  (4,  '2016-02-25', 'Wild Strawberries',                120,      4, '4.jpg'),
  (5,  '2016-02-26', 'Kill Bill', 20,       4, '5.jpg'),
  (6,  '2016-02-23', 'Blood Diamond',           200,      2, '6.jpg'),
  (7,  '2016-02-23', 'Apocalypse Now',           200,      2, '7.jpg'),
  (8,  '2016-02-23', 'One Flew Over The Cockoo`s Nest',           200,      2, '8.jpg'),
  (9,  '2016-02-23',  'The Godfather',           200,      2, '9.jpg'),
  (10,  '2016-02-23', 'Modern Times',           200,      2, '10.jpg'),
  (11,  '2016-02-23', 'Seven Samurai',           200,      2, '11.jpg'),
  (12,  '2016-02-23', 'The Shawshank Redemption',           200,      2, '12.jpg');

INSERT INTO auditoriums
(id, name,           seats, vipSeats)
VALUES
  (1,  'Green Room',   30,    '10,11,12'),
  (2,  'Red Room',     90,    '10,11,12'),
  (3,  'Blue Room',    120,   '10,11,12'),
  (4,  'Marsala Room', 10,    '8,9,10');

INSERT INTO tickets
(id, user_id, event_id, price, date, seat)
VALUES
  (1,     1,     1,    50, '2016-02-22', 5),
  (2,     2,     1,    50, '2016-02-22', 6),
  (3,     3,     1,    50, '2016-02-22', 7),
  (4,     4,     1,    75, '2016-02-22', 8);

INSERT INTO user_accounts
(id, user_id, money_prepaid)
VALUES
  (1, 1, 1000),
  (2, 2, 555),
  (3, 3, 333),
  (4, 4, 200),
  (5, 5, 600),
  (6, 6, 750),
  (7, 7, 210),
  (8, 8, 360),
  (9, 9, 50),
  (10, 10, -10),
  (11, 11, 0),
  (12, 12, 15);
