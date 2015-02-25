<?php

include "../src/db/Database.php";


class DatabaseTest extends PHPUnit_Framework_TestCase {
    public function __construct() {
        $this->setupDatabase();
    }

    public function testCanQueryTable() {
        $this->db_connection->exec("CREATE TABLE users (id INTEGER PRIMARY KEY, email TEXT)");
        $this->assertEquals(0, count($this->database->query("SELECT * FROM users")));

        $this->db_connection->exec("INSERT INTO users (email) VALUES ('bob@example.com')");
        $this->db_connection->exec("INSERT INTO users (email) VALUES ('ted@example.com')");

        $users = $this->database->query("SELECT * FROM users");
        $this->assertEquals(2, count($users));

        $this->assertEquals('bob@example.com', $users[0]->email);
        $this->assertEquals('ted@example.com', $users[1]->email);
    }

    public function testLogsExceptionsToLogFile() {
        $log_file = '../src/logs/database.log';

        if(file_exists($log_file))
            unlink($log_file);

        $this->database->query("SELECT * from NON_EXISTING_TABLE");
        $this->assertEquals(true, file_exists($log_file));
    }

    private function setupDatabase() {
        $this->db_connection = new PDO('sqlite::memory:');

        // Persist the database connection and set error mode to exception
        $this->db_connection->setAttribute(PDO::ATTR_PERSISTENT, true);
        $this->db_connection->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

        $this->database = new Database($this->db_connection);
    }

    private $database;
    private $db_connection;
}