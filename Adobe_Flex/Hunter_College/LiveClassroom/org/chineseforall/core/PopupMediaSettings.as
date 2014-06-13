package org.chineseforall.core
{
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import mx.controls.Alert;
	import mx.effects.Fade;
	
	import org.chineseforall.components.optMicCamMenu;
	
	import spark.components.CheckBox;
	import spark.effects.Scale;
	import spark.events.DropDownEvent;
	import spark.events.ListEvent;
	
	public class PopupMediaSettings extends ToolSubmenu implements IToolSubmenu
	{
		// These properties will be accessed by the singleton Settings object
		// Therefore, these are the properties holding the mic/webcam values
		public var micVolume:uint = 50;
		// We do not want to return the index so we will make a getter for these
		public var micQuality:uint = 7;
		public var camQuality:uint = 50;
		public var camFPS:uint = 4;
		public var micState:Boolean = true;
		public var camState:Boolean = true;
		public var micIndex:uint = 0;
		public var camIndex:uint = 0;
		
		private var micQualityIndex:uint = 7;
		private var camFPSIndex:uint = 4;
		
		// In case the user cancels their changes by clicking the 'X' (or close)
		// We will keep the user's temp changes in these variables and move
		// The values over upon the user clicking submit or revert on close
		private var tmp_micVolume:uint = 50;
		private var tmp_micQuality:uint = 7;
		private var tmp_camQuality:uint = 50;
		private var tmp_camFPS:uint = 4;
		private var tmp_micState:Boolean = true;
		private var tmp_camState:Boolean = true;
		private var tmp_micIndex:uint = 0;
		private var tmp_camIndex:uint = 0;
		private var changedItems:Array = null;
		
		private const BTN_MINUS_MIC:String = "ms_micVolumeDown";
		private const BTN_MINUS_CAM:String = "ms_camQualityDown";
		private const BTN_PLUS_MIC:String = "ms_micVolumeUp";
		private const BTN_PLUS_CAM:String = "ms_camQualityUp";
		private const SLD_MIC:String = "ms_micVolume";
		private const SLD_CAM:String = "ms_camQuality";
		private const CHK_MIC:String = "ms_micState";
		private const CHK_CAM:String = "ms_camState";
		private const LST_MIC:String = "ms_micList";
		private const LST_CAM:String = "ms_camList";
		private const LST_MIC_QUALITY:String = "ms_micQuality";
		private const LST_CAM_FPS:String = "ms_camFPS";
		private const BTN_CLOSE:String = "ms_btn_close";
		private const BTN_SUBMIT:String = "ms_btn_submit";
		
		private var popUp:optMicCamMenu = null;
		
		public function PopupMediaSettings(app_handle:App)
		{
			super(app_handle);
			initialize();
		}
		
		private function initialize():void
		{
			if(popUp === null) {
				popUp = new optMicCamMenu();
			}
		}
		
		public function display():void
		{
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
		
		private function addEventListeners():void
		{
			// Let's add our tool items' event listeners
			popUp.ms_micVolumeDown.addEventListener(MouseEvent.CLICK, handleToolClick);
			popUp.ms_micVolumeUp.addEventListener(MouseEvent.CLICK, handleToolClick);
			popUp.ms_camQualityDown.addEventListener(MouseEvent.CLICK, handleToolClick);
			popUp.ms_camQualityUp.addEventListener(MouseEvent.CLICK, handleToolClick);
			popUp.ms_micState.addEventListener(MouseEvent.CLICK, handleToolClick);
			popUp.ms_camState.addEventListener(MouseEvent.CLICK, handleToolClick);
			popUp.ms_micList.addEventListener(Event.CHANGE, handleToolChange);
			popUp.ms_camList.addEventListener(Event.CHANGE, handleToolChange);
			popUp.ms_micVolume.addEventListener(Event.CHANGE, handleToolChange);
			popUp.ms_camQuality.addEventListener(Event.CHANGE, handleToolChange);
			popUp.ms_micQuality.addEventListener(Event.CHANGE, handleToolChange);
			popUp.ms_camFPS.addEventListener(Event.CHANGE, handleToolChange);
			popUp.ms_btn_close.addEventListener(MouseEvent.CLICK, handleToolClick);
			popUp.ms_btn_submit.addEventListener(MouseEvent.CLICK, handleToolClick);
		}
		
		private function removeEventListeners():void
		{
			// Let's remove our tool items' event listeners
			popUp.ms_micVolumeDown.removeEventListener(MouseEvent.CLICK, handleToolClick);
			popUp.ms_micVolumeUp.removeEventListener(MouseEvent.CLICK, handleToolClick);
			popUp.ms_camQualityDown.removeEventListener(MouseEvent.CLICK, handleToolClick);
			popUp.ms_camQualityUp.removeEventListener(MouseEvent.CLICK, handleToolClick);
			popUp.ms_micState.removeEventListener(MouseEvent.CLICK, handleToolClick);
			popUp.ms_camState.removeEventListener(MouseEvent.CLICK, handleToolClick);
			popUp.ms_micList.removeEventListener(Event.CHANGE, handleToolChange);
			popUp.ms_camList.removeEventListener(Event.CHANGE, handleToolChange);
			popUp.ms_micVolume.removeEventListener(Event.CHANGE, handleToolChange);
			popUp.ms_camQuality.removeEventListener(Event.CHANGE, handleToolChange);
			popUp.ms_micQuality.removeEventListener(Event.CHANGE, handleToolChange);
			popUp.ms_camFPS.removeEventListener(Event.CHANGE, handleToolChange);
			popUp.ms_btn_close.removeEventListener(MouseEvent.CLICK, handleToolClick);
			popUp.ms_btn_submit.removeEventListener(MouseEvent.CLICK, handleToolClick);
		}
		
		private function handleToolClick(e:MouseEvent):void {
			switch(e.currentTarget.id) {
				case BTN_MINUS_MIC:
					if(popUp.ms_micVolume.value > 0){
						popUp.ms_micVolume.value = popUp.ms_micVolume.value - 1;
						tmp_micVolume = popUp.ms_micVolume.value;
					}
					break;
				case BTN_PLUS_MIC:
					if(popUp.ms_micVolume.value < 100){
						popUp.ms_micVolume.value = popUp.ms_micVolume.value + 1;
						tmp_micVolume = popUp.ms_micVolume.value;
					}
					break;
				case BTN_MINUS_CAM:
					if(popUp.ms_camQuality.value > 0){
						popUp.ms_camQuality.value = popUp.ms_camQuality.value - 1;
						tmp_camQuality = popUp.ms_camQuality.value;
					}
					break;
				case BTN_PLUS_CAM:
					if(popUp.ms_camQuality.value < 100){
						popUp.ms_camQuality.value = popUp.ms_camQuality.value + 1;
						tmp_camQuality = popUp.ms_camQuality.value;
					}
					break;
				case CHK_MIC:
					tmp_micState = popUp.ms_micState.selected;
					break;
				case CHK_CAM:
					tmp_camState = popUp.ms_camState.selected;
					break;
				case BTN_CLOSE:
					// User does not want to register changes so let's revert UI
					popUp.ms_camQuality.value = camQuality;
					popUp.ms_micVolume.value = micVolume;
					popUp.ms_micState.selected = micState;
					popUp.ms_camState.selected = camState;
					popUp.ms_micList.selectedIndex = micIndex;
					popUp.ms_camList.selectedIndex = camIndex;
					popUp.ms_micQuality.selectedIndex = micQualityIndex;
					popUp.ms_camFPS.selectedIndex = camFPSIndex;
					hide();
					break;
				case BTN_SUBMIT:
					// User wants to register settings so lets update
					changedItems = new Array();
					if(camQuality != tmp_camQuality) {
						changedItems.push("camQuality");
						camQuality = tmp_camQuality;
					}
					if(micVolume != tmp_micVolume) {
						changedItems.push("micVolume");
						micVolume = tmp_micVolume;
					}
					if(micState != tmp_micState) {
						changedItems.push("micState");
						micState = tmp_micState;
						app.instance.holdCtrlToSpeak = micState;
						app.settings.toggleMic(app.instance.micEnabled);
					}
					if(camState != tmp_camState) {
						changedItems.push("camState");
						camState = tmp_camState;
					}
					if(micIndex != tmp_micIndex) {
						changedItems.push("micIndex");
						micIndex = tmp_micIndex;
					}
					if(camIndex != tmp_camIndex) {
						changedItems.push("camIndex");
						camIndex = tmp_camIndex;
					}
					if(micQuality !=  popUp.ms_micQuality.selectedItem) {
						changedItems.push("micQuality");
						micQuality =  popUp.ms_micQuality.selectedItem;
					}
					if(camFPS != popUp.ms_camFPS.selectedItem) {
						changedItems.push("camFPS");
						camFPS = popUp.ms_camFPS.selectedItem;
					}
					if(camFPSIndex != tmp_camFPS) {
						changedItems.push("camFPSIndex");
						camFPSIndex = tmp_camFPS;
					}
					if(micQualityIndex != tmp_micQuality) {
						changedItems.push("micQualityIndex");
						micQualityIndex = tmp_micQuality;
					}
					//app.settings.updateMic();
					hide();
					break;
			}
		}
		
		private function handleToolChange(e:Event):void {
			switch(e.currentTarget.id) {
				case LST_MIC:
					tmp_micIndex = popUp.ms_micList.selectedIndex;
					break;
				case LST_CAM:
					tmp_camIndex = popUp.ms_camList.selectedIndex;
					break;
				case SLD_MIC:
					tmp_micVolume = popUp.ms_micVolume.value;
					break;
				case SLD_CAM:
					tmp_camQuality = popUp.ms_camQuality.value;
					break;
				case LST_MIC_QUALITY:
					tmp_micQuality = popUp.ms_micQuality.selectedIndex;
					break;
				case LST_CAM_FPS:
					tmp_camFPS = popUp.ms_camFPS.selectedIndex;
					break;
			}
		}
		
		public function hide():void
		{
			if(popUp !== null && bHidden === false) {
				removeEventListeners();
				app.instance.removeElement(popUp);
				bHidden = true;
				app.instance.setFocus();
			}
		}
		
	}
}
