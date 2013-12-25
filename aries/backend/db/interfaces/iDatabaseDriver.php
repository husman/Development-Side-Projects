<?php
namespace Ares\Db\Interfaces;

/**
 * This interface defines the core functionality
 * all database adapters must support.
*/
 
interface iDatabaseDriver {
	// Connection/Initialization
	public function connect($db_host, $db_user, $db_pass, $db_name);
	
	// Database querying
	public function query($sql_query);
	
	// Result fetching
	public function fetch_array();
	public function fetch_all();
	
	// Query/result information
	public function num_rows();
	public function num_fields();
	public function field_name($offset);
	public function field_type($offset);
	
	// Ideally, we would like to add more functionality.
	// Particularly, Active record based off SQL clause keywords
}

?>