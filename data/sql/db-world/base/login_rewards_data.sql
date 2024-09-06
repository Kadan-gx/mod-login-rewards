DROP TABLE IF EXISTS `acore_world`.`login_rewards_data`;
CREATE TABLE `acore_world`.`login_rewards_data` (
  `day` TINYINT NOT NULL,
  `item` INT NULL,
  `count` TINYINT NULL,
  PRIMARY KEY (`day`));