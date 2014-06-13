package org.chineseforall.core
{
	import org.chineseforall.components.popupLoginScreen;

	public final class PopupList
	{
		private var app:App;
		public var popupMediaSettings:PopupMediaSettings = null;
		public var popupFileManager:PopupFileManager = null;
		public var popupLoginScreen:PopupLoginScreen = null;
		
		public function PopupList(app_handle:App)
		{
			app = app_handle;
			initialize();
		}
		
		public function initialize():void
		{
			// Let's make sure a new instance of the popup menus are created
			// We call the respective function to create the instance.
			getPopupMediaSettings();
			
		}
		
		public function getPopupMediaSettings():PopupMediaSettings
		{
			if(popupMediaSettings === null) {
				popupMediaSettings = new PopupMediaSettings(app);
			}
			return popupMediaSettings;
		}
		
		public function getPopupFileManager():PopupFileManager
		{
			if(popupLoginScreen === null) {
				popupFileManager = new PopupFileManager(app);
			}
			return popupFileManager;
		}
		
		public function getPopupLoginScreen():PopupLoginScreen
		{
			if(popupLoginScreen === null) {
				popupLoginScreen = new PopupLoginScreen(app);
			}
			return popupLoginScreen;
		}
	}
}