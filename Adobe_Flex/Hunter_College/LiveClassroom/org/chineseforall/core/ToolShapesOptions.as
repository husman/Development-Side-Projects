package org.chineseforall.core
{
	import flash.events.KeyboardEvent;
	import flash.events.MouseEvent;
	import flash.ui.Keyboard;
	
	import mx.controls.Alert;
	import mx.controls.ColorPicker;
	import mx.effects.Fade;
	import mx.events.ColorPickerEvent;
	import mx.events.FlexEvent;
	
	import org.chineseforall.components.optShapeMenu;
	
	import spark.components.CheckBox;
	import spark.effects.Scale;

	public class ToolShapesOptions extends ToolShapes implements IToolSubmenu
	{
		private const BTN_CLOSE:String = "tool_shapes_opt_close";
		private const BTN_SUBMIT:String = "tool_shapes_opt_submit";
		
		private var optMenu:optShapeMenu = null;
		private var state:int = app.tools.STATE_HIDDEN;
		
		private var fillColorObjectIndex:int = -1;
		
		public function ToolShapesOptions(app_handle:App)
		{
			super(app_handle);
			initialize();
		}
		
		private function initialize():void
		{
			if(optMenu === null) {
				optMenu = new optShapeMenu();
			}
		}
		
		override public function display():void
		{
			if(optMenu !== null && state === app.tools.STATE_HIDDEN) {
				app.instance.addElement(optMenu);
				optMenu.x = app.instance.width / 2 - optMenu.width;
				optMenu.y = app.instance.height / 2 - optMenu.height/2;
				
				// Let's fade in the submenu
				var fade:Fade = new Fade();
				fade.alphaFrom = 0.0;
				fade.alphaTo = 1.0;
				fade.play([optMenu]);
				
				var scale:Scale = new Scale();
				scale.scaleYFrom = 0.0, scale.scaleXFrom = 0.2;
				scale.scaleYTo = 1.0, scale.scaleXTo = 1.0;
				scale.play([optMenu]);
				
				addEventListeners();
				state = app.tools.STATE_NOT_HIDDEN;
				if(app.tools.iToolOptionSubmenu !== this) {
					app.tools.iToolOptionSubmenu = this;
				}
				updateActiveOptions("all");
			}
		}
		
		private function addEventListeners():void
		{
			// Let's add our tool items' event listeners
			optMenu.tool_shapes_opt_close.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_shapes_opt_submit.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_shapes_opt_shape_arrow.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_shapes_opt_shape_bubble.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_shapes_opt_shape_ellipse.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_shapes_opt_shape_rectangle.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_shapes_opt_shape_line.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_shapes_opt_shape_triangle.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_shapes_opt_linestyle_solid.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_shapes_opt_linestyle_dashed.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_shapes_opt_linewidth_1.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_shapes_opt_linewidth_2.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_shapes_opt_linewidth_3.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_shapes_opt_linewidth_4.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_shapes_opt_linewidth_5.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_shapes_opt_arrowhead_none.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_shapes_opt_arrowhead_left.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_shapes_opt_arrowhead_right.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_shapes_opt_bubble_circle.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_shapes_opt_bubble_square.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_shapes_opt_bubble_ellipse.addEventListener(MouseEvent.CLICK, handleToolClick);
			
		}
		
		private function disableOptionalTools():void
		{
			if(optMenu.tool_shapes_opt_arrowhead.enabled === true) {
				optMenu.tool_shapes_opt_arrowhead.enabled = false;
			}
			if(optMenu.tool_shapes_opt_bubble.enabled === true) {
				optMenu.tool_shapes_opt_bubble.enabled = false;
			}
		}
		
		private function handleToolClick(e:MouseEvent):void {
			switch(e.currentTarget.id) {
				case app.tools.TOOL_SHAPES_OPTION_SHAPE_ELLIPSE:
					app.tools.activeOptions.shape = app.tools.TOOL_SHAPES_OPTION_SHAPE_ELLIPSE;
					app.tools.activeTool = app.tools.TOOL_SHAPES_SUBMENU_ELLIPSE;
					app.tools.activeOptions.sel_shape_tool = app.tools.TOOL_SHAPES_SUBMENU_ELLIPSE;
					updateActiveOptions("shapes");
					break;
				case app.tools.TOOL_SHAPES_OPTION_SHAPE_RECTANGLE:
					app.tools.activeOptions.shape = app.tools.TOOL_SHAPES_OPTION_SHAPE_RECTANGLE;
					app.tools.activeTool = app.tools.TOOL_SHAPES_SUBMENU_RECTANGLE;
					app.tools.activeOptions.sel_shape_tool = app.tools.TOOL_SHAPES_SUBMENU_RECTANGLE;
					updateActiveOptions("shapes");
					break;
				case app.tools.TOOL_SHAPES_OPTION_SHAPE_ARROW:
					app.tools.activeOptions.shape = app.tools.TOOL_SHAPES_OPTION_SHAPE_ARROW;
					app.tools.activeTool = app.tools.TOOL_SHAPES_SUBMENU_ARROW;
					app.tools.activeOptions.sel_shape_tool = app.tools.TOOL_SHAPES_SUBMENU_ARROW;
					updateActiveOptions("shapes");
					break;
				case app.tools.TOOL_SHAPES_OPTION_SHAPE_LINE:
					app.tools.activeOptions.shape = app.tools.TOOL_SHAPES_OPTION_SHAPE_LINE;
					app.tools.activeTool = app.tools.TOOL_SHAPES_SUBMENU_LINE;
					app.tools.activeOptions.sel_shape_tool = app.tools.TOOL_SHAPES_SUBMENU_LINE;
					updateActiveOptions("shapes");
					break;
				case app.tools.TOOL_SHAPES_OPTION_SHAPE_TRIANGLE:
					app.tools.activeOptions.shape = app.tools.TOOL_SHAPES_OPTION_SHAPE_TRIANGLE;
					app.tools.activeTool = app.tools.TOOL_SHAPES_SUBMENU_TRIANGLE;
					app.tools.activeOptions.sel_shape_tool = app.tools.TOOL_SHAPES_SUBMENU_TRIANGLE;
					updateActiveOptions("shapes");
					break;
				case app.tools.TOOL_SHAPES_OPTION_SHAPE_BUBBLE:
					app.tools.activeOptions.shape = app.tools.TOOL_SHAPES_OPTION_SHAPE_BUBBLE;
					app.tools.activeTool = app.tools.TOOL_SHAPES_SUBMENU_BUBBLE;
					app.tools.activeOptions.sel_shape_tool = app.tools.TOOL_SHAPES_SUBMENU_BUBBLE;
					updateActiveOptions("shapes");
					break;
				case app.tools.TOOL_SHAPES_OPTION_LINESTYLE_SOLID:
					app.tools.activeOptions.lineStyle = app.tools.TOOL_SHAPES_OPTION_LINESTYLE_SOLID;
					updateActiveOptions("lineStyle");
					break;
				case app.tools.TOOL_SHAPES_OPTION_LINESTYLE_DASHED:
					app.tools.activeOptions.lineStyle = app.tools.TOOL_SHAPES_OPTION_LINESTYLE_DASHED;
					updateActiveOptions("lineStyle");
					break;
				case app.tools.TOOL_SHAPES_OPTION_LINEWIDTH_1:
					app.tools.activeOptions.lineWidth = app.tools.TOOL_SHAPES_OPTION_LINEWIDTH_1;
					app.tools.activeOptions.lineWidthValue = 1;
					updateActiveOptions("lineWidth");
					break;
				case app.tools.TOOL_SHAPES_OPTION_LINEWIDTH_2:
					app.tools.activeOptions.lineWidth = app.tools.TOOL_SHAPES_OPTION_LINEWIDTH_2;
					app.tools.activeOptions.lineWidthValue = 3;
					updateActiveOptions("lineWidth");
					break;
				case app.tools.TOOL_SHAPES_OPTION_LINEWIDTH_3:
					app.tools.activeOptions.lineWidth = app.tools.TOOL_SHAPES_OPTION_LINEWIDTH_3;
					app.tools.activeOptions.lineWidthValue = 5;
					updateActiveOptions("lineWidth");
					break;
				case app.tools.TOOL_SHAPES_OPTION_LINEWIDTH_4:
					app.tools.activeOptions.lineWidth = app.tools.TOOL_SHAPES_OPTION_LINEWIDTH_4;
					app.tools.activeOptions.lineWidthValue = 7;
					updateActiveOptions("lineWidth");
					break;
				case app.tools.TOOL_SHAPES_OPTION_LINEWIDTH_5:
					app.tools.activeOptions.lineWidth = app.tools.TOOL_SHAPES_OPTION_LINEWIDTH_5;
					app.tools.activeOptions.lineWidthValue = 9;
					updateActiveOptions("lineWidth");
					break;
				case app.tools.TOOL_SHAPES_OPTION_ARROWHEAD_NONE:
					app.tools.activeOptions.arrowHead = app.tools.TOOL_SHAPES_OPTION_ARROWHEAD_NONE;
					updateActiveOptions("arrowHead");
					break;
				case app.tools.TOOL_SHAPES_OPTION_ARROWHEAD_LEFT:
					app.tools.activeOptions.arrowHead = app.tools.TOOL_SHAPES_OPTION_ARROWHEAD_LEFT;
					updateActiveOptions("arrowHead");
					break;
				case app.tools.TOOL_SHAPES_OPTION_ARROWHEAD_RIGHT:
					app.tools.activeOptions.arrowHead = app.tools.TOOL_SHAPES_OPTION_ARROWHEAD_RIGHT;
					updateActiveOptions("arrowHead");
					break;
				case app.tools.TOOL_SHAPES_OPTION_BUBBLE_ELLIPSE:
					app.tools.activeOptions.bubbleType = app.tools.TOOL_SHAPES_OPTION_BUBBLE_ELLIPSE;
					updateActiveOptions("bubbleType");
					break;
				case app.tools.TOOL_SHAPES_OPTION_BUBBLE_RECTANGLE:
					app.tools.activeOptions.bubbleType = app.tools.TOOL_SHAPES_OPTION_BUBBLE_RECTANGLE;
					updateActiveOptions("bubbleType");
					break;
				case app.tools.TOOL_SHAPES_OPTION_BUBBLE_CIRCLE:
					app.tools.activeOptions.bubbleType = app.tools.TOOL_SHAPES_OPTION_BUBBLE_CIRCLE;
					updateActiveOptions("bubbleType");
					break;
				case BTN_CLOSE:
					case BTN_SUBMIT:
						hide();
					break;
			}
		}
		
		private function updateActiveOptions(target:String):void
		{
			resetActiveOptions(target);
			if(target === "shapes" || target === "all") {
				switch(app.tools.activeOptions.shape) {
					case app.tools.TOOL_SHAPES_OPTION_SHAPE_ELLIPSE:
						disableOptionalTools();
						optMenu.tool_shapes_opt_shape_ellipse.styleName = "toolBorderActive";
						break;
					case app.tools.TOOL_SHAPES_OPTION_SHAPE_RECTANGLE:
						disableOptionalTools();
						optMenu.tool_shapes_opt_shape_rectangle.styleName = "toolBorderActive";
						break;
					case app.tools.TOOL_SHAPES_OPTION_SHAPE_TRIANGLE:
						disableOptionalTools();
						optMenu.tool_shapes_opt_shape_triangle.styleName = "toolBorderActive";
						break;
					case app.tools.TOOL_SHAPES_OPTION_SHAPE_LINE:
						if(optMenu.tool_shapes_opt_bubble.enabled === true) {
							optMenu.tool_shapes_opt_bubble.enabled = false;
						}
						if(optMenu.tool_shapes_opt_arrowhead.enabled === false) {
							optMenu.tool_shapes_opt_arrowhead.enabled = true;
						}
						if(optMenu.tool_shapes_opt_arrowhead_none.enabled === false) {
							optMenu.tool_shapes_opt_arrowhead_none.enabled = true;
						}
						optMenu.tool_shapes_opt_shape_line.styleName = "toolBorderActive";
						break;
					case app.tools.TOOL_SHAPES_OPTION_SHAPE_ARROW:
						if(optMenu.tool_shapes_opt_bubble.enabled === true) {
							optMenu.tool_shapes_opt_bubble.enabled = false;
						}
						if(optMenu.tool_shapes_opt_arrowhead.enabled === false) {
							optMenu.tool_shapes_opt_arrowhead.enabled = true;
						}
						if(optMenu.tool_shapes_opt_arrowhead_none.enabled === true) {
							optMenu.tool_shapes_opt_arrowhead_none.enabled = false;
						}
						optMenu.tool_shapes_opt_shape_arrow.styleName = "toolBorderActive";
						break;
					case app.tools.TOOL_SHAPES_OPTION_SHAPE_BUBBLE:
						if(optMenu.tool_shapes_opt_arrowhead.enabled === true) {
							optMenu.tool_shapes_opt_arrowhead.enabled = false;
						}
						if(optMenu.tool_shapes_opt_bubble.enabled === false) {
							optMenu.tool_shapes_opt_bubble.enabled = true;
						}
						optMenu.tool_shapes_opt_shape_bubble.styleName = "toolBorderActive";
						break;
				}
			}
			
			if(target === "lineStyle" || target === "all") {
				switch(app.tools.activeOptions.lineStyle) {
					case app.tools.TOOL_SHAPES_OPTION_LINESTYLE_SOLID:
					optMenu.tool_shapes_opt_linestyle_solid.styleName = "toolBorderActive";
					break;
					case app.tools.TOOL_SHAPES_OPTION_LINESTYLE_DASHED:
					optMenu.tool_shapes_opt_linestyle_dashed.styleName = "toolBorderActive";
					break;
				}
			}
			
			if(target === "lineWidth" || target === "all") {
				switch(app.tools.activeOptions.lineWidth) {
					case app.tools.TOOL_SHAPES_OPTION_LINEWIDTH_1:
						optMenu.tool_shapes_opt_linewidth_1.styleName = "toolBorderActive";
						break;
					case app.tools.TOOL_SHAPES_OPTION_LINEWIDTH_2:
						optMenu.tool_shapes_opt_linewidth_2.styleName = "toolBorderActive";
						break;
					case app.tools.TOOL_SHAPES_OPTION_LINEWIDTH_3:
						optMenu.tool_shapes_opt_linewidth_3.styleName = "toolBorderActive";
						break;
					case app.tools.TOOL_SHAPES_OPTION_LINEWIDTH_4:
						optMenu.tool_shapes_opt_linewidth_4.styleName = "toolBorderActive";
						break;
					case app.tools.TOOL_SHAPES_OPTION_LINEWIDTH_5:
						optMenu.tool_shapes_opt_linewidth_5.styleName = "toolBorderActive";
						break;
				}
			}
			
			if(target === "arrowHead" || target === "all") {
				switch(app.tools.activeOptions.arrowHead) {
					case app.tools.TOOL_SHAPES_OPTION_ARROWHEAD_NONE:
						optMenu.tool_shapes_opt_arrowhead_none.styleName = "toolBorderActive";
						break;
					case app.tools.TOOL_SHAPES_OPTION_ARROWHEAD_LEFT:
						optMenu.tool_shapes_opt_arrowhead_left.styleName = "toolBorderActive";
						break;
					case app.tools.TOOL_SHAPES_OPTION_ARROWHEAD_RIGHT:
						optMenu.tool_shapes_opt_arrowhead_right.styleName = "toolBorderActive";
						break;
				}
			}
			
			if(target === "bubbleType" || target === "all") {
				switch(app.tools.activeOptions.bubbleType) {
					case app.tools.TOOL_SHAPES_OPTION_BUBBLE_ELLIPSE:
						optMenu.tool_shapes_opt_bubble_ellipse.styleName = "toolBorderActive";
						break;
					case app.tools.TOOL_SHAPES_OPTION_BUBBLE_RECTANGLE:
						optMenu.tool_shapes_opt_bubble_square.styleName = "toolBorderActive";
						break;
					case app.tools.TOOL_SHAPES_OPTION_BUBBLE_CIRCLE:
						optMenu.tool_shapes_opt_bubble_circle.styleName = "toolBorderActive";
						break;
				}
			}
			
		}
		
		private function resetActiveOptions(target:String):void
		{
			if(target === "shapes" || target === "all") {
				optMenu.tool_shapes_opt_shape_ellipse.styleName = "toolBorder";
				optMenu.tool_shapes_opt_shape_rectangle.styleName = "toolBorder";
				optMenu.tool_shapes_opt_shape_triangle.styleName = "toolBorder";
				optMenu.tool_shapes_opt_shape_line.styleName = "toolBorder";
				optMenu.tool_shapes_opt_shape_arrow.styleName = "toolBorder";
				optMenu.tool_shapes_opt_shape_bubble.styleName = "toolBorder";
			}
			
			if(target === "lineStyle" || target === "all") {
				optMenu.tool_shapes_opt_linestyle_solid.styleName = "toolBorder";
				optMenu.tool_shapes_opt_linestyle_dashed.styleName = "toolBorder";
			}
			
			if(target === "lineWidth" || target === "all") {
				optMenu.tool_shapes_opt_linewidth_1.styleName = "toolBorder";
				optMenu.tool_shapes_opt_linewidth_2.styleName = "toolBorder";
				optMenu.tool_shapes_opt_linewidth_3.styleName = "toolBorder";
				optMenu.tool_shapes_opt_linewidth_4.styleName = "toolBorder";
				optMenu.tool_shapes_opt_linewidth_5.styleName = "toolBorder";
			}
			
			if(target === "arrowHead" || target === "all") {
				optMenu.tool_shapes_opt_arrowhead_none.styleName = "toolBorder";
				optMenu.tool_shapes_opt_arrowhead_left.styleName = "toolBorder";
				optMenu.tool_shapes_opt_arrowhead_right.styleName = "toolBorder";
			}
			
			if(target === "bubbleType" || target === "all") {
				optMenu.tool_shapes_opt_bubble_ellipse.styleName = "toolBorder";
				optMenu.tool_shapes_opt_bubble_square.styleName = "toolBorder";
				optMenu.tool_shapes_opt_bubble_circle.styleName = "toolBorder";
			}
		}
		
		override public function hide():void
		{
			if(optMenu !== null && state === app.tools.STATE_NOT_HIDDEN) {
				app.instance.removeElement(optMenu);
				state = app.tools.STATE_HIDDEN;
				app.tools.iToolOptionSubmenu = null;
			}
		}
	}
}