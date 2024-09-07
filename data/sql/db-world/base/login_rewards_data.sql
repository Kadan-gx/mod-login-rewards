CREATE TABLE IF NOT EXISTS `login_rewards_data` (
  `day` TINYINT NOT NULL,
  `item` INT NULL,
  `count` TINYINT NULL,
PRIMARY KEY (`day`));

INSERT INTO login_rewards_data (id, account_id, reward_id) VALUES 
(0, 1191, 1),
(1, 1191, 1),
(2, 1191, 1),
(3, 1191, 1),
(4, 1191, 1);