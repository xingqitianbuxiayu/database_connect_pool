DROP DATABASE testdb;
CREATE DATABASE testdb;

USE testdb;

CREATE TABLE person
(
    id      int         NOT NULL AUTO_INCREMENT,
    name    char(50)    NOT NULL,
    sex     char(50)    NOT NULL,
    age     int         NOT NULL,
    PRIMARY KEY(id)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;

insert into person values(1, 'Alice', 'female', 20);
insert into person values(2, 'Jack', 'male', 25);