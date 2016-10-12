"use strict";
(function( ) {

	var prefix = 'js/widgets/' ;

	var loading_job = window.Loader.loadScripts( [
		// abstract classes
		prefix + 'widget.js',
		prefix + 'scene.js',
		prefix + 'list_view.js',
		prefix + 'scroll_list_view.js',
		prefix + 'page_turning_list_view.js',
		prefix + 'list_view_delegate.js',
		prefix + 'menu_item.js',
		prefix + 'submenu.js',
		prefix + 'main_menu_screen.js',
		prefix + 'select_view.js',
		prefix + 'settings.js',
		prefix + 'input_item.js',
		prefix + 'input_item_vk.js',
		prefix + 'input_view.js',
		prefix + 'label_view.js',
		prefix + 'button_view.js',
		prefix + 'process_bar.js',
		prefix + 'focus_list.js',
		prefix + 'audiotrack_selectdlg.js',
		prefix + 'reminder_row.js',

		// controllers
		prefix + 'live_tv.js',
		prefix + 'volume_bar.js',
		prefix + 'channel_number.js',
		prefix + 'channel_bar.js',
		prefix + 'info_bar.js',
		prefix + 'epg.js',
		prefix + 'top_bar.js',
		prefix + 'ok_bar.js',
		prefix + 'Favourite_Channels.js',
		prefix + 'date_time.js',
		prefix + 'internet.js',
		prefix + 'main_menu.js',
		prefix + 'all_channels_screen.js',
		prefix + 'drives_screen.js',
		prefix + 'recording_list_screen.js',
		prefix + 'language_screen.js',
		prefix + 'messages_screen.js',
		prefix + 'service_info_screen.js',
		prefix + 'general_settings_screen.js',
		prefix + 'system_info_screen.js',
		prefix + 'ca_menu.js',
		prefix + 'network_conf/bottome_group.js',
		prefix + 'network_conf/mono_list.js',
		prefix + 'network_conf/list_manager.js',
		prefix + 'network_conf/lan_overlay.js',
//		prefix + 'network_conf/normalised_select_view.js',
		prefix + 'network_conf/connection_widget.js',
		prefix + 'network_conf/wifi_config.js',
		prefix + 'network_conf/wifi_key_input.js',
		prefix + 'keyinput_PopDlg.js',
		prefix + 'parental_lock.js',
		// prefix + 'vod_screen.js',
		// prefix + 'catchup_screen.js',
		// prefix + 'my_account_screen.js',
		prefix + 'statement_screen.js',
		prefix + 'tem_account.js',
		// prefix + 'parental_lock_screen.js',
		prefix + 'camenu/pin_code_screen.js',
		prefix + 'camenu/parental_rating_screen.js',
		prefix + 'camenu/smart_card_screen.js',
		prefix + 'camenu/work_time_screen.js',
		prefix + 'camenu/email_screen.js',
		prefix + 'camenu/stb_info_screen.js',
		prefix + 'camenu/ca_info_screen.js',
		prefix + 'camenu/nstv_ca.js',
		prefix + 'camenu/fingerPrinter.js',
		prefix + 'camenu/emailNotifyIcon.js',
		prefix + 'camenu/popDlg.js',
		prefix + 'camenu/buyMsgpopDlg.js',
		prefix + 'camenu/showOSDMsg.js',
		prefix + 'camenu/curtainNotify.js',
		prefix + 'camenu/confirmPopDlg.js',
		prefix + 'camenu/selectPopDlg.js',
		prefix + 'camenu/ippv_bar.js',
		prefix + 'camenu/progressStrip.js',
		// prefix + 'system_info_screen.js',
		prefix + 'progress_bar.js',
		prefix + 'scroll_bar.js',
		prefix + 'manual_scan_c_screen.js',
		prefix + 'auto_scan_c_screen.js',
		prefix + 'app_screen.js',
		prefix + 'on_demand_screen.js',
		prefix + 'timeshift_bar.js',
		prefix + 'timeshift_icon.js',
		prefix + 'favPopDlg.js',
		prefix + 'favorite_management.js',
		prefix + 'hbbtv_app.js',
        	prefix + 'reminder_pop.js',
		prefix + 'alert_window.js',
		prefix + 'program_reminder.js',
		prefix + 'reminder_screen.js',
		prefix + 'confirm_box.js',
		prefix + 'tuner_status.js',
	] ) ;


	window.AppManager = new function( ) {

		this.init = function( done ) {
			//log( "initializing app manager" ) ;
			//console.log( "initializing app manager" ) ;

			var callback = function( ) {
				//console.log( "app manager initialized" ) ;

				done && done( ) ;
			} ;

			$( document ).keydown( this.handleKeys ) ;
			//window.Platform.pvr.setReminderCallback( this.handleReminder );
			//MediaEnginePVR.nextReminderChanged.connect(this.handleReminder);

			loading_job.onDone( callback ) ;
		} ;


		var widgets = {} ;
		var scenes = {} ;

		this.addWidget = function( name, widget ) {
			var _widget = widgets[name] ;
			//console.log(_widget + "has added");

			if( _widget ) {
				throw new Error( "Widget '" + name + "' already exists." ) ;
			}

			widgets[name] = widget ;
		} ;

		this.addScene = function( name, scene ) {
			this.addWidget( name, scene ) ;
			//console.log(name +" scene add successful");
			scenes[name] = true ;
		} ;

		this.getWidget = function( name ) {
			var _widget = widgets[name] ;

			if( !_widget ) {
				throw new Error( "Widget '" + name + "' does not exists." ) ;
			}

			// lazy initialization
			if( _.isFunction( _widget ) ) {
				//console.log( "Creating", name ) ;

				_widget = widgets[name] = new _widget( ) ;
			}

			return _widget ;
		} ;

		this.getScene = function( name ) {
			var widget = this.getWidget( name ) ;

			if( !scenes[name] ) {
				throw new Error( "Widget '" + name + "' is not a scene." ) ;
			}

			return widget ;
		} ;


		var stack = [] ;

		this.current = function( ) {
			return stack[stack.length - 1] ;
		} ;

		this.enter = function( name ) {
			var screen = this.getScene( name ) ;
			if( !screen ) {
				return ;
			}

			var current = this.current( ) ;
			current && current.widget.leave( ) ;

			stack.push( { name: name, widget: screen } ) ;

			screen.enter( ) ;
		} ;

		this.back = function( ) {
			if( stack.length < 2 ) {
				return ;
			}

			var current = this.current( ) ;
			current.widget.exit( ) ;

			stack.pop( ) ;

			var current = this.current( ) ;
			current && current.widget.return( ) ;
		} ;

		this.isInStack = function( name ) {
			for( var i in stack ) {
				if( stack[i].name === name ) {
					return true ;
				}
			}
			return false ;
		} ;


		this.handleKeys = function( event ) {

			var getNumber = function( name ) {
				return name.length == 1 && parseInt( name ) ;
			} ;


			var code  = event.which ;
			var alt   = event.altKey ;
			var ctrl  = event.ctrlKey ;
			var shift = event.shiftKey ;

			var name = window.Platform.keymap[code] ;

			// console.log( "key pressed:", code, name, "alt:", alt, "ctrl:", ctrl, "shift:", shift ) ;
			console.log( "app_manager --> handleKeys --> key pressed:", code, name ) ;

			// handle power event
			if ( name == "POWER" ) {
				console.log ( "Press POWER!!!" );
				//MediaEngineBrowser.switchToMainView();
				//var mode = MediaEngineSystem.getStandbyMode( );
				switch (mode){
				  case Platform.system.MediaEngineStandbyMode.OFF :
					Platform.system.setStandbyMode( Platform.system.MediaEngineStandbyMode.ACTIVE_LED_ONLY );
					var liveTV = window.AppManager.getWidget("Live TV");
					liveTV.judgeAdultChannel();
					
					var liveTV_timeshift = window.AppManager.getWidget("Live TV");
					liveTV_timeshift.judgeIsTimeShift();
				    break;
				  case Platform.system.MediaEngineStandbyMode.ACTIVE_LED_ONLY :
					Platform.system.setStandbyMode( Platform.system.MediaEngineStandbyMode.OFF );
					var liveTV = window.AppManager.getWidget("Live TV");
					liveTV.activate(  );
				    break;
				  default :
				    break;
				}
				name && !(alt || ctrl || shift) && event.preventDefault( ) ;
				return ;
			}

			/*if ( MediaEngineSystem.getStandbyMode( ) == Platform.system.MediaEngineStandbyMode.ACTIVE_LED_ONLY ){
				console.log ("-->Key filtered!!! --> wake up only possible with POWER----------->" + MediaEngineSystem.getStandbyMode( ));
				name && !(alt || ctrl || shift) && event.preventDefault( ) ;
				return ;
			}*/

			// handle freeze event
			if ( window.Platform.ca.UIFreezed == true && name != "GREEN" ) {
				console.log ( "UI was freezed!!!" );
				name && !(alt || ctrl || shift) && event.preventDefault( ) ;
				return ;
			}

			// propagate the key event through the stack, stopping if any handler returns a truthy value.
			var stop_propagation = false ;
			for( var i = stack.length - 1 ; i >= 0 ; i-- ) {
				var widget = stack[i].widget ;

				stop_propagation = widget.onKey && widget.onKey( code, name ) ;

				if( name ) {
					var handler = "onKey" + name ;
					stop_propagation = (widget[handler] && widget[handler]( )) || stop_propagation ;

					var number = getNumber( name ) ;
					if( _.isNumber( number ) ) {
						stop_propagation = (widget.onKeyNUMBER && widget.onKeyNUMBER( number )) || stop_propagation ;
					}
				}

				if( stop_propagation ) break ;
			}

			if( !alt && !ctrl && !shift ) {
				// handle all key events
				// prevent them from being propagated any further
				event.stopPropagation( ) ;
				event.preventDefault( ) ;
			}

		} ;

		this.handleReminder = function( ) {
			console.log( "-> Open the Reminder Pop window");
			if( window.AppManager.current( ).name !== "on demand" && window.AppManager.current( ).name !== "APP"){
				window.AppManager.enter("Reminder Pop");
			}
			else {
				var reminder = window.AppManager.getWidget("Reminder Pop");
				reminder.deleteTheReminder();
			}
		}
	} ;

})( ) ;
