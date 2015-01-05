package org.chineseforall.core
{
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import mx.controls.Alert;
	import mx.effects.Fade;
	
	import org.chineseforall.components.optTextMenu;
	
	import spark.effects.Scale;
	import spark.events.IndexChangeEvent;
	
	public class ToolText extends ToolSubmenu implements IToolSubmenu
	{
		private const DROPLIST_FONTSTYLE:String = "tool_text_opt_fontStyle";
		private const DROPLIST_FONTSIZE:String = "tool_text_opt_fontSize";
		private const COLORPICKER_COLOR:String = "tool_text_opt_fillColor";
		private const BTN_CLOSE:String = "tool_text_opt_close";
		private const BTN_SUBMIT:String = "tool_text_opt_submit";
		
		private var subMenu:optTextMenu = null;
		
		public function ToolText(app_handle:App)
		{
			super(app_handle);
			initialize();
		}
		
		private function initialize():void
		{
			if(subMenu === null) {
				subMenu = new optTextMenu();
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
				
				var scale:Scale = new Scale();
				scale.scaleYFrom = 0.0, scale.scaleXFrom = 0.2;
				scale.scaleYTo = 1.0, scale.scaleXTo = 1.0;
				scale.play([subMenu]);
				
				addEventListeners();
				bHidden = false;
			}	
		}
		
		private function addEventListeners():void
		{
			// Let's add our tool items' event listeners
			subMenu.tool_text_opt_font.addEventListener(IndexChangeEvent.CHANGE, handleToolChange);
			subMenu.tool_text_opt_fontStyle.addEventListener(IndexChangeEvent.CHANGE, handleToolChange);
			subMenu.tool_text_opt_fontSize.addEventListener(IndexChangeEvent.CHANGE, handleToolChange);
			subMenu.tool_text_opt_close.addEventListener(MouseEvent.CLICK, handleToolClick);
			subMenu.tool_text_opt_submit.addEventListener(MouseEvent.CLICK, handleToolClick);
		}
		
		private function removeEventListeners():void
		{
			// Let's remove our tool items' event listeners
			subMenu.tool_text_opt_font.removeEventListener(IndexChangeEvent.CHANGE, handleToolChange);
			subMenu.tool_text_opt_fontStyle.removeEventListener(IndexChangeEvent.CHANGE, handleToolChange);
			subMenu.tool_text_opt_fontSize.removeEventListener(IndexChangeEvent.CHANGE, handleToolChange);
			subMenu.tool_text_opt_close.removeEventListener(MouseEvent.CLICK, handleToolClick);
			subMenu.tool_text_opt_submit.removeEventListener(MouseEvent.CLICK, handleToolClick);
		}
		
		private function handleToolChange(e:IndexChangeEvent):void {
			app.tools.activeTool = app.tools.TOOL_TEXT_OPTION_FONT;
			switch(e.currentTarget.id) {
				case app.tools.TOOL_TEXT_OPTION_FONT:
					app.tools.activeOptions.textFontFamily = subMenu.tool_text_opt_font.selectedItem;
					break;
				case DROPLIST_FONTSTYLE:
					app.tools.activeOptions.textFontStyle = subMenu.tool_text_opt_fontStyle.selectedItem;
					break;
				case DROPLIST_FONTSIZE:
					app.tools.activeOptions.textFontSize = subMenu.tool_text_opt_fontSize.selectedItem;
					break;
			}
			updateActiveTool();
		}
		
		private function handleToolClick(e:MouseEvent):void {
			app.tools.activeTool = app.tools.TOOL_TEXT_OPTION_FONT;
			updateActiveTool();
			hide();
		}
		
		private function updateActiveTool():void
		{
			if(app.tools.activeTool === app.tools.TOOL_TEXT_OPTION_FONT) {
				if(app.instance.tool_text.styleName !== app.tools.MAIN_TOOL_ACTIVE_STYLENAME) {
					app.tools.removeAllActiveState();
					app.instance.tool_text.styleName = app.tools.MAIN_TOOL_ACTIVE_STYLENAME;
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
