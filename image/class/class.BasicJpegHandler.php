<?php

/**
 * 
 * Adaptor for my custom jpeg encoder/decoder
 * 
 * @author Haleeq Usman
 * @copyright Completely Free. Use, extend, or distribute as you wish.
 *
 */
final class BasicJpegHandler  extends ImageHandler
{
	// Implementation coming soon.

	// File format to support: JFIF
	
	// Mathematical classes to implement: Linear Algebra
	
	// Image compression algorithms to implement:
	
	// 1: YUV420 down sample from RGB888 (Integer-based algorithm)
	// 2: DCT (mathematical coordinate rotation) of the YUV macroblocks (Fast)
	// 3: Quantization
	// 4: Run-length encoding (if necessary -AC components)
	// 5: Huffman encode
	// 6: Save in JFIF file format
	
	public function hResize
	(
		$canvas_width, $canvas_height,
		$content_width, $content_height
	)
	{
		
	}
	
	public function hRotate($degrees = 0)
	{
		
	}
	
	public function hScale($x_factor = 1, $y_factor = 1)
	{
		
	}
	
	public function hSave($filename)
	{
		
	}
}

?>