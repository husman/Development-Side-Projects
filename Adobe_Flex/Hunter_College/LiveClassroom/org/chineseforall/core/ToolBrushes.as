package org.chineseforall.core
{
	import flash.events.MouseEvent;
	
	import mx.effects.Fade;
	
	import org.chineseforall.components.smBrushMenu;
	
	public class ToolBrushes extends ToolSubmenu implements IToolSubmenu
	{	
		private var subMenu:smBrushMenu = null;
		private var subMenuOptions:ToolBrushesOptions = null;
		
		public function ToolBrushes(app_handle:App)
		{
			super(app_handle);
			initialize();
		}
		
		private function initialize():void
		{
			if(subMenu === null) {
				subMenu = new smBrushMenu();
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
			subMenu.tool_brushes_paintbrush.addEventListener(MouseEvent.CLICK, handleToolClick);
			subMenu.tool_brushes_highlighter.addEventListener(MouseEvent.CLICK, handleToolClick);
			
			// These events will open the submenu options (double click)
			subMenu.tool_brushes_paintbrush.doubleClickEnabled = true;
			subMenu.tool_brushes_highlighter.doubleClickEnabled = true;
			subMenu.tool_brushes_paintbrush.addEventListener(MouseEvent.DOUBLE_CLICK, handleToolDblClick);
			subMenu.tool_brushes_highlighter.addEventListener(MouseEvent.DOUBLE_CLICK, handleToolDblClick);
		}
		
		private function removeEventListeners():void
		{
			// Let's remove our tool items' event listeners
			subMenu.tool_brushes_paintbrush.removeEventListener(MouseEvent.CLICK, handleToolClick);
			subMenu.tool_brushes_highlighter.removeEventListener(MouseEvent.CLICK, handleToolClick);
			subMenu.tool_brushes_paintbrush.removeEventListener(MouseEvent.DOUBLE_CLICK, handleToolDblClick);
			subMenu.tool_brushes_highlighter.removeEventListener(MouseEvent.DOUBLE_CLICK, handleToolDblClick);
		}
		
		private function handleToolClick(e:MouseEvent):void {
			switch(e.currentTarget.id) {
				case app.tools.TOOL_BRUSHES_SUBMENU_PAINTBRUSH:
					app.tools.activeTool = app.tools.TOOL_BRUSHES_SUBMENU_PAINTBRUSH;
					app.tools.activeOptions.brushTool = app.tools.TOOL_BRUSHES_OPTION_TOOL_PAINTBRUSH;
					app.tools.activeOptions.sel_brush_tool = app.tools.TOOL_BRUSHES_SUBMENU_PAINTBRUSH;
					break;
				case app.tools.TOOL_BRUSHES_SUBMENU_HIGHLIGHTER:
					app.tools.activeTool = app.tools.TOOL_BRUSHES_SUBMENU_HIGHLIGHTER;
					app.tools.activeOptions.brushTool = app.tools.TOOL_BRUSHES_OPTION_TOOL_HIGHLIGHTER;
					app.tools.activeOptions.sel_brush_tool = app.tools.TOOL_BRUSHES_SUBMENU_HIGHLIGHTER;
					break;
			}
			updateActiveTool();
		}
		
		private function handleToolDblClick(e:MouseEvent):void
		{
			if(subMenuOptions === null) {
				subMenuOptions = new ToolBrushesOptions(app);
			}
			app.tools.cleanupSmToolMenus();
			if(app.tools.iToolOptionSubmenu !== subMenuOptions) {
				app.tools.cleanupOptToolMenus();
			}
			
			switch(e.currentTarget.id) {
				case app.tools.TOOL_BRUSHES_SUBMENU_PAINTBRUSH:
					app.tools.activeTool = app.tools.TOOL_BRUSHES_SUBMENU_PAINTBRUSH;
					app.tools.activeOptions.brushTool = app.tools.TOOL_BRUSHES_OPTION_TOOL_PAINTBRUSH;
					app.tools.activeOptions.sel_brush_tool = app.tools.TOOL_BRUSHES_SUBMENU_PAINTBRUSH;
					break;
				case app.tools.TOOL_BRUSHES_SUBMENU_HIGHLIGHTER:
					app.tools.activeTool = app.tools.TOOL_BRUSHES_SUBMENU_HIGHLIGHTER;
					app.tools.activeOptions.brushTool = app.tools.TOOL_BRUSHES_OPTION_TOOL_HIGHLIGHTER;
					app.tools.activeOptions.sel_brush_tool = app.tools.TOOL_BRUSHES_SUBMENU_HIGHLIGHTER;
					break;
			}
			
			subMenuOptions.display();
		}
		
		private function updateActiveTool():void
		{
			hideAllSmBullets();
			switch(app.tools.activeTool) {
				case app.tools.TOOL_BRUSHES_SUBMENU_PAINTBRUSH:
					subMenu.tool_brushes_paintbrush_bullet.visible = true;
					break;
				case app.tools.TOOL_BRUSHES_SUBMENU_HIGHLIGHTER:
					subMenu.tool_brushes_highlighter_bullet.visible = true;
					break;
			}
			updateActiveMainTool();
		}
		
		private function hideAllSmBullets():void
		{
			// Lets disable all active state and set the current active element
			subMenu.tool_brushes_paintbrush_bullet.visible = false;
			subMenu.tool_brushes_highlighter_bullet.visible = false;
		}
		
		private function updateActiveMainTool():void
		{
			if(app.tools.activeTool === app.tools.TOOL_BRUSHES_SUBMENU_PAINTBRUSH ||
			   app.tools.activeTool == app.tools.TOOL_BRUSHES_SUBMENU_HIGHLIGHTER) {
				if(app.instance.tool_brushes.styleName !== app.tools.MAIN_TOOL_ACTIVE_STYLENAME) {
					app.tools.removeAllActiveState();
					app.instance.tool_brushes.styleName = app.tools.MAIN_TOOL_ACTIVE_STYLENAME;
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
