
INSERT INTO users
  (id, email,                        name,            tickets,     birthday, systemMessage)
VALUES 
  (1,  'green@org',                  'Alex Green',          0, '1986-04-06', ''),
  (2,  'test@user',                  'Test User',           0, '1986-04-07', ''),
  (3,  'Kuzya@TheCat.org',           'Moon Light Cat',      0, '2011-09-15', ''),
  (4,  'Gomer@simpsons.org',         'Gomer Simpson',       0, '1986-04-08', ''),
  (5,  'BlueBeard@pirates.org',      'Blue Beard',          0, '1985-01-02', ''),
  (6,  'DedMoroz@newyear.party.ice', 'Ded Moroz',           0, '1984-07-03', ''),
  (7,  'YellouPukki@zloy.com',       'YellouPukki',         0, '1984-03-02', ''),
  (8,  'mr.president@gov',           'Mr. President',       0, '1988-12-22', ''),
  (9,  'sunflower@gmail.org',        'Jessica Smith',       0, '1986-11-15', ''),
  (10, 'John_Doe@unknown.person',    'John Doe',            0, '1975-07-18', ''),
  (11, 'ufo@gmail.com',              'UnknownFlyingObject', 0, '1989-03-25', ''),
  (12, 'say_me_hello@universe',      'World',               0, '1984-02-05', '');

INSERT INTO events
  (id, date,         name,                     price, rating, auditorium)
VALUES
  (1,  '2016-02-22', 'Terminator 1',           100,   1,      1),
  (2,  '2016-02-23', 'Terminator 2',           200,   2,      2),
  (3,  '2016-02-24', 'How I met Your Mom',     50,    1,      3),
  (4,  '2016-02-25', 'X Files',                120,   0,      4),
  (5,  '2016-02-26', 'Chronicles of Shannara', 20,    1,      4);

INSERT INTO auditoriums
  (id, name,           seats, vipSeats)
VALUES
  (1,  'Green Room',   30,    '10,11,12'),
  (2,  'Red Room',     90,    '10,11,12'),
  (3,  'Blue Room',    120,   '10,11,12'),
  (4,  'Marsala Room', 10,    '8,9,10');

INSERT INTO tickets
  (id, user, event, price, date, seat)
VALUES
  (1,     1,     1,    50, '2016-02-22', 5),
  (2,     2,     1,    50, '2016-02-22', 6),
  (3,     3,     1,    50, '2016-02-22', 7),
  (4,     4,     1,    75, '2016-02-22', 8);
