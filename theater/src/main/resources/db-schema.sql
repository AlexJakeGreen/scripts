
CREATE TABLE users (
  id INT PRIMARY KEY AUTO_INCREMENT NOT NULL,
  email VARCHAR(64) NOT NULL,
  name VARCHAR(64) NOT NULL,
  tickets INT(8) NOT NULL,
  birthday TIMESTAMP NOT NULL,
  systemMessage VARCHAR(255) NOT NULL
);

CREATE TABLE events (
  id INT PRIMARY KEY AUTO_INCREMENT NOT NULL,
  date TIMESTAMP NOT NULL,
  name VARCHAR(255) NOT NULL,
  price INT NOT NULL,
  rating INT NOT NULL,
  auditorium INT
);

CREATE TABLE tickets (
  id INT PRIMARY KEY AUTO_INCREMENT NOT NULL,
  user INT NOT NULL,
  event INT NOT NULL,
  price INT NOT NULL,
  date TIMESTAMP NOT NULL,
  seat INT NOT NULL
);

CREATE TABLE auditoriums (
  id INT PRIMARY KEY AUTO_INCREMENT NOT NULL,
  name VARCHAR(64) NOT NULL,
  seats INT NOT NULL,
  vipSeats VARCHAR(255) NOT NULL
);
   
CREATE TABLE counters (
  id INT PRIMARY KEY AUTO_INCREMENT NOT NULL,
  name VARCHAR(64) NOT NULL,
  type VARCHAR(16) NOT NULL,
  key INT NOT NULL,
  value INT DEFAULT 0
);
