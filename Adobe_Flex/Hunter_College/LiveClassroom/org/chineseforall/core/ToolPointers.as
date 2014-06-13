package org.chineseforall.core
{
	import flash.events.MouseEvent;
	
	import mx.effects.Fade;
	
	import org.chineseforall.components.smArrowMenu;
	
	public class ToolPointers extends ToolSubmenu implements IToolSubmenu
	{	
		private var subMenu:smArrowMenu = null;
		
		public function ToolPointers(app_handle:App)
		{
			super(app_handle);
			initialize();
		}
		
		private function initialize():void
		{
			if(subMenu === null) {
				subMenu = new smArrowMenu();
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
			subMenu.tool_pointers_pointer.addEventListener(MouseEvent.CLICK, handleToolPointer);
			subMenu.tool_pointers_move.addEventListener(MouseEvent.CLICK, handleToolMove);
		}
		
		private function removeEventListeners():void
		{
			// Let's remove our tool items' event listeners
			subMenu.tool_pointers_pointer.removeEventListener(MouseEvent.CLICK, handleToolPointer);
			subMenu.tool_pointers_move.removeEventListener(MouseEvent.CLICK, handleToolMove);
		}
		
		private function handleToolPointer(e:MouseEvent):void {
			if(app.tools.activeTool !== app.tools.TOOL_POINTERS_OPTION_POINTER) {
				app.tools.activeTool = app.tools.TOOL_POINTERS_OPTION_POINTER;
				updateActiveTool();
			}
		}
		
		private function handleToolMove(e:MouseEvent):void {
			if(app.tools.activeTool !== app.tools.TOOL_POINTERS_OPTION_MOVE) {
				app.tools.activeTool = app.tools.TOOL_POINTERS_OPTION_MOVE;
				updateActiveTool();
			}
		}
		
		private function updateActiveTool():void
		{
			hideAllSmBullets();
			if(app.tools.activeTool === app.tools.TOOL_POINTERS_OPTION_POINTER) {
				subMenu.tool_pointers_pointer_bullet.visible = true;
				app.tools.activeOptions.pointer = app.tools.TOOL_POINTERS_OPTION_POINTER;
			} else if(app.tools.activeTool === app.tools.TOOL_POINTERS_OPTION_MOVE) {
				subMenu.tool_pointers_move_bullet.visible = true;
				app.tools.activeOptions.pointer = app.tools.TOOL_POINTERS_OPTION_MOVE;
			}
			updateActiveMainTool();
		}
		
		private function hideAllSmBullets():void
		{
			// Lets disable all active state and set the current active element
			subMenu.tool_pointers_pointer_bullet.visible = false;
			subMenu.tool_pointers_move_bullet.visible = false;
		}
		
		private function updateActiveMainTool():void
		{
			if(app.tools.activeTool === app.tools.TOOL_POINTERS_OPTION_POINTER ||
			   app.tools.activeTool == app.tools.TOOL_POINTERS_OPTION_MOVE) {
				if(app.instance.tool_pointers.styleName !== app.tools.MAIN_TOOL_ACTIVE_STYLENAME) {
					app.tools.removeAllActiveState();
					app.instance.tool_pointers.styleName = app.tools.MAIN_TOOL_ACTIVE_STYLENAME;
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
