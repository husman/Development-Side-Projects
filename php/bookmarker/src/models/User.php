<?php

include_once "db/Database.php";


class User {
    public $id;
    public $firstname;
    public $lastname;
    public $email;
    public $password;

    function __construct($argments=NULL) {
        if($argments != NULL)
            foreach($argments as $column => $values)
                $this->{$column} = $values;

        $this->db = Database::getHandle();
    }

    function __destruct() {
        $this->db = NULL;
    }

    public static function get($arguments=NULL) {
        $db = Database::getHandle();
        if($arguments == NULL) {
            $statement = $db->prepare("Select * FROM users");
            $statement->execute();
            $statement->setFetchMode(PDO::FETCH_CLASS, 'User');
            return $statement->fetchAll();
        }

        if(!is_array($arguments)) {
            $statement = $db->prepare("Select * FROM users WHERE id = :id");
            $statement->execute(array(
                ':id' => $arguments
            ));
            $statement->setFetchMode(PDO::FETCH_CLASS, 'User');
            return $statement->fetch();
        }

        $sql = "Select * FROM users WHERE 1 ";
        $where_clause = [];
        foreach($arguments as $column => $value) {
            $where_clause[] = "AND {$column} = :{$column}";
            unset($arguments[$column]);
            $arguments[':'. $column] = $value;
        }
        $sql .= implode(' ', $where_clause);

        $statement = $db->prepare($sql);
        $statement->execute($arguments);
        $statement->setFetchMode(PDO::FETCH_CLASS, 'User');
        return $statement->fetch();
    }

    public function save() {
        $sql = "UPDATE users SET firstname = :firstname, lastname = :lastname, email = :email, password = :password";
        if($this->id == NULL)
            $sql = "INSERT INTO users (firstname, lastname, email, password) VALUES (:firstname, :lastname, :email, :password)";

        $statement = $this->db->prepare($sql);
        $statement->execute(array(
            ':firstname' => $this->firstname,
            ':lastname' => $this->lastname,
            ':email' => $this->email,
            ':password' => $this->password
        ));

        $this->id = $this->db->lastInsertId();
    }

    public function delete() {
        if($this->id == NULL)
            return;

        $statement = $this->db->prepare("DELETE FROM users WHERE id = :id");
        $statement->execute(array(
            ':id' => $this->id
        ));
    }

    private $db;
    private $statement;
}