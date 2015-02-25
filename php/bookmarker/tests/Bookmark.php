<?php

include "IntegrationTestCase.php";


class BookmarkTest extends IntegrationTestCase {
    /**
     * @expectedException Exception
     * @expectedExceptionMessage A user is required.
     */
    public function testCreateBookmarkRequiresUserToBeSpecified() {
        $user_info = array(
            'firstname' => 'bob',
            'lastname' => 'example',
            'email' => 'bob@example.com'
        );
        $user = $this->test_factory->createUser($user_info);

        $bookmark_info = array(
            'title' => 'Hearst Corporation',
            'url' => 'http://www.hearst.com/',
        );
        $new_bookmark = new BookMark($bookmark_info);
        $new_bookmark->save();
    }

    public function testCreatingBookmarkBackPopulatesUser() {
        $user_info = array(
            'firstname' => 'bob',
            'lastname' => 'example',
            'email' => 'bob@example.com'
        );
        $user = $this->test_factory->createUser($user_info);

        $bookmark_info = array(
            'title' => 'Hearst Corporation',
            'url' => 'http://www.hearst.com/',
            'user' => $user
        );
        $new_bookmark = new BookMark($bookmark_info);
        $new_bookmark->save();

        $bookmark = Bookmark::get($new_bookmark->id);
        $this->assertNotEmpty($bookmark);
        $this->assertInstanceOf('Bookmark', $bookmark);
        $this->assertEquals($new_bookmark->id, $bookmark->id);
        $this->assertEquals('Hearst Corporation', $bookmark->title);

        $this->assertNotEmpty($bookmark->user);
        $this->assertInstanceOf('User', $bookmark->user);
        $this->assertEquals($new_bookmark->user->id, $bookmark->user->id);
        $this->assertEquals('bob', $bookmark->user->firstname);
    }

    /**
     * @expectedException PDOException
     * @expectedExceptionCode 23000
     */
    public function testUsersCannotCreateDuplicateBookmarks() {
        $user_info = array(
            'firstname' => 'bob',
            'lastname' => 'example',
            'email' => 'bob@example.com'
        );
        $user = $this->test_factory->createUser($user_info);

        $bookmark_info = array(
            'title' => 'Hearst Corporation',
            'url' => 'http://www.hearst.com/',
            'user' => $user
        );

        $new_bookmark = new BookMark($bookmark_info);
        $new_bookmark->save();

        $new_bookmark = new BookMark($bookmark_info);
        $new_bookmark->save();
    }
}
