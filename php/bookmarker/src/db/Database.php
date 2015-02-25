<?php

require '../vendor/autoload.php';

class Database {

    public static function getHandle() {
        if(self::$db_handle != NULL)
            return self::$db_handle;

        return self::connect();
    }

    private static function connect() {
        // Make the database connection and log any exceptions
        try {
            self::$db_handle = new PDO(
                "mysql:host=". self::$databse_host.
                ";dbname=". self::$databse_name,
                self::$databse_user,
                self::$databse_password
            );

            // Persist the database connection and set error mode to exception
            self::$db_handle->setAttribute(PDO::ATTR_PERSISTENT, true);
            self::$db_handle->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

            return self::$db_handle;
        } catch(PDOException $exception) {
            self::$logger->addError($exception->getMessage(), $exception->getTrace());
        }
    }

    private static function initializeLogger() {
        self::$logger = new Monolog\Logger('database');
        self::$logger->pushHandler(new Monolog\Handler\StreamHandler('logs/database', Monolog\Logger::ERROR));
    }


    // Database connection handle and resources
    private static $db_handle;

    // Logging
    private static $logger;

    // Connection parameters
    private static $databse_host = 'localhost';
    private static $databse_name = 'bookmarker';
    private static $databse_user = 'root';
    private static $databse_password = 'orc123';
}