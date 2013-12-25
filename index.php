<?php

define('ENVIRONMENT', 'development');

// What folder relative to this file is our app in?
$app_folder = 'app';

// What folder relative to this file is the ares framework?
$ares_folder = 'aries';

// Let's get the canonicalized absolute path of the ares folder
if (realpath($ares_folder) !== FALSE) {
	$ares_path = realpath($ares_folder).'/';
}

// ensure there's a trailing slash
$ares_path = rtrim($ares_path, '/').'/';

// Is the ares path correctly pointing to a directory?
if ( !is_dir($ares_path)) {
	exit("Your ares folder path does not appear to be set correctly.
			 Please open the following file and correct this: "
			.pathinfo(__FILE__, PATHINFO_BASENAME)
		);
}


/*
 * ------------------------------------------------------------------
*  Now that we know the path, set the main path constants
* -------------------------------------------------------------------
*/

// Path to the ares folder. Make sure paths follow Unix/Linux convention
define('BASEPATH', str_replace("\\", "/", $ares_path));

// Name of the "ares folder"
define('SYSDIR', trim(strrchr(trim(BASEPATH, '/'), '/'), '/'));


// The path to the "app" folder
if (is_dir($app_folder)) {
	define('APPPATH', $app_folder.'/');
}
else {
	if ( !is_dir(BASEPATH.$app_folder.'/')) {
		exit("Your app folder path does not appear to be set correctly. 
				Please open the following file and correct this: "
				.pathinfo(__FILE__, PATHINFO_BASENAME));
	}

	define('APPPATH', BASEPATH.$app_folder.'/');
}

/*
 * --------------------------------------------------------------------
* LOAD ARES BOOTSTRAP FILE
* --------------------------------------------------------------------
*
* The God of War (against inconsistent codes) has awaken...
*
*/

require_once BASEPATH.'backend/core/Aries.php';

?>