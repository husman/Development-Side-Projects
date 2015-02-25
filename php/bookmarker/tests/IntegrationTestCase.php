<?php

include "TestFactory.php";


class IntegrationTestCase extends PHPUnit_Framework_TestCase {
    protected function setUp() {
        $this->test_factory = new TestFactory();
        $this->cleanDatabase();
    }

    protected function tearDown() {
        $this->cleanDatabase();
    }

    private function cleanDatabase() {
        foreach(User::get() as $user)
            $user->delete();
        foreach(Bookmark::get() as $bookmark)
            $bookmark->delete();
    }

    protected $test_factory;
}