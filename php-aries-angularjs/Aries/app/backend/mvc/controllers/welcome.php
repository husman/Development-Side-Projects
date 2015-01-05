<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Welcome extends ARIES_Controller {

	/**
	 * Index Page for this controller.
	 *
	 * Maps to the following URL
	 * 		http://example.com/welcome
	 *	- or -  
	 * 		http://example.com/welcome/index
	 *	- or -
	 * Since this controller is set as the default controller in 
	 * config/routes.php, it's displayed at http://localhost/
	 *
	 */
	public function index()
	{
		$query = $this->db->query("SELECT * FROM accounts");
		
		$data = array(
			'users'			=> $query->fetch_all(),
			'num_rows'		=> $query->num_rows(),
			'num_fields'	=> $query->num_fields()
		);
		
		$this->load->view('welcome_message', $data);
	}
}

?>