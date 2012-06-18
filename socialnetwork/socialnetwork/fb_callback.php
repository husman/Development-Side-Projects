<?php session_start();

require_once 'class/class.Facebook.php';

// Let's make sure the state values match 
// by recovering the expected value from cache
$state = $_GET['state'];
if (Facebook::getState($state) == $state) {
    
    // We no longer need the 'state' hash so let's remove it.
    Facebook::delStateCache($state);
    
    $code = $_GET['code'];
    // Let's request a new access token and store it into the session
    $data = Facebook::requestAccessToken($code);
    $_SESSION["fb_atoken"] = $data[0];
    $_SESSION["fb_atoken_expire"] = intval($data[1]);

    // Redirect the user to the first page witht he desired code
    header("location: /haleeq/sample/facebook_integration.php");
} else {
    die("Your state could not be validated! HAX?");
}
?>