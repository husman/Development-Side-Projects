<?php  if ( ! defined('BASEPATH')) exit('No direct script access allowed');

// ------------------------------------------------------------------------

/**
 * Router Class
 *
 * Parses URIs and determines routing
 *
 * @package		Aries
 * @author		Haleeq Usman
 */
class ARIES_Router {
	private $controller;
	private $action;
	
	public function setRouting()
	{
		if ( !isset($_GET['controller'])) {
			include APPPATH.'backend/config/routes.php';
			$this->controller = ucfirst($route['default_controller']);
			$this->action = "index";
			return;
		}
		
		$req_controller = $_GET['controller'];
		
		if ( !isset($_GET['action'])) {
			$req_action = "index";
		} else {
			$req_action = $_GET['action'];
		}
		
		// Set the controller/action instance variables
		$this->controller = ucfirst($req_controller);
		$this->action = $req_action;
	}
	
	public function getController()
	{
		return $this->controller;
	}
	
	public function getAction()
	{
		return $this->action;
	}

}

?>