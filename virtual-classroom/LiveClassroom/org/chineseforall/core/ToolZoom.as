package org.chineseforall.core
{
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import mx.controls.Alert;
	import mx.effects.Fade;
	
	import org.chineseforall.components.optZoomMenu;
	
	import spark.effects.Scale;
	
	public class ToolZoom extends ToolSubmenu implements IToolSubmenu
	{
		private const BTN_MINUS:String = "tool_zoom_opt_minus";
		private const BTN_PLUS:String = "tool_zoom_opt_plus";
		private const BTN_CLOSE:String = "tool_zoom_opt_close";
		private const BTN_SUBMIT:String = "tool_zoom_opt_submit";
		
		private var subMenu:optZoomMenu = null;
		
		public function ToolZoom(app_handle:App)
		{
			super(app_handle);
			initialize();
		}
		
		private function initialize():void
		{
			if(subMenu === null) {
				subMenu = new optZoomMenu();
			}
		}
		
		public function display():void
		{
			if(subMenu !== null && bHidden === true) {
				app.instance.addElement(subMenu);
				subMenu.x = app.instance.width / 2 - subMenu.width;
				subMenu.y = app.instance.height / 2 - subMenu.height/2;
				updateActiveTool();
				
				// Let's fade in the submenu
				var fade:Fade = new Fade();
				fade.alphaFrom = 0.0;
				fade.alphaTo = 1.0;
				fade.play([subMenu]);
				
				addEventListeners();
				bHidden = false;
			}	
		}
		
		private function addEventListeners():void
		{
			// Let's add our tool items' event listeners
			subMenu.tool_zoom_opt_minus.addEventListener(MouseEvent.CLICK, handleToolClick);
			subMenu.tool_zoom_opt_plus.addEventListener(MouseEvent.CLICK, handleToolClick);
			subMenu.tool_zoom_opt_close.addEventListener(MouseEvent.CLICK, handleToolClick);
			subMenu.tool_zoom_opt_submit.addEventListener(MouseEvent.CLICK, handleToolClick);
			subMenu.tool_zoom_opt_size.addEventListener(Event.CHANGE, handleToolChange);
		}
		
		private function removeEventListeners():void
		{
			// Let's remove our tool items' event listeners
			subMenu.tool_zoom_opt_minus.removeEventListener(MouseEvent.CLICK, handleToolClick);
			subMenu.tool_zoom_opt_plus.removeEventListener(MouseEvent.CLICK, handleToolClick);
			subMenu.tool_zoom_opt_close.removeEventListener(MouseEvent.CLICK, handleToolClick);
			subMenu.tool_zoom_opt_submit.removeEventListener(MouseEvent.CLICK, handleToolClick);
			subMenu.tool_zoom_opt_size.removeEventListener(Event.CHANGE, handleToolChange);
		}
		
		private function handleToolClick(e:MouseEvent):void {
			switch(e.currentTarget.id) {
				case BTN_MINUS:
					if(subMenu.tool_zoom_opt_size.value > 0){
						subMenu.tool_zoom_opt_size.value = subMenu.tool_zoom_opt_size.value - 1;
					}
					break;
				case BTN_PLUS:
					if(subMenu.tool_zoom_opt_size.value < 100){
						subMenu.tool_zoom_opt_size.value = subMenu.tool_zoom_opt_size.value + 1;
					}
					break;
				case BTN_CLOSE:
				case BTN_SUBMIT:
					hide();
					break;
			}
			app.tools.activeTool = app.tools.TOOL_ZOOM_OPTION_SIZE;
			app.tools.activeOptions.zoomSize = subMenu.tool_zoom_opt_size.value;
			updateActiveTool();
		}
		
		private function handleToolChange(e:Event):void {
			app.tools.activeTool = app.tools.TOOL_ZOOM_OPTION_SIZE;
			app.tools.activeOptions.zoomSize = subMenu.tool_zoom_opt_size.value;
			updateActiveTool();
		}
		
		private function updateActiveTool():void
		{
			if(app.tools.activeTool === app.tools.TOOL_ZOOM_OPTION_SIZE) {
				if(app.instance.tool_zoom.styleName !== app.tools.MAIN_TOOL_ACTIVE_STYLENAME) {
					app.tools.removeAllActiveState();
					app.instance.tool_zoom.styleName = app.tools.MAIN_TOOL_ACTIVE_STYLENAME;
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
