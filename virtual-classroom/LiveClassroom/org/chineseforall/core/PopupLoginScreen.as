package org.chineseforall.core
{
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import mx.controls.Alert;
	import mx.effects.Fade;
	import mx.rpc.events.HeaderEvent;
	
	import org.chineseforall.components.popupLoginScreen;
	
	import spark.components.CheckBox;
	import spark.effects.Scale;
	import spark.events.DropDownEvent;
	import spark.events.ListEvent;
	
	public class PopupLoginScreen extends ToolSubmenu implements IToolSubmenu
	{
		public var username:String = "";
		public var firstname:String = "";
		public var lastname:String = "";
		public var color:String = "";
		
		private var tmp_username:String = "";
		private var tmp_firstname:String = "";
		private var tmp_lastname:String = "";
		
		private const BTN_CLOSE:String = "lgn_btn_close";
		private const BTN_SUBMIT:String = "lgn_btn_submit";
		
		private var popUp:popupLoginScreen = null;
		
		public function PopupLoginScreen(app_handle:App)
		{
			super(app_handle);
			initialize();
		}
		
		private function initialize():void
		{
			if(popUp === null) {
				popUp = new popupLoginScreen();	
			}
		}
		
		public function display():void
		{
			if(!app.net.isConnected()) {
				if(popUp !== null && bHidden === true) {
					app.instance.addElement(popUp);
					popUp.x = app.instance.width / 2 - popUp.width;
					popUp.y = app.instance.height / 2 - popUp.height/2;
					
					// Let's fade in the submenu
					var fade:Fade = new Fade();
					fade.alphaFrom = 0.0;
					fade.alphaTo = 1.0;
					fade.play([popUp]);
					
					addEventListeners();
					bHidden = false;
				}	
			}
		}
		
		private function addEventListeners():void
		{
			// Let's add our tool items' event listeners
			popUp.lgn_btn_close.addEventListener(MouseEvent.CLICK, handleToolClick);
			popUp.lgn_btn_submit.addEventListener(MouseEvent.CLICK, handleToolClick);
		}
		
		private function removeEventListeners():void
		{
			// Let's remove our tool items' event listeners
			popUp.lgn_btn_close.removeEventListener(MouseEvent.CLICK, handleToolClick);
			popUp.lgn_btn_submit.removeEventListener(MouseEvent.CLICK, handleToolClick);
		}
		
		private function getSelectedColor():String
		{
			var hexString:* = popUp.lgn_color.selectedColor.toString(16).toUpperCase();
			var cnt:int = 6 - hexString.length;
			var zeros:String = "";
			
			for (var i:int = 0; i < cnt; i++) 
			{
				zeros += "0";
			}
			
			return "#" + zeros + hexString;
		}
		
		private function handleToolClick(e:MouseEvent):void {
			switch(e.currentTarget.id) {
				case BTN_CLOSE:
					// User does not want to register changes so let's revert UI
					popUp.lgn_username.text = username;
					popUp.lgn_firstname.text = firstname;
					popUp.lgn_lastname.text = lastname;
					hide();
					break;
				case BTN_SUBMIT:
					// User wants to register settings so lets update
					username = popUp.lgn_username.text;
					firstname = popUp.lgn_firstname.text;
					lastname = popUp.lgn_lastname.text;
					app.settings.username = username;
					app.settings.firstname = firstname;
					app.settings.lastname = lastname;
					var user_info:Object = {
						username: username,
						firstname: firstname,
						lastname: lastname,
						color: getSelectedColor(),
						icon: "assets/images/bullet_points/eicon.gif"
					};
					app.net.connect("rtmp://chineseforall.org/liveclassroom/room2", user_info);
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
