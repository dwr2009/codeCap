"use strict";
(function( ) {

	var name = "Drives" ;

	var W = 250 ;
	var H = 250 ;

	var free_color = '#0b8bca' ;
	var used_color = 'white' ;

	var $canvas = null ;

	var createCanvas = function( ) {
		if( $canvas ) {
			$canvas.detach( ) ;
		}
		$canvas = $( '<canvas>' ).attr( {
			'width': W,
			'height': H,
		} ).css( {
			'width': W + 'px',
			'height': H + 'px',
		} ) ;
	} ;

	var drawDisc = function( c, color, radius, angle ) {
		var center = { 'x': W / 2, 'y': H / 2 } ;

		c.save( ) ;
			c.fillStyle = color ;
			c.beginPath( ) ;
				c.moveTo( center.x, center.y ) ;
				c.arc( center.x, center.y, radius, -Math.PI, angle - Math.PI ) ;
				c.lineTo( center.x, center.y ) ;
			c.fill( ) ;
		c.restore( ) ;
	} ;

	var drawPie = function( ratio ) {
		createCanvas( ) ;
		var c = $canvas[0].getContext( '2d' ) ;

		var size = Math.min( W, H ) / 2 ;
		var radius = size * 0.9 ;
		var TWO_PI = Math.PI * 2 ;

		drawDisc( c, 'gray',     radius + 3, TWO_PI ) ;
		drawDisc( c, used_color, radius,     TWO_PI ) ;
		drawDisc( c, free_color, radius,     TWO_PI * (1 - ratio) ) ;
	} ;


	var Drives = function( ) {

		var parent = new window.MainMenuScreen( name ) ;
		var self = Object.create( parent ) ;

		self.$root.attr( 'id', 'drives' ) ;

        var $drives_info = $( '<div>' ).addClass( 'drives_info' ).appendTo( self.$root ) ;
		var $headline_text = $( '<div>' ).addClass( 'headline_text' ).append( $TR( 'drives screen :: drives' ) ).appendTo( $drives_info ) ;
		var $content = $( '<div>' ).addClass( 'content' ).appendTo( $drives_info ) ;
        
        var switchToStick =  function(item) {
//            console.log("---> calling switchToStick to update drive item!");
            updateSelectedDrive(item);
        };
        
        var stickDescription = 
        {
            name: "drives screen :: drive_name",
            items:[],
            action: switchToStick,
        };
        var $stick_info = $( '<div>' ).addClass( 'stick_info' ).appendTo( $content ) ;
        var stickList = new window.SelectView(stickDescription, stickDescription.action);
        stickList.$root.appendTo($stick_info);
        stickList.show();
        
		var $left = $( '<div>' ).addClass( 'column_left' ).appendTo( $content ) ;
		var $icon = $( '<div>' ).addClass( 'usb_icon' ).appendTo( $left ) ;
		var $name = $( '<div>' ).addClass( 'name' ).appendTo( $left ) ;
		var $size = $( '<div>' ).addClass( 'size' ).appendTo( $left ) ;
		var $used = $( '<div>' ).addClass( 'used' ).appendTo( $left ) ;
		var $used_icon = $( '<div>' ).addClass( 'used_icon icon' ).appendTo( $left ) ;
		var $used_text = $( '<div>' ).addClass( 'used_text text' ).appendTo( $left ) ;
		var $free = $( '<div>' ).addClass( 'free' ).appendTo( $left ) ;
		var $free_icon = $( '<div>' ).addClass( 'free_icon icon' ).appendTo( $left ) ;
		var $free_text = $( '<div>' ).addClass( 'free_text text' ).appendTo( $left ) ;
        
        var $right = $( '<div>' ).addClass( 'column_right' ).appendTo( $content ) ;

		$( '<div>' ).addClass( 'clear' ).appendTo( $content ) ;
		$( '<div>' ).addClass( 'clear' ).appendTo( $used ) ;
		$( '<div>' ).addClass( 'clear' ).appendTo( $free ) ;
        
        var select_view = null;
		
		var drive_pop_Dlg = new window.popDlg( );
        drive_pop_Dlg.$root.appendTo( self.$root ) ;
        drive_pop_Dlg.hide();
        
        var updateDrives = function(drives) {
            console.log("the num of drives: " + drives.length + " : \n" + JSON.stringify(drives));
            Platform.storage.getDrives( populateDrives ) ;
        } ;
		
		var populateDrives = function( error, drives_list ) {
			if( error ) { throw new Error( error.message ) ; }
			
            console.log("the num of drives_list: " + drives_list.length + " : \n" + JSON.stringify(drives_list));
			if(drives_list.length == 0){
				$drives_info.hide();
				var str = "popDlg :: drive" ;
				drive_pop_Dlg.show();
				drive_pop_Dlg.showDlgMsg( str );
				//setTimeout(function( ) { drive_pop_Dlg.hide(); },3000);
			}
			else {
                stickList.updateItems(drives_list);
                stickList.changeSelection(0);
                drive_pop_Dlg.hide();
				$drives_info.show();
			}
		} ;
        
        var updateSelectedDrive =  function (item) {
            updateText( item ) ;
            updateCanvas( item ) ;
        } ;

		var updateText = function( drive ) {
			$name.text( drive.name ) ;
			$size.text( TR( "drives screen :: format" ) + ': ' + drive.fileSystem + " " + Format.formatByteSize( drive.size, 3 ) ) ;
			$used_icon.css( 'background', used_color ) ;
			$used_text.text( TR( "drives screen :: free" ) + ": " + Format.formatByteSize( drive.free, 3 ) ) ;
			$free_icon.css( 'background', free_color ) ;
			$free_text.text( TR( "drives screen :: used" ) + ": " + Format.formatByteSize( drive.size - drive.free, 3 ) ) ;

			console.log( "Drive: " + drive.name + " ---> URL: " + drive.mediaUrl + " Path: " + drive.path + " UUID: " + drive.uuid + " isWritable: " + drive.writable + " isMounted: " + drive.mounted + " ---> " + Format.formatByteSize( drive.size, 3 ) + "(" + drive.fileSystem + ") - " + Format.formatByteSize( drive.free, 3 ) + " Free" ) ;
		} ;

		var updateCanvas = function( drive ) {
			var ratio = drive.free / drive.size ;
			drawPie( ratio ) ;
			$canvas.appendTo( $right ) ;
		} ;
        
        var start = function( ) {
			Platform.storage.setOnDrivesChangedCallback( updateDrives ) ;
//            MediaEngineDrives.onDrivesChanged.connect( updateDrives );
			Platform.storage.getDrives( populateDrives ) ;
		} ;

        var stop = function( ) {
            $drives_info.hide();
		} ;

		self.onKey = function( ) {
			return true ;
		} ;
        
        self.onKeyOK = function() {
            if (!select_view) {
                select_view = stickList;
                select_view.activate(true);
            } else {
                select_view.activate(false);
                select_view = null;
            }
        } ;
        
        self.onKeyLEFT = function() {
            if (select_view) {
                select_view.selectPrev();
                return;
            } else {
                window.AppManager.back();
            }
        } ;
        
        self.onKeyRIGHT = function() {
            if (select_view) {
                select_view.selectNext();
                return;
            }
        } ;
        
        self.onKeyEXIT = function () {
            if (!select_view) {
                window.AppManager.back();
            } else {
                select_view.activate(false);
                select_view = null;
            }
        };

		self.onShow = function( ) {
			start( ) ;
		} ;

		self.onHide = function( ) {
			stop( ) ;
		} ;

		return self ;
	} ;

	window.AppManager.addScene( name, Drives ) ;

})( ) ;