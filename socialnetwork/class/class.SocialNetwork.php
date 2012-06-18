<?php

interface oAuth {
    public static function requestAccessToken();
    public static function getAccessToken();
    public static function setAccessToken($value);
}

class SocialNetwork {

    public static function sendRequest($url = NULL, $data = array(), $method = "GET") {
        $return_array = array(); // initialize the final return value
        
        if (!isset($url) || $url == NULL) {
            trigger_error("Please provide a valid url", E_USER_WARNING);
            return $return_array;
        }

        // Lets build our request's params
        $curl = curl_init($url);
        curl_setopt($curl, CURLOPT_POST, TRUE);
        curl_setopt($curl, CURLOPT_RETURNTRANSFER, TRUE);
        curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, FALSE);

        // Let's build the query string
        $query_string = http_build_query($data);
        curl_setopt($curl, CURLOPT_POSTFIELDS, http_build_query($data));
        
        if ($method == "POST") {
            curl_setopt($curl, CURLOPT_POST, 1);
        }
        
        $return_array = curl_exec($curl); // Lets make our request
        curl_close($curl);

        return $return_array;
    }

    public static function getRandomHash() {
        if (($result = session_id()) != NULL) {
            return $result;
        }

        // If there is no existing session, then generate a random sequence
        $charPool = '0123456789abcdefghijklmnopqrstuvwxyz';
        for ($p = 0; $p < 15; $p++) {
            $result .= $charPool[mt_rand(0, strlen($charPool) - 1)];
        }

        return md5($result . time());
    }
}
?>