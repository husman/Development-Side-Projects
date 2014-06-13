package org.chineseforall.core.graphics
{
	import flash.display.GraphicsPath;
	import flash.display.GraphicsSolidFill;
	import flash.display.IGraphicsData;
	import flash.display.Sprite;
	import flash.geom.Point;
	
	import mx.controls.Alert;
	
	import org.chineseforall.net.Net;

	public class Shapes
	{
		public static const PI:Number = Math.PI;
		public static const TAU:Number = 2*Math.PI;
		public static const E:Number = Math.E;

		public static function drawCircle(shape:Sprite, fillColor:uint = 0xffffff,
										  lineColor:uint = 0x000000, fillEnabled:Boolean = true,
										  strokeEnabled:Boolean = true, lineWidth:uint = 1,
										  dashed:Boolean = false, dashLength:Number = 10,
										  x_center:int = 0, y_center:int = 0,
										  radius:Number = 2, segments:int = 360):void {
			if(fillEnabled || strokeEnabled) {
				if (dashed) {
					var positive_radius:Number = Math.abs(radius),
						circumference:Number = TAU * positive_radius;
					if (circumference <= dashLength) {
						shape.graphics.drawCircle(x_center, y_center, radius);
						return;
					}
					
					//Alert.show("made it");
					
					var fill_factor:Number = 0.93,
						angleStep:Number = TAU/segments,
						angle:Number = 0,
						x:Number = Math.cos(angle) * radius,
						y:Number = Math.sin(angle) * radius,
						distance:Number,
						dashNum:Number,
						dashedPath:GraphicsPath = new GraphicsPath(new Vector.<int>(), new Vector.<Number>()),
						fillPath:GraphicsPath = new GraphicsPath(new Vector.<int>(), new Vector.<Number>()),
						colorFill:GraphicsSolidFill = new GraphicsSolidFill(fillColor,0.9);
					
					// Let's make the dash more visible for the smaller
					if(positive_radius >= 50 && positive_radius <= 100) {
						fill_factor = 0.95;
					}
					else if(positive_radius > 100 && positive_radius <= 150) {
						fill_factor = 0.95;
					}
					else if(positive_radius > 150 && positive_radius <= 200) {
						fill_factor = 0.98;
					}
					else if(positive_radius > 200) {
						fill_factor = 0.99;
					}
					
					if(strokeEnabled) {
					dashedPath.commands.push(1);
					dashedPath.data.push(x_center + x, y_center + y);
					}
					if(fillEnabled) {
						fillPath.commands.push(1);
						fillPath.data.push(x_center + x*fill_factor, y_center + y*fill_factor);
					}
	
					for (angle = angleStep; angle < TAU; angle += angleStep) {
						x = Math.cos(angle) * radius;
						y = Math.sin(angle) * radius;
						distance = angle * positive_radius;
						dashNum = Math.floor((distance / dashLength) % 2); 
						// determine whether to draw the dashed line or move ahead
						if (dashNum == 0) {
							// approximate the circle with a line (step size is small)
							if(strokeEnabled) {
								dashedPath.commands.push(2);
								dashedPath.data.push(x_center + x, y_center + y);
							}
							if(fillEnabled) {
								fillPath.commands.push(2);
								fillPath.data.push(x_center + x*fill_factor, y_center + y*fill_factor);
							}
						} else {
							if(strokeEnabled) {
								dashedPath.commands.push(1);
								dashedPath.data.push(x_center + x, y_center + y);
							}
							if(fillEnabled) {
								fillPath.commands.push(2);
								fillPath.data.push(x_center + x*fill_factor, y_center + y*fill_factor);
							}
						}
					}
	
					// Let's draw our shape
					var drawDashedStroke:Vector.<IGraphicsData> = new Vector.<IGraphicsData>(),
						drawSolidFill:Vector.<IGraphicsData> = new Vector.<IGraphicsData>();
					
					if(strokeEnabled) {
						drawDashedStroke.push(dashedPath);
						shape.graphics.lineStyle(lineWidth, lineColor);
						shape.graphics.drawGraphicsData(drawDashedStroke);
					}
					
					if(fillEnabled) {
						drawSolidFill.push(colorFill, fillPath);
						shape.graphics.lineStyle(lineWidth, colorFill.color);
						shape.graphics.drawGraphicsData(drawSolidFill);
					}
					return;
				}
			}
			
			if(strokeEnabled) {
				shape.graphics.lineStyle(lineWidth, lineColor, 1,false, "none");
			}
			if(fillEnabled) {
				shape.graphics.beginFill(fillColor);
			}
			shape.graphics.drawCircle(x_center, y_center, radius);
		}
		
		/**
		 * Draws a straight line between the starting and ending points.  
		 * The line defaults to a solid line, but can also be a dashed line.
		 * If dashed is false then the Graphics.lineTo() function is used.
		 * @param start the starting point
		 * @param end the end point
		 * @param dashed if true then the line will be dashed
		 * @param dashLength the length of the dash (only applies if dashed is true)
		 */
		public static function drawLine(shape:Sprite, start:Point, end:Point,
										lineColor:uint = 0x000000, strokeEnabled:Boolean = true,
										lineWidth:uint = 1,dashed:Boolean = false,
										dashLength:Number = 10):void {
			if(strokeEnabled) {
				shape.graphics.lineStyle(lineWidth, lineColor, 1, false, "none");
				shape.graphics.moveTo(start.x, start.y);
				if (dashed) {
					// the distance between the two points
					var total:Number = Point.distance(start, end);
					// divide the distance into segments
					if (total <= dashLength) {
						// just draw a solid line since the dashes won't show up
						shape.graphics.lineTo(end.x, end.y);
					} else {
						// divide the line into segments of length dashLength 
						var step:Number = dashLength / total;
						var dashOn:Boolean = false;
						var p:Point;
						for (var t:Number = step; t <= 1; t += step) {
							p = getLinearValue(t, start, end);
							dashOn = !dashOn;
							if (dashOn) {
								shape.graphics.lineTo(p.x, p.y);
							} else {
								shape.graphics.moveTo(p.x, p.y);
							}
						}
						// finish the line if necessary
						dashOn = !dashOn;
						if (dashOn && !end.equals(p)) {
							shape.graphics.lineTo(end.x, end.y);
						}
					}
				} else {
					// use the built-in lineTo function
					shape.graphics..lineTo(end.x, end.y);
				}
			}
		}
		
		/**
		 * Draws a rectangle at the given x, y, width, and height coordinates.
		 * You can also specify the corner radii for the rectangle.  If the cornerRadii contains a single
		 * Number then that radius is doubled and used as the ellipse width and height to make rounded corners.
		 * If the cornerRadii has two numbers then those numbers are doubled and used as the ellipse width/height.
		 * If the cornerRadii has four numbers then those are used as the four corner radii.
		 * Otherwise a rectangle is drawn with no corner radius.
		 */
		public static function drawRect(shape:Sprite,
										start_x:Number, start_y:Number,
										width:Number, height:Number,
										fillColor:uint = 0xffffff, lineColor:uint = 0x000000,
										fillEnabled:Boolean = true, strokeEnabled:Boolean = true,
										lineWidth:uint = 1, dashed:Boolean = false, dashLength:Number = 10, selTool:Boolean = false):void {
			if(strokeEnabled || fillEnabled) {
				var coord_top_left:Point = new Point(start_x, start_y),
					coord_top_right:Point = new Point(width, start_y),
					coord_bottom_right:Point = new Point(width, height),
					coord_bottom_left:Point = new Point(start_x, height);
				
				if(selTool) {
					shape.graphics.beginFill(0x336600, 0.7);
					shape.graphics.drawCircle(coord_top_left.x, coord_top_left.y, 2);
					shape.graphics.endFill();
				}
				drawLine(shape, coord_top_left, coord_top_right,
						 lineColor, strokeEnabled, lineWidth, dashed, dashLength);
				
				if(selTool) {
					shape.graphics.beginFill(0xFF0000, 0.7);
					shape.graphics.drawCircle(coord_top_right.x, coord_top_right.y, 2);
					shape.graphics.endFill();
				}
				drawLine(shape, coord_top_right, coord_bottom_right,
						 lineColor, strokeEnabled, lineWidth, dashed, dashLength);
				
				if(selTool) {
					shape.graphics.beginFill(0x336600, 0.7);
					shape.graphics.drawCircle(coord_bottom_right.x, coord_bottom_right.y, 2);
					shape.graphics.endFill();
				}
				drawLine(shape, coord_bottom_right, coord_bottom_left,
						 lineColor, strokeEnabled, lineWidth, dashed, dashLength);
				
				if(selTool) {
					shape.graphics.beginFill(0xFF0000, 0.7);
					shape.graphics.drawCircle(coord_bottom_left.x, coord_bottom_left.y, 2);
					shape.graphics.endFill();
				}
				drawLine(shape, coord_bottom_left, coord_top_left,
						 lineColor, strokeEnabled, lineWidth, dashed, dashLength);
				
				if(fillEnabled) {
					shape.graphics.lineStyle(1, fillColor, 1, false, "none");
					shape.graphics.beginFill(fillColor);
					shape.graphics.drawRect(start_x+1, start_y+1, width-2, height-2);
				}
			}
		}
		
		public static function drawTriangle(shape:Sprite,
										start_x:Number, start_y:Number,
										width:Number, height:Number,
										fillColor:uint = 0xffffff, lineColor:uint = 0x000000,
										fillEnabled:Boolean = true, strokeEnabled:Boolean = true,
										lineWidth:uint = 1, dashed:Boolean = false, dashLength:Number = 10):void {
			if(strokeEnabled || fillEnabled) {
				var verticesPath:GraphicsPath = new GraphicsPath(new Vector.<int>(), new Vector.<Number>());
				
				
				var coord_point_1:Point = new Point(start_x, start_y),
					coord_point_2:Point = new Point(width/2, height),
					coord_point_3:Point = new Point(width, start_y);
				
				if(strokeEnabled) {
					drawLine(shape, coord_point_1, coord_point_2,
						lineColor, strokeEnabled, lineWidth, dashed, dashLength);
					
					drawLine(shape, coord_point_2, coord_point_3,
						lineColor, strokeEnabled, lineWidth, dashed, dashLength);
					
					drawLine(shape, coord_point_3, coord_point_1,
						lineColor, strokeEnabled, lineWidth, dashed, dashLength);
				}
				
				if(fillEnabled) {
					var x_sign:int = width < 0? -1: 1,
						y_sign:int = height < 0? -1 : 1;
					
					width = x_sign * (Math.abs(width) - 3);
					height = y_sign * (Math.abs(height) - 2);
					start_x = start_x + x_sign * 3;
					start_y = start_y + y_sign * 1;
					
					shape.graphics.lineStyle(1, fillColor, 1, false, "none");
					shape.graphics.beginFill(fillColor);
					
					shape.graphics.moveTo(start_x, start_y);	
					shape.graphics.lineTo((width+x_sign*2)/2, height);
					shape.graphics.lineTo(width, start_y);
				}
			}
		}
		
		public static function drawArrowHead(shape:Sprite, start:Point, end:Point,
											 fillColor:uint = 0xffffff, lineColor:uint = 0x000000,
											 fillEnabled:Boolean = true, strokeEnabled:Boolean = true,
											 lineWidth:uint = 1, dashed:Boolean = false,
											 arrowType:String = "right", dashLength:Number = 10):void {
			if(strokeEnabled) {
				var vector_angle:Number = Math.atan((end.y - start.y)/(end.x - start.x)),
					angle1:Number = vector_angle + 45*PI/180,
					angle2:Number = vector_angle - 45*PI/180,
					vector_angle2:Number = Math.atan((start.y - end.y)/(start.x - end.x)),
					angle1b:Number = vector_angle2 + 45*PI/180,
					angle2b:Number = vector_angle2 - 45*PI/180,
					arrowHeadLen:uint = 3*lineWidth < 15? 4.5*lineWidth : 3.5*lineWidth,
					x_coord_polarity:int = end.x < 0? 1 : -1,
					x_coord_polarity2:int = end.x > 0? 1 : -1;
				
				// Adjust arrowhead size
				if(arrowHeadLen < 10) {
					arrowHeadLen = 12;
				}
				var arrowHeadPoint1:Point = new Point(
					end.x + x_coord_polarity*arrowHeadLen*Math.cos(angle1),
					end.y + x_coord_polarity*arrowHeadLen*Math.sin(angle1)
					),
					arrowHeadPoint2:Point = new Point(
						end.x + x_coord_polarity*arrowHeadLen*Math.cos(angle2),
						end.y + x_coord_polarity*arrowHeadLen*Math.sin(angle2)
					),
					arrowHeadPoint1b:Point = new Point(
						start.x + x_coord_polarity2*arrowHeadLen*Math.cos(angle1b),
						start.y + x_coord_polarity2*arrowHeadLen*Math.sin(angle1b)
					),
					arrowHeadPoint2b:Point = new Point(
						start.x + x_coord_polarity2*arrowHeadLen*Math.cos(angle2b),
						start.y + x_coord_polarity2*arrowHeadLen*Math.sin(angle2b)
					);
				shape.graphics.lineStyle(lineWidth, lineColor, 1, false, "none");
				shape.graphics.moveTo(start.x, start.y);
				if (dashed) {
					// the distance between the two points
					var total:Number = Point.distance(start, end);
					// divide the distance into segments
					if (total <= dashLength) {
						// just draw a solid line since the dashes won't show up
						shape.graphics.lineTo(end.x, end.y);
					} else {
						// divide the line into segments of length dashLength 
						var step:Number = dashLength / total;
						var dashOn:Boolean = false;
						var p:Point;
						for (var t:Number = step; t <= 1; t += step) {
							p = getLinearValue(t, start, end);
							dashOn = !dashOn;
							if (dashOn) {
								shape.graphics.lineTo(p.x, p.y);
							} else {
								shape.graphics.moveTo(p.x, p.y);
							}
						}
						// finish the line if necessary
						dashOn = !dashOn;
						if (dashOn && !end.equals(p)) {
							shape.graphics.lineTo(end.x, end.y);
						}
					}
					if(arrowType === "right" || arrowType === "both") {
						shape.graphics.moveTo(end.x, end.y);
						if(fillEnabled) {
							shape.graphics.lineStyle(lineWidth, fillColor, 1, false, "none");
							shape.graphics.beginFill(fillColor);
						} else {
							shape.graphics.beginFill(lineColor);
						}
						shape.graphics.lineTo(arrowHeadPoint1.x, arrowHeadPoint1.y);
						shape.graphics.moveTo(end.x, end.y);
						shape.graphics.lineTo(arrowHeadPoint2.x, arrowHeadPoint2.y);
						shape.graphics.lineTo(arrowHeadPoint1.x, arrowHeadPoint1.y);
					}
					if(arrowType === "left" || arrowType === "both") {
						shape.graphics.moveTo(start.x, start.y);
						if(fillEnabled) {
							shape.graphics.lineStyle(lineWidth, fillColor, 1, false, "none");
							shape.graphics.beginFill(fillColor);
						} else {
							shape.graphics.beginFill(lineColor);
						}
						shape.graphics.lineTo(arrowHeadPoint1b.x, arrowHeadPoint1b.y);
						shape.graphics.moveTo(start.x, start.y);
						shape.graphics.lineTo(arrowHeadPoint2b.x, arrowHeadPoint2b.y);
						shape.graphics.lineTo(arrowHeadPoint1b.x, arrowHeadPoint1b.y);
					}
				} else {
					// use the built-in lineTo function
					shape.graphics.lineTo(end.x, end.y);
					
					if(arrowType === "right" || arrowType === "both") {
						shape.graphics.moveTo(end.x, end.y);
						if(fillEnabled) {
							shape.graphics.lineStyle(lineWidth, fillColor, 1, false, "none");
							shape.graphics.beginFill(fillColor);
						} else {
							shape.graphics.beginFill(lineColor);
						}
						shape.graphics.lineTo(arrowHeadPoint1.x, arrowHeadPoint1.y);
						shape.graphics.moveTo(end.x, end.y);
						shape.graphics.lineTo(arrowHeadPoint2.x, arrowHeadPoint2.y);
						shape.graphics.lineTo(arrowHeadPoint1.x, arrowHeadPoint1.y);
					}
					if(arrowType === "left" || arrowType === "both") {
						shape.graphics.moveTo(start.x, start.y);
						if(fillEnabled) {
							shape.graphics.lineStyle(lineWidth, fillColor, 1, false, "none");
							shape.graphics.beginFill(fillColor);
						} else {
							shape.graphics.beginFill(lineColor);
						}
						shape.graphics.lineTo(arrowHeadPoint1b.x, arrowHeadPoint1b.y);
						shape.graphics.moveTo(start.x, start.y);
						shape.graphics.lineTo(arrowHeadPoint2b.x, arrowHeadPoint2b.y);
						shape.graphics.lineTo(arrowHeadPoint1b.x, arrowHeadPoint1b.y);
					}
				}
			}
		}
		
		/**
		 * Calculates the point along the linear line at the given "time" t (between 0 and 1).
		 * Formula came from
		 * http://en.wikipedia.org/wiki/B%C3%A9zier_curve#Linear_B.C3.A9zier_curves
		 * @param t the position along the line [0, 1]
		 * @param start the starting point
		 * @param end the end point
		 */
		public static function getLinearValue(t:Number, start:Point, end:Point):Point {
			t = Math.max(Math.min(t, 1), 0);
			var x:Number = start.x + (t * (end.x - start.x));
			var y:Number = start.y + (t * (end.y - start.y));
			return new Point(x, y);    
		}
		
	}
}