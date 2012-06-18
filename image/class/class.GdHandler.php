<?php

/**
 * 
 * Adaptor for built-in PHP GD library
 * 
 * @author Haleeq Usman
 * @copyright Completely Free. Use, extend, or distribute as you wish.
 *
 */
final class GdHandler extends ImageHandler {
	private $canvas;
	
	public function __construct($filename)
	{
		parent::__construct($filename);	
	}
	
	private function allocateResource($width, $height)
	{
		if(!is_numeric($width) || !is_numeric($height)) {
			trigger_error("Please enter a valid width/height", E_USER_ERROR);
			return FALSE;
		}
		
		if($this->canvas != NULL) {
			imagedestroy($this->canvas);
		}
		
		if(!$this->canvas = @imagecreatetruecolor($width, $height)) {
			trigger_error(
				"The image resource could not be allocated. " .
				"Perhaps you do not have enough memory?", E_USER_ERROR
			);
			return FALSE;
		}
		
		return TRUE;
	}
	
	public function hResize(
							$width, $height,
							$content_width, 
							$content_height,
							$backgroundColor = 0xFFFFFF
							)
	{
		if(!isset($backgroundColor) || $backgroundColor == NULL) {
			$backgroundColor = 0xFFFFFF;
		}
		
		if($this->allocateResource($width, $height)) {
			$source = imagecreatefromjpeg($this->filename);
			
			$color_value = imagecolorallocate(
								$this->canvas,
								$backgroundColor >> 16,
								($backgroundColor & 0x00FF00) >> 8,
								$backgroundColor & 0x0000FF
							);
			imagefill($this->canvas, 0, 0, $color_value);
			
			$dest_x = intval(($width - $content_width) / 2);
			$dest_y = intval(($height - $content_height) / 2);
			
			$resized = imagecopyresized
			(
				$this->canvas, $source, $dest_x, $dest_y, 0, 0,
				$content_width, $content_height, $this->width, $this->height
			);
		}
		
		return $resized? $this : NULL;
	}
	
	public function hRotate($degrees = 0, $backgroundColor = 0x0000FF)
	{
		if(!is_numeric($degrees)) {
			trigger_error("Please enter a numeric degree!", E_USER_ERROR);
			return FALSE;
		}
		if(!isset($backgroundColor) || $backgroundColor == NULL) {
			$backgroundColor = 0xFFFFFF;
		}
		
		$source = imagecreatefromjpeg($this->filename);
			
		$color_value = imagecolorallocate(
							$source,
							$backgroundColor >> 16,
							($backgroundColor & 0x00FF00) >> 8,
							$backgroundColor & 0x0000FF
						);
						
		$this->canvas = imagerotate($source, $degrees, $color_value);
		
		return  $this->canvas !== FALSE? $this : NULL;
	}
	
	public function hScale($x_factor = 1, $y_factor = 1)
	{
		$width = $this->width * $x_factor;
		$height = $this->height * $y_factor;
		
		if($this->allocateResource($width, $height)) {
			$source = imagecreatefromjpeg($this->filename);
			
			$resized = imagecopyresized
			(
				$this->canvas, $source, 0, 0, 0, 0,
				$width, $height, $this->width, $this->height
			);
		}
		
		return $resized? $this : NULL;
	}
	
	public function hSave($filename, $quality = 80)
	{
		if(!isset($filename) || $filename == NULL) {
			trigger_error("Please enter a valid filename!", E_USER_WARNING);
			return FALSE;
		}
		if($this->canvas == NULL) {
			trigger_error("A valid image resource was not found!", E_USER_WARNING);
			return FALSE;
		}
		
		if (!imagejpeg($this->canvas, $filename, $quality)) {
			return FALSE;
		}
		
		return TRUE;
	} 
}

?>