<?php

// Facebook Initialization
if (isset($_SESSION['fb_atoken'])) {
    Facebook::setAccessToken($_SESSION['fb_atoken']);
    Facebook::setAccessTokenExpire($_SESSION['fb_atoken_expire']);
}

?>