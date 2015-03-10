DROP TABLE IF EXISTS `parry_to_percent`;

CREATE TABLE `parry_to_percent` (
  `class_level` int(11) unsigned NOT NULL,
  `perc_cap` float NOT NULL,
  PRIMARY KEY (`class_level`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `parry_to_percent` */

insert  into `parry_to_percent`(`class_level`,`perc_cap`) values (0,3.96118),(1,3.96118),(2,4.15924),(3,4.15924),(4,4.3573),(5,4.3573),(6,4.55536),(7,4.55536),(8,4.75342),(9,5.14954),(10,5.34759),(11,5.34759),(12,5.54567),(13,5.54567),(14,5.94177),(15,5.94177),(16,6.13983),(17,6.3379),(18,6.3379),(19,7.13012),(20,7.13012),(21,7.32821),(22,7.52627),(23,7.52627),(24,7.92236),(25,7.92236),(26,8.12044),(27,8.3185),(28,8.31857),(29,9.1107),(30,9.30873),(31,9.30873),(32,9.50679),(33,9.70487),(34,9.90295),(35,10.101),(36,10.2991),(37,10.2991),(38,10.4971),(39,11.2894),(40,11.4874),(41,11.4874),(42,11.6855),(43,11.8835),(44,12.2797),(45,12.4777),(46,12.4777),(47,12.6757),(48,12.8738),(49,13.6834),(50,13.864),(51,14.0744),(52,14.2686),(53,14.2761),(54,14.6563),(55,14.8683),(56,15.0602),(57,15.2653),(58,15.4646),(59,16.2615),(60,16.7529),(61,16.9696),(62,17.5713),(63,17.9144),(64,18.2822),(65,18.6033),(66,18.9079),(67,19.4243),(68,19.7359),(69,20.0409),(70,21.5522),(71,23.1825),(72,24.9433),(73,26.8348),(74,28.8401),(75,31.0723),(76,33.4247),(77,35.9505),(78,38.6877),(79,41.6406),(80,59.2909),(81,84.4024),(82,120.163),(83,171.116),(84,243.724),(85,319.387),(86,419.992),(87,551.572),(88,724.113),(89,951.159),(90,10000),(91,10000),(92,10000),(93,10000),(94,10000),(95,10000),(96,10000),(97,10000),(98,10000),(99,10000),(100,3.96118),(101,3.96118),(102,4.15924),(103,4.15924),(104,4.3573),(105,4.3573),(106,4.55536),(107,4.55536),(108,4.75342),(109,5.14954),(110,5.34759),(111,5.34759),(112,5.54567),(113,5.54567),(114,5.94177),(115,5.94177),(116,6.13983),(117,6.3379),(118,6.3379),(119,7.13012),(120,7.13012),(121,7.32821),(122,7.52627),(123,7.52627),(124,7.92236),(125,7.92236),(126,8.12044),(127,8.3185),(128,8.31857),(129,9.1107),(130,9.30873),(131,9.30873),(132,9.50679),(133,9.70487),(134,9.90295),(135,10.101),(136,10.2991),(137,10.2991),(138,10.4971),(139,11.2894),(140,11.4874),(141,11.4874),(142,11.6855),(143,11.8835),(144,12.2797),(145,12.4777),(146,12.4777),(147,12.6757),(148,12.8738),(149,13.6834),(150,13.864),(151,14.0744),(152,14.2686),(153,14.2761),(154,14.6563),(155,14.8683),(156,15.0602),(157,15.2653),(158,15.4646),(159,16.2615),(160,16.7529),(161,16.9696),(162,17.5713),(163,17.9144),(164,18.2822),(165,18.6033),(166,18.9079),(167,19.4243),(168,19.7359),(169,20.0409),(170,21.5522),(171,23.1825),(172,24.9433),(173,26.8348),(174,28.8401),(175,31.0723),(176,33.4247),(177,35.9505),(178,38.6877),(179,41.6406),(180,59.2909),(181,84.4024),(182,120.163),(183,171.116),(184,243.724),(185,319.387),(186,419.992),(187,551.572),(188,724.113),(189,951.159),(190,10000),(191,10000),(192,10000),(193,10000),(194,10000),(195,10000),(196,10000),(197,10000),(198,10000),(199,10000),(200,10000),(201,10000),(202,10000),(203,10000),(204,10000),(205,10000),(206,10000),(207,10000),(208,10000),(209,10000),(210,10000),(211,10000),(212,10000),(213,10000),(214,10000),(215,10000),(216,10000),(217,10000),(218,10000),(219,10000),(220,10000),(221,10000),(222,10000),(223,10000),(224,10000),(225,10000),(226,10000),(227,10000),(228,10000),(229,10000),(230,10000),(231,10000),(232,10000),(233,10000),(234,10000),(235,10000),(236,10000),(237,10000),(238,10000),(239,10000),(240,10000),(241,10000),(242,10000),(243,10000),(244,10000),(245,10000),(246,10000),(247,10000),(248,10000),(249,10000),(250,10000),(251,10000),(252,10000),(253,10000),(254,10000),(255,10000),(256,10000),(257,10000),(258,10000),(259,10000),(260,10000),(261,10000),(262,10000),(263,10000),(264,10000),(265,10000),(266,10000),(267,10000),(268,10000),(269,10000),(270,10000),(271,10000),(272,10000),(273,10000),(274,10000),(275,10000),(276,10000),(277,10000),(278,10000),(279,10000),(280,10000),(281,10000),(282,10000),(283,10000),(284,10000),(285,10000),(286,10000),(287,10000),(288,10000),(289,10000),(290,10000),(291,10000),(292,10000),(293,10000),(294,10000),(295,10000),(296,10000),(297,10000),(298,10000),(299,10000),(300,10000),(301,10000),(302,10000),(303,10000),(304,10000),(305,10000),(306,10000),(307,10000),(308,10000),(309,10000),(310,10000),(311,10000),(312,10000),(313,10000),(314,10000),(315,10000),(316,10000),(317,10000),(318,10000),(319,10000),(320,10000),(321,10000),(322,10000),(323,10000),(324,10000),(325,10000),(326,10000),(327,10000),(328,10000),(329,10000),(330,10000),(331,10000),(332,10000),(333,10000),(334,10000),(335,10000),(336,10000),(337,10000),(338,10000),(339,10000),(340,10000),(341,10000),(342,10000),(343,10000),(344,10000),(345,10000),(346,10000),(347,10000),(348,10000),(349,10000),(350,10000),(351,10000),(352,10000),(353,10000),(354,10000),(355,10000),(356,10000),(357,10000),(358,10000),(359,10000),(360,10000),(361,10000),(362,10000),(363,10000),(364,10000),(365,10000),(366,10000),(367,10000),(368,10000),(369,10000),(370,10000),(371,10000),(372,10000),(373,10000),(374,10000),(375,10000),(376,10000),(377,10000),(378,10000),(379,10000),(380,10000),(381,10000),(382,10000),(383,10000),(384,10000),(385,10000),(386,10000),(387,10000),(388,10000),(389,10000),(390,10000),(391,10000),(392,10000),(393,10000),(394,10000),(395,10000),(396,10000),(397,10000),(398,10000),(399,10000),(400,10000),(401,10000),(402,10000),(403,10000),(404,10000),(405,10000),(406,10000),(407,10000),(408,10000),(409,10000),(410,10000),(411,10000),(412,10000),(413,10000),(414,10000),(415,10000),(416,10000),(417,10000),(418,10000),(419,10000),(420,10000),(421,10000),(422,10000),(423,10000),(424,10000),(425,10000),(426,10000),(427,10000),(428,10000),(429,10000),(430,10000),(431,10000),(432,10000),(433,10000),(434,10000),(435,10000),(436,10000),(437,10000),(438,10000),(439,10000),(440,10000),(441,10000),(442,10000),(443,10000),(444,10000),(445,10000),(446,10000),(447,10000),(448,10000),(449,10000),(450,10000),(451,10000),(452,10000),(453,10000),(454,10000),(455,10000),(456,10000),(457,10000),(458,10000),(459,10000),(460,10000),(461,10000),(462,10000),(463,10000),(464,10000),(465,10000),(466,10000),(467,10000),(468,10000),(469,10000),(470,10000),(471,10000),(472,10000),(473,10000),(474,10000),(475,10000),(476,10000),(477,10000),(478,10000),(479,10000),(480,10000),(481,10000),(482,10000),(483,10000),(484,10000),(485,10000),(486,10000),(487,10000),(488,10000),(489,10000),(490,10000),(491,10000),(492,10000),(493,10000),(494,10000),(495,10000),(496,10000),(497,10000),(498,10000),(499,10000),(500,3.96118),(501,3.96118),(502,4.15924),(503,4.15924),(504,4.3573),(505,4.3573),(506,4.55536),(507,4.55536),(508,4.75342),(509,5.14954),(510,5.34759),(511,5.34759),(512,5.54567),(513,5.54567),(514,5.94177),(515,5.94177),(516,6.13983),(517,6.3379),(518,6.3379),(519,7.13012),(520,7.13012),(521,7.32821),(522,7.52627),(523,7.52627),(524,7.92236),(525,7.92236),(526,8.12044),(527,8.3185),(528,8.31857),(529,9.1107),(530,9.30873),(531,9.30873),(532,9.50679),(533,9.70487),(534,9.90295),(535,10.101),(536,10.2991),(537,10.2991),(538,10.4971),(539,11.2894),(540,11.4874),(541,11.4874),(542,11.6855),(543,11.8835),(544,12.2797),(545,12.4777),(546,12.4777),(547,12.6757),(548,12.8738),(549,13.6834),(550,13.864),(551,14.0744),(552,14.2686),(553,14.2761),(554,14.6563),(555,14.8683),(556,15.0602),(557,15.2653),(558,15.4646),(559,16.2615),(560,16.7529),(561,16.9696),(562,17.5713),(563,17.9144),(564,18.2822),(565,18.6033),(566,18.9079),(567,19.4243),(568,19.7359),(569,20.0409),(570,21.5522),(571,23.1825),(572,24.9433),(573,26.8348),(574,28.8401),(575,31.0723),(576,33.4247),(577,35.9505),(578,38.6877),(579,41.6406),(580,59.2909),(581,84.4024),(582,120.163),(583,171.116),(584,243.724),(585,319.387),(586,419.992),(587,551.572),(588,724.113),(589,951.159),(590,10000),(591,10000),(592,10000),(593,10000),(594,10000),(595,10000),(596,10000),(597,10000),(598,10000),(599,10000),(600,10000),(601,10000),(602,10000),(603,10000),(604,10000),(605,10000),(606,10000),(607,10000),(608,10000),(609,10000),(610,10000),(611,10000),(612,10000),(613,10000),(614,10000),(615,10000),(616,10000),(617,10000),(618,10000),(619,10000),(620,10000),(621,10000),(622,10000),(623,10000),(624,10000),(625,10000),(626,10000),(627,10000),(628,10000),(629,10000),(630,10000),(631,10000),(632,10000),(633,10000),(634,10000),(635,10000),(636,10000),(637,10000),(638,10000),(639,10000),(640,10000),(641,10000),(642,10000),(643,10000),(644,10000),(645,10000),(646,10000),(647,10000),(648,10000),(649,10000),(650,10000),(651,10000),(652,10000),(653,10000),(654,10000),(655,10000),(656,10000),(657,10000),(658,10000),(659,10000),(660,10000),(661,10000),(662,10000),(663,10000),(664,10000),(665,10000),(666,10000),(667,10000),(668,10000),(669,10000),(670,10000),(671,10000),(672,10000),(673,10000),(674,10000),(675,10000),(676,10000),(677,10000),(678,10000),(679,10000),(680,10000),(681,10000),(682,10000),(683,10000),(684,10000),(685,10000),(686,10000),(687,10000),(688,10000),(689,10000),(690,10000),(691,10000),(692,10000),(693,10000),(694,10000),(695,10000),(696,10000),(697,10000),(698,10000),(699,10000),(700,10000),(701,10000),(702,10000),(703,10000),(704,10000),(705,10000),(706,10000),(707,10000),(708,10000),(709,10000),(710,10000),(711,10000),(712,10000),(713,10000),(714,10000),(715,10000),(716,10000),(717,10000),(718,10000),(719,10000),(720,10000),(721,10000),(722,10000),(723,10000),(724,10000),(725,10000),(726,10000),(727,10000),(728,10000),(729,10000),(730,10000),(731,10000),(732,10000),(733,10000),(734,10000),(735,10000),(736,10000),(737,10000),(738,10000),(739,10000),(740,10000),(741,10000),(742,10000),(743,10000),(744,10000),(745,10000),(746,10000),(747,10000),(748,10000),(749,10000),(750,10000),(751,10000),(752,10000),(753,10000),(754,10000),(755,10000),(756,10000),(757,10000),(758,10000),(759,10000),(760,10000),(761,10000),(762,10000),(763,10000),(764,10000),(765,10000),(766,10000),(767,10000),(768,10000),(769,10000),(770,10000),(771,10000),(772,10000),(773,10000),(774,10000),(775,10000),(776,10000),(777,10000),(778,10000),(779,10000),(780,10000),(781,10000),(782,10000),(783,10000),(784,10000),(785,10000),(786,10000),(787,10000),(788,10000),(789,10000),(790,10000),(791,10000),(792,10000),(793,10000),(794,10000),(795,10000),(796,10000),(797,10000),(798,10000),(799,10000),(800,10000),(801,10000),(802,10000),(803,10000),(804,10000),(805,10000),(806,10000),(807,10000),(808,10000),(809,10000),(810,10000),(811,10000),(812,10000),(813,10000),(814,10000),(815,10000),(816,10000),(817,10000),(818,10000),(819,10000),(820,10000),(821,10000),(822,10000),(823,10000),(824,10000),(825,10000),(826,10000),(827,10000),(828,10000),(829,10000),(830,10000),(831,10000),(832,10000),(833,10000),(834,10000),(835,10000),(836,10000),(837,10000),(838,10000),(839,10000),(840,10000),(841,10000),(842,10000),(843,10000),(844,10000),(845,10000),(846,10000),(847,10000),(848,10000),(849,10000),(850,10000),(851,10000),(852,10000),(853,10000),(854,10000),(855,10000),(856,10000),(857,10000),(858,10000),(859,10000),(860,10000),(861,10000),(862,10000),(863,10000),(864,10000),(865,10000),(866,10000),(867,10000),(868,10000),(869,10000),(870,10000),(871,10000),(872,10000),(873,10000),(874,10000),(875,10000),(876,10000),(877,10000),(878,10000),(879,10000),(880,10000),(881,10000),(882,10000),(883,10000),(884,10000),(885,10000),(886,10000),(887,10000),(888,10000),(889,10000),(890,10000),(891,10000),(892,10000),(893,10000),(894,10000),(895,10000),(896,10000),(897,10000),(898,10000),(899,10000),(900,10000),(901,10000),(902,10000),(903,10000),(904,10000),(905,10000),(906,10000),(907,10000),(908,10000),(909,10000),(910,10000),(911,10000),(912,10000),(913,10000),(914,10000),(915,10000),(916,10000),(917,10000),(918,10000),(919,10000),(920,10000),(921,10000),(922,10000),(923,10000),(924,10000),(925,10000),(926,10000),(927,10000),(928,10000),(929,10000),(930,10000),(931,10000),(932,10000),(933,10000),(934,10000),(935,10000),(936,10000),(937,10000),(938,10000),(939,10000),(940,10000),(941,10000),(942,10000),(943,10000),(944,10000),(945,10000),(946,10000),(947,10000),(948,10000),(949,10000),(950,10000),(951,10000),(952,10000),(953,10000),(954,10000),(955,10000),(956,10000),(957,10000),(958,10000),(959,10000),(960,10000),(961,10000),(962,10000),(963,10000),(964,10000),(965,10000),(966,10000),(967,10000),(968,10000),(969,10000),(970,10000),(971,10000),(972,10000),(973,10000),(974,10000),(975,10000),(976,10000),(977,10000),(978,10000),(979,10000),(980,10000),(981,10000),(982,10000),(983,10000),(984,10000),(985,10000),(986,10000),(987,10000),(988,10000),(989,10000),(990,10000),(991,10000),(992,10000),(993,10000),(994,10000),(995,10000),(996,10000),(997,10000),(998,10000),(999,10000),(1000,10000),(1001,10000),(1002,10000),(1003,10000),(1004,10000),(1005,10000),(1006,10000),(1007,10000),(1008,10000),(1009,10000),(1010,10000),(1011,10000),(1012,10000),(1013,10000),(1014,10000),(1015,10000),(1016,10000),(1017,10000),(1018,10000),(1019,10000),(1020,10000),(1021,10000),(1022,10000),(1023,10000),(1024,10000),(1025,10000),(1026,10000),(1027,10000),(1028,10000),(1029,10000),(1030,10000),(1031,10000),(1032,10000),(1033,10000),(1034,10000),(1035,10000),(1036,10000),(1037,10000),(1038,10000),(1039,10000),(1040,10000),(1041,10000),(1042,10000),(1043,10000),(1044,10000),(1045,10000),(1046,10000),(1047,10000),(1048,10000),(1049,10000),(1050,10000),(1051,10000),(1052,10000),(1053,10000),(1054,10000),(1055,10000),(1056,10000),(1057,10000),(1058,10000),(1059,10000),(1060,10000),(1061,10000),(1062,10000),(1063,10000),(1064,10000),(1065,10000),(1066,10000),(1067,10000),(1068,10000),(1069,10000),(1070,10000),(1071,10000),(1072,10000),(1073,10000),(1074,10000),(1075,10000),(1076,10000),(1077,10000),(1078,10000),(1079,10000),(1080,10000),(1081,10000),(1082,10000),(1083,10000),(1084,10000),(1085,10000),(1086,10000),(1087,10000),(1088,10000),(1089,10000),(1090,10000),(1091,10000),(1092,10000),(1093,10000),(1094,10000),(1095,10000),(1096,10000),(1097,10000),(1098,10000),(1099,10000);