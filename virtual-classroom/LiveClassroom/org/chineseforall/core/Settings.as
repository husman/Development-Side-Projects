package org.chineseforall.core
{
	import flash.events.StatusEvent;
	import flash.media.Microphone;
	import flash.media.MicrophoneEnhancedMode;
	import flash.media.MicrophoneEnhancedOptions;
	import flash.media.Sound;
	import flash.media.SoundChannel;
	import flash.media.SoundCodec;
	import flash.media.SoundTransform;
	import flash.net.NetStream;
	import flash.net.URLRequest;
	import flash.system.Security;
	
	import mx.controls.Alert;
	
	import org.chineseforall.net.Net;
	
	public class Settings
	{
		private var app:App = null;
		private var sendingAudio:Boolean = false;
		private var notice:NoticeBar = null;
		
		public var mic:Microphone = null;
		public var outStream:NetStream = null;
		public var outStreamName:String = null;
		public var inStreams:Vector.<NetStream>;
		public var micCaptureRate:uint = 22;
		
		public var username:String = null;
		public var firstname:String = null;
		public var lastname:String = null;
		public var icon:String = null;
		
		public function Settings(app_handle:App)
		{
			app = app_handle;
			notice = new NoticeBar(app_handle);
		}
		
		private function displayNotice(type:String, msg:String, duration:uint):void
		{
			notice.setNotice(type, msg, duration);
			notice.display();
		}
		
		private function getMicrophone():Microphone
		{
			return Microphone.getEnhancedMicrophone(app.tools.popupList.getPopupMediaSettings().micIndex);
		}
		
		public function setupMic():Microphone
		{
			var mic_options:MicrophoneEnhancedOptions = new MicrophoneEnhancedOptions();
			
			mic_options.mode = MicrophoneEnhancedMode.FULL_DUPLEX;
			mic_options.echoPath = 128;
			mic_options.nonLinearProcessing = true;
			
			mic = getMicrophone();
			//Security.showSettings();
			mic.addEventListener(StatusEvent.STATUS, handleMicStatus);
			mic.enhancedOptions = mic_options;
			
			if (mic)
			{
				mic.setSilenceLevel(0);
				mic.codec = SoundCodec.SPEEX;
				mic.framesPerPacket = 1;
				mic.encodeQuality = app.tools.popupList.getPopupMediaSettings().micQuality;
				mic.rate = micCaptureRate;
				if(outStream == null) {
					outStream = new NetStream(app.net.getConnection());
					outStream.attachAudio(mic);
				}
			}
			return mic;
		}
		
		private function handleMicStatus(e:StatusEvent):void
		{
			if(e.code == "Microphone.Unmuted") {
				outStream.attachAudio(null);
				outStream.publish(outStreamName);
				app.net.msgServer("registerStream", {name: outStreamName}, null);
				displayNotice("info", "Your microphone has been activated. Please press and hold the CONTROL key to speak. Close this notice to begin", 10000);
			} else if(e.code == "Microphone.Muted") {
				//Alert.show("Microphone access was denied. You will not be heard.");
				outStream.close();
				outStream = null;
			}
		}
		
		public function toggleMic(micState:Boolean):void
		{
			if(outStream != null) {
				app.instance.micEnabled = micState;
				var snd:Sound;
				var trans:SoundTransform = new SoundTransform(0.1); 
				var channel:SoundChannel;
				if(app.instance.micEnabled || !app.instance.holdCtrlToSpeak) {
					if(!sendingAudio) {
						snd = new Sound(new URLRequest("assets/sounds/speakEnabled.mp3"));
						channel = snd.play(0, 1, trans);
						outStream.attachAudio(mic);
						sendingAudio = true;
					}
				} else {
					outStream.attachAudio(null);
					sendingAudio = false;
					snd = new Sound(new URLRequest("assets/sounds/speakDisabled.mp3"));
					channel = snd.play(0, 1, trans);
				}
			}
		}
		
		public function isMicMuted():Boolean
		{
			return mic.muted;
		}
		
	}
}