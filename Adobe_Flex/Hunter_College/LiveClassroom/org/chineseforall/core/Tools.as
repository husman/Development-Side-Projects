package org.chineseforall.core
{
	import flash.display.DisplayObject;
	import flash.display.DisplayObjectContainer;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.ui.Mouse;
	import flash.ui.MouseCursor;
	import flash.ui.MouseCursorData;
	
	import mx.binding.utils.BindingUtils;
	import mx.controls.Alert;
	import mx.controls.ColorPicker;
	import mx.controls.Label;
	import mx.core.IVisualElement;
	import mx.effects.Fade;
	import mx.utils.ColorUtil;
	
	import org.chineseforall.components.smArrowMenu;
	import org.chineseforall.components.smBrushMenu;
	import org.chineseforall.components.smDeleteMenu;
	import org.chineseforall.components.smShapeMenu;
	
	import spark.components.CheckBox;
	import spark.components.Label;
	
	public class Tools
	{
		// Constants defining the selected tool's string name (passed to app.tools.activeTool)
		public const TOOL_SHAPES_SUBMENU_ELLIPSE:String = "tool_shapes_ellipse";
		public const TOOL_SHAPES_SUBMENU_RECTANGLE:String = "tool_shapes_rectangle";
		public const TOOL_SHAPES_SUBMENU_TRIANGLE:String = "tool_shapes_triangle";
		public const TOOL_SHAPES_SUBMENU_LINE:String = "tool_shapes_line";
		public const TOOL_SHAPES_SUBMENU_ARROW:String = "tool_shapes_arrow";
		public const TOOL_SHAPES_SUBMENU_BUBBLE:String = "tool_shapes_bubble";
		public const TOOL_BRUSHES_SUBMENU_PAINTBRUSH:String = "tool_brushes_paintbrush";
		public const TOOL_BRUSHES_SUBMENU_HIGHLIGHTER:String = "tool_brushes_highlighter";
		public const TOOL_POINTERS_OPTION_POINTER:String = "tool_pointers_pointer";
		public const TOOL_POINTERS_OPTION_MOVE:String = "tool_pointers_move";
		public const TOOL_SHAPES_OPTION_SHAPE_ELLIPSE:String = "tool_shapes_opt_shape_ellipse";
		public const TOOL_SHAPES_OPTION_SHAPE_RECTANGLE:String = "tool_shapes_opt_shape_rectangle";
		public const TOOL_SHAPES_OPTION_SHAPE_TRIANGLE:String = "tool_shapes_opt_shape_triangle";
		public const TOOL_SHAPES_OPTION_SHAPE_LINE:String = "tool_shapes_opt_shape_line";
		public const TOOL_SHAPES_OPTION_SHAPE_ARROW:String = "tool_shapes_opt_shape_arrow";
		public const TOOL_SHAPES_OPTION_SHAPE_BUBBLE:String = "tool_shapes_opt_shape_bubble";
		public const TOOL_SHAPES_OPTION_LINESTYLE_SOLID:String = "tool_shapes_opt_linestyle_solid";
		public const TOOL_SHAPES_OPTION_LINESTYLE_DASHED:String = "tool_shapes_opt_linestyle_dashed";
		public const TOOL_SHAPES_OPTION_LINEWIDTH_1:String = "tool_shapes_opt_linewidth_1";
		public const TOOL_SHAPES_OPTION_LINEWIDTH_2:String = "tool_shapes_opt_linewidth_2";
		public const TOOL_SHAPES_OPTION_LINEWIDTH_3:String = "tool_shapes_opt_linewidth_3";
		public const TOOL_SHAPES_OPTION_LINEWIDTH_4:String = "tool_shapes_opt_linewidth_4";
		public const TOOL_SHAPES_OPTION_LINEWIDTH_5:String = "tool_shapes_opt_linewidth_5";
		public const TOOL_SHAPES_OPTION_ARROWHEAD_NONE:String = "tool_shapes_opt_arrowhead_none";
		public const TOOL_SHAPES_OPTION_ARROWHEAD_LEFT:String = "tool_shapes_opt_arrowhead_left";
		public const TOOL_SHAPES_OPTION_ARROWHEAD_RIGHT:String = "tool_shapes_opt_arrowhead_right";
		public const TOOL_SHAPES_OPTION_BUBBLE_ELLIPSE:String = "tool_shapes_opt_bubble_ellipse";
		public const TOOL_SHAPES_OPTION_BUBBLE_RECTANGLE:String = "tool_shapes_opt_bubble_square";
		public const TOOL_SHAPES_OPTION_BUBBLE_CIRCLE:String = "tool_shapes_opt_bubble_circle";
		public const TOOL_BRUSHES_OPTION_TOOL_PAINTBRUSH:String = "tool_brushes_opt_tool_paintbrush";
		public const TOOL_BRUSHES_OPTION_TOOL_HIGHLIGHTER:String = "tool_brushes_opt_tool_highlighter";
		public const TOOL_BRUSHES_OPTION_LINESTYLE_SOLID:String = "tool_brushes_opt_linestyle_solid";
		public const TOOL_BRUSHES_OPTION_LINESTYLE_DASHED:String = "tool_brushes_opt_linestyle_dashed";
		public const TOOL_BRUSHES_OPTION_LINEWIDTH_1:String = "tool_brushes_opt_linewidth_1";
		public const TOOL_BRUSHES_OPTION_LINEWIDTH_2:String = "tool_brushes_opt_linewidth_2";
		public const TOOL_BRUSHES_OPTION_LINEWIDTH_3:String = "tool_brushes_opt_linewidth_3";
		public const TOOL_BRUSHES_OPTION_LINEWIDTH_4:String = "tool_brushes_opt_linewidth_4";
		public const TOOL_BRUSHES_OPTION_LINEWIDTH_5:String = "tool_brushes_opt_linewidth_5";
		public const TOOL_BRUSHES_OPTION_ARROWHEAD_NONE:String = "tool_brushes_opt_arrowhead_none";
		public const TOOL_BRUSHES_OPTION_ARROWHEAD_LEFT:String = "tool_brushes_opt_arrowhead_left";
		public const TOOL_BRUSHES_OPTION_ARROWHEAD_RIGHT:String = "tool_brushes_opt_arrowhead_right";
		public const TOOL_DELETES_OPTION_CLICK:String = "tool_deletes_click";
		public const TOOL_DELETES_OPTION_SELECT:String = "tool_deletes_select";
		public const TOOL_ZOOM_OPTION_SIZE:String = "tool_zoom_opt_size";
		public const TOOL_TEXT_OPTION_FONT:String = "tool_text_opt_font";
		
		// Tool submenu state constants
		public const STATE_HIDDEN:int = 0x1;
		public const STATE_NOT_HIDDEN:int = 0x2;
		
		// Main toolbar icon border style
		public const TOOL_INACTIVE_STYLENAME:String = "toolBorder";
		public const MAIN_TOOL_ACTIVE_STYLENAME:String = "toolBorderActive";
		
		private var app:App = null; // Our App handle
		private var iToolSubmenu:IToolSubmenu = null; // Dynamic tools submenu (custom comp.)
		private var toolList:ToolList = null; // Tool list adapter.
		
		private var iPopupPanel:IToolSubmenu = null; // Dynamic popup panel (custom comp.)
		public var popupList:PopupList = null; // Popup list adapter.
		
		public var iToolOptionSubmenu:IToolSubmenu = null; // Dynamic tool options submenu (custom comp.)
		public var activeTool:String = ""; // Current tool the user is working with
		public var activeOptions:Object = null;
		
		public function Tools(app_handle:App)
		{
			app = app_handle;
			if(app !== null) {
				toolList = new ToolList(app);
				popupList = new PopupList(app);
				initialize();
			}
		}
		
		private function initialize():void
		{
			setupEventListeners();
			setupDefaultOptions();
		}
		
		private function setupEventListeners():void
		{
			// Let' add our event listeners for the microphone/webcam checkbox
			//app.instance
			setupMainToolbarEvents();
		}
		
		/* Main tools events & actions */
		// Add event listeners to the Main tool items
		private function setupMainToolbarEvents():void
		{
			// Double click events for Tool Menu
			app.instance.tool_pointers.doubleClickEnabled = true;
			app.instance.tool_pointers.addEventListener(MouseEvent.DOUBLE_CLICK,
				function(e:MouseEvent):void {
					showSmToolMenu("tool_pointers");
				});
			app.instance.tool_shapes.doubleClickEnabled = true;
			app.instance.tool_shapes.addEventListener(MouseEvent.DOUBLE_CLICK,
				function(e:MouseEvent):void {
					showSmToolMenu("tool_shapes");
				});
			app.instance.tool_brushes.doubleClickEnabled = true;
			app.instance.tool_brushes.addEventListener(MouseEvent.DOUBLE_CLICK,
				function(e:MouseEvent):void {
					showSmToolMenu("tool_brushes");
				});
			app.instance.tool_deletes.doubleClickEnabled = true;
			app.instance.tool_deletes.addEventListener(MouseEvent.DOUBLE_CLICK,
				function(e:MouseEvent):void {
					showSmToolMenu("tool_deletes");
				});
			app.instance.tool_zoom.doubleClickEnabled = true;
			app.instance.tool_zoom.addEventListener(MouseEvent.DOUBLE_CLICK,
				function(e:MouseEvent):void {
					showSmToolMenu("tool_zoom");
				});
			app.instance.tool_text.doubleClickEnabled = true;
			app.instance.tool_text.addEventListener(MouseEvent.DOUBLE_CLICK,
				function(e:MouseEvent):void {
					showSmToolMenu("tool_text");
				});
			
			// Single click events for Tool Menu
			app.instance.tool_pointers.addEventListener(MouseEvent.CLICK,
				function(e:MouseEvent):void {
					activeTool = activeOptions.pointer;
					if(app.instance.tool_pointers.styleName !== MAIN_TOOL_ACTIVE_STYLENAME) {
						removeAllActiveState();
						app.instance.tool_pointers.styleName = MAIN_TOOL_ACTIVE_STYLENAME;
					}
				});
			app.instance.tool_shapes.addEventListener(MouseEvent.CLICK,
				function(e:MouseEvent):void {
					activeTool = activeOptions.sel_shape_tool;
					if(app.instance.tool_shapes.styleName !== MAIN_TOOL_ACTIVE_STYLENAME) {
						removeAllActiveState();
						app.instance.tool_shapes.styleName = MAIN_TOOL_ACTIVE_STYLENAME;
					}
				});
			app.instance.tool_brushes.addEventListener(MouseEvent.CLICK,
				function(e:MouseEvent):void {
					activeTool = activeOptions.sel_brush_tool;
					if(app.instance.tool_brushes.styleName !== MAIN_TOOL_ACTIVE_STYLENAME) {
						removeAllActiveState();
						app.instance.tool_brushes.styleName = MAIN_TOOL_ACTIVE_STYLENAME;
					}
				});
			app.instance.tool_deletes.addEventListener(MouseEvent.CLICK,
				function(e:MouseEvent):void {
					activeTool = activeOptions.deleteType;
					if(app.instance.tool_deletes.styleName !== MAIN_TOOL_ACTIVE_STYLENAME) {
						removeAllActiveState();
						app.instance.tool_deletes.styleName = MAIN_TOOL_ACTIVE_STYLENAME;
					}
				});
			app.instance.tool_text.addEventListener(MouseEvent.CLICK,
				function(e:MouseEvent):void {
					activeTool = TOOL_TEXT_OPTION_FONT;
					if(app.instance.tool_text.styleName !== MAIN_TOOL_ACTIVE_STYLENAME) {
						removeAllActiveState();
						app.instance.tool_text.styleName = MAIN_TOOL_ACTIVE_STYLENAME;
					}
				});
			
			// Other buttons/tools in main application
			app.instance.btn_mediaSettings.addEventListener(MouseEvent.CLICK,
				function(e:MouseEvent):void {
					iPopupPanel = popupList.getPopupMediaSettings();
					iPopupPanel.display();
				});
			app.instance.btn_stewie.addEventListener(MouseEvent.CLICK,
				function(e:MouseEvent):void {
					iPopupPanel = popupList.getPopupLoginScreen();
					iPopupPanel.display();
				});
			app.instance.btn_settings.addEventListener(MouseEvent.CLICK,
				function(e:MouseEvent):void {
					iPopupPanel = popupList.getPopupFileManager();
					iPopupPanel.display();
				});
		}
		
		// Display tool submenu when a tool in the toolbar is clicked
		public function showSmToolMenu(tool:String):void
		{
			cleanupSmToolMenus();
				switch(tool) {
					case 'tool_pointers':
						iToolSubmenu = toolList.getToolPointersSubmenu();
						iToolSubmenu.display();
						break;
					case 'tool_shapes':
						iToolSubmenu = toolList.getToolShapesSubmenu();
						iToolSubmenu.display();
						break;
					case 'tool_brushes':
						iToolSubmenu = toolList.getToolBrushesSubmenu();
						iToolSubmenu.display();
						break;
					case 'tool_deletes':
						iToolSubmenu = toolList.getToolDeletesSubmenu();
						iToolSubmenu.display();
						break;
					case 'tool_zoom':
						if(iToolOptionSubmenu !== toolList.getToolZoomSubmenu()) {
							app.tools.cleanupOptToolMenus();
						}
						iToolOptionSubmenu = toolList.getToolZoomSubmenu();
						iToolOptionSubmenu.display();
						break;
					case 'tool_text':
						if(iToolOptionSubmenu !== toolList.getToolTextSubmenu()) {
							app.tools.cleanupOptToolMenus();
						}
						iToolOptionSubmenu = toolList.getToolTextSubmenu();
						iToolOptionSubmenu.display();
						break;
				}
		}
		
		// clean up for submenu popup
		public function cleanupSmToolMenus():void
		{
			if(iToolSubmenu !== null) {
				iToolSubmenu.hide();
			}
		}
		
		// clean up for submenu options popup
		public function cleanupOptToolMenus():void
		{
			if(iToolOptionSubmenu !== null) {
				iToolOptionSubmenu.hide();
			}
		}
		
		public function getActiveTool():IToolSubmenu
		{
			return iToolSubmenu;
		}
		
		public function removeAllActiveState():void
		{
			app.instance.tool_pointers.styleName = TOOL_INACTIVE_STYLENAME;
			app.instance.tool_shapes.styleName = TOOL_INACTIVE_STYLENAME;
			app.instance.tool_brushes.styleName = TOOL_INACTIVE_STYLENAME;
			app.instance.tool_zoom.styleName = TOOL_INACTIVE_STYLENAME;
			app.instance.tool_deletes.styleName = TOOL_INACTIVE_STYLENAME;
		}
		
		private function setupDefaultOptions():void
		{
			activeOptions = new Object();
			activeOptions.pointer = TOOL_POINTERS_OPTION_POINTER;
			activeOptions.shape = TOOL_SHAPES_OPTION_SHAPE_ELLIPSE;
			activeOptions.sel_shape_tool = TOOL_SHAPES_SUBMENU_ELLIPSE;
			activeOptions.lineStyle = TOOL_SHAPES_OPTION_LINESTYLE_SOLID;
			activeOptions.lineWidth = TOOL_SHAPES_OPTION_LINEWIDTH_1;
			activeOptions.lineWidthValue = 1;
			activeOptions.arrowHead = TOOL_SHAPES_OPTION_ARROWHEAD_LEFT;
			activeOptions.bubbleType = TOOL_SHAPES_OPTION_BUBBLE_ELLIPSE;
			activeOptions.sel_brush_tool = TOOL_BRUSHES_SUBMENU_PAINTBRUSH;
			activeOptions.deleteType = TOOL_DELETES_OPTION_CLICK;	
			activeOptions.brushTool = TOOL_BRUSHES_OPTION_TOOL_PAINTBRUSH;
			activeOptions.brushLineStyle = TOOL_BRUSHES_OPTION_LINESTYLE_SOLID;
			activeOptions.brushLineWidth = TOOL_BRUSHES_OPTION_LINEWIDTH_1;
			activeOptions.brushArrowHead = TOOL_BRUSHES_OPTION_ARROWHEAD_NONE;
			activeOptions.textFontFamily = "Helvetica Neue LT";
			activeOptions.textFontStyle = "Normal";
			activeOptions.textFontSize = 12;
			activeOptions.textFontEmbedded = true;
		}
		
	}
}