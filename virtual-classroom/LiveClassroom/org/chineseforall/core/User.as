package org.chineseforall.core
{
	import flash.media.Microphone;
	import flash.net.NetStream;

	public class User
	{
		private var app:App = null;
		private var settings:Settings = null;
		private var username:String = null;
		private var firstname:String = null;
		private var lastname:String = null;
		private var icon:String = null;
		
		public function User(app_handle:App)
		{
			app = app_handle;
			settings = app.settings;
		}
		
		public function set username(value:String):void
		{
			username = value;
		}
		
		public function get username():String
		{
			return username;
		}
		
		public function set lastname(value:String):void
		{
			lastname = value;
		}
		
		public function get lastname():String
		{
			return lastname;
		}
		
		public function set lastname(value:String):void
		{
			lastname = value;
		}
		
		public function get icon():String
		{
			return icon;
		}
		
		public function set icon(value:String):void
		{
			icon = value;
		}
		
		public function get firstname():String
		{
			return firstname;
		}
		
		public function sendAudio():void
		{
			if(app.net.isConnected() && app.instance.micEnabled) {
				if(settings.outStream == null) {
					settings.outStream = new NetStream(app.net.getConnection());
					if(!settings.isMicMuted()) {
						settings.outStream.publish(settings.outStreamName);
					}
				}
			}
		}
		
		
	}
}