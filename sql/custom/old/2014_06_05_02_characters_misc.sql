DROP TABLE IF EXISTS
  autobroadcasts;

ALTER TABLE
  gm_tickets
MODIFY COLUMN
  closedBy INT UNSIGNED NOT NULL DEFAULT 0,
ADD COLUMN
  haveTicket TINYINT UNSIGNED NOT NULL AFTER viewed;
