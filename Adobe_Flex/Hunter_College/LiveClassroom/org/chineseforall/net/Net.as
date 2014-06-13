package org.chineseforall.net
{
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.DisplayObject;
	import flash.display.GraphicsPath;
	import flash.display.IGraphicsData;
	import flash.display.Sprite;
	import flash.events.NetStatusEvent;
	import flash.events.SyncEvent;
	import flash.geom.Matrix;
	import flash.geom.Point;
	import flash.geom.Rectangle;
	import flash.media.Sound;
	import flash.net.NetConnection;
	import flash.net.NetStream;
	import flash.net.Responder;
	import flash.net.SharedObject;
	import flash.net.URLRequest;
	import flash.utils.ByteArray;
	
	import mx.collections.ArrayList;
	import mx.controls.Alert;
	
	import org.chineseforall.core.App;
	import org.chineseforall.core.NoticeBar;
	import org.chineseforall.core.graphics.ImageResizer;
	import org.chineseforall.core.graphics.ResizeMath;
	import org.chineseforall.core.graphics.Shapes;

	public class Net
	{
		private var app:App = null;
		private var conn:NetConnection = null;
		private var notice:NoticeBar = null;
		
		// Shared Objects
		private var shapes_so:SharedObject = null;
		private var text_so:SharedObject = null;
		private var drawing_so:SharedObject = null;
		
		// Our stream placeholders
		private var inStreams:Object = null;
		
		// Our client object. This object will hold the client-side functions
		// Our server may need to call from time-to-time.
		public var nc_client:Object = null;
		
		public function Net(app_handle:App)
		{
			app = app_handle;
			
			if(app !== null) {
				inStreams = new Object();
				conn = new NetConnection();
				
				// Let's initialize and set our client object
				nc_client = new Object();
				conn.client = nc_client;
				setupClientFunctions();
				
				// Let's initialize our notice prompt
				notice = new NoticeBar(app);
			}
			
		}
		
		private function displayNotice(type:String, msg:String, duration:uint):void
		{
			notice.setNotice(type, msg, duration);
			notice.display();
		}
		
		public function connect(url:String, user_info:Object):Boolean
		{
			if(!conn.connected) {
				var Now:Date = new Date();
				app.settings.outStreamName = user_info.username + (Now.getDate()+1).toString() + (Now.getMonth()+1).toString()
													  + Now.getFullYear().toString() + Now.getHours().toString()
													  + Now.getMinutes().toString() + Now.getSeconds().toString();
				conn.connect(url, user_info);
				conn.addEventListener(NetStatusEvent.NET_STATUS, handleConnectStatus);
				return true;
			}
			return false;
		}
		
		private function handleConnectStatus(e:NetStatusEvent):void
		{
			if(e.info.code === "NetConnection.Connect.Success") {
				setupSharedObjects();
				startMicrophone();
			} else {
				Alert.show(e.info.code);
			}
		}
		
		private function setupSharedObjects():void
		{
			shapes_so = SharedObject.getRemote("shapes", conn.uri, false);
			shapes_so.connect(conn);
			shapes_so.addEventListener(SyncEvent.SYNC, syncSharedShape);
			
			drawing_so = SharedObject.getRemote("drawing", conn.uri, false);
			drawing_so.connect(conn);
			drawing_so.addEventListener(SyncEvent.SYNC, syncSharedDrawing);
		}
		
		private function syncSharedShape(e:SyncEvent):void
		{
			if(shapes_so.data.shape != null) {
				// Return (do not do anything) if current user was the broadcaster
				if(shapes_so.data.shape.broadcaster == app.settings.username) {
					return;
				}
				var shape:Sprite = new Sprite();
				var canvas_width_ratio:Number = app.instance.canvas.width/shapes_so.data.shape.src_canvas_width;
				var canvas_height_ratio:Number = app.instance.canvas.height/shapes_so.data.shape.src_canvas_height;
				var ba:ByteArray;
				var bitmapData:BitmapData;
				var bitmap:Bitmap;
				var sprite:Sprite;
				//shape.name = shapes_so.data.shape.name;
				//shape.x = shapes_so.data.shape.x * canvas_width_ratio;
				//shape.y = shapes_so.data.shape.y * canvas_height_ratio;
				//app.instance.canvas.addChild(shape);
				
				switch(shapes_so.data.shape.type) {
					case "circle":
						/*
						Shapes.drawCircle(shape,
							shapes_so.data.shape.fillColor,
							shapes_so.data.shape.lineColor,
							shapes_so.data.shape.fillEnabled,
							shapes_so.data.shape.strokeEnabled,
							shapes_so.data.shape.lineWidth,
							shapes_so.data.shape.dashed,
							shapes_so.data.shape.dashLength,
							shapes_so.data.shape.x_center,
							shapes_so.data.shape.y_center,
							shapes_so.data.shape.radius * (shapes_so.data.shape.radius_direction == "x"? canvas_width_ratio : canvas_height_ratio));
						shape.width = shapes_so.data.shape.width * canvas_width_ratio;
						shape.height = shapes_so.data.shape.height * canvas_height_ratio;
						*/
						ba = shapes_so.data.shape.data;
						ba.position = 0;
						ba.uncompress();
						
						bitmapData = new BitmapData(shapes_so.data.shape.width, shapes_so.data.shape.height, true, 0);
						bitmapData.setPixels(bitmapData.rect, ba);
						bitmap = new Bitmap(bitmapData, "auto", true);
						
						sprite = new Sprite();
						sprite.addChild(bitmap);
						app.whiteboard.boardBitmapData.draw(sprite);
						break;
					case "line":
						if(shapes_so.data.shape.arrowHead == "none") {
							Shapes.drawLine(shape,
								new Point(0, 0),
								new Point(shapes_so.data.shape.width, shapes_so.data.shape.height),
								shapes_so.data.shape.lineColor,
								shapes_so.data.shape.strokeEnabled,
								shapes_so.data.shape.lineWidth,
								shapes_so.data.shape.dashed,
								shapes_so.data.shape.dashLength);
						} else {
							Shapes.drawArrowHead(shape,
								new Point(0, 0),
								new Point(shapes_so.data.shape.width, shapes_so.data.shape.height),
								shapes_so.data.shape.fillColor,
								shapes_so.data.shape.lineColor,
								shapes_so.data.shape.fillEnabled,
								shapes_so.data.shape.strokeEnabled,
								shapes_so.data.shape.lineWidth,
								shapes_so.data.shape.dashed,
								shapes_so.data.shape.arrowHead,
								shapes_so.data.shape.dashLength);
						}
						break;
					case "rectangle":
						Shapes.drawRect(shape,
							0, 0, shapes_so.data.shape.width,
							shapes_so.data.shape.height,
							shapes_so.data.shape.fillColor,
							shapes_so.data.shape.lineColor,
							shapes_so.data.shape.fillEnabled,
							shapes_so.data.shape.strokeEnabled,
							shapes_so.data.shape.lineWidth,
							shapes_so.data.shape.dashed,
							shapes_so.data.shape.dashLength);
						break;
					case "triangle":
						Shapes.drawTriangle(shape,
							0, 0, shapes_so.data.shape.width,
							shapes_so.data.shape.height,
							shapes_so.data.shape.fillColor,
							shapes_so.data.shape.lineColor,
							shapes_so.data.shape.fillEnabled,
							shapes_so.data.shape.strokeEnabled,
							shapes_so.data.shape.lineWidth,
							shapes_so.data.shape.dashed,
							shapes_so.data.shape.dashLength);
						break;
					case "arrow":
						Shapes.drawArrowHead(shape,
							new Point(0, 0),
							new Point(shapes_so.data.shape.width, shapes_so.data.shape.height),
							shapes_so.data.shape.fillColor,
							shapes_so.data.shape.lineColor,
							shapes_so.data.shape.fillEnabled,
							shapes_so.data.shape.strokeEnabled,
							shapes_so.data.shape.lineWidth,
							shapes_so.data.shape.dashed,
							shapes_so.data.shape.arrowHead,
							shapes_so.data.shape.dashLength);
						break;
				}
			}
		}
		
		private function syncSharedText(e:SyncEvent):void
		{
			if(shapes_so.data.shape != null) {
				var shape:Sprite = new Sprite();
				var canvas_width_ratio:Number = app.instance.canvas.width/shapes_so.data.shape.src_canvas_width;
				var canvas_height_ratio:Number = app.instance.canvas.height/shapes_so.data.shape.src_canvas_height;
				shape.name = shapes_so.data.shape.name;
				shape.x = shapes_so.data.shape.x * canvas_width_ratio;
				shape.y = shapes_so.data.shape.y * canvas_height_ratio;
				app.instance.canvas.addChild(shape);
			}
		}
		
		private function BitmapScaled(do_source:DisplayObject, thumbWidth:Number, thumbHeight:Number):BitmapData {
			var mat:Matrix = new Matrix();
			mat.scale(thumbWidth/do_source.width, thumbHeight/do_source.height);
			var bmpd_draw:BitmapData = new BitmapData(thumbWidth, thumbHeight, true, 0);
			bmpd_draw.draw(do_source, mat, null, null, null, true);
			return bmpd_draw;
		}
		
		private function assignMatrix(str:String, display_obj:DisplayObject):void 
		{
			var values:Array = str.split(':');
			var m:Matrix = new Matrix();
			m.a = values[0];
			m.b = values[1];
			m.c = values[2];
			m.d = values[3];
			m.tx = values[4];
			m.ty = values[5];
			display_obj.transform.matrix = m;
		}
		
		private function syncSharedDrawing(e:SyncEvent):void
		{
			if(drawing_so.data.drawing != null) {
				// Return (do not do anything) if current user was the broadcaster
				if(drawing_so.data.drawing.broadcaster == app.settings.username) {
					return;
				}
				var ba:ByteArray;
				var bitmapData:BitmapData;
				var bitmap:Bitmap;
				var sprite:Sprite;
				switch(drawing_so.data.drawing.type) {
					case "paintbrush":
					/*if(drawing_so.data.drawing.vector_data != null) {
						var brush_data:GraphicsPath = new GraphicsPath(new Vector.<int>(), new Vector.<Number>());
						var shape:Sprite = new Sprite();
						var canvas_width_ratio:Number = app.canvas.canvas.width/drawing_so.data.drawing.src_canvas_width;
						var canvas_height_ratio:Number = app.canvas.canvas.height/drawing_so.data.drawing.src_canvas_height;
						var drawing:Vector.<IGraphicsData> = new Vector.<IGraphicsData>();
						
						brush_data.data = drawing_so.data.drawing.vector_data.data;
						brush_data.commands = drawing_so.data.drawing.vector_data.commands;
						shape.name = drawing_so.data.drawing.name;
						shape.x = drawing_so.data.drawing.x * canvas_width_ratio;
						shape.y = drawing_so.data.drawing.y * canvas_height_ratio;
						app.canvas.canvas.addChild(shape);
						
						shape.graphics.lineStyle(drawing_so.data.drawing.lineWidth,
												 drawing_so.data.drawing.lineColor,
												 1,false, "none");
						drawing.push(brush_data);
						shape.graphics.drawGraphicsData(drawing);
					}*/
					
					//ba.position = 0;
					ba = drawing_so.data.drawing.pbGTipPath;
					ba.position = 0;
					ba.uncompress();
					
					bitmapData = new BitmapData(drawing_so.data.drawing.width, drawing_so.data.drawing.height, true, 0);
					bitmapData.setPixels(bitmapData.rect, ba); // position of data is now at 5th byte
					bitmap = new Bitmap(bitmapData, "auto", true);
					
					sprite = new Sprite();
					sprite.addChild(bitmap);
					app.whiteboard.boardBitmapData.draw(sprite);
					
					/*Alert.show(drawing_so.data.drawing.width.toString() + "x" + drawing_so.data.drawing.height.toString() + " AND "
							   + drawing_so.data.drawing.twidth.toString() + "x" + drawing_so.data.drawing.theight.toString());*/
					//boardBitmapData.draw(sprite);
					
					//var bmd:BitmapData = new BitmapData(drawing_so.data.drawing.width, drawing_so.data.drawing.height, true, 0); // 32 bit transparent bitmap
					//bmd.setPixels(bmd.rect, pixels); // position of data is now at 5th byte
					
					//var bm:Bitmap = new Bitmap(bmd);
					//app.instance.canvas.addChild(bm);
					break;
					case "selection_move":
						ba = drawing_so.data.drawing.data;
						ba.position = 0;
						ba.uncompress();
						
						bitmapData = new BitmapData(drawing_so.data.drawing.width, drawing_so.data.drawing.height, true, 0);
						bitmapData.setPixels(bitmapData.rect, ba); // position of data is now at 5th byte
						bitmap = new Bitmap(bitmapData, "auto", true);
						
						sprite = new Sprite();
						sprite.addChild(bitmap);
						assignMatrix(drawing_so.data.drawing.matrix_string, bitmap);
						
						var rectFill:Rectangle = new Rectangle(drawing_so.data.drawing.rect_x,
															   drawing_so.data.drawing.rect_y,
															   drawing_so.data.drawing.rect_width,
															   drawing_so.data.drawing.rect_height);
						app.whiteboard.boardBitmapData.fillRect(rectFill, 0);
						app.whiteboard.boardBitmapData.draw(bitmap, bitmap.transform.matrix);
						break;
				}
			}
		}
		
		public function shareShape(properties:Object):void
		{
			if(isConnected()) {
				properties.broadcaster = app.settings.username;
				shapes_so.setProperty("shape", properties);
			}
		}
		
		public function shareDrawing(properties:Object):void
		{
			if(isConnected()) {
				properties.broadcaster = app.settings.username;
				drawing_so.setProperty("drawing", properties);
			}
		}
		
		public function shareText(properties:Object):void
		{
			if(isConnected()) {
				properties.broadcaster = app.settings.username;
				text_so.setProperty("text", properties);
			}
		}
		
		public function disconnect():void
		{
			if(conn.connected) {
				conn.close();
			}
		}
		
		public function getConnection():NetConnection
		{
			return conn;
		}
		
		public function isConnected():Boolean
		{
			return conn.connected;
		}
		
		public function startMicrophone():void
		{
			if(conn.connected) {
				app.settings.setupMic();
			}
		}
		
		public function setupClientFunctions():void
		{	
			// Now let's map our client-side functions (aka functions the server may need to call)
			nc_client.addStream = Client_addStream; // call from the server telling us a new stream is available.
			nc_client.removeStream = Client_removeStream; // call from the server telling us a stream is no longer avail.
			nc_client.updateID = Client_updateID; // Server makes a call to this shortly after connection to send over the client id.
			nc_client.userJoined = Client_userJoined; // Server makes this call when an user joins the room. We add to room list etc.
			nc_client.userLeaves = Client_userLeaves; // Server makes this call when an user exits. We del from room list etc.
			
			nc_client.getBitmap = Client_getBitmap;
		}
		
		public function Client_getBitmap(client_id:String, arg:Object):void
		{
			var bm:BitmapData = arg.bm as BitmapData;
			if(bm != null) {
				displayNotice("info", "getBitmap called by server. Client id = " + client_id + " and prop1 = " + arg.prop1
						  + " and BitmapData = " + bm.height.toString(), 0);
			}
		}
		
		public function Client_userJoined(client_id:String, room_list:Array):void
		{
			var i:uint = room_list.length - 1,
				z:uint = 0;
			if(client_id != nc_client.id) {
				displayNotice("userJoined", room_list[i].username + ", " + room_list[i].firstname + " " + room_list[i].lastname
								 + ", has joined the room.", 5000);
				var snd:Sound = new Sound(new URLRequest("assets/sounds/newalert.mp3"));
				snd.play();
			}
			
			app.instance.roomListArray.removeAll();
			++i;
			for(z = 0; z < i; ++z)
				app.instance.roomListArray.addItem(room_list[z]);
		}
		
		public function Client_userLeaves(client_id:String, room_list:Array, user_info:Object):void
		{
			var i:uint = room_list.length - 1,
				z:uint = 0;
			if(client_id != nc_client.id) {
				displayNotice("userLeft", user_info.username + ", " + user_info.firstname + " " + user_info.lastname
					+ ", has left the room.", 5000);
				var snd:Sound = new Sound(new URLRequest("assets/sounds/userLeaves.mp3"));
				snd.play();
			}
			app.instance.roomListArray.removeAll();
			++i;
			for(z = 0; z < i; ++z)
				app.instance.roomListArray.addItem(room_list[z]);
		}
		
		public function Client_addStream(streamName:String):void
		{
			if(streamName != app.settings.outStreamName) {
				inStreams[streamName] = new NetStream(conn);
				inStreams[streamName].bufferTime = 0;
				inStreams[streamName].play(streamName);
				//Alert.show("playing stream (add stream): " + streamName);
			}
		}
		
		public function Client_removeStream(client_id:String, streamName:String):void
		{
			if(inStreams[streamName] != null && client_id != nc_client.id) {
				inStreams[streamName].close();
				//Alert.show("Closing stream: " + streamName);
			}
		}
		
		public function Client_updateID(client_id:String, stream_list:Array):String
		{
			nc_client.id = client_id;
			
			var i:uint = 0;
			for(i=0; i<stream_list.length; ++i) {
				if(stream_list[i] != app.settings.outStreamName) {
					inStreams[stream_list[i]] = new NetStream(conn);
					inStreams[stream_list[i]].bufferTime = 0;
					inStreams[stream_list[i]].play(stream_list[i]);
					//Alert.show("playing stream (update id): " + stream_list[i]);
				}
			}
			return nc_client.id;
		}
		
		public function msgServer(func:String, arg:Object, callback:Responder = null):void
		{
			if(isConnected()) {
				conn.call(func, callback, nc_client.id, arg);
			}
		}
		
	}
}