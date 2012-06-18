<?php
require_once 'class/class.Facebook.php';

// Let's load our application's global configuration file
require_once 'config/config.global.php';
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title>Facebook integration example</title>
    </head>

    <body>
        <?php
        // Let's set the user permissions we need.
        $scope = "user_about_me";
        if (!Facebook::isLoggedIn($scope)) { ?>
            Please <a href="<?= Facebook::getLoginUrl(); ?>">click here</a> to 
            login to Facebook. After logging in, you will automatically be taken 
            back here.<br /><br />
        <?php } ?>

        <!-- For readability, lets open a new block -->
        <?php
        if (Facebook::isLoggedIn()) {
            // Let's print out the user's information attained from Facebook
            $user_info = Facebook::getUserInformation();
            echo '<b>Your Information:</b><br />'
                 . '<pre>' . print_r($user_info, 1) . '</pre>';
        }
        ?>
    </body>
</html>