<?php

include "IntegrationTestCase.php";


class UserTest extends IntegrationTestCase {
    public function testCanCreateUser() {
        $new_user = new User(array(
            'firstname' => 'bob',
            'lastname' => 'example',
            'email' => 'bob@example.com'
        ));
        $new_user->save();

        $user = User::get($new_user->id);
        $this->assertNotEmpty($user);
        $this->assertEquals($new_user->id, $user->id);
        $this->assertEquals('bob', $user->firstname);
        $this->assertEquals('example', $user->lastname);
        $this->assertEquals('bob@example.com', $user->email);
    }

    public function testCanFindUserByIdIfParameterIsGivenAsAnInteger() {
        $user_info = array(
            'firstname' => 'fred',
            'lastname' => 'example',
        );
        $user = $this->test_factory->createUser($user_info);
        $user_id = $user->id;

        $user = User::get($user_id);
        $this->assertNotEmpty($user);
        $this->assertGreaterThan(0, $user->id);
        $this->assertEquals('fred', $user->firstname);
    }

    public function testCanFindUserByColumnIfTheParameterIsGivenAsAnAssociativeArray() {
        $user_info = array(
            'firstname' => 'ted',
            'lastname' => 'example',
            'email' => 'ted@example.com',
        );
        $user = $this->test_factory->createUser($user_info);
        $user_id = $user->id;

        $user = User::get(array(
            'email' => $user_info['email']
        ));
        $this->assertNotEmpty($user);
        $this->assertInstanceOf('User', $user);
        $this->assertEquals($user_id, $user->id);
        $this->assertEquals('ted@example.com', $user->email);

        $user = User::get(array(
            'firstname' => $user_info['firstname']
        ));
        $this->assertInstanceOf('User', $user);
        $this->assertEquals('ted@example.com', $user->email);
        $this->assertEquals('ted', $user->firstname);

        $user = User::get(array(
            'firstname' => $user_info['firstname'],
            'email' => $user_info['email']
        ));
        $this->assertInstanceOf('User', $user);
        $this->assertEquals('ted@example.com', $user->email);
        $this->assertEquals('ted', $user->firstname);
    }

    public function testCanRetrieveAllUsers() {
        $user_info_1 = array(
            'firstname' => 'User A',
            'lastname' => 'User AL',
            'email' => 'userA@example.com'
        );
        $user_info_2 = array(
            'firstname' => 'User B',
            'lastname' => 'User BL',
            'email' => 'userB@example.com'
        );
        $user1 = $this->test_factory->createUser($user_info_1);
        $user1 = $this->test_factory->createUser($user_info_2);

        $users = User::get();
        $this->assertEquals(2, count($users));
        $this->assertInstanceOf('User', $users[0]);
        $this->assertInstanceOf('User', $users[1]);
        $this->assertEquals('User A', $users[0]->firstname);
        $this->assertEquals('User B', $users[1]->firstname);
    }

    public function testCanDeleteUser() {
        $user_info = array(
            'firstname' => 'bob',
            'lastname' => 'example',
            'email' => 'bob@example.com'
        );
        $user = $this->test_factory->createUser($user_info);
        $user_id = $user->id;

        $new_user = User::get($user_id);
        $this->assertInstanceOf('User', $new_user);
        $this->assertEquals($user_id, $new_user->id);

        $user->delete();
        $new_user = User::get($user_id);
        $this->assertNotInstanceOf('User', $new_user);
    }

    public function testCanUpdateUserInformation() {
        $user_info = array(
            'firstname' => 'ted',
            'lastname' => 'example',
            'email' => 'ted@example.com'
        );
        $user = $this->test_factory->createUser($user_info);
        $user_id = $user->id;

        $new_user = User::get($user_id);
        $this->assertInstanceOf('User', $new_user);
        $this->assertEquals($user_id, $new_user->id);
        $this->assertEquals('ted', $new_user->firstname);
        $this->assertEquals('example', $new_user->lastname);
        $this->assertEquals('ted@example.com', $new_user->email);

        $new_user->firstname = 'Hearst';
        $new_user->email = 'hearst@example.com';
        $new_user->save();

        $new_user = User::get($user_id);
        $this->assertInstanceOf('User', $new_user);
        $this->assertEquals($user_id, $new_user->id);
        $this->assertEquals('Hearst', $new_user->firstname);
        $this->assertEquals('example', $new_user->lastname);
        $this->assertEquals('hearst@example.com', $new_user->email);


    }
}
