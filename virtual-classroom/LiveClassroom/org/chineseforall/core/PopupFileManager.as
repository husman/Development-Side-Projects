package org.chineseforall.core
{
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import mx.controls.Alert;
	import mx.effects.Fade;
	
	import org.chineseforall.components.fileManagementMenu;
	
	import spark.components.CheckBox;
	import spark.effects.Scale;
	import spark.events.DropDownEvent;
	import spark.events.ListEvent;
	
	public class PopupFileManager extends ToolSubmenu implements IToolSubmenu
	{
		private const BTN_CLOSE:String = "ms_btn_close";
		
		private var popUp:fileManagementMenu = null;
		
		public function PopupFileManager(app_handle:App)
		{
			super(app_handle);
			initialize();
		}
		
		private function initialize():void
		{
			if(popUp === null) {
				popUp = new fileManagementMenu();	
			}
		}
		
		public function display():void
		{
			if(popUp !== null && bHidden === true) {
				app.instance.addElement(popUp);
				popUp.x = (app.instance.canvas.width - popUp.width)/2;
				popUp.y = (app.instance.canvas.height - popUp.height)/2;
				
				// Let's fade in the submenu
				var fade:Fade = new Fade();
				fade.alphaFrom = 0.0;
				fade.alphaTo = 1.0;
				fade.play([popUp]);
				
				addEventListeners();
				bHidden = false;
			}	
		}
		
		private function addEventListeners():void
		{
			// Let's add our tool items' event listeners
			popUp.ms_btn_close.addEventListener(MouseEvent.CLICK, handleToolClick);
		}
		
		private function removeEventListeners():void
		{
			// Let's remove our tool items' event listeners
			popUp.ms_btn_close.removeEventListener(MouseEvent.CLICK, handleToolClick);
		}
		
		private function handleToolClick(e:MouseEvent):void {
			switch(e.currentTarget.id) {
				case BTN_CLOSE:
					hide();
					break;
			}
		}
		
		public function hide():void
		{
			if(popUp !== null && bHidden === false) {
				removeEventListeners();
				app.instance.removeElement(popUp);
				bHidden = true;
			}
		}
		
	}
}
