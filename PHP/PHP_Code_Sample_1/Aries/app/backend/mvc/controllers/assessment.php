<?php if (!defined('BASEPATH')) exit('No direct script access allowed');
class Assessment extends ARIES_Controller {
	
    public function add() {
    	$data = array();
    	$this->load->view("createAssessment", $data);
    }
    
    function templates() {
    	$return = "";
    	// Once more functionality is added to the Router, then there would be no need for direct access to 
    	// these variables. Instead, the params will be broken down into pieces and accessed through a variable.
    	$template = $_GET['param'];
    	switch($template) {
    		case "section":
    			$return = file_get_contents('./app/backend/mvc/views/templates/assessment/section.html');
    			break;
    		case "multipleChoice":
    			$return = file_get_contents('./app/backend/mvc/views/templates/assessment/questions/multipleChoice.html');
    			break;
    		case "fillInTheBlanks":
    			$return = file_get_contents('./app/backend/mvc/views/templates/assessment/questions/fillInTheBlanks.html');
    			break;
    		case "matching":
    			$return = file_get_contents('./app/backend/mvc/views/templates/assessment/questions/matching.html');
    			break;
    	}
    	echo $return;
    }
    
}
?>