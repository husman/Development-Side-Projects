<?php  if ( ! defined('BASEPATH')) exit('No direct script access allowed');
/**
 * Aries
 * 
 * Some of these of these functions come from Codeigniter. However,
 * they have been modified to suit this custom framework's needs.
 * Mose of the modifications are related to the database, since
 * custom database classes were written.
 *
 * @package		Aries
 * @author		Haleeq Usman
 * 
 */

// ------------------------------------------------------------------------

/**
 * Loader Class
 *
 * Loads views/models
 *
 */
class ARIES_Loader {
	
	/**
	 * List of paths to load views from
	 *
	 * @var array
	 * @access protected
	 */
	protected $_aries_view_paths		= array();
	
	/**
	 * List of cached variables
	 *
	 * @var array
	 * @access protected
	 */
	protected $_aries_cached_vars		= array();

	/**
	 * Constructor
	 *
	 * Sets the path to the view/model files
	 */
	public function __construct()
	{
		$this->_aries_model_paths = array(APPPATH.'backend/mvc/models/');
		$this->_aries_view_paths = array(APPPATH.'backend/mvc/views/'	=> TRUE);
	}

	// --------------------------------------------------------------------

	/**
	 * Initialize the Loader
	 *
	 * This method is called once in ARIES_Controller.
	 *
	 * @param 	array
	 * @return 	object
	 */
	public function initialize()
	{
		$this->_aries_classes = array();
		$this->_aries_models = array();
		
		$this->_aries_autoloader();

		return $this;
	}

	// --------------------------------------------------------------------

	/**
	 * Database Loader
	 *
	 * @return	void
	 */
	public function database()
	{
		// Grab the super object
		$CI =& get_instance();
		
		require_once(BASEPATH.'backend/db/Database.php');
		
		// Initialize the db variable. Using our custom database classes.
		$CI->db = Ares\Db\Database::getDB(DB_DRIVER);
	}

	// --------------------------------------------------------------------
	
	/**
	 * Object to Array
	 *
	 * Takes an object as input and converts the class variables to array key/vals
	 *
	 * @param	object
	 * @return	array
	 */
	protected function _aries_object_to_array($object)
	{
		return (is_object($object)) ? get_object_vars($object) : $object;
	}
	
	// --------------------------------------------------------------------
	
	/**
	 * Set Variables
	 *
	 * Once variables are set they become available within
	 * the controller class and its "view" files.
	 *
	 * @param	array
	 * @param 	string
	 * @return	void
	 */
	public function vars($vars = array(), $val = '')
	{
		if ($val != '' AND is_string($vars))
		{
			$vars = array($vars => $val);
		}
	
		$vars = $this->_ci_object_to_array($vars);
	
		if (is_array($vars) AND count($vars) > 0)
		{
			foreach ($vars as $key => $val)
			{
				$this->_aries_cached_vars[$key] = $val;
			}
		}
	}
	
	// --------------------------------------------------------------------
	
