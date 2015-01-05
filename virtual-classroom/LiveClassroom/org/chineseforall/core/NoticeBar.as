package org.chineseforall.core
{
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.utils.setTimeout;
	
	import mx.controls.Alert;
	import mx.effects.Fade;
	import mx.rpc.events.HeaderEvent;
	
	import org.chineseforall.components.idleNotice;
	
	import spark.components.CheckBox;
	import spark.effects.Scale;
	import spark.events.DropDownEvent;
	import spark.events.ListEvent;
	
	public class NoticeBar extends ToolSubmenu implements IToolSubmenu
	{	
		private const BTN_CLOSE:String = "btn_close";
		private var popUp:idleNotice = null;
		private var duration:uint = 0;
		
		public function NoticeBar(app_handle:App)
		{
			super(app_handle);
			initialize();
		}
		
		private function initialize():void
		{
			if(popUp === null) {
				popUp = new idleNotice();	
			}
		}
		
		public function display():void
		{
			if(popUp !== null && bHidden === true) {
				popUp.visible = true;
				// Let's fade in the submenu
				var fade:Fade = new Fade();
				fade.alphaFrom = 0.0;
				fade.alphaTo = 0.75;
				fade.play([popUp]);
				
				addEventListeners();
				bHidden = false;
				
				if(duration > 0) {
					setTimeout(hide, duration);
				}
			}	
			
		}
		
		private function addEventListeners():void
		{
			// Let's add our tool items' event listeners
			popUp.btn_close.addEventListener(MouseEvent.CLICK, handleToolClick);
		}
		
		private function removeEventListeners():void
		{
			// Let's remove our tool items' event listeners
			popUp.btn_close.removeEventListener(MouseEvent.CLICK, handleToolClick);
		}
		
		private function handleToolClick(e:MouseEvent):void {
			switch(e.currentTarget.id) {
				case BTN_CLOSE:
					// User does not want to register changes so let's revert UI
					hide();
					break;
			}
		}
		
		public function setNotice(type:String, msg:String, display_duration:uint):void
		{
			popUp.visible = false;
			app.instance.body.addElement(popUp);
			popUp.width = app.instance.canvas.width - 8;
			popUp.x = 3;
			popUp.y = 3;
			
			duration = display_duration;
			popUp.msg.text = msg;
			switch(type) {
				case "userJoined":
					popUp.icon.source = "assets/images/icons/Users-icon.png";
					popUp.setStyle("backgroundColor", "#669900");
					popUp.setStyle("borderColor", "#999999");
					popUp.msg.setStyle("color", "#FFFFFF");
					popUp.btn_close.setStyle("color", "#FFFFFF");
					break;
				case "userLeft":
					popUp.icon.source = "assets/images/icons/userLeft.png"
					popUp.setStyle("backgroundColor", "#555555");
					popUp.setStyle("borderColor", "#999999");
					popUp.msg.setStyle("color", "#FFFFFF");
					popUp.btn_close.setStyle("color", "#FFFFFF");
					break;
				case "success":
					popUp.icon.source = "assets/images/icons/good.png";
					popUp.setStyle("backgroundColor", "#669900");
					popUp.setStyle("borderColor", "#999999");
					popUp.msg.setStyle("color", "#FFFFFF");
					popUp.btn_close.setStyle("color", "#FFFFFF");
					break;
				default:
					popUp.icon.source = "assets/images/icons/infoIcon.png"
					popUp.setStyle("backgroundColor", "#519CEA");
					popUp.setStyle("borderColor", "#999999");
					popUp.msg.setStyle("color", "#FFFFFF");
					popUp.btn_close.setStyle("color", "#FFFFFF");
					break;
			}
		}
		
		public function hide():void
		{
			if(popUp !== null && bHidden === false) {
				removeEventListeners();
				app.instance.body.removeElement(popUp);
				bHidden = true;
			}
		}
		
	}
}
