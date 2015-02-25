CREATE TABLE users
(
id int NOT NULL AUTO_INCREMENT,
email varchar(255) NOT NULL,
PRIMARY KEY (id)
)

CREATE TABLE bookmarks
(
id int NOT NULL AUTO_INCREMENT,
title varchar(255) NOT NULL,
url varchar(255) NOT NULL,
user_id int NOT NULL,
CONSTRAINT uc_title_url_user_id UNIQUE (title, url, user_id),
CONSTRAINT fk_user_id FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
PRIMARY KEY (id)
)