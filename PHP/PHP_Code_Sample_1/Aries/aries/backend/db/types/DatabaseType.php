<?php
namespace Ares\Db;

class DatabaseType {
	
	// Open source databases
	const MySQL = "mysql";
	const MySQLi = "mysqli";
	
	// Other databases
	const Oracle_oci8 = "oracle_oci8";
	
	// Default database
	const DEFAULT_ADAPTER = self::MySQL;
}
?>
