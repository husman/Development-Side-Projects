<?php

/**
 * 
 * Template class for the image handlers. All image handlers must
 * extend and implement the abstract methods of this class. This
 * class contains the properties/methods common to all image handlers.
 * 
 * @author Haleeq Usman
 * @copyright Completely Free. Use, extend, or distribute as you wish.
 *
 */
abstract class ImageHandler
{
	protected $width;
	protected $height;
	protected $filename;
	protected $type;
	protected $ratio;
	
	public function __construct($filename)
	{
		if(!isset($filename) || $filename == NULL) {
			trigger_error("Please specific a filename!", E_USER_WARNING);
		}
		
		// Lets make sure the file exists and is accessible
		$this->filename = $filename;
		if(!file_exists($this->filename)) {
			trigger_error
			(
				"The file: ".$this->filename. " could not be found or is " .
				"inaccessible." , E_USER_WARNING
			);
		}
		
		// Let's initialize the image's width, height, and type
		list($this->width, $this->height, $this->type) = getimagesize($this->filename);
		
		if($this->width > 0 && $this->height > 0) {
			$this->ratio = $this->width/$this->height;
		} else {
			trigger_error("The image could not be initialized!", E_USER_WARNING);
		}
		
	}
	
	public function getHeight()
	{
		return $this->height;
	}
	
	public function getWidth()
	{
		return $this->width;
	}
	
	public function getImageType()
	{
		return $this->type;
	}
	
	protected function strToHex($string)
	{
		$hex='';
    	for ($i=0; $i < strlen($string); $i++)
    	{
        	$hex .= dechex(ord($string[$i]));
    	}
    	
    	return $hex;
	}
	
	//Template functions/methods. These are the methods that will be called.
	public function resize($width, $height, $backgroundColor = 0xFFFFFF)
	{
		// Let's see which property is bigger 
		// and set the dimentions based of that
		
		if($width > $height) {
			$content_width = $width;
			$content_height = $content_width/$this->ratio;
		} else {
			$content_height = $height;
			$content_width = $content_height*$this->ratio;
		}
		
		return  $this->hResize(
								$width,
								$height,
								$content_width,
								$content_height,
								$backgroundColor
							  );
	}
	
	public function rotate($degrees = 0, $backgroundColor = 0x0000FF)
	{
		return $this->hRotate($degrees, $backgroundColor);
	}
	
	public function scale($x_factor = 1, $y_factor = 1)
	{
		return $this->hScale($x_factor, $y_factor);
	}
	
	public function save($filename)
	{
		return $this->hSave($filename);
	}
	
	// Image handler specific algorithms.
	public abstract function hResize
	(
		$canvas_width, $canvas_height,
		$content_width, $content_height
	);
	
	public abstract function hRotate($degrees = 0);
	
	public abstract function hScale($x_factor = 1, $y_factor = 1);
	
	public abstract function hSave($filename);
	
}

?>