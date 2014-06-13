package org.chineseforall.components
{	
	import flash.display.GradientType;
	import flash.display.Shape;
	import flash.geom.Matrix;
	
	import mx.core.UIComponent;
	
	public class Canvas extends UIComponent
	{
		/*private var _box:Shape;
		private var _zoom:Number;
		private var _newZoom:Number;*/
		
		public function Canvas()
		{
			super();
			//_zoom = 1;
			//_newZoom = 1;
		}
		
		/*[Bindable]
		public function get zoom():Number { return _zoom * 100; }
		public function set zoom(value:Number):void
		{
			if (value / 100 == _zoom) return;
			
			_newZoom = value / 100;
			
			if (!isNaN(explicitWidth)) width = _box.width / _zoom * _newZoom;
			if (!isNaN(explicitHeight)) height = _box.height / _zoom * _newZoom;
			
			invalidateSize();
		}
		
		public function addBox(w:Number, h:Number):void
		{
			_box = new Shape();
			_drawBox(w * _zoom, h * _zoom);
			addChild(_box);
			
			if (!isNaN(explicitWidth)) width = _box.width / _zoom * _newZoom;
			if (!isNaN(explicitHeight)) height = _box.height / _zoom * _newZoom;
			
			invalidateSize();
			invalidateDisplayList();
		}
		
		override protected function createChildren():void
		{
			super.createChildren();
		}
		
		override protected function measure():void
		{
			super.measure();
			
			if (_box == null) return;
			measuredWidth = _box.width / _zoom * _newZoom;
			measuredHeight = _box.height / _zoom * _newZoom;
			_zoom = _newZoom;
		}
		
		override protected function updateDisplayList(unscaledWidth:Number, unscaledHeight:Number):void
		{
			if (_box == null) return;
			_drawBox(unscaledWidth, unscaledHeight);
			_zoom = _newZoom;
		}
		
		private function _drawBox(width:Number, height:Number):void
		{
			var matrix:Matrix = new Matrix();
			matrix.createGradientBox(width, height, 45 * Math.PI / 180);		
			
			_box.graphics.clear();
			_box.graphics.beginGradientFill(GradientType.LINEAR, [0xFF0000, 0xFFFF00], [1, 1], [0, 255], matrix);
			_box.graphics.drawRect(0, 0, width, height);
			_box.graphics.endFill();
		}*/
	}
}