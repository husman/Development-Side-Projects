package org.chineseforall.core
{
	import flash.events.MouseEvent;
	
	import mx.controls.Alert;
	import mx.effects.Fade;
	
	import org.chineseforall.components.smShapeMenu;
	
	import spark.effects.Move;
	import spark.effects.Resize;
	import spark.effects.Rotate;
	import spark.effects.Scale;
	
	public class ToolShapes extends ToolSubmenu implements IToolSubmenu
	{
		private var subMenu:smShapeMenu = null;
		private var subMenuOptions:ToolShapesOptions = null;
		
		public function ToolShapes(app_handle:App)
		{
			super(app_handle);
			initialize();
		}
		
		private function initialize():void
		{
			if(subMenu === null) {
				subMenu = new smShapeMenu();
			}
		}
		
		public function display():void
		{
			if(subMenu !== null) {
				app.instance.addElement(subMenu);
				subMenu.x = app.instance.mouseX + 5;
				subMenu.y = app.instance.mouseY - 5;
				updateActiveTool();
				
				// Let's fade in the submenu
				var fade:Fade = new Fade();
				fade.alphaFrom = 0.5;
				fade.alphaTo = 1.0;
				fade.play([subMenu]);
				
				addEventListeners();
				bHidden = false;
			}	
		}
		
		private function addEventListeners():void
		{
			// Let's add our tool items' event listeners
			subMenu.tool_shapes_ellipse.addEventListener(MouseEvent.CLICK, handleToolClick);
			subMenu.tool_shapes_rectangle.addEventListener(MouseEvent.CLICK, handleToolClick);
			subMenu.tool_shapes_triangle.addEventListener(MouseEvent.CLICK, handleToolClick);
			subMenu.tool_shapes_line.addEventListener(MouseEvent.CLICK, handleToolClick);
			subMenu.tool_shapes_arrow.addEventListener(MouseEvent.CLICK, handleToolClick);
			subMenu.tool_shapes_bubble.addEventListener(MouseEvent.CLICK, handleToolClick);
			
			// These events will open the submenu options (double click)
			subMenu.tool_shapes_ellipse.doubleClickEnabled = true;
			subMenu.tool_shapes_rectangle.doubleClickEnabled = true;
			subMenu.tool_shapes_triangle.doubleClickEnabled = true;
			subMenu.tool_shapes_line.doubleClickEnabled = true;
			subMenu.tool_shapes_arrow.doubleClickEnabled = true;
			subMenu.tool_shapes_bubble.doubleClickEnabled = true;
			subMenu.tool_shapes_ellipse.addEventListener(MouseEvent.DOUBLE_CLICK, handleToolDblClick);
			subMenu.tool_shapes_rectangle.addEventListener(MouseEvent.DOUBLE_CLICK, handleToolDblClick);
			subMenu.tool_shapes_triangle.addEventListener(MouseEvent.DOUBLE_CLICK, handleToolDblClick);
			subMenu.tool_shapes_line.addEventListener(MouseEvent.DOUBLE_CLICK, handleToolDblClick);
			subMenu.tool_shapes_arrow.addEventListener(MouseEvent.DOUBLE_CLICK, handleToolDblClick);
			subMenu.tool_shapes_bubble.addEventListener(MouseEvent.DOUBLE_CLICK, handleToolDblClick);
		}
		
		private function removeEventListeners():void
		{
			// Let's remove our tool items' event listeners
			subMenu.tool_shapes_ellipse.removeEventListener(MouseEvent.CLICK, handleToolClick);
			subMenu.tool_shapes_rectangle.removeEventListener(MouseEvent.CLICK, handleToolClick);
			subMenu.tool_shapes_triangle.removeEventListener(MouseEvent.CLICK, handleToolClick);
			subMenu.tool_shapes_line.removeEventListener(MouseEvent.CLICK, handleToolClick);
			subMenu.tool_shapes_arrow.removeEventListener(MouseEvent.CLICK, handleToolClick);
			subMenu.tool_shapes_bubble.removeEventListener(MouseEvent.CLICK, handleToolClick);
			
			subMenu.tool_shapes_ellipse.removeEventListener(MouseEvent.DOUBLE_CLICK, handleToolDblClick);
			subMenu.tool_shapes_rectangle.removeEventListener(MouseEvent.DOUBLE_CLICK, handleToolDblClick);
			subMenu.tool_shapes_triangle.removeEventListener(MouseEvent.DOUBLE_CLICK, handleToolDblClick);
			subMenu.tool_shapes_line.removeEventListener(MouseEvent.DOUBLE_CLICK, handleToolDblClick);
			subMenu.tool_shapes_arrow.removeEventListener(MouseEvent.DOUBLE_CLICK, handleToolDblClick);
			subMenu.tool_shapes_bubble.removeEventListener(MouseEvent.DOUBLE_CLICK, handleToolDblClick);
		}
		
		private function handleToolDblClick(e:MouseEvent):void
		{
			if(subMenuOptions === null) {
				subMenuOptions = new ToolShapesOptions(app);
			}
			app.tools.cleanupSmToolMenus();
			if(app.tools.iToolOptionSubmenu !== subMenuOptions) {
				app.tools.cleanupOptToolMenus();
			}
			
			switch(e.currentTarget.id) {
				case app.tools.TOOL_SHAPES_SUBMENU_ELLIPSE:
					app.tools.activeTool = app.tools.TOOL_SHAPES_SUBMENU_ELLIPSE;
					app.tools.activeOptions.shape = app.tools.TOOL_SHAPES_OPTION_SHAPE_ELLIPSE;
					app.tools.activeOptions.sel_shape_tool = app.tools.TOOL_SHAPES_SUBMENU_ELLIPSE;
					break;
				case app.tools.TOOL_SHAPES_SUBMENU_RECTANGLE:
					app.tools.activeTool = app.tools.TOOL_SHAPES_SUBMENU_RECTANGLE;
					app.tools.activeOptions.shape = app.tools.TOOL_SHAPES_OPTION_SHAPE_RECTANGLE;
					app.tools.activeOptions.sel_shape_tool = app.tools.TOOL_SHAPES_SUBMENU_RECTANGLE;
					break;
				case app.tools.TOOL_SHAPES_SUBMENU_TRIANGLE:
					app.tools.activeTool = app.tools.TOOL_SHAPES_SUBMENU_TRIANGLE;
					app.tools.activeOptions.shape = app.tools.TOOL_SHAPES_OPTION_SHAPE_TRIANGLE;
					app.tools.activeOptions.sel_shape_tool = app.tools.TOOL_SHAPES_SUBMENU_TRIANGLE;
					break;
				case app.tools.TOOL_SHAPES_SUBMENU_LINE:
					app.tools.activeTool = app.tools.TOOL_SHAPES_SUBMENU_LINE;
					app.tools.activeOptions.shape = app.tools.TOOL_SHAPES_OPTION_SHAPE_LINE;
					app.tools.activeOptions.sel_shape_tool = app.tools.TOOL_SHAPES_SUBMENU_LINE;
					break;
				case app.tools.TOOL_SHAPES_SUBMENU_ARROW:
					app.tools.activeTool = app.tools.TOOL_SHAPES_SUBMENU_ARROW;
					app.tools.activeOptions.shape = app.tools.TOOL_SHAPES_OPTION_SHAPE_ARROW;
					app.tools.activeOptions.sel_shape_tool = app.tools.TOOL_SHAPES_SUBMENU_ARROW;
					break;
				case app.tools.TOOL_SHAPES_SUBMENU_BUBBLE:
					app.tools.activeTool = app.tools.TOOL_SHAPES_SUBMENU_BUBBLE;
					app.tools.activeOptions.shape = app.tools.TOOL_SHAPES_OPTION_SHAPE_BUBBLE;
					app.tools.activeOptions.sel_shape_tool = app.tools.TOOL_SHAPES_SUBMENU_BUBBLE;
					break;
			}
			
			subMenuOptions.display();
		}
		
		private function handleToolClick(e:MouseEvent):void {
			switch(e.currentTarget.id) {
				case app.tools.TOOL_SHAPES_SUBMENU_ELLIPSE:
					if(app.tools.activeTool !== app.tools.TOOL_SHAPES_SUBMENU_ELLIPSE) {
						app.tools.activeTool = app.tools.TOOL_SHAPES_SUBMENU_ELLIPSE;
						app.tools.activeOptions.sel_shape_tool = app.tools.TOOL_SHAPES_SUBMENU_ELLIPSE;
					}
					break;
				case app.tools.TOOL_SHAPES_SUBMENU_RECTANGLE:
					if(app.tools.activeTool !== app.tools.TOOL_SHAPES_SUBMENU_RECTANGLE) {
						app.tools.activeTool = app.tools.TOOL_SHAPES_SUBMENU_RECTANGLE;
						app.tools.activeOptions.sel_shape_tool = app.tools.TOOL_SHAPES_SUBMENU_RECTANGLE;
					}
					break;
				case app.tools.TOOL_SHAPES_SUBMENU_TRIANGLE:
					if(app.tools.activeTool !== app.tools.TOOL_SHAPES_SUBMENU_TRIANGLE) {
						app.tools.activeTool = app.tools.TOOL_SHAPES_SUBMENU_TRIANGLE;
						app.tools.activeOptions.sel_shape_tool = app.tools.TOOL_SHAPES_SUBMENU_TRIANGLE;
					}
					break;
				case app.tools.TOOL_SHAPES_SUBMENU_LINE:
					if(app.tools.activeTool !== app.tools.TOOL_SHAPES_SUBMENU_LINE) {
						app.tools.activeTool = app.tools.TOOL_SHAPES_SUBMENU_LINE;
						app.tools.activeOptions.sel_shape_tool = app.tools.TOOL_SHAPES_SUBMENU_LINE;
					}
					break;
				case app.tools.TOOL_SHAPES_SUBMENU_ARROW:
					if(app.tools.activeTool !== app.tools.TOOL_SHAPES_SUBMENU_ARROW) {
						app.tools.activeTool = app.tools.TOOL_SHAPES_SUBMENU_ARROW;
						app.tools.activeOptions.sel_shape_tool = app.tools.TOOL_SHAPES_SUBMENU_ARROW;
					}
					break;
				case app.tools.TOOL_SHAPES_SUBMENU_BUBBLE:
					if(app.tools.activeTool !== app.tools.TOOL_SHAPES_SUBMENU_BUBBLE) {
						app.tools.activeTool = app.tools.TOOL_SHAPES_SUBMENU_BUBBLE;
						app.tools.activeOptions.sel_shape_tool = app.tools.TOOL_SHAPES_SUBMENU_BUBBLE;
					}
					break;
			}
			updateActiveTool();
		}
		
		private function updateActiveTool():void
		{
			hideAllSmBullets();
			switch(app.tools.activeTool) {
				case app.tools.TOOL_SHAPES_SUBMENU_ELLIPSE:
					subMenu.tool_shapes_ellipse_bullet.visible = true;
					break;
				case app.tools.TOOL_SHAPES_SUBMENU_RECTANGLE:
					subMenu.tool_shapes_rectangle_bullet.visible = true;
					break;
				case app.tools.TOOL_SHAPES_SUBMENU_TRIANGLE:
					subMenu.tool_shapes_triangle_bullet.visible = true;
					break;
				case app.tools.TOOL_SHAPES_SUBMENU_LINE:
					subMenu.tool_shapes_line_bullet.visible = true;
					break;
				case app.tools.TOOL_SHAPES_SUBMENU_ARROW:
					subMenu.tool_shapes_arrow_bullet.visible = true;
					break;
				case app.tools.TOOL_SHAPES_SUBMENU_BUBBLE:
					subMenu.tool_shapes_bubble_bullet.visible = true;
					break;
			}
			updateActiveMainTool();
		}
		
		private function hideAllSmBullets():void
		{
			// Lets disable all active state and set the current active element
			subMenu.tool_shapes_ellipse_bullet.visible = false;
			subMenu.tool_shapes_rectangle_bullet.visible = false;
			subMenu.tool_shapes_triangle_bullet.visible = false;
			subMenu.tool_shapes_line_bullet.visible = false;
			subMenu.tool_shapes_arrow_bullet.visible = false;
			subMenu.tool_shapes_bubble_bullet.visible = false;
		}
		
		private function updateActiveMainTool():void
		{
			if(app.tools.activeTool === app.tools.TOOL_SHAPES_SUBMENU_ELLIPSE || app.tools.activeTool == app.tools.TOOL_SHAPES_SUBMENU_RECTANGLE ||
			   app.tools.activeTool == app.tools.TOOL_SHAPES_SUBMENU_TRIANGLE || app.tools.activeTool == app.tools.TOOL_SHAPES_SUBMENU_LINE ||
			   app.tools.activeTool == app.tools.TOOL_SHAPES_SUBMENU_ARROW || app.tools.activeTool == app.tools.TOOL_SHAPES_SUBMENU_BUBBLE) {
				if(app.instance.tool_shapes.styleName !== app.tools.MAIN_TOOL_ACTIVE_STYLENAME) {
					app.tools.removeAllActiveState();
					app.instance.tool_shapes.styleName = app.tools.MAIN_TOOL_ACTIVE_STYLENAME;
				}
			}
		}
		
		public function hide():void
		{
			if(subMenu !== null && bHidden === false) {
				removeEventListeners();
				app.instance.removeElement(subMenu);
				bHidden = true;
			}
		}
		
	}
}
