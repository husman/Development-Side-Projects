package org.chineseforall.core
{
	import flash.events.KeyboardEvent;
	import flash.events.MouseEvent;
	import flash.ui.Keyboard;
	
	import mx.controls.Alert;
	import mx.controls.ColorPicker;
	import mx.effects.Fade;
	import mx.events.ColorPickerEvent;
	import mx.events.FlexEvent;
	
	import org.chineseforall.components.optBrusheMenu;
	
	import spark.effects.Scale;
	
	public class ToolBrushesOptions extends ToolShapes implements IToolSubmenu
	{
		private const BTN_CLOSE:String = "tool_brushes_opt_close";
		private const BTN_SUBMIT:String = "tool_brushes_opt_submit";
		
		private var optMenu:optBrusheMenu = null;
		private var state:int = 0;
		
		public function ToolBrushesOptions(app_handle:App)
		{
			super(app_handle);
			initialize();
		}
		
		private function initialize():void
		{
			if(optMenu === null) {
				optMenu = new optBrusheMenu();
				state = app.tools.STATE_HIDDEN;
			}
		}
		
		override public function display():void
		{
			if(optMenu !== null && state === app.tools.STATE_HIDDEN) {
				app.instance.addElement(optMenu);
				optMenu.x = app.instance.width / 2 - optMenu.width;
				optMenu.y = app.instance.height / 2 - optMenu.height/2;
				
				// Let's fade in the submenu
				var fade:Fade = new Fade();
				fade.alphaFrom = 0.0;
				fade.alphaTo = 1.0;
				fade.play([optMenu]);
				
				var scale:Scale = new Scale();
				scale.scaleYFrom = 0.0, scale.scaleXFrom = 0.2;
				scale.scaleYTo = 1.0, scale.scaleXTo = 1.0;
				scale.play([optMenu]);
				
				addEventListeners();
				state = app.tools.STATE_NOT_HIDDEN;
				if(app.tools.iToolOptionSubmenu !== this) {
					app.tools.iToolOptionSubmenu = this;
				}
				updateActiveOptions("all");
			}
		}
		
		private function addEventListeners():void
		{
			// Let's add our tool items' event listeners
			optMenu.tool_brushes_opt_close.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_brushes_opt_submit.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_brushes_opt_tool_paintbrush.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_brushes_opt_tool_highlighter.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_brushes_opt_linestyle_solid.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_brushes_opt_linestyle_dashed.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_brushes_opt_linewidth_1.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_brushes_opt_linewidth_2.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_brushes_opt_linewidth_3.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_brushes_opt_linewidth_4.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_brushes_opt_linewidth_5.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_brushes_opt_arrowhead_none.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_brushes_opt_arrowhead_left.addEventListener(MouseEvent.CLICK, handleToolClick);
			optMenu.tool_brushes_opt_arrowhead_right.addEventListener(MouseEvent.CLICK, handleToolClick);
		}
		
		private function handleToolClick(e:MouseEvent):void {
			switch(e.currentTarget.id) {
				case app.tools.TOOL_BRUSHES_OPTION_TOOL_PAINTBRUSH:
					app.tools.activeOptions.brushTool = app.tools.TOOL_BRUSHES_OPTION_TOOL_PAINTBRUSH;
					app.tools.activeTool = app.tools.TOOL_BRUSHES_SUBMENU_PAINTBRUSH;
					app.tools.activeOptions.sel_brush_tool = app.tools.TOOL_BRUSHES_SUBMENU_PAINTBRUSH;
					updateActiveOptions("brushes");
					break;
				case app.tools.TOOL_BRUSHES_OPTION_TOOL_HIGHLIGHTER:
					app.tools.activeOptions.brushTool = app.tools.TOOL_BRUSHES_OPTION_TOOL_HIGHLIGHTER;
					app.tools.activeTool = app.tools.TOOL_BRUSHES_SUBMENU_HIGHLIGHTER;
					app.tools.activeOptions.sel_brush_tool = app.tools.TOOL_BRUSHES_SUBMENU_HIGHLIGHTER;
					updateActiveOptions("brushes");
					break;
				case app.tools.TOOL_BRUSHES_OPTION_LINESTYLE_SOLID:
					app.tools.activeOptions.brushLineStyle = app.tools.TOOL_BRUSHES_OPTION_LINESTYLE_SOLID;
					updateActiveOptions("lineStyle");
					break;
				case app.tools.TOOL_BRUSHES_OPTION_LINESTYLE_DASHED:
					app.tools.activeOptions.brushLineStyle = app.tools.TOOL_BRUSHES_OPTION_LINESTYLE_DASHED;
					updateActiveOptions("lineStyle");
					break;
				case app.tools.TOOL_BRUSHES_OPTION_LINEWIDTH_1:
					app.tools.activeOptions.brushLineWidth = app.tools.TOOL_BRUSHES_OPTION_LINEWIDTH_1;
					app.tools.activeOptions.lineWidthValue = 1;
					updateActiveOptions("lineWidth");
					break;
				case app.tools.TOOL_BRUSHES_OPTION_LINEWIDTH_2:
					app.tools.activeOptions.brushLineWidth = app.tools.TOOL_BRUSHES_OPTION_LINEWIDTH_2;
					app.tools.activeOptions.lineWidthValue = 3;
					updateActiveOptions("lineWidth");
					break;
				case app.tools.TOOL_BRUSHES_OPTION_LINEWIDTH_3:
					app.tools.activeOptions.brushLineWidth = app.tools.TOOL_BRUSHES_OPTION_LINEWIDTH_3;
					app.tools.activeOptions.lineWidthValue = 5;
					updateActiveOptions("lineWidth");
					break;
				case app.tools.TOOL_BRUSHES_OPTION_LINEWIDTH_4:
					app.tools.activeOptions.brushLineWidth = app.tools.TOOL_BRUSHES_OPTION_LINEWIDTH_4;
					app.tools.activeOptions.lineWidthValue = 7;
					updateActiveOptions("lineWidth");
					break;
				case app.tools.TOOL_BRUSHES_OPTION_LINEWIDTH_5:
					app.tools.activeOptions.brushLineWidth = app.tools.TOOL_BRUSHES_OPTION_LINEWIDTH_5;
					app.tools.activeOptions.lineWidthValue = 9;
					updateActiveOptions("lineWidth");
					break;
				case app.tools.TOOL_BRUSHES_OPTION_ARROWHEAD_NONE:
					app.tools.activeOptions.brushArrowHead = app.tools.TOOL_BRUSHES_OPTION_ARROWHEAD_NONE;
					updateActiveOptions("arrowHead");
					break;
				case app.tools.TOOL_BRUSHES_OPTION_ARROWHEAD_LEFT:
					app.tools.activeOptions.brushArrowHead = app.tools.TOOL_BRUSHES_OPTION_ARROWHEAD_LEFT;
					updateActiveOptions("arrowHead");
					break;
				case app.tools.TOOL_BRUSHES_OPTION_ARROWHEAD_RIGHT:
					app.tools.activeOptions.brushArrowHead = app.tools.TOOL_BRUSHES_OPTION_ARROWHEAD_RIGHT;
					updateActiveOptions("arrowHead");
					break;
				case BTN_CLOSE:
				case BTN_SUBMIT:
					hide();
					break;
			}
		}
		
		private function updateActiveOptions(target:String):void
		{
			resetActiveOptions(target);
			if(target === "brushes" || target === "all") {
				switch(app.tools.activeOptions.brushTool) {
					case app.tools.TOOL_BRUSHES_OPTION_TOOL_PAINTBRUSH:
						optMenu.tool_brushes_opt_tool_paintbrush.styleName = "toolBorderActive";
						break;
					case app.tools.TOOL_BRUSHES_OPTION_TOOL_HIGHLIGHTER:
						optMenu.tool_brushes_opt_tool_highlighter.styleName = "toolBorderActive";
						break;
				}
			}
			
			if(target === "lineStyle" || target === "all") {
				switch(app.tools.activeOptions.brushLineStyle) {
					case app.tools.TOOL_BRUSHES_OPTION_LINESTYLE_SOLID:
						optMenu.tool_brushes_opt_linestyle_solid.styleName = "toolBorderActive";
						break;
					case app.tools.TOOL_BRUSHES_OPTION_LINESTYLE_DASHED:
						optMenu.tool_brushes_opt_linestyle_dashed.styleName = "toolBorderActive";
						break;
				}
			}
			
			if(target === "lineWidth" || target === "all") {
				switch(app.tools.activeOptions.brushLineWidth) {
					case app.tools.TOOL_BRUSHES_OPTION_LINEWIDTH_1:
						optMenu.tool_brushes_opt_linewidth_1.styleName = "toolBorderActive";
						break;
					case app.tools.TOOL_BRUSHES_OPTION_LINEWIDTH_2:
						optMenu.tool_brushes_opt_linewidth_2.styleName = "toolBorderActive";
						break;
					case app.tools.TOOL_BRUSHES_OPTION_LINEWIDTH_3:
						optMenu.tool_brushes_opt_linewidth_3.styleName = "toolBorderActive";
						break;
					case app.tools.TOOL_BRUSHES_OPTION_LINEWIDTH_4:
						optMenu.tool_brushes_opt_linewidth_4.styleName = "toolBorderActive";
						break;
					case app.tools.TOOL_BRUSHES_OPTION_LINEWIDTH_5:
						optMenu.tool_brushes_opt_linewidth_5.styleName = "toolBorderActive";
						break;
				}
			}
			
			if(target === "arrowHead" || target === "all") {
				switch(app.tools.activeOptions.brushArrowHead) {
					case app.tools.TOOL_BRUSHES_OPTION_ARROWHEAD_NONE:
						optMenu.tool_brushes_opt_arrowhead_none.styleName = "toolBorderActive";
						break;
					case app.tools.TOOL_BRUSHES_OPTION_ARROWHEAD_LEFT:
						optMenu.tool_brushes_opt_arrowhead_left.styleName = "toolBorderActive";
						break;
					case app.tools.TOOL_BRUSHES_OPTION_ARROWHEAD_RIGHT:
						optMenu.tool_brushes_opt_arrowhead_right.styleName = "toolBorderActive";
						break;
				}
			}
		}
		
		private function resetActiveOptions(target:String):void
		{
			if(target === "brushes" || target === "all") {
				optMenu.tool_brushes_opt_tool_paintbrush.styleName = "toolBorder";
				optMenu.tool_brushes_opt_tool_highlighter.styleName = "toolBorder";
			}
			
			if(target === "lineStyle" || target === "all") {
				optMenu.tool_brushes_opt_linestyle_solid.styleName = "toolBorder";
				optMenu.tool_brushes_opt_linestyle_dashed.styleName = "toolBorder";
			}
			
			if(target === "lineWidth" || target === "all") {
				optMenu.tool_brushes_opt_linewidth_1.styleName = "toolBorder";
				optMenu.tool_brushes_opt_linewidth_2.styleName = "toolBorder";
				optMenu.tool_brushes_opt_linewidth_3.styleName = "toolBorder";
				optMenu.tool_brushes_opt_linewidth_4.styleName = "toolBorder";
				optMenu.tool_brushes_opt_linewidth_5.styleName = "toolBorder";
			}
			
			if(target === "arrowHead" || target === "all") {
				optMenu.tool_brushes_opt_arrowhead_none.styleName = "toolBorder";
				optMenu.tool_brushes_opt_arrowhead_left.styleName = "toolBorder";
				optMenu.tool_brushes_opt_arrowhead_right.styleName = "toolBorder";
			}
			
		}
		
		override public function hide():void
		{
			if(optMenu !== null && state === app.tools.STATE_NOT_HIDDEN) {
				app.instance.removeElement(optMenu);
				state = app.tools.STATE_HIDDEN;
				app.tools.iToolOptionSubmenu = null;
			}
		}
	}
}