	/**
	 * Loader
	 *
	 * This function is used to load views and files.
	 * Variables are prefixed with _ci_ to avoid symbol collision with
	 * variables made available to view files
	 *
	 * @param	array
	 * @return	void
	 */
	protected function _aries_load($_aries_data)
	{
		// Set the default data variables
		foreach (array('_aries_view', '_aries_vars', '_aries_path') as $_aries_val)
		{
			$$_aries_val = ( !isset($_aries_data[$_aries_val])) ? FALSE : $_aries_data[$_aries_val];
		}
	
		$file_exists = FALSE;
	
		// Set the path to the requested file
		if ($_aries_path != '')
		{
			$_aries_x = explode('/', $_aries_path);
			$_aries_file = end($_aries_x);
		}
		else
		{
			$_aries_ext = pathinfo($_aries_view, PATHINFO_EXTENSION);
			$_aries_file = ($_aries_ext == '') ? $_aries_view.'.php' : $_aries_view;
	
			foreach ($this->_aries_view_paths as $view_file => $cascade)
			{
				if (file_exists($view_file.$_aries_file))
				{
					$_aries_path = $view_file.$_aries_file;
					$file_exists = TRUE;
					break;
				}
	
				if (!$cascade)
				{
					break;
				}
			}
		}
	
		if ( !$file_exists && ! file_exists($_aries_path))
		{
			trigger_error('Unable to load the requested file: '.$_aries_file, E_USER_ERROR);
		}
	
		// This allows anything loaded using $this->load (views, files, etc.)
		// to become accessible from within the Controller and Model functions.
	
		$_aries_ARIES =& get_instance();
		foreach (get_object_vars($_aries_ARIES) as $_aries_key => $_aries_var)
		{
			if ( !isset($this->$_aries_key))
			{
				$this->$_aries_key =& $_aries_ARIES->$_aries_key;
			}
		}
	
		/*
		 * Extract and cache variables
		*
		* You can either set variables using the dedicated $this->load_vars()
		* function or via the second parameter of this function. We'll merge
		* the two types and cache them so that views that are embedded within
		* other views can have access to these variables.
		*/
		if (is_array($_aries_vars))
		{
			$this->_aries_cached_vars = array_merge($this->_aries_cached_vars, $_aries_vars);
		}
		extract($this->_aries_cached_vars);
		
		include($_aries_path);
	}
	
	// --------------------------------------------------------------------

	/**
	 * Load View
	 *
	 * This function is used to load a "view" file.  It has two parameters:
	 *
	 * 1. The name of the "view" file to be included.
	 * 2. An associative array of data to be extracted for use in the view.
	 *
	 * @param	string
	 * @param	array
	 * @return	void
	 */
	public function view($view, $vars = array())
	{
		return $this->_aries_load(
				array(
						'_aries_view' => $view, 
						'_aries_vars' => $this->_aries_object_to_array($vars)
				)
		);
	}
	
	// --------------------------------------------------------------------
	
	/**
	 * Autoloader
	 *
	 * The config/autoload.php file contains an array that permits sub-systems,
	 * libraries, and helpers to be loaded automatically.
	 *
	 * @param	array
	 * @return	void
	 */
	private function _aries_autoloader()
	{
		if (defined('ENVIRONMENT') AND file_exists(APPPATH.'backend/config/'.ENVIRONMENT.'/autoload.php'))
		{
			include(APPPATH.'backend/config/'.ENVIRONMENT.'/autoload.php');
		}
		else
		{
			include(APPPATH.'backend/config/autoload.php');
		}
	
		if ( ! isset($autoload))
		{
			return FALSE;
		}
	
		// Autoload packages
		if (isset($autoload['packages']))
		{
			foreach ($autoload['packages'] as $package_path)
			{
				$this->add_package_path($package_path);
			}
		}
	
		// Load any custom config file
		if (count($autoload['config']) > 0)
		{
			$CI =& get_instance();
			foreach ($autoload['config'] as $key => $val)
			{
				$CI->config->load($val);
			}
		}
	
		// Autoload helpers and languages
		foreach (array('helper', 'language') as $type)
		{
			if (isset($autoload[$type]) AND count($autoload[$type]) > 0)
			{
				$this->$type($autoload[$type]);
			}
		}
	
		// A little tweak to remain backward compatible
		// The $autoload['core'] item was deprecated
		if ( ! isset($autoload['libraries']) AND isset($autoload['core']))
		{
			$autoload['libraries'] = $autoload['core'];
		}
	
		// Load libraries
		if (isset($autoload['libraries']) AND count($autoload['libraries']) > 0)
		{
			// Load the database driver.
			if (in_array('database', $autoload['libraries']))
			{
				$this->database();
				$autoload['libraries'] = array_diff($autoload['libraries'], array('database'));
			}
	
			// Load all other libraries
			foreach ($autoload['libraries'] as $item)
			{
				$this->library($item);
			}
		}
	}
}
?>