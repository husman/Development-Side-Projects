<?php

// Define our global database settings.

/**
 *  @see aries/backend/db/types/DatabaseType.php
 * 		 for a list of supported drivers.
 */
define('DB_DRIVER', 'oracle_oci8');

/**
 * 
 * @var DB_HOST - Database location/server
 * @var DB_USER - Database username
 * @var DB_PASS - Database password
 * @var DB_NAME - Database/Schema name.
 * 
 */
define('DB_HOST', 'localhost');
define('DB_USER', 'php_usr');
define('DB_PASS', 'orc123');
define('DB_NAME', 'xe');

?>
