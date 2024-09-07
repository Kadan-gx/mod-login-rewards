CREATE TABLE IF NOT EXISTS `login_rewards` (
  `guid` INT NOT NULL,
  `lastClaimed` INT NULL,
  `reward` TINYINT NULL,
  PRIMARY KEY (`guid`));