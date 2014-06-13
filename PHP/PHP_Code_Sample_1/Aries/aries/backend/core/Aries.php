<?php
if ( !defined('BASEPATH')) {
	exit('Huh?');
}

/**
 * Ares
 *
 * Super light framework for NextSource Demostration
 *
 * @package		Ares
 * @author		Haleeq Usman
 * @license		MIT 
 *
 */

/**
 * Ares Version
 *
 * @var string
 *
 */
define('ARIES_VERSION', '0.1.0');

/*
 * ------------------------------------------------------
*  Load the global functions
* ------------------------------------------------------
*/
require(BASEPATH.'backend/core/Common.php');

/*
 * ------------------------------------------------------
*  Instantiate the routing class and set the routing
* ------------------------------------------------------
*/
$RTR =& load_class('Router', 'core');
$RTR->setRouting();

/*
 * ------------------------------------------------------
 *  Load the app controller and local controller
 * ------------------------------------------------------
 *
 */
// Load the base controller class
require BASEPATH.'backend/core/Controller.php';

function &get_instance()
{
	return ARIES_Controller::get_instance();
}

if ( ! file_exists(APPPATH.'backend/mvc/controllers/'.$RTR->getController().'.php'))
{
	trigger_error("Could not find controller definition for '"
				.htmlentities($RTR->getController())."'", E_USER_ERROR);
}

include(APPPATH.'backend/mvc/controllers/'.$RTR->getController().'.php');

$class  = $RTR->getController();
$action = $RTR->getAction();

if ( ! method_exists($class, $action))
{
	trigger_error("Could not find action '".htmlentities($action)."' within controller '"
					.htmlentities($class)."'", E_USER_ERROR);
}

// Call the requested method on controller
$ARIES = new $class();
$ARIES->$action();
?>