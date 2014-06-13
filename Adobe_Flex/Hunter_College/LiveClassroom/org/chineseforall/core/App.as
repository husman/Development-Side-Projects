package org.chineseforall.core
{
	import org.chineseforall.net.Net;

	public class App
	{
		public var instance:LiveClassroom; // Our flex app handle/instance
		public var tools:Tools = null;
		public var whiteboard:Whiteboard = null;
		public var net:Net = null;
		public var settings:Settings = null;
		
		public function App(app_instance:LiveClassroom)
		{
			instance = app_instance;
			settings = new Settings(this);;
			net = new Net(this);
			
			if(instance !== null) {
				tools = new Tools(this);
				whiteboard = new Whiteboard(this);
			}
		}
		
	}
}