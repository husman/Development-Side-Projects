<?php
namespace Ares\Db;

require_once APPPATH.'backend/config/'.ENVIRONMENT.'/Database.php';
require_once BASEPATH.'backend/db/types/DatabaseType.php';

class Database {
	public static function getDB($dbType=DatabaseType::DEFAULT_ADAPTER) {
		switch($dbType) {
			case DatabaseType::MySQL:
				require_once BASEPATH.'backend/db/drivers/mysql/MySQLDriver.php';
				$db = MySQLDriver::getInstance();
			break;
			case DatabaseType::MySQLi:
				require_once BASEPATH.'backend/db/drivers/mysqli/MySQLiDriver.php';
				$db = MySQLiDriver::getInstance();
				break;
			case DatabaseType::Oracle_oci8:
				require_once BASEPATH.'backend/db/drivers/oracle/OracleOci8Driver.php';
				$db = OracleOci8Driver::getInstance();
				break;
			default:
				trigger_error("The database back-end you have select is not supported!",
								E_USER_ERROR);
			break;
		}
		
		// Connect to the database (non-persistent)
		$db->connect(DB_HOST, DB_USER, DB_PASS, DB_NAME);
		return $db;
	}
}
?>