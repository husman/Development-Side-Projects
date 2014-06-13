package org.chineseforall.core
{
	import mx.controls.Alert;

	public class ToolSubmenu
	{	
		protected var app:App = null;
		protected var bHidden:Boolean = true;
		
		public function ToolSubmenu(app_handle:App)
		{
			app = app_handle;
		}
	}
}