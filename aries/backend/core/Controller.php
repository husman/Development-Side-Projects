<?php  if ( ! defined('BASEPATH')) exit('No direct script access allowed');
/**
 * Aries Controller
 *
 *
 * @package		Areies
 * @author		Haleeq Usman
 * 
 */

// ------------------------------------------------------------------------

/**
 * Controller Definition from CodeIgniter 
 * 
 * Modified By: Haleeq Usman on 12-24-2013.
 *
 * This class object is the super class that every library in
 * CodeIgniter will be assigned to.
 *
 * @author		ExpressionEngine Dev Team, Modiefied by Haleeq Usman
 * 
 */
class ARIES_Controller {

	private static $instance;

	/**
	 * Constructor
	 */
	public function __construct()
	{
		self::$instance =& $this;
		
		// Assign all the class objects that were instantiated by the
		// bootstrap file (CodeIgniter.php) to local class variables
		// so that CI can run as one big super object.
		foreach (is_loaded() as $var => $class)
		{
			$this->$var =& load_class($class);
		}

		$this->load =& load_class('Loader', 'core');

		$this->load->initialize();
		
	}

	public static function &get_instance()
	{
		return self::$instance;
	}
}

?>