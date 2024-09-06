CREATE TABLE `acore_characters`.`login_rewards` (
  `guid` INT NOT NULL,
  `lastClaimed` INT NULL,
  `reward` TINYINT NULL,
  PRIMARY KEY (`guid`));