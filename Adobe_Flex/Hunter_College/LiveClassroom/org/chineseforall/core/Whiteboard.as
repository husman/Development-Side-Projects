package org.chineseforall.core
{
	import com.gskinner.utils.PerformanceTest;
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.DisplayObject;
	import flash.display.Graphics;
	import flash.display.GraphicsPath;
	import flash.display.GraphicsSolidFill;
	import flash.display.IGraphicsData;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.FocusEvent;
	import flash.events.MouseEvent;
	import flash.filters.BitmapFilterQuality;
	import flash.filters.GlowFilter;
	import flash.geom.Matrix;
	import flash.geom.Point;
	import flash.geom.Rectangle;
	import flash.utils.ByteArray;
	import flash.utils.setTimeout;
	
	import mx.binding.utils.BindingUtils;
	import mx.controls.Alert;
	import mx.core.FlexGlobals;
	import mx.core.INavigatorContent;
	import mx.core.IVisualElement;
	import mx.core.UIComponent;
	import mx.events.ResizeEvent;
	import mx.graphics.GradientStroke;
	import mx.managers.FocusManager;
	import mx.messaging.channels.StreamingAMFChannel;
	import mx.utils.GraphicsUtil;
	import mx.utils.StringUtil;
	
	import org.chineseforall.core.graphics.Shapes;
	import org.chineseforall.net.Net;
	
	import spark.components.RichEditableText;
	
	public class Whiteboard
	{
		public const MOUSE_STATE_DOWN:uint = 0x1;
		public const MOUSE_STATE_UP:uint = 0x2;
		
		private const MAX_SHAPE_RESIZE_SPEED:uint = 200;
		private const BRUSH_MAX_DASHLEN:Number = 15;
		
		private var app:App = null;
		public var canvas:UIComponent = null; /* @TODO REVISE */
		private var tmp_sprite:Sprite = null;
		private var tmp_user_text:RichEditableText = null;
		private var last_mouse_location:Point = null;
		private var last_mouse_move_time:uint = 0;
		private var focused_object:String = "none";
		
		private var original_sprite_x:Number = 0;
		private var original_sprite_y:Number = 0;
		private var brush_data:GraphicsPath = new GraphicsPath(new Vector.<int>(), new Vector.<Number>());
		private var brush_dash_next:Boolean = false;
		private var brush_dash_last_loc:Point = null;
		private var brush_dashlen:Number = BRUSH_MAX_DASHLEN;
		
		private var lineLayer:Sprite;
		private var lastSmoothedMouseX:Number;
		private var lastSmoothedMouseY:Number;
		private var lastMouseX:Number;
		private var lastMouseY:Number;
		private var lastThickness:Number;
		private var lastRotation:Number;
		private var lineColor:uint;
		private var lineThickness:Number;
		private var lineRotation:Number;
		private var L0Sin0:Number;
		private var L0Cos0:Number;
		private var L1Sin1:Number;
		private var L1Cos1:Number;
		private var sin0:Number;
		private var cos0:Number;
		private var sin1:Number;
		private var cos1:Number;
		private var dx:Number;
		private var dy:Number;
		private var dist:Number;
		private var targetLineThickness:Number;
		private var colorLevel:Number;
		private var targetColorLevel:Number;
		private var smoothedMouseX:Number;
		private var smoothedMouseY:Number;
		private var tipLayer:Sprite;
		public var bitmapHolder:Sprite;
		private var boardWidth:Number;
		private var boardHeight:Number;
		private var smoothingFactor:Number;
		private var mouseMoved:Boolean;
		private var dotRadius:Number;
		private var startX:Number;
		private var startY:Number;
		private var undoStack:Vector.<BitmapData>;
		private var minThickness:Number;
		private var thicknessFactor:Number;
		private var mouseChangeVectorX:Number;
		private var mouseChangeVectorY:Number;
		private var lastMouseChangeVectorX:Number;
		private var lastMouseChangeVectorY:Number;
		private var tipTaperFactor:Number;
		private var controlPanel:Sprite;
		private var swatchColors:Vector.<uint>;
		private var paintColorR1:Number;
		private var paintColorG1:Number;
		private var paintColorB1:Number;
		private var paintColorR2:Number;
		private var paintColorG2:Number;
		private var paintColorB2:Number;
		private var red:Number;
		private var green:Number;
		private var blue:Number;
		private var colorChangeRate:Number;
		private var controlVecX:Number;
		private var controlVecY:Number;
		private var controlX1:Number;
		private var controlY1:Number;
		private var controlX2:Number;
		private var controlY2:Number;
		private var thicknessSmoothingFactor:Number;
		
		private var selection_obj:Object = null;
		
		
		public var mouse_state:uint = MOUSE_STATE_UP;
		public var boardBitmap:Bitmap;
		public var boardBitmapData:BitmapData;
		
		public function Whiteboard(app_handle:App)
		{
			app = app_handle;
			if(app !== null) {
				initialize();
			}
		}
		
		private function initialize():void
		{
			canvas = app.instance.canvas;
			canvas.width = app.instance.body.width;
			canvas.height = app.instance.body.height;
			//canvas.visible = true;
			//app.instance.whiteBoard.addElement(canvas);
			
			BindingUtils.bindProperty(canvas, "width", app.instance.body, "width");
			BindingUtils.bindProperty(canvas, "height", app.instance.body, "height");
			
			// Initialize our objects
			last_mouse_location = new Point();
			tmp_sprite = new Sprite();
			selection_obj = {
				content: null,
				haveSelection: false,
				rect: null
			};
			
			setupEventListeners();
			
			// Others
			minThickness = 0.2;
			thicknessFactor = 0.25;
			
			smoothingFactor = 0.3;  //Should be set to something between 0 and 1.  Higher numbers mean less smoothing.
			thicknessSmoothingFactor = 0.3;
			
			dotRadius = 2; //radius for drawn dot if there is no mouse movement between mouse down and mouse up.
			
			tipTaperFactor = 0.8;
			
			colorChangeRate = 0.05;
			
			paintColorR1 = 16;
			paintColorG1 = 0;
			paintColorB1 = 0;
			paintColorR2 = 128;
			paintColorG2 = 0;
			paintColorB2 = 0;
			
			boardBitmapData = new BitmapData(canvas.width-5, canvas.height-5, true, 0);
			boardBitmap = new Bitmap(boardBitmapData, "auto", true);
			
			//The undo buffer will hold the previous drawing.
			//If we want more levels of undo, we would have to record several undo buffers.  We only use one
			//here for simplicity.
			undoStack = new Vector.<BitmapData>;
			bitmapHolder = new Sprite();
			lineLayer = new Sprite();
			
			/*
			The tipLayer holds the tip portion of the line.
			Because of the smoothing technique we are using, while the user is drawing the drawn line will not
			extend all the way from the last position to the current mouse position.  We use a small 'tip' to 
			complete this line all the way to the current mouse position.
			*/
			tipLayer = new Sprite();
			tipLayer.mouseEnabled = false;
			
			/*
			Bitmaps cannot receive mouse events.  so we add it to a holder sprite.
			*/
			canvas.addChild(bitmapHolder);
			bitmapHolder.x = 2;
			bitmapHolder.y = 2;
			bitmapHolder.addChild(boardBitmap);
			bitmapHolder.addChild(tipLayer);
			
		}
		
		public function setupEventListeners():void
		{
			app.instance.body.addEventListener(MouseEvent.MOUSE_DOWN, handleMouseDown);
			app.instance.body.addEventListener(ResizeEvent.RESIZE, handleCanvasResize);
		}
		
		public function removeEventListeners():void
		{
			app.instance.body.removeEventListener(MouseEvent.MOUSE_DOWN, handleMouseDown);
			app.instance.body.removeEventListener(ResizeEvent.RESIZE, handleCanvasResize);
		}
		
		private function handleCanvasResize(e:ResizeEvent):void
		{
			if(e.oldWidth != app.instance.body.width) {
				//Alert.show("from: "+e.oldWidth.toString() + " to: "+ app.instance.whiteBoard.width.toString());
				bitmapHolder.width = canvas.width -5;
				bitmapHolder.height = canvas.height -5;
				if(app.instance.idle != null)
					app.instance.idle.width = canvas.width - 8;
			}
		}
		
		private function matrixToString(displayObject:DisplayObject, decimals:int = 5):String 
		{
			var str:String = '';
			var m:Matrix = displayObject.transform.matrix;
			
			str += m.a.toFixed(decimals) + ':';
			str += m.b.toFixed(decimals) + ':';
			str += m.c.toFixed(decimals) + ':';
			str += m.d.toFixed(decimals) + ':';
			str += m.tx.toFixed(1) + ':';
			str += m.ty.toFixed(1);
			
			return str;
		}
		
		private var pbGPath:GraphicsPath = null;
		private var sharedBitMap:Bitmap = null;
		private var sharedBitmapData:BitmapData = null;
		private function handleMouseDown(e:MouseEvent):void
		{
			app.tools.cleanupSmToolMenus();
			if(tmp_sprite != null) {
				var coord:Point = e.currentTarget.globalToLocal(new Point(e.stageX, e.stageY));
				var pt:PerformanceTest = PerformanceTest.getInstance();
				var arrowHead:String = "none";
				var ba:ByteArray;
				var tmp_shared_obj:Object;
				
				switch(app.tools.activeTool) {
					case app.tools.TOOL_POINTERS_OPTION_POINTER:
						if(selection_obj.content != null) {
							//selection_obj.rect = selection_obj.content.getRect(bitmapHolder);
							if(original_sprite_x != selection_obj.content.x && original_sprite_y != selection_obj.content.y) {
								sharedBitmapData = selection_obj.bitmap.bitmapData;
								boardBitmapData.draw(selection_obj.bitmap, selection_obj.content.transform.matrix);
								/*Alert.show(selection_obj.content.x.toString() + "," +
										   selection_obj.content.y.toString() + " --> " +
										   selection_obj.content.transform.matrix.toString());*/
								ba = sharedBitmapData.getPixels(sharedBitmapData.rect);
								ba.compress();
								ba.position = 0;
								tmp_shared_obj = {
									type: "selection_move",
									width: sharedBitmapData.width,
									height: sharedBitmapData.height,
									rect_x: selection_obj.rect.x,
									rect_y: selection_obj.rect.y,
									rect_width: selection_obj.rect.width,
									rect_height: selection_obj.rect.height,
									data: ba,
									matrix_string: matrixToString(selection_obj.content)
								};
								app.net.shareDrawing(tmp_shared_obj);
							}
							canvas.removeChild(selection_obj.content);
						}
						// Create the container
						selection_obj.content = new Sprite();			
						selection_obj.content.x = coord.x;
						selection_obj.content.y = coord.y;
						canvas.addChild(selection_obj.content);
						
						selection_obj.content.alpha = 0.5;
						
						original_sprite_x = coord.x;
						original_sprite_y = coord.y;
						
						mouse_state = MOUSE_STATE_DOWN;
						app.instance.body.addEventListener(MouseEvent.MOUSE_MOVE, handleMouseMove);
						break;
					case app.tools.TOOL_SHAPES_SUBMENU_ELLIPSE:
						// Create the container
						tmp_sprite = new Sprite();			
						tmp_sprite.x = coord.x;
						tmp_sprite.y = coord.y;
						canvas.addChild(tmp_sprite);
						
						Shapes.drawCircle(tmp_sprite,
							app.instance.toolFillColor,
							app.instance.toolLineColor,
							app.instance.bolFillColorEnabled,
							app.instance.bolLineColorEnabled,
							app.tools.activeOptions.lineWidthValue,
							app.tools.activeOptions.lineStyle === app.tools.TOOL_SHAPES_OPTION_LINESTYLE_DASHED? true : false,
							10, -1,-1, 2);
						
						tmp_sprite.alpha = 0.2;
						
						original_sprite_x = coord.x;
						original_sprite_y = coord.y;
						
						mouse_state = MOUSE_STATE_DOWN;
						app.instance.body.addEventListener(MouseEvent.MOUSE_MOVE, handleMouseMove);
						break;
					case app.tools.TOOL_SHAPES_SUBMENU_LINE:
						// Create the container
						tmp_sprite = new Sprite();			
						tmp_sprite.x = coord.x;
						tmp_sprite.y = coord.y;
						canvas.addChild(tmp_sprite);
						
						if(app.tools.activeOptions.arrowHead === app.tools.TOOL_SHAPES_OPTION_ARROWHEAD_LEFT) {
							arrowHead = "left";
						}
						if(app.tools.activeOptions.arrowHead === app.tools.TOOL_SHAPES_OPTION_ARROWHEAD_RIGHT) {
							arrowHead = "right";
						}
						
						if(arrowHead === "none") {
							Shapes.drawLine(tmp_sprite,
								new Point(0, 0),
								new Point(0, 0),
								app.instance.toolLineColor,
								app.instance.bolLineColorEnabled,
								app.tools.activeOptions.lineWidthValue,
								app.tools.activeOptions.lineStyle === app.tools.TOOL_SHAPES_OPTION_LINESTYLE_DASHED? true : false,
								1);
						} else {
							Shapes.drawArrowHead(tmp_sprite,
								new Point(0, 0),
								new Point(0, 0),
								app.instance.toolFillColor,
								app.instance.toolLineColor,
								app.instance.bolFillColorEnabled,
								app.instance.bolLineColorEnabled,
								app.tools.activeOptions.lineWidthValue,
								app.tools.activeOptions.lineStyle === app.tools.TOOL_SHAPES_OPTION_LINESTYLE_DASHED? true : false,
								arrowHead, 
								1);
						}
						
						tmp_sprite.alpha = 0.2;
						
						original_sprite_x = coord.x;
						original_sprite_y = coord.y;
						
						mouse_state = MOUSE_STATE_DOWN;
						app.instance.body.addEventListener(MouseEvent.MOUSE_MOVE, handleMouseMove);
						break;
					case app.tools.TOOL_SHAPES_SUBMENU_RECTANGLE:
						// Create the container
						tmp_sprite = new Sprite();			
						tmp_sprite.x = coord.x;
						tmp_sprite.y = coord.y;
						canvas.addChild(tmp_sprite);
						
						Shapes.drawRect(tmp_sprite,
							0, 0, 1, 1,
							app.instance.toolFillColor,
							app.instance.toolLineColor,
							app.tools.activeOptions.lineWidthValue,
							app.tools.activeOptions.lineStyle === app.tools.TOOL_SHAPES_OPTION_LINESTYLE_DASHED? true : false,
							10);
						
						tmp_sprite.alpha = 0.2;
						
						original_sprite_x = coord.x;
						original_sprite_y = coord.y;
						
						mouse_state = MOUSE_STATE_DOWN;
						app.instance.body.addEventListener(MouseEvent.MOUSE_MOVE, handleMouseMove);
						break;
					case app.tools.TOOL_SHAPES_SUBMENU_TRIANGLE:
						// Create the container
						tmp_sprite = new Sprite();			
						tmp_sprite.x = coord.x;
						tmp_sprite.y = coord.y;
						canvas.addChild(tmp_sprite);
						
						Shapes.drawTriangle(tmp_sprite,
							0, 0, 1, 1,
							app.instance.toolFillColor,
							app.instance.toolLineColor,
							app.tools.activeOptions.lineWidthValue,
							app.tools.activeOptions.lineStyle === app.tools.TOOL_SHAPES_OPTION_LINESTYLE_DASHED? true : false,
							10);
						
						tmp_sprite.alpha = 0.2;
						
						original_sprite_x = coord.x;
						original_sprite_y = coord.y;
						
						mouse_state = MOUSE_STATE_DOWN;
						app.instance.body.addEventListener(MouseEvent.MOUSE_MOVE, handleMouseMove);
						break;
					case app.tools.TOOL_SHAPES_SUBMENU_ARROW:
						if(app.tools.activeOptions.arrowHead === app.tools.TOOL_SHAPES_OPTION_ARROWHEAD_LEFT) {
							arrowHead = "left";
						}
						if(app.tools.activeOptions.arrowHead === app.tools.TOOL_SHAPES_OPTION_ARROWHEAD_RIGHT) {
							arrowHead = "right";
						}
						
						tmp_sprite = new Sprite();			
						tmp_sprite.x = coord.x;
						tmp_sprite.y = coord.y;
						canvas.addChild(tmp_sprite);
						
						Shapes.drawArrowHead(tmp_sprite,
							new Point(0, 0),
							new Point(0, 0),
							app.instance.toolFillColor,
							app.instance.toolLineColor,
							app.instance.bolFillColorEnabled,
							app.instance.bolLineColorEnabled,
							app.tools.activeOptions.lineWidthValue,
							app.tools.activeOptions.lineStyle === app.tools.TOOL_SHAPES_OPTION_LINESTYLE_DASHED? true : false,
							arrowHead, 
							1);
						
						tmp_sprite.alpha = 0.2;
						
						original_sprite_x = coord.x;
						original_sprite_y = coord.y;
						
						mouse_state = MOUSE_STATE_DOWN;
						app.instance.body.addEventListener(MouseEvent.MOUSE_MOVE, handleMouseMove);
						break;
					case app.tools.TOOL_TEXT_OPTION_FONT:
						
						if(focused_object !== "tool_text_object") {
							tmp_user_text = new RichEditableText();
							tmp_user_text.x = coord.x;
							tmp_user_text.y = coord.y;
							tmp_user_text.text = "Type your text...";
							
							tmp_user_text.setStyle("paddingTop", "3");
							tmp_user_text.setStyle("paddingBottom", "3");
							tmp_user_text.setStyle("paddingRight", "3");
							tmp_user_text.setStyle("paddingLeft", "3");
							
							tmp_user_text.setStyle("fontFamily", app.tools.activeOptions.textFontFamily);
							
							if(app.tools.activeOptions.textFontStyle === "Italic") {
								tmp_user_text.setStyle("fontStyle", app.tools.activeOptions.textFontStyle.toLowerCase());
							} else {
								tmp_user_text.setStyle("fontWeight", app.tools.activeOptions.textFontStyle.toLowerCase());
							};
							
							tmp_user_text.setStyle("fontSize", app.tools.activeOptions.textFontSize);
							
							if(app.tools.activeOptions.textFontEmbedded == true) {
								tmp_user_text.setStyle("fontLookup", "embeddedCFF");
							}
							if(app.instance.bolFillColorEnabled) {
								tmp_user_text.setStyle("color", app.instance.toolFillColor);
							}
							
							app.instance.body.addElement(tmp_user_text);
							
							setTimeout(function():void {
								tmp_user_text.setFocus();
								tmp_user_text.selectAll();
								focused_object = "tool_text_object";
							}, 100);
							
							tmp_user_text.addEventListener(MouseEvent.ROLL_OVER, function(e:MouseEvent):void {
								app.instance.body.removeEventListener(MouseEvent.MOUSE_DOWN, handleMouseDown);
								app.instance.body.removeEventListener(MouseEvent.MOUSE_UP, handleMouseUp);
								app.instance.body.removeEventListener(MouseEvent.MOUSE_MOVE, handleMouseMove);
							});
							tmp_user_text.addEventListener(MouseEvent.ROLL_OUT, function(e:MouseEvent):void {
								app.instance.body.addEventListener(MouseEvent.MOUSE_DOWN, handleMouseDown);
								app.instance.body.addEventListener(MouseEvent.MOUSE_UP, handleMouseUp);
								app.instance.body.addEventListener(MouseEvent.MOUSE_MOVE, handleMouseMove);
							});
							tmp_user_text.addEventListener(FocusEvent.FOCUS_OUT, function(e:FocusEvent):void {
								var elem:RichEditableText = e.currentTarget as RichEditableText;
								if(StringUtil.trim(e.currentTarget.text) === "") {
									app.instance.body.removeElement(elem);
								} else {
									if(e.currentTarget.editable === true) {
										e.currentTarget.editable = false;
									}
								}
							});
							tmp_user_text.addEventListener(MouseEvent.CLICK, function(e:MouseEvent):void {
								focused_object = "tool_text_object";
								if(e.currentTarget.editable === false) {
									e.currentTarget.editable = true;
								}
							});
						}
						
						break;
					case app.tools.TOOL_BRUSHES_SUBMENU_PAINTBRUSH:
						if(app.instance.bolLineColorEnabled) {
							/*tmp_sprite = new Sprite();			
							tmp_sprite.x = coord.x;
							tmp_sprite.y = coord.y;
							canvas.addChild(tmp_sprite);
							
							original_sprite_x = coord.x;
							original_sprite_y = coord.y;
							
							brush_data.commands.length = 0;
							brush_data.data.length = 0;
							brush_data.commands.push(1);
							brush_data.data.push(0, 0);
							
							if(app.tools.activeOptions.lineWidthValue < 7 && app.tools.activeOptions.lineWidthValue >= 5) {
							brush_dashlen = 10;
							} else if(app.tools.activeOptions.lineWidthValue < 5 && app.tools.activeOptions.lineWidthValue >= 3) {
							brush_dashlen = 8;
							} else if(app.tools.activeOptions.lineWidthValue < 3) {
							brush_dashlen = 5;
							}
							
							brush_dash_last_loc = new Point(0, 0);
							
							// Let's begin painting for the current user
							tmp_sprite.graphics.lineStyle(app.tools.activeOptions.lineWidthValue, app.instance.toolLineColor);
							tmp_sprite.graphics.moveTo(0, 0);*/
							
							sharedBitmapData = new BitmapData(boardBitmapData.width, boardBitmapData.height, true, 0);
							pbGPath = new GraphicsPath(new Vector.<int>(), new Vector.<Number>());
							
							startX = lastMouseX = smoothedMouseX = lastSmoothedMouseX = bitmapHolder.mouseX;
							startY = lastMouseY = smoothedMouseY = lastSmoothedMouseY = bitmapHolder.mouseY;
							lastThickness = 0;
							lastRotation = Math.PI/2;
							colorLevel = 0;
							lastMouseChangeVectorX = 0;
							lastMouseChangeVectorY = 0;
							
							mouse_state = MOUSE_STATE_DOWN;
							app.instance.body.addEventListener(MouseEvent.MOUSE_MOVE, handleMouseMove);
						}
						break;
				}
			}
			canvas.setFocus();
			focused_object = "canvas";
			app.instance.body.addEventListener(MouseEvent.MOUSE_UP, handleMouseUp);
		}
		
		private function handleMouseMove(e:MouseEvent):void
		{	
			if(mouse_state === MOUSE_STATE_DOWN && tmp_sprite != null) {
				var coord:Point = e.currentTarget.globalToLocal(new Point(e.stageX, e.stageY)),
					tmp_sprite_x:Number = tmp_sprite.x,
					tmp_sprite_y:Number = tmp_sprite.y,
					tmp_sprite_delta_x:int = coord.x - original_sprite_x,
					tmp_sprite_delta_y:int = coord.y - original_sprite_y,
					arrowHead:String = "none";
				
				switch(app.tools.activeTool) {
					case app.tools.TOOL_POINTERS_OPTION_POINTER:
						selection_obj.content.graphics.clear();
						Shapes.drawRect(selection_obj.content,
							0, 0, tmp_sprite_delta_x, tmp_sprite_delta_y,
							0x000000,
							app.instance.toolLineColor,
							false,
							true,
							1,
							true,
							30, true);
						break;
					case app.tools.TOOL_SHAPES_SUBMENU_ELLIPSE:	
						tmp_sprite.graphics.clear();
						Shapes.drawCircle(tmp_sprite,
							app.instance.toolFillColor,
							app.instance.toolLineColor,
							app.instance.bolFillColorEnabled,
							app.instance.bolLineColorEnabled,
							app.tools.activeOptions.lineWidthValue,
							app.tools.activeOptions.lineStyle === app.tools.TOOL_SHAPES_OPTION_LINESTYLE_DASHED? true : false,
							10, 0,0, tmp_sprite_delta_x > tmp_sprite_delta_y? tmp_sprite_delta_x : tmp_sprite_delta_y);
						
						tmp_sprite.width = Math.abs(tmp_sprite_delta_x);
						tmp_sprite.height = Math.abs(tmp_sprite_delta_y);
						
						tmp_sprite.x = original_sprite_x + tmp_sprite_delta_x/2;
						tmp_sprite.y = original_sprite_y + tmp_sprite_delta_y/2;
						
						break;
					case app.tools.TOOL_SHAPES_SUBMENU_LINE:
						tmp_sprite.graphics.clear();
						if(app.tools.activeOptions.arrowHead === app.tools.TOOL_SHAPES_OPTION_ARROWHEAD_LEFT) {
							arrowHead = "left";
						}
						if(app.tools.activeOptions.arrowHead === app.tools.TOOL_SHAPES_OPTION_ARROWHEAD_RIGHT) {
							arrowHead = "right";
						}
						
						if(arrowHead === "none") {
							Shapes.drawLine(tmp_sprite,
								new Point(0, 0),
								new Point(tmp_sprite_delta_x, tmp_sprite_delta_y),
								app.instance.toolLineColor,
								app.instance.bolLineColorEnabled,
								app.tools.activeOptions.lineWidthValue,
								app.tools.activeOptions.lineStyle === app.tools.TOOL_SHAPES_OPTION_LINESTYLE_DASHED? true : false, 10);
						} else {
							Shapes.drawArrowHead(tmp_sprite,
								new Point(0, 0),
								new Point(tmp_sprite_delta_x, tmp_sprite_delta_y),
								app.instance.toolFillColor,
								app.instance.toolLineColor,
								app.instance.bolFillColorEnabled,
								app.instance.bolLineColorEnabled,
								app.tools.activeOptions.lineWidthValue,
								app.tools.activeOptions.lineStyle === app.tools.TOOL_SHAPES_OPTION_LINESTYLE_DASHED? true : false,
								arrowHead, 10);
						}
						break;
					case app.tools.TOOL_SHAPES_SUBMENU_RECTANGLE:	
						tmp_sprite.graphics.clear();
						
						Shapes.drawRect(tmp_sprite,
							0, 0, tmp_sprite_delta_x, tmp_sprite_delta_y,
							app.instance.toolFillColor,
							app.instance.toolLineColor,
							app.instance.bolFillColorEnabled,
							app.instance.bolLineColorEnabled,
							app.tools.activeOptions.lineWidthValue,
							app.tools.activeOptions.lineStyle === app.tools.TOOL_SHAPES_OPTION_LINESTYLE_DASHED? true : false,
							10);
						break;
					case app.tools.TOOL_SHAPES_SUBMENU_TRIANGLE:
						tmp_sprite.graphics.clear();
						Shapes.drawTriangle(tmp_sprite,
							0, 0, tmp_sprite_delta_x, tmp_sprite_delta_y,
							app.instance.toolFillColor,
							app.instance.toolLineColor,
							app.instance.bolFillColorEnabled,
							app.instance.bolLineColorEnabled,
							app.tools.activeOptions.lineWidthValue,
							app.tools.activeOptions.lineStyle === app.tools.TOOL_SHAPES_OPTION_LINESTYLE_DASHED? true : false,
							10);
						break;
					case app.tools.TOOL_SHAPES_SUBMENU_ARROW:
						
						if(app.tools.activeOptions.arrowHead === app.tools.TOOL_SHAPES_OPTION_ARROWHEAD_LEFT) {
							arrowHead = "left";
						}
						if(app.tools.activeOptions.arrowHead === app.tools.TOOL_SHAPES_OPTION_ARROWHEAD_RIGHT) {
							arrowHead = "right";
						}
						
						tmp_sprite.graphics.clear();	
						Shapes.drawArrowHead(tmp_sprite,
							new Point(0, 0),
							new Point(tmp_sprite_delta_x, tmp_sprite_delta_y),
							app.instance.toolFillColor,
							app.instance.toolLineColor,
							app.instance.bolFillColorEnabled,
							app.instance.bolLineColorEnabled,
							app.tools.activeOptions.lineWidthValue,
							app.tools.activeOptions.lineStyle === app.tools.TOOL_SHAPES_OPTION_LINESTYLE_DASHED? true : false,
							arrowHead, 10);
						break;
					case app.tools.TOOL_BRUSHES_SUBMENU_PAINTBRUSH:
						/*if(app.instance.bolLineColorEnabled) {
						var deltaPoint:Point = new Point(tmp_sprite_delta_x, tmp_sprite_delta_y);
						//last_distance:Number = Point.distance(brush_dash_last_loc, deltaPoint),
						
						app.instance.txt_chatBox.text += "{"+brush_dashlen+"}";
						
						if(brush_dash_next &&
						app.tools.activeOptions.brushLineStyle === app.tools.TOOL_BRUSHES_OPTION_LINESTYLE_DASHED) {
						brush_data.commands.push(1);
						brush_data.data.push(tmp_sprite_delta_x, tmp_sprite_delta_y);
						tmp_sprite.graphics.moveTo(tmp_sprite_delta_x, tmp_sprite_delta_y);
						} else {
						brush_data.commands.push(2);
						brush_data.data.push(tmp_sprite_delta_x, tmp_sprite_delta_y);
						tmp_sprite.graphics.lineTo(tmp_sprite_delta_x, tmp_sprite_delta_y);
						}
						
						brush_dashlen--;
						if(brush_dashlen === 0) {
						brush_dash_next = !brush_dash_next;
						brush_dashlen = BRUSH_MAX_DASHLEN;
						}
						}*/
						lineLayer.graphics.clear();
						
						mouseChangeVectorX = bitmapHolder.mouseX - lastMouseX;
						mouseChangeVectorY = bitmapHolder.mouseY - lastMouseY;
						
						
						//Cusp detection - if the mouse movement is more than 90 degrees
						//from the last motion, we will draw all the way out to the last
						//mouse position before proceeding.  We handle this by drawing the
						//previous tipLayer, and resetting the last smoothed mouse position
						//to the last actual mouse position.
						//We use a dot product to determine whether the mouse movement is
						//more than 90 degrees from the last motion.
						if (mouseChangeVectorX*lastMouseChangeVectorX + mouseChangeVectorY*lastMouseChangeVectorY < 0) {
							boardBitmapData.draw(tipLayer);
							smoothedMouseX = lastSmoothedMouseX = lastMouseX;
							smoothedMouseY = lastSmoothedMouseY = lastMouseY;
							lastRotation += Math.PI;
							lastThickness = tipTaperFactor*lastThickness;
						}
						
						
						//We smooth out the mouse position.  The drawn line will not extend to the current mouse position; instead
						//it will be drawn only a portion of the way towards the current mouse position.  This creates a nice
						//smoothing effect.
						smoothedMouseX = smoothedMouseX + smoothingFactor*(bitmapHolder.mouseX - smoothedMouseX);
						smoothedMouseY = smoothedMouseY + smoothingFactor*(bitmapHolder.mouseY - smoothedMouseY);
						
						//We determine how far the mouse moved since the last position.  We use this distance to change
						//the thickness and brightness of the line.
						dx = smoothedMouseX - lastSmoothedMouseX;
						dy = smoothedMouseY - lastSmoothedMouseY;
						dist = Math.sqrt(dx*dx + dy*dy);
						
						if (dist != 0) {
							lineRotation = Math.PI/2 + Math.atan2(dy,dx);
						}
						else {
							lineRotation = 0;
						}
						
						//We use a similar smoothing technique to change the thickness of the line, so that it doesn't
						//change too abruptly.
						targetLineThickness = minThickness+thicknessFactor*dist;
						lineThickness = lastThickness + thicknessSmoothingFactor*(targetLineThickness - lastThickness);
						
						/*
						The "line" being drawn is actually composed of filled in shapes.  This is what allows
						us to create a varying thickness of the line.
						*/
						sin0 = Math.sin(lastRotation);
						cos0 = Math.cos(lastRotation);
						sin1 = Math.sin(lineRotation);
						cos1 = Math.cos(lineRotation);
						L0Sin0 = lastThickness*sin0;
						L0Cos0 = lastThickness*cos0;
						L1Sin1 = lineThickness*sin1;
						L1Cos1 = lineThickness*cos1;
						targetColorLevel = Math.min(1,colorChangeRate*dist);
						colorLevel = colorLevel + 0.2*(targetColorLevel - colorLevel);
						
						red = paintColorR1 + colorLevel*(paintColorR2 - paintColorR1);
						green = paintColorG1 + colorLevel*(paintColorG2  - paintColorG1);
						blue = paintColorB1 + colorLevel*(paintColorB2 - paintColorB1);
						
						lineColor = (red << 16) | (green << 8) | (blue);
						
						controlVecX = 0.33*dist*sin0;
						controlVecY = -0.33*dist*cos0;
						controlX1 = lastSmoothedMouseX + L0Cos0 + controlVecX;
						controlY1 = lastSmoothedMouseY + L0Sin0 + controlVecY;
						controlX2 = lastSmoothedMouseX - L0Cos0 + controlVecX;
						controlY2 = lastSmoothedMouseY - L0Sin0 + controlVecY;
						
						lineLayer.graphics.lineStyle(1,app.instance.toolLineColor);
						lineLayer.graphics.beginFill(app.instance.toolLineColor);
						pbGPath.commands.push(1);
						pbGPath.data.push(lastSmoothedMouseX + L0Cos0, lastSmoothedMouseY + L0Sin0);
						lineLayer.graphics.moveTo(lastSmoothedMouseX + L0Cos0, lastSmoothedMouseY + L0Sin0);
						pbGPath.commands.push(3);
						pbGPath.data.push(controlX1,controlY1,smoothedMouseX + L1Cos1, smoothedMouseY + L1Sin1);
						lineLayer.graphics.curveTo(controlX1,controlY1,smoothedMouseX + L1Cos1, smoothedMouseY + L1Sin1);
						pbGPath.commands.push(2);
						pbGPath.data.push(smoothedMouseX - L1Cos1, smoothedMouseY - L1Sin1);
						lineLayer.graphics.lineTo(smoothedMouseX - L1Cos1, smoothedMouseY - L1Sin1);
						pbGPath.commands.push(3);
						pbGPath.data.push(controlX2, controlY2, lastSmoothedMouseX - L0Cos0, lastSmoothedMouseY - L0Sin0);
						lineLayer.graphics.curveTo(controlX2, controlY2, lastSmoothedMouseX - L0Cos0, lastSmoothedMouseY - L0Sin0);
						pbGPath.commands.push(2);
						pbGPath.data.push(lastSmoothedMouseX + L0Cos0, lastSmoothedMouseY + L0Sin0);
						lineLayer.graphics.lineTo(lastSmoothedMouseX + L0Cos0, lastSmoothedMouseY + L0Sin0);
						lineLayer.graphics.endFill();
						boardBitmapData.draw(lineLayer);
						sharedBitmapData.draw(lineLayer);
						
						//We draw the tip, which completes the line from the smoothed mouse position to the actual mouse position.
						//We won't actually add this to the drawn bitmap until a mouse up completes the drawing of the current line.
						
						//round tip:
						var taperThickness:Number = tipTaperFactor*lineThickness;
						tipLayer.graphics.clear();
						tipLayer.graphics.beginFill(app.instance.toolLineColor);
						tipLayer.graphics.drawEllipse(bitmapHolder.mouseX - taperThickness, bitmapHolder.mouseY - taperThickness, 2*taperThickness, 2*taperThickness);
						tipLayer.graphics.endFill();
						//quad segment
						tipLayer.graphics.lineStyle(1,app.instance.toolLineColor);
						tipLayer.graphics.beginFill(app.instance.toolLineColor);
						tipLayer.graphics.moveTo(smoothedMouseX + L1Cos1, smoothedMouseY + L1Sin1);
						tipLayer.graphics.lineTo(bitmapHolder.mouseX + tipTaperFactor*L1Cos1, bitmapHolder.mouseY + tipTaperFactor*L1Sin1);
						tipLayer.graphics.lineTo(bitmapHolder.mouseX - tipTaperFactor*L1Cos1, bitmapHolder.mouseY - tipTaperFactor*L1Sin1);
						tipLayer.graphics.lineTo(smoothedMouseX - L1Cos1, smoothedMouseY - L1Sin1);
						tipLayer.graphics.lineTo(smoothedMouseX + L1Cos1, smoothedMouseY + L1Sin1);
						tipLayer.graphics.endFill();
						
						lastSmoothedMouseX = smoothedMouseX;
						lastSmoothedMouseY = smoothedMouseY;
						lastRotation = lineRotation;
						lastThickness = lineThickness;
						lastMouseChangeVectorX = mouseChangeVectorX;
						lastMouseChangeVectorY = mouseChangeVectorY;
						lastMouseX = bitmapHolder.mouseX;
						lastMouseY = bitmapHolder.mouseY;
						break;
				}	
			}
		}
		
		private function getBitmap(clip:DisplayObject):Bitmap
		{
			var bounds:Rectangle = clip.getBounds(clip);
			var bitmapData:BitmapData = new BitmapData(boardBitmapData.width, boardBitmapData.height, true, 0);
			bitmapData.draw(clip, clip.transform.matrix);
			var bitmap:Bitmap = new Bitmap(bitmapData, "auto", true);
			return bitmap;
		}
		
		private var sel_region:Rectangle;
		private function handleMouseUp(e:MouseEvent):void
		{
			var ba:ByteArray;
			if(tmp_sprite != null) {
				var coord:Point = e.currentTarget.globalToLocal(new Point(e.stageX, e.stageY)),
					tmp_sprite_x:Number = tmp_sprite.x,
					tmp_sprite_y:Number = tmp_sprite.y,
					tmp_sprite_delta_x:int = coord.x - original_sprite_x,
					tmp_sprite_delta_y:int = coord.y - original_sprite_y,
					arrowHead:String = "none",
					tmp_shared_obj:Object = null,
					bitmap:Bitmap = null;
				
				if(mouse_state === MOUSE_STATE_DOWN) {
					tmp_sprite.alpha = 1.0;
					app.instance.body.removeEventListener(MouseEvent.MOUSE_MOVE, handleMouseMove);
					switch(app.tools.activeTool) {
						case app.tools.TOOL_POINTERS_OPTION_POINTER:
							if(tmp_sprite_delta_x == 0 && selection_obj.content != null) {
								canvas.removeChild(selection_obj.content);
								selection_obj.content = null;
							} else {
									selection_obj.content.alpha = 1;
									selection_obj.rect = selection_obj.content.getRect(bitmapHolder);
									var bitmapBA:ByteArray = boardBitmapData.getPixels(selection_obj.rect);
									selection_obj.bitmapData = new BitmapData(selection_obj.rect.width, selection_obj.rect.height, true, 0);
									bitmapBA.position = 0;
									selection_obj.bitmapData.setPixels(selection_obj.bitmapData.rect, bitmapBA);
									selection_obj.bitmap = new Bitmap(selection_obj.bitmapData, "auto", true);
									selection_obj.content.addChild(selection_obj.bitmap);
									selection_obj.haveSelection = true;
									var offset_x:Number = selection_obj.rect.width - selection_obj.content.width,
										offset_y:Number = selection_obj.rect.height - selection_obj.content.height;
									selection_obj.bitmap.x += offset_x;
									selection_obj.bitmap.y += offset_y;
									selection_obj.content.addEventListener(MouseEvent.MOUSE_DOWN, handleToolSelMousedown);
							}
							// fill
							break;
						case app.tools.TOOL_SHAPES_SUBMENU_ELLIPSE:
							bitmap = getBitmap(tmp_sprite);
							boardBitmapData.draw(bitmap);
							canvas.removeChild(tmp_sprite);
							
							sharedBitmapData = new BitmapData(bitmap.bitmapData.width, bitmap.bitmapData.height, true, 0);
							sharedBitmapData.draw(bitmap);
							
							ba = sharedBitmapData.getPixels(sharedBitmapData.rect);
							ba.compress();
							ba.position = 0;
							
							tmp_shared_obj = {
							type : "circle",
								width : sharedBitmapData.width,
								height : sharedBitmapData.height,
								data : ba
							};
							app.net.shareShape(tmp_shared_obj);
							/*tmp_shared_obj = {
							"type" : "circle",
								"src_canvas_width" : canvas.width,
								"src_canvas_height" : canvas.height,
								"name" : tmp_sprite.name,
								"x" : tmp_sprite.x,
								"y" : tmp_sprite.y,
								"width" : tmp_sprite.width,
								"height" : tmp_sprite.height,
								"fillColor" : app.instance.toolFillColor,
								"lineColor" : app.instance.toolLineColor,
								"fillEnabled" : app.instance.bolFillColorEnabled,
								"strokeEnabled" : app.instance.bolLineColorEnabled,
								"lineWidth" : app.tools.activeOptions.lineWidthValue,
								"dashed" : app.tools.activeOptions.lineStyle === app.tools.TOOL_SHAPES_OPTION_LINESTYLE_DASHED? true : false,
								"dashLength" : 10,
								"x_center" : 0, "y_center" : 0,
								"radius" : tmp_sprite_delta_x > tmp_sprite_delta_y? tmp_sprite_delta_x : tmp_sprite_delta_y,
								"radius_direction" : tmp_sprite_delta_x > tmp_sprite_delta_y? "x" : "y"
							};*/
							break;
						case app.tools.TOOL_SHAPES_SUBMENU_LINE:
							if(app.tools.activeOptions.arrowHead === app.tools.TOOL_SHAPES_OPTION_ARROWHEAD_LEFT) {
								arrowHead = "left";
							}
							if(app.tools.activeOptions.arrowHead === app.tools.TOOL_SHAPES_OPTION_ARROWHEAD_RIGHT) {
								arrowHead = "right";
							}
							tmp_shared_obj = {
							"type" : "line",
							"src_canvas_width" : canvas.width,
								"src_canvas_height" : canvas.height,
								"name" : tmp_sprite.name,
								"x" : tmp_sprite.x,
								"y" : tmp_sprite.y,
								"width" : tmp_sprite_delta_x,
								"height" : tmp_sprite_delta_y,
								"fillColor" : app.instance.toolFillColor,
								"lineColor" : app.instance.toolLineColor,
								"fillEnabled" : app.instance.bolFillColorEnabled,
								"strokeEnabled" : app.instance.bolLineColorEnabled,
								"lineWidth" : app.tools.activeOptions.lineWidthValue,
								"dashed" : app.tools.activeOptions.lineStyle === app.tools.TOOL_SHAPES_OPTION_LINESTYLE_DASHED? true : false,
								"arrowHead" : arrowHead,
								"dashLength" : 10
						};
							app.net.shareShape(tmp_shared_obj);
							break;		
						case app.tools.TOOL_SHAPES_SUBMENU_RECTANGLE:
							tmp_shared_obj = {
							"type" : "rectangle",
							"src_canvas_width" : canvas.width,
								"src_canvas_height" : canvas.height,
								"name" : tmp_sprite.name,
								"x" : tmp_sprite.x,
								"y" : tmp_sprite.y,
								"width" : tmp_sprite_delta_x,
								"height" : tmp_sprite_delta_y,
								"fillColor" : app.instance.toolFillColor,
								"lineColor" : app.instance.toolLineColor,
								"fillEnabled" : app.instance.bolFillColorEnabled,
								"strokeEnabled" : app.instance.bolLineColorEnabled,
								"lineWidth" : app.tools.activeOptions.lineWidthValue,
								"dashed" : app.tools.activeOptions.lineStyle === app.tools.TOOL_SHAPES_OPTION_LINESTYLE_DASHED? true : false,
								"dashLength" : 10
						};
							app.net.shareShape(tmp_shared_obj);
							break;
						case app.tools.TOOL_SHAPES_SUBMENU_TRIANGLE:
							tmp_shared_obj = {
							"type" : "triangle",
							"src_canvas_width" : canvas.width,
								"src_canvas_height" : canvas.height,
								"name" : tmp_sprite.name,
								"x" : tmp_sprite.x,
								"y" : tmp_sprite.y,
								"width" : tmp_sprite_delta_x,
								"height" : tmp_sprite_delta_y,
								"fillColor" : app.instance.toolFillColor,
								"lineColor" : app.instance.toolLineColor,
								"fillEnabled" : app.instance.bolFillColorEnabled,
								"strokeEnabled" : app.instance.bolLineColorEnabled,
								"lineWidth" : app.tools.activeOptions.lineWidthValue,
								"dashed" : app.tools.activeOptions.lineStyle === app.tools.TOOL_SHAPES_OPTION_LINESTYLE_DASHED? true : false,
								"dashLength" : 10
						};
							app.net.shareShape(tmp_shared_obj);
							break;
						case app.tools.TOOL_SHAPES_SUBMENU_ARROW:
							if(app.tools.activeOptions.arrowHead === app.tools.TOOL_SHAPES_OPTION_ARROWHEAD_LEFT) {
								arrowHead = "left";
							}
							if(app.tools.activeOptions.arrowHead === app.tools.TOOL_SHAPES_OPTION_ARROWHEAD_RIGHT) {
								arrowHead = "right";
							}
							tmp_shared_obj = {
							"type" : "arrow",
							"src_canvas_width" : canvas.width,
								"src_canvas_height" : canvas.height,
								"name" : tmp_sprite.name,
								"x" : tmp_sprite.x,
								"y" : tmp_sprite.y,
								"width" : tmp_sprite_delta_x,
								"height" : tmp_sprite_delta_y,
								"fillColor" : app.instance.toolFillColor,
								"lineColor" : app.instance.toolLineColor,
								"fillEnabled" : app.instance.bolFillColorEnabled,
								"strokeEnabled" : app.instance.bolLineColorEnabled,
								"lineWidth" : app.tools.activeOptions.lineWidthValue,
								"dashed" : app.tools.activeOptions.lineStyle === app.tools.TOOL_SHAPES_OPTION_LINESTYLE_DASHED? true : false,
								"arrowHead" : arrowHead,
								"dashLength" : 10
						};
							app.net.shareShape(tmp_shared_obj);
							break;
						case app.tools.TOOL_BRUSHES_SUBMENU_PAINTBRUSH:
							/*if(app.instance.bolLineColorEnabled) {
							tmp_shared_obj = {
							"type" : "paintbrush",
							"src_canvas_width" : canvas.width,
							"src_canvas_height" : canvas.height,
							"name" : tmp_sprite.name,
							"x" : tmp_sprite.x,
							"y" : tmp_sprite.y,
							"lineColor" : app.instance.toolLineColor,
							"strokeEnabled" : app.instance.bolLineColorEnabled,
							"lineWidth" : app.tools.activeOptions.lineWidthValue,
							"vector_data" : brush_data
							};
							app.net.shareDrawing(tmp_shared_obj);
							}*/
							
							//We add the tipLayer to complete the line all the way to the current mouse position:
							boardBitmapData.draw(tipLayer);
							sharedBitmapData.draw(tipLayer);
							ba = sharedBitmapData.getPixels(sharedBitmapData.rect);
							ba.compress();
							ba.position = 0;
							
							tmp_shared_obj = {
								type : "paintbrush",
								width : sharedBitmapData.width,
								height : sharedBitmapData.height,
								twidth : tipLayer.width,
								theight : tipLayer.height,
								name : tmp_sprite.name,
								x : tmp_sprite.x,
								y : tmp_sprite.y,
								pbGPath: pbGPath,
								pbGTipPath : ba,
								type : "paintbrush"
							};
							app.net.shareDrawing(tmp_shared_obj);
							
							
							/*ba.uncompress();
							bitmapData.setPixels(bitmapData.rect, ba); // position of data is now at 5th byte
							var bit1:Bitmap = new Bitmap(bitmapData);
							var sprite:Sprite = new Sprite();
							sprite.x = 400;
							sprite.addChild(bit1);
							sprite.height = tipLayer.height;
							sprite.width = tipLayer.width;
							boardBitmapData.draw(sprite);*/
							
							//ba.position = 0;
							/*var bmd:BitmapData = new BitmapData(boardBitmapData.width, boardBitmapData.height, true, 0); // 32 bit transparent bitmap
							bmd.setPixels(bmd.rect, ba); // position of data is now at 5th byte
							
							var bm:Bitmap = new Bitmap(bmd);
							bm.x = 100;
							bm.y = 200;
							app.instance.canvas.addChild(bm);*/
							//Alert.show("dfg");
							//var bit:Bitmap = new Bitmap();
							//bit.bitmapData = ba.readObject(); // Returned object in o, no bitmapdata
							//app.instance.addChild(bit); 
							//app.instance.addChild(b);
							break;
					}
				}
			}
			app.instance.body.removeEventListener(MouseEvent.MOUSE_UP, handleMouseUp);
		}
		
		private function handleToolSelMousedown(e:MouseEvent):void
		{
			if(selection_obj.content != null) {
				removeEventListeners();
				selection_obj.content.startDrag();
				selection_obj.content.addEventListener(MouseEvent.MOUSE_MOVE, handleToolSelMousemove);
				selection_obj.content.addEventListener(MouseEvent.MOUSE_UP, handleToolSelMouseup);
			}
		}
		
		private function handleToolSelMouseup(e:MouseEvent):void
		{
			selection_obj.content.stopDrag();
			selection_obj.content.removeEventListener(MouseEvent.MOUSE_UP, handleToolSelMouseup);
			/*if(selection_obj.content != null) {
				//selection_obj.rect = selection_obj.content.getRect(bitmapHolder);
				boardBitmapData.draw(selection_obj.bitmap, selection_obj.content.transform.matrix);
				canvas.removeChild(selection_obj.content);
			}*/
			setupEventListeners();
		}
		
		private function handleToolSelMousemove(e:MouseEvent):void
		{
			boardBitmapData.fillRect(selection_obj.rect, 0);
			selection_obj.content.removeEventListener(MouseEvent.MOUSE_MOVE, handleToolSelMousemove);
		}
		
		public static function out(str:String):void
		{
			FlexGlobals.topLevelApplication.txt_chatBox.appendText(str+"\n");
		}
		
		private function addBox():void
		{
			var mc:MovieClip = new MovieClip();
			mc.graphics.lineStyle(2, 0x434B54);
			mc.graphics.drawRect(0, 0, 100, 100);
			mc.graphics.endFill();
			mc.visible = true;
			
			canvas.addChild(mc);
		}
		
	}
}