https://github.com/bauman-robotics/PostgreSQL/blob/master/2023/infa.txt

http://84.252.143.212/phpinfo.php

http://84.252.143.212

Создать базу данных 
watter_bd 
Создать таблицу 
watter_tb
int cold
int hot
time 
Создать пользователя
watter_user //pas ******


//---------------------------------------------------------------
1.Войти в оболочку psql под именем postgress:
sudo -u postgres psql
-u - позволяет указать, от имени какого пользователя нужно выполнять программу;

2.Выйти из оболочки:
\q

3. Просмотр списка пользователей Postgress
\du

4. Список баз данных
\l

4.1	Список таблиц
\dt

5. CREATE ROLE имя;
CREATE USER watter_user;  

6. 
CREATE DATABASE watter_bd;

7.
GRANT ALL PRIVILEGES ON DATABASE watter_bd to watter_user;
GRANT ALL PRIVILEGES ON watter_tb TO "watter_user"; 

GRANT SELECT, UPDATE, INSERT ON ALL TABLES IN SCHEMA public TO "watter_user";
// !!!!!!!
GRANT pg_read_all_data TO watter_user;
GRANT pg_write_all_data TO watter_user;
//======
8. 
ALTER USER watter_user WITH PASSWORD '*****';

9. 
Теперь подключаемся к базе, к которой хотим дать доступ:

\c watter_bd

10.
CREATE TABLE watter_tb(
    user_id         varchar(80),
	user_location   varchar(80),
    cold            int,
    hot             int,
    alarm_time      int,
	created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE watter_tb(user_id varchar(80), user_location varchar(80), cold int, hot int, alarm_time int, created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP);

11. Просмотр таблицы 
SELECT * FROM watter_tb;

13. Удаление таблицы 
DROP TABLE watter_tb;

14. Вставка строки 
INSERT INTO watter_tb VALUES ('Arkhan', 'Home',1, 2, 1930);
INSERT INTO SensorData (value1) VALUES (1);

15. Удалить всё из таблицы 
DELETE FROM watter_tb; 

16.  Удалить базу данных 
DROP DATABASE database1 WITH (FORCE);

17. Скрипты PHP path
cd /var/www/html

18. Подключение к базе данных
psql -U watter_user watter_bd -h127.0.0.1

//=========
find -name  esp-data.php
./PHP/PHP_read_DB/esp-data.php

/home/arkhan/PHP/PHP_read_DB/esp-data.php

/home/arkhan/PHP/PHP_watter_scripts/esp_watter_tb_read.php


//=======================================
http://84.252.143.212/watter_read.php
http://84.252.143.212/test_post_watter.php

//  http://84.252.143.212/watter_post.php 
curl --data "api_key=******&user_id=NoName&user_location=NoPlace&cold=10&hot=11&alarm_time=1140" http://127.0.0.1/watter_post.php

curl --data "api_key=******" http://127.0.0.1/watter_clear.php

http://84.252.143.212/watter_clear_force.php 
//=======================================

nano /etc/apache2/apache2.conf

sudo cp esp_watter_tb_read.php /var/www/html/

sudo systemctl restart apache2
//===========
php esp-data.php
test_post_watter.php
watter_post.php  // need curl 
watter_clear_force.php  
//===========
header("Content-Type: text/plain; charset=utf-8");
echo "one ...\n";

CREATE TABLE watter_tb(
	line_num        serial,
    user_id         varchar(80),
	user_location   varchar(80),
    cold            int,
    hot             int,
    alarm_time      int,
	created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);


SELECT * FROM watter_tb;
INSERT INTO test_tb VALUES ('Arkhan', 'Home',1, 2, 1930);
INSERT INTO test_tb (user_id) VALUES ('Arkhan');

DROP TABLE watter_tb;
//=============
sudo systemctl restart apache2
sudo -u postgres psql
//=============

GRANT ALL PRIVILEGES ON DATABASE watter_bd to watter_user;
GRANT ALL PRIVILEGES ON watter_tb TO "watter_user";  // ???
GRANT SELECT, UPDATE, INSERT ON ALL TABLES IN SCHEMA public TO "watter_user";
 
INSERT INTO watter_tb (cold, hot) VALUES (1, 2);
 
/etc/postgresql/14/main


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
GRANT pg_read_all_data TO watter_user;
GRANT pg_write_all_data TO watter_user;
https://www.dmosk.ru/miniinstruktions.php?mini=postgresql-users


UPDATE watter_tb set new_id = new_id + 1000;
UPDATE watter_tb set line_num = line_num + 1000;
UPDATE watter_tb set line_num = 0;

// Not work 
SELECT setval(pg_get_serial_sequence('watter_tb', 'line_num'), COALESCE(max(line_num) + 1, 1), false) FROM   watter_tb;

// PHP Notebook 
https://dbfiddle.uk/Eb22bmMU