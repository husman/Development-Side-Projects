<?php

require_once 'class/class.ImageHandler.php';
require_once 'class/class.GdHandler.php';
require_once 'class/class.ImHandler.php';
require_once 'class/class.BasicJpegHandler.php';

/**
 * 
 * Adaptor class providing an abstraction of the image handlers.
 * The image manipulation methods should be accessed through this
 * adaptor's $handler property. Example: $image->handler->resize(150, 200);
 * 
 * @author Haleeq Usman
 * @copyright Completely Free. Use, extend, or distribute as you wish.
 *
 */
class Image
{
	
	public $handler = NULL; // used to access the image manipulation functions
	private $filename;
	
	public function __construct($filename, $image_handler = "GD")
	{
		if(!isset($filename) || $filename == NULL) {
			trigger_error("Please specific a filename!", E_USER_WARNING);
		}
		if(!isset($image_handler) || $image_handler == NULL) {
			$image_handler = "GD";
		}
		
		$this->filename = $filename;
		$this->setHandler("GD");
	}
		
	/**
	 * 
	 * Sets the image handler (or library) to use for processing.
	 * 
	 * @param String $image_handler The string representation of
	 * the handler/library to use:
	 * 
	 * GD = GD Library/Handler (Default)
	 * IM = ImageMagick Library/Handler
	 * BJPEG = BasicJpegHandler library/Handler
	 * 
	 * @return Object The image handler.
	 * 
	 */
	public function setHandler($image_handler = "GD")
	{
		if(!isset($image_handler) || $image_handler == NULL) {
			$image_handler = "GD";
		}
		
		switch (strtoupper($image_handler)) {
			case "GD":
				if($this->handler instanceof GdHandler === FALSE)
					$this->handler = new GdHandler($this->filename);
			break;
			case "IM":
				if($this->handler instanceof ImHandler === FALSE)
					$this->handler = new ImHandler();
			break;
			case "BJPEG":
				if($this->handler instanceof BasicJpegHandler === FALSE)
					$this->handler = new BasicJpegHandler();
			break;
			default:
				if($this->handler instanceof GdHandler === FALSE)
					$this->handler = new GdHandler();
			break;
		}
		
		return $this->handler;
	}
	
	/**
	 * 
	 * Invokes if an undefined method within this object is called.
	 * 
	 * @param String $name_The name of the method called.
	 * @param Array $args An enumerated array containing the 
	 * parameters passed to the method.
	 * 
	 * 
	 */
	public function __call($name, $args)
	{
		trigger_error(
						'Calling non-existing object method: $image->' . $name .
						'(' . implode(', ', $args) . ');<br /><br />' .
						'Perhaps you meant: $image-><b>handler</b>->' . $name .
						'(' . implode(', ', $args) . ')?' , E_USER_WARNING
					 );
	}
	
}

?>
