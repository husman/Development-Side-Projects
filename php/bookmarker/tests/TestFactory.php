<?php

include "../src/models/User.php";
include "../src/models/Bookmark.php";


class TestFactory {
    public function __construct() {

    }

    public function createUser($user_info) {
        $user = new User($user_info);
        $user->save();

        return $user;
    }

}