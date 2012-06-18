<?php session_start();

require_once 'class/class.SocialNetwork.php';

class Facebook extends SocialNetwork implements oAuth {

    const APP_ID = 0; // Enter your app's id
    const APP_SECRET = ""; // Enter your app's secret
    const CALLBACK_URL = ""; // Enter your app's callback
    const CACHE_PATH = "cache/";

    private static $access_token = NULL;
    private static $access_token_expire = 0;

    public static function requestAccessToken($code = NULL) {
        if (!isset($code) || $code == NULL) {
            trigger_error("Please provide a valid code!", E_USER_WARNING);
            return "";
        }

        $request_url = "https://graph.facebook.com/oauth/access_token";
        $data = array(
            "client_id" => self::APP_ID,
            "client_secret" => self::APP_SECRET,
            "redirect_uri" => self::CALLBACK_URL,
            "code" => $code,
        );

        $access_token_data = self::sendRequest($request_url, $data, "POST");

        // Lets process the encoded url query returned
        $query_array = array();
        $a = explode('&', $access_token_data);
        for ($i = 0; $i < count($a); $i++) {
            $b = split('=', $a[$i]);
            $query_array[] = $b[1]; // Push value into array
        }

        return $query_array; // we return the value of the access token
    }

    public static function getAccessToken() {
        if (self::$access_token != NULL) {
            return self::$access_token;
        }

        return "";
    }

    public static function setAccessToken($value) {
        self::$access_token = $value;
    }
    
    public static function getAccessTokenExpire() {
        return self::$access_token_expire;
    }
    
    public static function setAccessTokenExpire($value) {
        self::$access_token_expire = $value;
    }

    public static function isLoggedIn() {
        return self::getAccessToken() != NULL ? TRUE : FALSE;
    }

    public static function setState($state_value = NULL) {
        // Smart cache implementation to not further populate the session
        // or risk utilizing an invalid session (in case it was regenerated
        //  some where). We use the random 'state' hash value as the file
        // name. Later, we will look for this file using the value
        // Facebook sends back to us (which should be the same)
        $cache_file = self::CACHE_PATH . $state_value;
        $bytes_written = file_put_contents($cache_file, $state_value);

        return $bytes_written;
    }

    public static function getState($state_value = NULL) {
        // Smart cache implementation to not further populate the session
        // or risk utilizing an invalid session (in case it was regenerated
        //  some where). We use the random 'state' hash value as the file
        // name. Here, we look for the file matching the value Facebook
        // sends back to us (which should be the same)
        $cache_file = self::CACHE_PATH . $state_value;
        if (file_exists($cache_file)) {
            $cached_value = file_get_contents($cache_file);
        } else {
            $cached_value = "";
        }

        return $cached_value;
    }
    
    public static function delStateCache($state_value) {
        // State is only require to validate the authenticity of
        // the user's login. It is no longer required after that
        // point so let's be conservative.
         $cache_file = self::CACHE_PATH . $state_value;
        if (file_exists($cache_file)) {
            unlink($cache_file);
            return TRUE;
        }
        return FALSE;
    }

    public static function getLoginUrl($scope = NULL) {
        if (self::getAccessToken() == NULL) {

            // Facebook will return this value to us for validation purposes.
            $state = self::getRandomHash(); // Used in $login_url's query.
            self::setState($state); // Lets make sure to save it for later!

            $login_url = 'https://www.facebook.com/dialog/oauth?'
                    . 'client_id=' . self::APP_ID
                    . '&redirect_uri=' . self::CALLBACK_URL
                    . ($scope != NULL ? '&scope=' . $scope : '')
                    . '&state=' . $state;

            return $login_url;
        }
        return "An access token already exists";
    }

    public static function getUserInformation($username = NULL) {
        if (!isset($username) || $username == NULL) {
            $username = "me";
        }
        $data = array(
            'access_token' => self::$access_token
        );
        $request_url = "https://graph.facebook.com/" . $username;
        
        return self::sendRequest($request_url, $data, "GET");
    }
}
?>