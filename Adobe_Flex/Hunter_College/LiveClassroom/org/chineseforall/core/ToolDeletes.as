package org.chineseforall.core
{
	import flash.events.MouseEvent;
	
	import mx.effects.Fade;
	
	import org.chineseforall.components.smDeleteMenu;
	
	public class ToolDeletes extends ToolSubmenu implements IToolSubmenu
	{	
		private var subMenu:smDeleteMenu = null;
		
		public function ToolDeletes(app_handle:App)
		{
			super(app_handle);
			initialize();
		}
		
		private function initialize():void
		{
			if(subMenu === null) {
				subMenu = new smDeleteMenu();
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
			subMenu.tool_deletes_click.addEventListener(MouseEvent.CLICK, handleToolClick);
			subMenu.tool_deletes_select.addEventListener(MouseEvent.CLICK, handleToolClick);
		}
		
		private function removeEventListeners():void
		{
			// Let's remove our tool items' event listeners
			subMenu.tool_deletes_click.removeEventListener(MouseEvent.CLICK, handleToolClick);
			subMenu.tool_deletes_select.removeEventListener(MouseEvent.CLICK, handleToolClick);
		}
		
		private function handleToolClick(e:MouseEvent):void {
			switch(e.currentTarget.id) {
				case app.tools.TOOL_DELETES_OPTION_CLICK:
					app.tools.activeTool = app.tools.TOOL_DELETES_OPTION_CLICK;
					app.tools.activeOptions.deleteType = app.tools.TOOL_DELETES_OPTION_CLICK;
					break;
				case app.tools.TOOL_DELETES_OPTION_SELECT:
					app.tools.activeTool = app.tools.TOOL_DELETES_OPTION_SELECT;
					app.tools.activeOptions.deleteType = app.tools.TOOL_DELETES_OPTION_SELECT;
					break;
			}
			updateActiveTool();
		}
		
		private function updateActiveTool():void
		{
			hideAllSmBullets();
			switch(app.tools.activeTool) {
				case app.tools.TOOL_DELETES_OPTION_CLICK:
					subMenu.tool_deletes_click_bullet.visible = true;
					break;
				case app.tools.TOOL_DELETES_OPTION_SELECT:
					subMenu.tool_deletes_select_bullet.visible = true;
					break;
			}
			updateActiveMainTool();
		}
		
		private function hideAllSmBullets():void
		{
			// Lets disable all active state and set the current active element
			subMenu.tool_deletes_click_bullet.visible = false;
			subMenu.tool_deletes_select_bullet.visible = false;
		}
		
		private function updateActiveMainTool():void
		{
			if(app.tools.activeTool === app.tools.TOOL_DELETES_OPTION_CLICK ||
				app.tools.activeTool == app.tools.TOOL_DELETES_OPTION_SELECT) {
				if(app.instance.tool_deletes.styleName !== app.tools.MAIN_TOOL_ACTIVE_STYLENAME) {
					app.tools.removeAllActiveState();
					app.instance.tool_deletes.styleName = app.tools.MAIN_TOOL_ACTIVE_STYLENAME;
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