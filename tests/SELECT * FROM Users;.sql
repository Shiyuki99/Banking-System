SELECT *
FROM Users;
-- @Block
DELETE FROM Users;
-- @Block
INSERT INTO Users
VALUES('username', 'password', 10);
-- @Block
SHOW WARNINGS;
-- @Block
UPDATE Users
SET balance = 100
WHERE Username = 'mokaa';