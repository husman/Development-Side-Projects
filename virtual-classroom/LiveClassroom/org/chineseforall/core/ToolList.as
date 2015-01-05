package org.chineseforall.core
{
	public final class ToolList
	{
		private var app:App;
		public var toolPointers:ToolPointers = null;
		public var toolShapes:ToolShapes = null;
		public var toolBrushes:ToolBrushes = null;
		public var toolDeletes:ToolDeletes = null;
		public var toolZoom:ToolZoom = null;
		public var toolText:ToolText = null;
		
		public function ToolList(app_handle:App)
		{
			app = app_handle;
		}
		
		public function getToolPointersSubmenu():ToolPointers
		{
			if(toolPointers === null) {
				toolPointers = new ToolPointers(app);
			}
			return toolPointers;
		}
		
		public function getToolShapesSubmenu():ToolShapes
		{
			if(toolShapes === null) {
				toolShapes = new ToolShapes(app);
			}
			return toolShapes;
		}
		
		public function getToolBrushesSubmenu():ToolBrushes
		{
			if(toolBrushes === null) {
				toolBrushes = new ToolBrushes(app);
			}
			return toolBrushes;
		}
		
		public function getToolDeletesSubmenu():ToolDeletes
		{
			if(toolDeletes === null) {
				toolDeletes = new ToolDeletes(app);
			}
			return toolDeletes;
		}
		
		public function getToolZoomSubmenu():ToolZoom
		{
			if(toolZoom === null) {
				toolZoom = new ToolZoom(app);
			}
			return toolZoom;
		}
		
		public function getToolTextSubmenu():ToolText
		{
			if(toolText === null) {
				toolText = new ToolText(app);
			}
			return toolText;
		}
		
	}
}