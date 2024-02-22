SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for sungmahee_tower
-- ----------------------------
DROP TABLE IF EXISTS `sungmahee_tower`;
CREATE TABLE `sungmahee_tower`  (
  `id` int(32) NOT NULL AUTO_INCREMENT,
  `element` enum('FIRE','ICE','ELEC','WIND','EARTH','DARK','ALL') CHARACTER SET utf32 COLLATE utf32_general_ci NOT NULL DEFAULT 'FIRE',
  `vnum1` int(32) NOT NULL DEFAULT 0,
  `count1` int(8) NOT NULL DEFAULT 0,
  `vnum2` int(32) NOT NULL DEFAULT 0,
  `count2` int(8) NOT NULL DEFAULT 0,
  `vnum3` int(32) NOT NULL DEFAULT 0,
  `count3` int(8) NOT NULL DEFAULT 0,
  `vnum4` int(32) NOT NULL DEFAULT 0,
  `count4` int(8) NOT NULL DEFAULT 0,
  `vnum5` int(32) NOT NULL DEFAULT 0,
  `count5` int(8) NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `id`(`id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 51 CHARACTER SET = utf32 COLLATE = utf32_general_ci ROW_FORMAT = Fixed;

-- ----------------------------
-- Records of sungmahee_tower
-- ----------------------------
INSERT INTO `sungmahee_tower` VALUES (1, 'FIRE', 70401, 1, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (2, 'ICE', 72351, 1, 31246, 1, 0, 0, 0, 0, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (3, 'ELEC', 72349, 1, 31246, 2, 0, 0, 0, 0, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (4, 'WIND', 72354, 1, 31246, 4, 31249, 1, 0, 0, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (5, 'EARTH', 50527, 1, 50528, 3, 50529, 3, 0, 0, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (6, 'DARK', 72353, 1, 31245, 2, 31246, 2, 31247, 2, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (7, 'FIRE', 31250, 1, 31246, 3, 31249, 3, 31248, 3, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (8, 'ICE', 72349, 1, 31246, 4, 31249, 4, 31248, 4, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (9, 'ELEC', 72353, 2, 50325, 1, 31246, 5, 31247, 5, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (10, 'ALL', 50527, 1, 50528, 4, 50529, 4, 0, 0, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (11, 'WIND', 50325, 1, 70401, 3, 0, 0, 0, 0, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (12, 'EARTH', 72351, 1, 31246, 10, 31249, 10, 31248, 10, 31247, 10);
INSERT INTO `sungmahee_tower` VALUES (13, 'DARK', 72349, 1, 31246, 10, 31250, 10, 31248, 10, 31247, 10);
INSERT INTO `sungmahee_tower` VALUES (14, 'FIRE', 72354, 1, 31246, 10, 31249, 10, 31248, 10, 31247, 10);
INSERT INTO `sungmahee_tower` VALUES (15, 'ICE', 50527, 1, 50528, 3, 50529, 3, 0, 0, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (16, 'ELEC', 72353, 3, 50325, 1, 100300, 1, 100700, 1, 50255, 1);
INSERT INTO `sungmahee_tower` VALUES (17, 'WIND', 72351, 2, 31246, 10, 31249, 10, 31248, 10, 31247, 10);
INSERT INTO `sungmahee_tower` VALUES (18, 'EARTH', 72349, 2, 31246, 10, 31250, 10, 31248, 10, 31247, 10);
INSERT INTO `sungmahee_tower` VALUES (19, 'DARK', 72354, 2, 31246, 10, 31249, 10, 31248, 10, 31247, 10);
INSERT INTO `sungmahee_tower` VALUES (20, 'ALL', 50527, 1, 50528, 5, 50529, 5, 0, 0, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (21, 'FIRE', 50325, 1, 70401, 5, 0, 0, 0, 0, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (22, 'ICE', 72346, 1, 31251, 2, 31252, 2, 0, 0, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (23, 'ELEC', 72349, 1, 31251, 2, 31252, 2, 0, 0, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (24, 'WIND', 72354, 1, 31251, 3, 31252, 3, 0, 0, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (25, 'EARTH', 50527, 1, 50528, 5, 50529, 5, 0, 0, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (26, 'DARK', 72353, 3, 50325, 1, 100300, 1, 100700, 1, 50255, 2);
INSERT INTO `sungmahee_tower` VALUES (27, 'FIRE', 72351, 3, 31246, 10, 31249, 10, 31248, 10, 31247, 10);
INSERT INTO `sungmahee_tower` VALUES (28, 'ICE', 72349, 3, 31246, 10, 31250, 10, 31248, 10, 31247, 10);
INSERT INTO `sungmahee_tower` VALUES (29, 'ELEC', 72354, 3, 31246, 10, 31249, 10, 31248, 10, 31247, 10);
INSERT INTO `sungmahee_tower` VALUES (30, 'ALL', 50527, 1, 50528, 6, 50529, 6, 0, 0, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (31, 'WIND', 50325, 1, 70401, 10, 0, 0, 0, 0, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (32, 'EARTH', 72346, 1, 31251, 3, 31252, 3, 0, 0, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (33, 'DARK', 72349, 1, 31251, 3, 31252, 3, 0, 0, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (34, 'FIRE', 72354, 1, 31251, 3, 31252, 3, 0, 0, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (35, 'ICE', 50527, 1, 50528, 8, 50529, 8, 0, 0, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (36, 'ELEC', 72353, 3, 50325, 1, 100300, 2, 100700, 1, 50255, 3);
INSERT INTO `sungmahee_tower` VALUES (37, 'WIND', 72351, 4, 31246, 10, 31249, 10, 31248, 10, 31247, 10);
INSERT INTO `sungmahee_tower` VALUES (38, 'EARTH', 72349, 4, 31246, 10, 31250, 10, 31248, 10, 31247, 10);
INSERT INTO `sungmahee_tower` VALUES (39, 'DARK', 72354, 4, 31246, 10, 31249, 10, 31248, 10, 31247, 10);
INSERT INTO `sungmahee_tower` VALUES (40, 'ALL', 50527, 1, 50528, 9, 50529, 9, 0, 0, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (41, 'FIRE', 50325, 1, 70401, 15, 0, 0, 0, 0, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (42, 'ICE', 72346, 4, 31251, 5, 31252, 5, 0, 0, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (43, 'ELEC', 72349, 4, 31251, 5, 31252, 5, 0, 0, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (44, 'WIND', 72354, 4, 31251, 5, 31252, 5, 0, 0, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (45, 'EARTH', 50527, 2, 50528, 10, 50529, 10, 30618, 1, 0, 0);
INSERT INTO `sungmahee_tower` VALUES (46, 'DARK', 72353, 3, 50325, 1, 100300, 3, 100700, 1, 50255, 5);
INSERT INTO `sungmahee_tower` VALUES (47, 'FIRE', 72351, 5, 31246, 10, 31249, 10, 31248, 10, 31247, 10);
INSERT INTO `sungmahee_tower` VALUES (48, 'ICE', 72349, 5, 31246, 10, 31250, 10, 31248, 10, 31247, 10);
INSERT INTO `sungmahee_tower` VALUES (49, 'ELEC', 72354, 5, 31246, 10, 31249, 10, 31248, 10, 31247, 10);
INSERT INTO `sungmahee_tower` VALUES (50, 'ALL', 50527, 3, 50528, 10, 50529, 10, 31225, 1, 70402, 1);

SET FOREIGN_KEY_CHECKS = 1;
